//=========================================================
//
// ゲーム画面処理 [game.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "game.h"
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
#include "enemy.h"

//===============================================
// 静的メンバ変数
//===============================================
CPlayer *CGame::m_pPlayer = NULL;						// プレイヤークラスのポインタ
CMeshField *CGame::m_pMeshField = NULL;					// メッシュフィールドクラスのポインタ
CObject3D *CGame::m_pObject3D = NULL;					// オブジェクト3Dクラスのポインタ
CPause *CGame::m_pPause = NULL;							// ポーズクラスのポインタ
CEnemy *CGame::m_pEnemy = NULL;							// 敵クラスのポインタ

bool CGame::m_bPause = false;				// ポーズ状態
bool CGame::m_bStateReady = false;			// GAMSESTATE_READYかどうか
bool CGame::m_bPauseCamera = false;			// ポーズ時のカメラ操作可能か

//===============================================
// コンストラクタ
//===============================================
CGame::CGame() : CScene()
{
	// 値のクリア
	m_state = STATE_NONE;
	m_nCounterState = 0;
}

//===============================================
// デストラクタ
//===============================================
CGame::~CGame()
{
	
}

//===============================================
// 初期化処理
//===============================================
HRESULT CGame::Init(HWND hWnd)
{
	m_bPause = false;
	m_bStateReady = true;		// 待機状態にする
	m_bPauseCamera = false;

	// カメラの初期化処理
	CManager::GetInstance()->GetCamera()->Init();

	// オブジェクトXファイルの生成
	CObjectX::Load(hWnd);

	// プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 210.0f, -350.0f), 4);

	// 敵の生成
	CEnemy::Load();

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
void CGame::Uninit(void)
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
void CGame::Update(void)
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
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_BACK, 0) == true || m_pPlayer->GetPos().z > 1400.0f)
	{// BackSpace
		CRenderer::GetFade()->Set(CScene::MODE_RESULT);		// リザルト画面へ移動
	}
//#endif

	if (m_pPlayer->GetPos().y < -100.0f)
	{// 落下死
		CRenderer::GetFade()->Set(CScene::MODE_GAME);		// リザルト画面へ移動
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
			m_nCounterState = TIME_STATEREADY;
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
				m_nCounterState = TIME_STATEFEVER;	// 次の状態への時間を設定
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
void CGame::Draw(void)
{
	
}

//===============================================
// ポーズ状態の設定
//===============================================
void CGame::SetEnablePause(const bool bPause)
{
	m_bPause = bPause;
}