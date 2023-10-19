//=========================================================
//
// オブジェクトメッシュ処理 [objectMesh.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "objectMesh.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_MESH			(9)			// 最大メッシュ数
#define INDEXMESH_BLOCK_X	(2)			// メッシュフィールドのブロック数（X）
#define INDEXMESH_SIZE_X	(200.0f)	// メッシュフィールドのサイズ（X）

//===============================================
// 静的メンバ変数
//===============================================
int CObjectMesh::m_nIdxTexture = 0;						// 使用するテクスチャの番号

//===============================================
// コンストラクタ
//===============================================
CObjectMesh::CObjectMesh() : CObject(3)
{
	// 値をクリアする
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pldxBuff = NULL;
	m_mtxWorld;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntMeshX = 0;
	m_nCntMeshY = 0;
	m_fSizeX = 0.0f;
	m_fSizeZ = 0.0f;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CObjectMesh::CObjectMesh(int nPriority) : CObject(nPriority)
{
	// 値をクリアする
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pldxBuff = NULL;
	m_mtxWorld;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntMeshX = 0;
	m_nCntMeshY = 0;
	m_fSizeX = 0.0f;
	m_fSizeZ = 0.0f;
}

//===============================================
// デストラクタ
//===============================================
CObjectMesh::~CObjectMesh()
{

}

//===============================================
// 生成処理
//===============================================
CObjectMesh *CObjectMesh::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeZ, int nPriority)
{
	CObjectMesh *pObjectMesh;

	// プレイヤーの生成
	pObjectMesh = new CObjectMesh(nPriority);

	// 種類の設定
	pObjectMesh->SetType(TYPE_FIELD);

	// サイズの設定
	pObjectMesh->SetSize(fSizeX, fSizeZ);

	// 初期化処理
	pObjectMesh->Init(pos);

	// テクスチャの設定
	m_nIdxTexture = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\field000.jpg");

	// テクスチャの割り当て
	pObjectMesh->BindTexture(m_nIdxTexture);

	return pObjectMesh;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CObjectMesh::Init(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスの取得
	int nMaxMesh = 9;

	// 変数の初期化
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nMaxMesh,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D * pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	//for (int nCnt = 0; nCnt < m_nCntMeshX; nCnt++)
	//{
	//	for (int nCntMesh = 0; nCntMesh < m_nCntMeshY; nCntMesh++)
	//	{
	//		pVtx[nCnt * nCntMesh].pos = D3DXVECTOR3(-m_nCntMeshX + m_fSizeX * nCnt, 0.0f, m_nCntMeshY - m_fSizeZ * nCntMesh);
	//	}
	//}

	pVtx[0].pos = D3DXVECTOR3(-m_fSizeX, 0.0f, m_fSizeZ);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, m_fSizeZ);
	pVtx[2].pos = D3DXVECTOR3(m_fSizeX, 0.0f, m_fSizeZ);
	pVtx[3].pos = D3DXVECTOR3(-m_fSizeX, 0.0f, 0.0f);
	pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(m_fSizeX, 0.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(-m_fSizeX, 0.0f, -m_fSizeZ);
	pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, -m_fSizeZ);
	pVtx[8].pos = D3DXVECTOR3(m_fSizeX, 0.0f, -m_fSizeZ);

	// 法線ベクトルの設定
	for (int nCntMesh = 0; nCntMesh < nMaxMesh; nCntMesh++)
	{
		pVtx[nCntMesh].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	// 頂点カラーの設定
	for (int nCntMesh = 0; nCntMesh < nMaxMesh; nCntMesh++)
	{
		pVtx[nCntMesh].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// テクスチャ座標の設定
	//for (nCntMesh = 0; nCntMesh < nMaxMesh; nCntMesh++)
	//{

	//}
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pldxBuff,
		NULL);

	WORD * pldx;	// インデックス情報へのポインタ

	// インデックスバッファをロックし、頂点番号データへのポインタを取得
	m_pldxBuff->Lock(0, 0, (void**)&pldx, 0);

	// 頂点番号データの設定
	pldx[0] = 3;
	pldx[1] = 0;
	pldx[2] = 4;
	pldx[3] = 1;
	pldx[4] = 5;
	pldx[5] = 2;
	pldx[6] = 2;
	pldx[7] = 6;
	pldx[8] = 6;
	pldx[9] = 3;
	pldx[10] = 7;
	pldx[11] = 4;
	pldx[12] = 8;
	pldx[13] = 5;

	// インデックスバッファをアンロックする
	m_pldxBuff->Unlock();

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CObjectMesh::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if (m_pldxBuff != NULL)
	{
		m_pldxBuff->Release();
		m_pldxBuff = NULL;
	}

	this->Release();
}

//===============================================
// 更新処理
//===============================================
void CObjectMesh::Update(void)
{
	// 前回の位置を保存
	m_posOld = m_pos;
}

//===============================================
// 描画処理
//===============================================
void CObjectMesh::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;										// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(m_pldxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		9,
		0,
		12);
}

//===============================================
// テクスチャの割り当て
//===============================================
void CObjectMesh::BindTexture(int nIdx)
{
	m_pTexture = CManager::GetInstance()->GetTexture()->GetAddress(nIdx);
}

//===============================================
// 位置設定
//===============================================
void CObjectMesh::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===============================================
// 移動量設定
//===============================================
void CObjectMesh::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//===============================================
// サイズの設定処理
//===============================================
void CObjectMesh::SetSize(const float fSizeX, const float fSizeZ)
{
	m_fSizeX = fSizeX;
	m_fSizeZ = fSizeZ;
}

//===============================================
// 色の設定処理
//===============================================
void CObjectMesh::SetCol(const COL col)
{
	VERTEX_3D * pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 指定した色へ変更
	switch (col)
	{
	case COL_NORMAL:	// 通常
		// 頂点カラーの設定
		for (int nCntMesh = 0; nCntMesh < MAX_MESH; nCntMesh++)
		{
			pVtx[nCntMesh].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		break;

	case COL_RED:		// 赤
		// 頂点カラーの設定
		for (int nCntMesh = 0; nCntMesh < MAX_MESH; nCntMesh++)
		{
			pVtx[nCntMesh].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		break;

	case COL_BLUE:		// 青
		// 頂点カラーの設定
		for (int nCntMesh = 0; nCntMesh < MAX_MESH; nCntMesh++)
		{
			pVtx[nCntMesh].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		}
		break;

	case COL_GREEN:		//緑
		// 頂点カラーの設定
		for (int nCntMesh = 0; nCntMesh < MAX_MESH; nCntMesh++)
		{
			pVtx[nCntMesh].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		}
		break;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===============================================
// サイズの設定処理
//===============================================
void CObjectMesh::SetSize(D3DXVECTOR3 size)
{
	m_fSizeX = size.x;
	m_fSizeZ = size.z;
}

//===============================================
// サイズの設定処理
//===============================================
void CObjectMesh::SetSizeMin(D3DXVECTOR3 size)
{
	m_fSizeX = size.x;
	m_fSizeZ = size.z;
}