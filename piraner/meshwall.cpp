//=========================================================
//
// メッシュウォール描画処理 [meshwall.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "meshwall.h"
#include "bullet.h"
#include "camera.h"

//===============================================
// 静的メンバ変数
//===============================================
int CMeshwall::m_nIdxTexture = NULL;				// 使用するテクスチャの番号
LPDIRECT3DTEXTURE9 CMeshwall::m_pTexture = NULL;	// テクスチャへのポインタ

//===============================================
// コンストラクタ
//===============================================
CMeshwall::CMeshwall() : CObjectMesh(1)
{
	//値をクリアする
	
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CMeshwall::CMeshwall(int nPriority) : CObjectMesh(nPriority)
{
	//値をクリアする
}

//===============================================
// デストラクタ
//===============================================
CMeshwall::~CMeshwall()
{

}

//===============================================
// 生成処理
//===============================================
CMeshwall *CMeshwall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSizeX, float fSizeZ, int nPriority)
{
	CMeshwall *pObjectMesh;

	// メッシュフィールドの生成
	pObjectMesh = new CMeshwall(nPriority);

	// 種類の設定
	pObjectMesh->SetType(TYPE_FIELD);

	// サイズの設定
	pObjectMesh->SetSize(fSizeX, fSizeZ);

	// 初期化処理
	pObjectMesh->Init(pos);

	// 設定処理
	//pObjectMesh->Set(pos, rot);

	// テクスチャの割り当て
	pObjectMesh->BindTexture(m_nIdxTexture);

	return pObjectMesh;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CMeshwall::Init(D3DXVECTOR3 pos)
{
	// オブジェクトメッシュの初期化処理
	CObjectMesh::Init(pos);

	//m_PosMin = D3DXVECTOR3(MIN_FIELDPOS, MIN_FIELDPOS, MIN_FIELDPOS);
	//m_PosMax = D3DXVECTOR3(MAX_FIELDPOS, MAX_FIELDPOS, MAX_FIELDPOS);

	// 床の位置の最小値を比較
	if (m_pos.x <= m_PosMin.x)
	{// 最小値を更新
		m_PosMin.x = m_pos.x;
	}

	if (m_pos.z <= m_PosMin.z)
	{// 最小値を更新
		m_PosMin.z = m_pos.z;
	}

	// 床の位置の最大値を比較
	if (m_pos.x >= m_PosMax.x)
	{// 最大値を更新
		m_PosMax.x = m_pos.x;
	}

	if (m_pos.z >= m_PosMax.z)
	{// 最大値を更新
		m_PosMax.z = m_pos.z;
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CMeshwall::Uninit(void)
{
	// オブジェクトメッシュの終了処理
	CObjectMesh::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CMeshwall::Update(void)
{

}

//===============================================
// 描画処理
//===============================================
void CMeshwall::Draw(void)
{
	// オブジェクトメッシュの描画処理
	CObjectMesh::Draw();
}

//===============================================
// 設定処理
//===============================================
void CMeshwall::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int type)
{
	m_pos = pos;
	m_rot = rot;

	srand(timeGetTime());										// randの値を初期化
	int nNumXRand = 1;											// randの値を取得（数X）
	int nNumYRand = rand() % 2 + 1;								// randの値を取得（数Y）
}