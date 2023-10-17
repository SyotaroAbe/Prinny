//=========================================================
//
// 背景処理 [bg.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//===============================================
// マクロ定義
//===============================================
#define TEXTURE_TITLE		"data\\TEXTURE\\title000.png"		// タイトルのテクスチャ
#define TEXTURE_TUTORIAL	"data\\TEXTURE\\tutorial000.png"	// チュートリアルのテクスチャ
#define TEXTURE_RESULT		"data\\TEXTURE\\ranking000.png"		// リザルトのテクスチャ
#define TEX_MOVE	(0.00005f)									// テクスチャの移動量

//===============================================
// 静的メンバ変数
//===============================================
int CBg::m_aIdxTexture[TYPE_MAX] = {};				// 使用するテクスチャの番号

//===============================================
// コンストラクタ
//===============================================
CBg::CBg() : CObject2D(0)
{
	// 値のクリア
	m_bMove = true;
	m_fTexU = 0.0f;
	m_fTexV = 0.0f;

	m_nIdx++;		// 背景数をインクリメント
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CBg::CBg(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_bMove = true;
	m_fTexU = 0.0f;
	m_fTexV = 0.0f;

	m_nIdx++;		// 背景数をインクリメント
}

//===============================================
// デストラクタ
//===============================================
CBg::~CBg()
{
	m_nIdx--;		// 背景数をデクリメント
}

//===============================================
// 生成処理
//===============================================
CBg *CBg::Create(int nIdx, int nPriority)
{
	CBg *pBg;

	// 背景の生成
	pBg = new CBg(nPriority);

	// 種類の設定
	pBg->SetType(TYPE_BG);

	// サイズの設定
	pBg->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

	// 初期化処理
	pBg->Init(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

	// テクスチャの割り当て
	pBg->BindTexture(m_aIdxTexture[nIdx]);

	return pBg;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CBg::Init(D3DXVECTOR3 pos)
{
	// オブジェクト2Dの初期化処理
	CObject2D::Init(pos);

	// テクスチャの設定
	m_aIdxTexture[TYPE_TITLE] = CManager::GetInstance()->GetTexture()->Regist(TEXTURE_TITLE);
	m_aIdxTexture[TYPE_TUTORIAL] = CManager::GetInstance()->GetTexture()->Regist(TEXTURE_TUTORIAL);
	m_aIdxTexture[TYPE_RESULT] = CManager::GetInstance()->GetTexture()->Regist(TEXTURE_RESULT);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CBg::Uninit(void)
{
	// オブジェクト2Dの終了処理
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CBg::Update(void)
{
	if (m_bMove == true)
	{//テクスチャ座標を動かす
		// テクスチャ座標の開始位置の更新
		m_fTexU -= TEX_MOVE * m_nIdx;
		//m_fTexV -= TEX_MOVE * m_nIdx;

		if (m_fTexU < 0.0f)
		{// 0を下回った
			m_fTexU += 1.0f;	// 1へ戻す
		}

		if (m_fTexV < 0.0f)
		{// 0を下回った
			m_fTexV += 1.0f;	// 1へ戻す
		}

		//テクスチャ座標の更新処理
		//CObject2D::UpdateTex(m_fTexU, m_fTexV);
	}
}

//===============================================
// 描画処理
//===============================================
void CBg::Draw(void)
{
	// オブジェクト2Dの描画処理
	CObject2D::Draw();
}

//===============================================
// 設定処理
//===============================================
void CBg::Set(const D3DXVECTOR3 pos, const bool bMove)
{
	m_bMove = bMove;

	// 位置設定
	SetPos(pos);
}