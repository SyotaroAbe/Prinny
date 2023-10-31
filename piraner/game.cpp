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
#include "gamebg.h"
#include "time.h"

//===============================================
// 静的メンバ変数
//===============================================
CPlayer *CGame::m_pPlayer = NULL;						// プレイヤークラスのポインタ
CMeshField *CGame::m_pMeshField = NULL;					// メッシュフィールドクラスのポインタ
CObject3D *CGame::m_pObject3D = NULL;					// オブジェクト3Dクラスのポインタ
CPause *CGame::m_pPause = NULL;							// ポーズクラスのポインタ
CEnemy *CGame::m_pEnemy = NULL;							// 敵クラスのポインタ
CGameBg *CGame::m_pGameBg = NULL;						// 背景クラスのポインタ
CTime *CGame::m_pTime = NULL;							// タイムクラスのポインタ

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
	m_hWnd = NULL;
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

	m_hWnd = hWnd;		// HWND保存

	// カメラの初期化処理
	CManager::GetInstance()->GetCamera()->Init();

	// オブジェクトXファイルの生成
	CObjectX::Load(hWnd);

	// プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 210.0f, -350.0f), 4);
	
	// 背景の生成
	m_pGameBg = CGameBg::Create(D3DXVECTOR3(m_pPlayer->GetPos().x, 201.0f, CManager::GetInstance()->GetCamera()->GetPosR().z), CGameBg::TEX_GAME, 0);

	// 敵の生成
	CEnemy::Load(hWnd);

	// タイムの生成
	m_pTime = CTime::Create(5);

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
	// タイムの終了処理
	if (m_pTime != NULL)
	{
		m_pTime->Uninit();
		delete m_pTime;
		m_pTime = NULL;
	}

	// ポーズの終了処理
	if (m_pPause != NULL)
	{
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = NULL;
	}

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

#if _DEBUG
	if (m_bPause == true && CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_F3) == true)
	{// ポーズ中カメラ操作
		m_bPauseCamera = m_bPauseCamera ? false : true;		// ポーズ状態切り替え
	}

	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_BACKSPACE) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_BACK, 0) == true)
	{// BackSpace
		CRenderer::GetFade()->Set(CScene::MODE_BOSS);		// リザルト画面へ移動
		SetTime(m_pTime->Get());		// 時間の設定
	}
#endif

	if (m_pPlayer->GetPos().z > 7500.0f)
	{// BackSpace
		CRenderer::GetFade()->Set(CScene::MODE_BOSS);		// リザルト画面へ移動
		SetTime(m_pTime->Get());		// 時間の設定
	}

	if (m_pPlayer->GetPos().y < -100.0f)
	{// 落下死
		GetPlayer()->SetState(CPlayer::STATE_DEATH);
		CManager::GetInstance()->AddCountDeath(CManager::GetMode());			// 死亡回数をカウント
		CRenderer::GetFade()->Set(CScene::MODE_GAME);							// リザルト画面へ移動
		//Reset();	// リセット
		SetTime(m_pTime->Get());		// 時間の設定
	}

	if (m_bStateReady == false)
	{// 待機状態じゃない
		if (m_bPause == false)
		{// ポーズ状態じゃない
			// タイムの更新処理
			if (m_pTime != NULL)
			{
				m_pTime->Update();
			}
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
		if (m_pPause != NULL)
		{
			m_pPause->Update();
		}
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
	// タイムの描画処理
	if (m_pTime != NULL)
	{
		m_pTime->Draw();
	}
}

//===============================================
// リセット処理
//===============================================
void CGame::Reset(void)
{
	//CRenderer::GetFade()->Set(CScene::MODE_GAME, false);

	////if (CRenderer::GetFade()->Get() == CFade::STATE_IN)
	////{
	//	// サウンドの停止
	//	CManager::GetInstance()->GetSound()->Stop(CSound::LABEL_BGM_GAME);

	//	// ポーズの終了処理
	//	m_pPause->Uninit();
	//	delete m_pPause;
	//	m_pPause = NULL;

	//	// オブジェクトのリセット
	//	CObject::Reset();

	//	m_bPause = false;
	//	m_bStateReady = true;		// 待機状態にする
	//	m_bPauseCamera = false;

	//	// カメラの初期化処理
	//	CManager::GetInstance()->GetCamera()->Init();

	//	//// オブジェクトXファイルの生成
	//	//CObjectX::Load(m_hWnd);

	//	// プレイヤーの初期化
	//	m_pPlayer->Init(D3DXVECTOR3(0.0f, 210.0f, -350.0f));

	//	// 背景の初期化
	//	m_pGameBg->Init(D3DXVECTOR3(m_pPlayer->GetPos().x, m_pPlayer->GetPos().y, CManager::GetInstance()->GetCamera()->GetPosR().z));

	//	// 敵の生成
	//	CEnemy::Load(m_hWnd);

	//	// ポーズの生成
	//	m_pPause = CPause::Create(6);

	//	// 通常状態に設定
	//	m_state = STATE_NORMAL;
	//	m_nCounterState = 0;

	//	// サウンドの再生
	//	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);
	////}
}

//===============================================
// ポーズ状態の設定
//===============================================
void CGame::SetEnablePause(const bool bPause)
{
	m_bPause = bPause;
}