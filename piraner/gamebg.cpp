//=========================================================
//
// メインモード背景処理 [gamebg.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "gamebg.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "objectBillboard.h"
#include "texture.h"
#include "camera.h"

//===============================================
// マクロ定義
//===============================================
#define GAMEBG_SIZEX		(8.0f)			// 幅（半分）
#define GAMEBG_SIZEY		(8.0f)			// 高さ（半分）

//===============================================
// 静的メンバ変数
//===============================================
int CGameBg::m_nIdxTexture = 0;						// 使用するテクスチャの番号

//===============================================
// コンストラクタ
//===============================================
CGameBg::CGameBg() : CObjectBillboard(2)
{
	// 値をクリア
	m_tex = TEX_GAME;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CGameBg::CGameBg(int nPriority) : CObjectBillboard(nPriority)
{
	// 値をクリア
	m_tex = TEX_GAME;
}

//===============================================
// デストラクタ
//===============================================
CGameBg::~CGameBg()
{
	
}

//===============================================
// 生成処理
//===============================================
CGameBg *CGameBg::Create(D3DXVECTOR3 pos, ETex tex, int nPriority)
{
	CGameBg *pGameBg;

	// 弾の生成
	pGameBg = new CGameBg(2);

	// 種類の設定
	pGameBg->SetType(TYPE_BG);

	// 初期化処理
	pGameBg->Init(pos, tex);

	// テクスチャの割り当て
	pGameBg->BindTexture(m_nIdxTexture);

	return pGameBg;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CGameBg::Init(D3DXVECTOR3 pos, ETex tex)
{
	m_tex = tex;

	if (m_tex == TEX_TUTORIAL)
	{
		m_nIdxTexture = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\tutorial000.png");			// テクスチャの設定
		SetSize(SCREEN_WIDTH * 0.45f, SCREEN_HEIGHT * 0.45f);														// サイズの設定

		// オブジェクト2Dの初期化処理
		CObjectBillboard::Init(D3DXVECTOR3(pos.x, pos.y + 100.0f, pos.z));
	}
	else
	{
		m_nIdxTexture = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\game000.png");				// テクスチャの設定
		SetSize(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.6f);														// サイズの設定
	
		// オブジェクト2Dの初期化処理
		CObjectBillboard::Init(D3DXVECTOR3(pos.x, pos.y, pos.z));
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CGameBg::Uninit(void)
{
	// オブジェクト2Dの終了処理
	CObjectBillboard::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CGameBg::Update(void)
{
	// 位置をカメラの注視点に設定
	if (m_tex != TEX_TUTORIAL)
	{
		CObjectBillboard::SetPos(D3DXVECTOR3(m_pos.x, CManager::GetInstance()->GetCamera()->GetPosR().y, CManager::GetInstance()->GetCamera()->GetPosR().z));
	}
}

//===============================================
// 描画処理
//===============================================
void CGameBg::Draw(void)
{
	CObjectBillboard::Draw();
}

//===============================================
// 設定処理
//===============================================
void CGameBg::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move)
{
	m_pos = pos;
	m_move = move;

	//// サウンドの再生
	//CManager::GetSound()->Play(CSound::LABEL_SE_SHOT);
}