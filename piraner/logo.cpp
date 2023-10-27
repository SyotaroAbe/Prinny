//=========================================================
//
// タイトルロゴ描画処理 [logo.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "logo.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

//===============================================
// マクロ定義
//===============================================
#define TEXTURE	"data\\TEXTURE\\logo001.png"	// UIのテクスチャ
#define SIZE_X		(358.0f)					// 横サイズ
#define SIZE_Y		(76.0f)						// 縦サイズ

//===============================================
// 静的メンバ変数
//===============================================
int CLogo::m_IdxTexture = 0;				// 使用するテクスチャの番号

//===============================================
// コンストラクタ
//===============================================
CLogo::CLogo() : CObject2D(0)
{
	// 値のクリア
	m_bEndAnim = false;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CLogo::CLogo(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_bEndAnim = false;
}

//===============================================
// デストラクタ
//===============================================
CLogo::~CLogo()
{
	
}

//===============================================
// 生成処理
//===============================================
CLogo *CLogo::Create(int nPriority)
{
	CLogo *pUi;

	// 背景の生成
	pUi = new CLogo(nPriority);

	// 種類の設定
	pUi->SetType(TYPE_BG);

	// サイズの設定
	pUi->SetSize(SIZE_X, SIZE_Y);

	// 初期化処理
	pUi->Init(D3DXVECTOR3(620.0f, 180.0f, 0.0f));

	// テクスチャの割り当て
	pUi->BindTexture(m_IdxTexture);

	return pUi;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CLogo::Init(D3DXVECTOR3 pos)
{
	// オブジェクト2Dの初期化処理
	CObject2D::Init(pos);

	// テクスチャの設定
	m_IdxTexture = CManager::GetInstance()->GetTexture()->Regist(TEXTURE);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CLogo::Uninit(void)
{
	// オブジェクト2Dの終了処理
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CLogo::Update(void)
{
	// 位置補正処理
	if (CObject2D::RevisionPos(D3DXVECTOR3(620.0f, 250.0f, 0.0f), 0.02f) == true)
	{// 位置補正終了
		m_bEndAnim = true;		// アニメーション終了
	}

	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_RETURN) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_A, 0) == true)
	{// ENTER入力
		// 位置設定処理
		CObject2D::SetPos(D3DXVECTOR3(620.0f, 250.0f, 0.0f));
	}
}

//===============================================
// 描画処理
//===============================================
void CLogo::Draw(void)
{
	// オブジェクト2Dの描画処理
	CObject2D::Draw();
}