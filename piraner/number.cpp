//=========================================================
//
// ナンバー処理 [number.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "score.h"
#include "time.h"
#include "object.h"

//===============================================
// コンストラクタ
//===============================================
CNumber::CNumber()
{
	// 値をクリアする
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CNumber::CNumber(int nPriority)
{
	// 値をクリアする
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//===============================================
// デストラクタ
//===============================================
CNumber::~CNumber()
{
	
}

//===============================================
// 初期化処理
//===============================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nPriority)
{
	m_pObject2D = new CObject2D(nPriority);

	m_pObject2D->SetSize(fSizeX, fSizeY);

	// オブジェクト2Dの初期化処理
	m_pObject2D->Init(pos);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CNumber::Uninit(void)
{
	// オブジェクト2Dの終了処理
	m_pObject2D->Uninit();
	m_pObject2D = NULL;
}

//===============================================
// 更新処理
//===============================================
void CNumber::Update(void)
{
	
}

//===============================================
// 描画処理
//===============================================
void CNumber::Draw(void)
{
	// オブジェクト2Dの描画処理
	//m_pObject2D->Draw();
}

//===============================================
// テクスチャの割り当て
//===============================================
void CNumber::BindTexture(int nIdx)
{
	m_pObject2D->BindTexture(nIdx);
}

//===============================================
// ナンバーの種類の設定
//===============================================
void CNumber::SetType(const TYPE type)
{
	m_type = type;
}

//===============================================
// 設定
//===============================================
void CNumber::Set(const int nNumber)
{
	m_pObject2D->SetNumber(nNumber);
}

//===============================================
// 位置設定
//===============================================
void CNumber::SetPos(const D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	m_pObject2D->UpdatePos(pos, fSizeX, fSizeY);
}

//===============================================
// 色の設定
//===============================================
void CNumber::SetCol(const D3DXCOLOR col)
{
	// 色の設定処理
	m_pObject2D->SetCol(col);
}