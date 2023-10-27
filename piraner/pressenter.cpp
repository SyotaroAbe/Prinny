//=========================================================
//
// PressEnter描画処理 [pressenter.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "pressenter.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "title.h"

//===============================================
// マクロ定義
//===============================================
#define TEXTURE			"data\\TEXTURE\\PRESS_ENTER_OR_A.png"	// UIのテクスチャ
#define BRIGHTNESS		(0.02f)									// 明るさの値
#define FADE_BRIGHTNESS	(0.4f)									// フェード時の点滅
#define SIZE_X			(325.0f)								// 横サイズ
#define SIZE_Y			(50.0f)									// 縦サイズ

//===============================================
// 静的メンバ変数
//===============================================
int CPressEnter::m_IdxTexture = 0;				// 使用するテクスチャの番号

//===============================================
// コンストラクタ
//===============================================
CPressEnter::CPressEnter() : CObject2D(0)
{
	// 値のクリア
	m_fBrightness = 0.0f;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CPressEnter::CPressEnter(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_fBrightness = 0.0f;
}

//===============================================
// デストラクタ
//===============================================
CPressEnter::~CPressEnter()
{
	
}

//===============================================
// 生成処理
//===============================================
CPressEnter *CPressEnter::Create(int nPriority)
{
	CPressEnter *pUi;

	// 背景の生成
	pUi = new CPressEnter(nPriority);

	// 種類の設定
	pUi->SetType(TYPE_BG);

	// サイズの設定
	pUi->SetSize(SIZE_X, SIZE_Y);

	// 初期化処理
	pUi->Init(D3DXVECTOR3(620.0f, 550.0f, 0.0f));

	// テクスチャの割り当て
	pUi->BindTexture(m_IdxTexture);

	return pUi;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CPressEnter::Init(D3DXVECTOR3 pos)
{
	// オブジェクト2Dの初期化処理
	CObject2D::Init(pos);

	// テクスチャの設定
	m_IdxTexture = CManager::GetInstance()->GetTexture()->Regist(TEXTURE);

	// 値の初期化
	m_fBrightness = BRIGHTNESS;

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CPressEnter::Uninit(void)
{
	// オブジェクト2Dの終了処理
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CPressEnter::Update(void)
{
	// 点滅処理
	CObject2D::Brightness(m_fBrightness);
}

//===============================================
// 描画処理
//===============================================
void CPressEnter::Draw(void)
{
	// オブジェクト2Dの描画処理
	CObject2D::Draw();
}

//===============================================
// 描画処理
//===============================================
void CPressEnter::SetBrightnessFade(bool bFade)
{
	if (bFade == false)
	{
		m_fBrightness = BRIGHTNESS;			// ゆっくり点滅
	}
	else
	{
		m_fBrightness = FADE_BRIGHTNESS;	// 素早く点滅
	}
}