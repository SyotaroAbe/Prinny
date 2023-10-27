//=========================================================
//
// ボス戦処理 [bossBattle.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "bossBattle.h"
#include "object3D.h"
#include "objectX.h"
#include "meshfield.h"
#include "pause.h"
#include "input.h"
#include "renderer.h"
#include "fade.h"
#include "camera.h"
#include "debugproc.h"
#include "sound.h"
#include "player.h"
#include "boss.h"
#include "gamebg.h"
#include "clear.h"

//===============================================
// 静的メンバ変数
//===============================================
CPlayer *CBossBattle::m_pPlayer = NULL;						// プレイヤークラスのポインタ
CMeshField *CBossBattle::m_pMeshField = NULL;					// メッシュフィールドクラスのポインタ
CObject3D *CBossBattle::m_pObject3D = NULL;					// オブジェクト3Dクラスのポインタ
CPause *CBossBattle::m_pPause = NULL;							// ポーズクラスのポインタ
CBoss *CBossBattle::m_pBoss = NULL;							// ボスクラスのポインタ
CGameBg *CBossBattle::m_pGameBg = NULL;						// 背景クラスのポインタ
CClear *CBossBattle::m_pClear = NULL;						// クリア表示クラスのポインタ

bool CBossBattle::m_bPause = false;				// ポーズ状態
bool CBossBattle::m_bStateReady = false;			// GAMSESTATE_READYかどうか
bool CBossBattle::m_bPauseCamera = false;			// ポーズ時のカメラ操作可能か

//===============================================
// コンストラクタ
//===============================================
CBossBattle::CBossBattle() : CScene()
{
	// 値のクリア
	m_state = STATE_NONE;
	m_nCounterState = 0;
}

//===============================================
// デストラクタ
//===============================================
CBossBattle::~CBossBattle()
{
	
}

//===============================================
// 初期化処理
//===============================================
HRESULT CBossBattle::Init(HWND hWnd)
{
	m_bPause = false;
	m_bStateReady = true;		// 待機状態にする
	m_bPauseCamera = false;

	// カメラの初期化処理
	CManager::GetInstance()->GetCamera()->Init();

	// オブジェクトXファイルの生成
	CObjectX::Load(hWnd);

	// プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 330.0f, -100.0f), 4);

	// 背景の生成
	m_pGameBg = CGameBg::Create(D3DXVECTOR3(m_pPlayer->GetPos().x, m_pPlayer->GetPos().y, CManager::GetInstance()->GetCamera()->GetPosR().z), CGameBg::TEX_BOSS, 0);

	// ボスの生成
	m_pBoss = CBoss::Create(D3DXVECTOR3(50.0f, 420.0f, 900.0f), CBoss::TYPE_WALK, 4);

	// クリア表示の生成
	m_pClear = CClear::Create();

	// ポーズの生成
	m_pPause = CPause::Create(6);

	// 通常状態に設定
	m_state = STATE_NORMAL;
	m_nCounterState = 0;

	// サウンドの再生
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CBossBattle::Uninit(void)
{
	// ポーズの終了処理
	m_pPause->Uninit();
	delete m_pPause;
	m_pPause = NULL;

	// 全てのオブジェクトの破棄
	CObject::ReleaseAll();
}

//===============================================
// 更新処理
//===============================================
void CBossBattle::Update(void)
{
	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_P) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_START, 0) == true)
	{// ポーズ入力
		m_bPause = m_bPause ? false : true;		// ポーズ状態切り替え

		if (m_bPause == true)
		{
			// サウンドの再生
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_PAUSE);
		}
		else
		{
			// サウンドの再生
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_PAUSE_CANCEL);
		}
	}

//#if _DEBUG
	if (m_bPause == true && CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_F3) == true)
	{// ポーズ中カメラ操作
		m_bPauseCamera = m_bPauseCamera ? false : true;		// ポーズ状態切り替え
	}

	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_BACKSPACE) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_BACK, 0) == true || m_pPlayer->GetPos().z > 7500.0f)
	{// BackSpace
		CRenderer::GetFade()->Set(CScene::MODE_RESULT);		// リザルト画面へ移動
	}
//#endif

	if (m_pPlayer->GetPos().y < -200.0f)
	{// 落下死
		GetPlayer()->SetState(CPlayer::STATE_DEATH);
		CRenderer::GetFade()->Set(CScene::MODE_BOSS);		// リザルト画面へ移動
	}

	if (m_bStateReady == false)
	{// 待機状態じゃない
		if (m_bPause == false)
		{// ポーズ状態じゃない
			
		}
	}
	else if (m_bStateReady == true)
	{// 待機状態のフラグが立っている
		if (m_state == STATE_NORMAL)
		{
			// 待機状態へ切り替える
			m_state = STATE_READY;
			m_nCounterState = 120;
		}
	}

	if (m_bPause == true && m_bPauseCamera == false)
	{// ポーズ状態
		// ポーズの更新処理
		m_pPause->Update();
	}

	switch (m_state)
	{
	case STATE_NORMAL:  // 通常状態
		m_nCounterState--;
		break;

	case STATE_READY:	// 開始待機状態
		if (m_bPause == false)
		{// ポーズ状態じゃない
			m_nCounterState--;

			if (m_nCounterState <= 0)
			{
				m_state = STATE_NORMAL;				// 通常状態に設定
				m_nCounterState = 5400;
				m_bStateReady = false;
			}
		}
		break;

	case STATE_END:     // 終了状態
		if (m_bPause == false)
		{// ポーズ状態じゃない
			m_nCounterState--;

			if (m_nCounterState <= 0)
			{
				m_state = STATE_NONE;  // 何もしていない状態に設定
			}
		}
		break;
	}

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(" 現在の状態：%d\n", m_state);
}

//===============================================
// 描画処理
//===============================================
void CBossBattle::Draw(void)
{
	
}

//===============================================
// ポーズ状態の設定
//===============================================
void CBossBattle::SetEnablePause(const bool bPause)
{
	m_bPause = bPause;
}