//=========================================================
//
// 影の描画処理 [shadow.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//===============================================
// 静的メンバ変数
//===============================================
int CShadow::m_nIdxTexture = 0;						// 使用するテクスチャの番号

//===============================================
// コンストラクタ
//===============================================
CShadow::CShadow() : CObject3D(1)
{
	
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority)
{
	
}

//===============================================
// デストラクタ
//===============================================
CShadow::~CShadow()
{

}

//===============================================
// 生成処理
//===============================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, float m_fSizeX, float m_fSizeZ, int nPriority)
{
	CShadow *pShadow;

	// 影の生成
	pShadow = new CShadow(nPriority);

	// 種類の設定
	pShadow->SetType(TYPE_SHADOW);

	// サイズの設定
	pShadow->SetSize(m_fSizeX, m_fSizeZ);

	// 初期化処理
	pShadow->Init(pos);

	// テクスチャの設定
	m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\shadow000.jpg");

	// テクスチャの割り当て
	pShadow->BindTexture(m_nIdxTexture);

	return pShadow;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CShadow::Init(D3DXVECTOR3 pos)
{
	// オブジェクト3Dの初期化処理
	CObject3D::Init(pos);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CShadow::Uninit(void)
{
	// オブジェクト3Dの終了処理
	CObject3D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CShadow::Update(void)
{

}

//===============================================
// 描画処理
//===============================================
void CShadow::Draw(void)
{
	// 影の描画処理
	CObject3D::DrawShadow();
}