//=========================================================
//
// チュートリアル画面処理 [tutorial.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "tutorial.h"
#include "renderer.h"
#include "fade.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "meshfield.h"
#include "sound.h"
#include "bg.h"
#include "player.h"
#include "objectX.h"
#include "objectBillboard.h"
#include "gamebg.h"

//===============================================
// マクロ定義
//===============================================
#define FIRSTSET_FIELD			(10)			// 床の初期配置数
#define DISTANCE_FIELD			(-1000.0f)		// 床の配置間隔

#define TIME_CREATEFIELD		(116)			// 床の生成時間間隔
#define TIME_CREATEDUMPSTER		(1300)			// ゴミステーションの生成時間間隔
#define POS_CREATEOBJ			(-9120.0f)		// 生成位置間隔

//===============================================
// 静的メンバ変数
//===============================================
CPlayer *CTutorial::m_pPlayer = NULL;			// プレイヤークラスのポインタ
CNumber *CTutorial::m_pNumber = NULL;			// ナンバークラスのポインタ
CMeshField *CTutorial::m_pMeshField = NULL;		// メッシュフィールドクラスのポインタ
CBg *CTutorial::m_pBg = NULL;						// 背景クラスのポインタ
CGameBg *CTutorial::m_pGameBg = NULL;						// 背景クラスのポインタ

//===============================================
// コンストラクタ
//===============================================
CTutorial::CTutorial() : CScene()
{
	// 値のクリア
	m_nTime = 0;
	m_bFade = false;
}

//===============================================
// デストラクタ
//===============================================
CTutorial::~CTutorial()
{
	
}

//===============================================
// 初期化処理
//===============================================
HRESULT CTutorial::Init(HWND hWnd)
{
	// カメラの初期化処理
	CManager::GetInstance()->GetCamera()->Init();

	// オブジェクトXファイルの生成
	CObjectX::Load(hWnd);

	// プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 510.0f, -350.0f), 4);

	// 背景の生成
	m_pGameBg = CGameBg::Create(CManager::GetInstance()->GetCamera()->GetPosR(), CGameBg::TEX_TUTORIAL, 0);

	// サウンドの再生
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_TUTORIAL);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CTutorial::Uninit(void)
{
	// 全てのオブジェクトの破棄
	CObject::ReleaseAll();
}

//===============================================
// 更新処理
//===============================================
void CTutorial::Update(void)
{
	m_nTime++;	// 時間をカウント

	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_BACKSPACE) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_BACK, 0) == true)
	{
		if (m_bFade == false)
		{// フェードバグ防止
			CRenderer::GetFade()->Set(CScene::MODE_GAME);	// ゲーム画面へ移行
			m_bFade = true;
		}
	}
}

//===============================================
// 描画処理
//===============================================
void CTutorial::Draw(void)
{
	
}