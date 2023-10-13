//=========================================================
//
// Xファイル管理処理 [xfile.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "xfile.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//===============================================
// マクロ定義
//===============================================
#define MIN_VTX				(900.0f)	// vtx最小値初期値
#define MAX_VTX				(-900.0f)	// vtx最大値初期値

//===============================================
// 静的メンバ変数
//===============================================
int CXFile::m_nNumAll = 0;								// Xファイルの総数
const char *CXFile::m_apFileName[MAX_XFILE] = {};		// Xファイルファイル名

//===============================================
// コンストラクタ
//===============================================
CXFile::CXFile()
{
	// 値をクリアする
	for (int nCntXFile = 0; nCntXFile < MAX_XFILE; nCntXFile++)
	{
		m_aXFile[nCntXFile].m_dwNumMat = 0;
		m_aXFile[nCntXFile].m_pBuffMat = NULL;
		m_aXFile[nCntXFile].m_pIdxTexture = NULL;
		m_aXFile[nCntXFile].m_pMesh = NULL;
		m_aXFile[nCntXFile].m_pTexture = NULL;
		m_aXFile[nCntXFile].m_vtxMin = D3DXVECTOR3(MIN_VTX, MIN_VTX, MIN_VTX);
		m_aXFile[nCntXFile].m_vtxMax = D3DXVECTOR3(MAX_VTX, MAX_VTX, MAX_VTX);
	}
	m_nIdx = 0;
	m_col = COL_NORMAL;
}

//===============================================
// デストラクタ
//===============================================
CXFile::~CXFile()
{

}

//===============================================
// モデルの生成（読み込み）
//===============================================
HRESULT CXFile::Load(void)
{
	return S_OK;
}

//===============================================
// Xファイルの破棄
//===============================================
void CXFile::Unload(void)
{
	// メッシュの破棄
	for (int nCntMesh = 0; nCntMesh < MAX_XFILE; nCntMesh++)
	{
		if (m_aXFile[nCntMesh].m_pMesh != NULL)
		{// 使用されている
			m_aXFile[nCntMesh].m_pMesh->Release();
			m_aXFile[nCntMesh].m_pMesh = NULL;
		}
	}

	// マテリアルの破棄
	for (int nCntBuff = 0; nCntBuff < MAX_XFILE; nCntBuff++)
	{
		if (m_aXFile[nCntBuff].m_pBuffMat != NULL)
		{// 使用されている
			m_aXFile[nCntBuff].m_pBuffMat->Release();
			m_aXFile[nCntBuff].m_pBuffMat = NULL;
		}
	}

	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < MAX_XFILE; nCntTexture++)
	{
		if (m_aXFile[nCntTexture].m_pTexture != NULL)
		{// 使用されている
			// メモリの開放
			delete[] m_aXFile[nCntTexture].m_pTexture;
			m_aXFile[nCntTexture].m_pTexture = NULL;
		}
	}

	// テクスチャ番号のポインタの破棄
	for (int nCntIdTexture = 0; nCntIdTexture < MAX_XFILE; nCntIdTexture++)
	{
		if (m_aXFile[nCntIdTexture].m_pIdxTexture != NULL)
		{// 使用されている
			// メモリの開放
			delete[] m_aXFile[nCntIdTexture].m_pIdxTexture;
			m_aXFile[nCntIdTexture].m_pIdxTexture = NULL;
		}
	}
}

//===============================================
// Xファイルの読み込み
//===============================================
int CXFile::Regist(const char *pFilename)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();   // デバイスの取得

	for (int nCntXFile = 0; nCntXFile < m_nNumAll; nCntXFile++)
	{
		if (m_apFileName[nCntXFile] == pFilename)
		{// 既に読み込まれたXファイルファイル
			return nCntXFile;		// 同じ番号で返す
		}
	}

	for (int nCntXFile = 0; nCntXFile < MAX_XFILE; nCntXFile++)
	{
		if (m_apFileName[nCntXFile] == NULL)
		{// 使用されていない
			m_apFileName[nCntXFile] = pFilename;

			// Ｘファイルに読み込み
			D3DXLoadMeshFromX(m_apFileName[nCntXFile],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_aXFile[nCntXFile].m_pBuffMat,
				NULL,
				&m_aXFile[nCntXFile].m_dwNumMat,
				&m_aXFile[nCntXFile].m_pMesh);

			// Xファイルの割り当て
			BindX(nCntXFile);

			m_nNumAll++;
			return nCntXFile;
		}
	}

	return -1;
}

//===============================================
// Xファイルの割り当て
//===============================================
void CXFile::BindX(int nIdx)
{
	// Vtxサイズ設定
	SetVtxSize(nIdx);

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATERIAL *pMat;		// マテリアルへのポインタ

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_aXFile[nIdx].m_pBuffMat->GetBufferPointer();

	m_aXFile[nIdx].m_pTexture = new LPDIRECT3DTEXTURE9[m_aXFile[nIdx].m_dwNumMat];
	m_aXFile[nIdx].m_pIdxTexture = new int[m_aXFile[nIdx].m_dwNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_aXFile[nIdx].m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// テクスチャファイルが存在する
			// テクスチャの設定
			m_aXFile[nIdx].m_pIdxTexture[nCntMat] = CManager::GetTexture()->Regist(pMat[nCntMat].pTextureFilename);
			m_aXFile[nIdx].m_pTexture[nCntMat] = CManager::GetTexture()->GetAddress(m_aXFile[nIdx].m_pIdxTexture[nCntMat]);
		}
		else
		{// 存在しない
			m_aXFile[nIdx].m_pIdxTexture[nCntMat] = NULL;
			m_aXFile[nIdx].m_pTexture[nCntMat] = NULL;
		}
	}
}

//===============================================
// Vtxサイズ設定
//===============================================
void CXFile::SetVtxSize(int nIdx)
{
	int nNumVtx;		// 頂点数
	DWORD dwSizeFVF;	// 頂点フォーマットのサイズ
	BYTE * pVtxBuff;	// 頂点バッファへのポインタ

	// 頂点数を取得
	nNumVtx = m_aXFile[nIdx].m_pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(m_aXFile[nIdx].m_pMesh->GetFVF());

	// 頂点バッファをロック
	m_aXFile[nIdx].m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

		// 全ての頂点を比較してモデルの最小値・最大値を抜き出す
		// 最小値の比較
		if (vtx.x <= m_aXFile[nIdx].m_vtxMin.x)
		{
			m_aXFile[nIdx].m_vtxMin.x = vtx.x;
		}

		if (vtx.y <= m_aXFile[nIdx].m_vtxMin.y)
		{
			m_aXFile[nIdx].m_vtxMin.y = vtx.y;
		}

		if (vtx.z <= m_aXFile[nIdx].m_vtxMin.z)
		{
			m_aXFile[nIdx].m_vtxMin.z = vtx.z;
		}

		// 最大値の比較
		if (vtx.x >= m_aXFile[nIdx].m_vtxMax.x)
		{
			m_aXFile[nIdx].m_vtxMax.x = vtx.x;
		}

		if (vtx.y >= m_aXFile[nIdx].m_vtxMax.y)
		{
			m_aXFile[nIdx].m_vtxMax.y = vtx.y;
		}

		if (vtx.z >= m_aXFile[nIdx].m_vtxMax.z)
		{
			m_aXFile[nIdx].m_vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファをアンロック
	m_aXFile[nIdx].m_pMesh->UnlockVertexBuffer();
}

//===============================================
// 描画処理
//===============================================
void CXFile::Draw(int nIdx, COL col)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	D3DMATERIAL9 matDef;												// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;													// マテリアルデータ

	m_col = col;		// 色情報を代入

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_aXFile[nIdx].m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_aXFile[nIdx].m_dwNumMat; nCntMat++)
	{
		// マテリアルを保存
		D3DXMATERIAL pmat = pMat[nCntMat];

		// 色情報別に色を設定
		switch (m_col)
		{
		case COL_NORMAL:		// 通常
			break;

		case COL_RED:			// 赤
			pmat.MatD3D.Diffuse.r = 1.0f;
			pmat.MatD3D.Diffuse.g = 0.0f;
			pmat.MatD3D.Diffuse.b = 0.0f;
			break;

		case COL_GREEN:			// 緑
			pmat.MatD3D.Diffuse.r = 0.0f;
			pmat.MatD3D.Diffuse.g = 1.0f;
			pmat.MatD3D.Diffuse.b = 0.0f;
			break;

		case COL_BLUE:			// 青
			pmat.MatD3D.Diffuse.r = 0.0f;
			pmat.MatD3D.Diffuse.g = 0.0f;
			pmat.MatD3D.Diffuse.b = 1.0f;
			break;

		case COL_YELLOW:		// 黄
			pmat.MatD3D.Diffuse.r = 0.9f;
			pmat.MatD3D.Diffuse.g = 0.75f;
			pmat.MatD3D.Diffuse.b = 0.2f;
			break;

		case COL_ORANGE:		// 橙
			pmat.MatD3D.Diffuse.r = 0.9f;
			pmat.MatD3D.Diffuse.g = 0.5f;
			pmat.MatD3D.Diffuse.b = 0.2f;
			break;

		case COL_LIGHTBLUE:		// 水色
			pmat.MatD3D.Diffuse.r = 0.4f;
			pmat.MatD3D.Diffuse.g = 0.9f;
			pmat.MatD3D.Diffuse.b = 1.0f;
			break;

		case COL_YELLOWGREEN:	// 黄緑
			pmat.MatD3D.Diffuse.r = 0.2f;
			pmat.MatD3D.Diffuse.g = 1.0f;
			pmat.MatD3D.Diffuse.b = 0.2f;
			break;
		}

		// マテリアルの設定
		pDevice->SetMaterial(&pmat.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, m_aXFile[nIdx].m_pTexture[nCntMat]);

		// モデル（パーツ）の描画
		m_aXFile[nIdx].m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//===============================================
// 透明描画処理
//===============================================
void CXFile::CleannessDraw(int nIdx)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	D3DMATERIAL9 matDef;												// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;													// マテリアルデータ
	D3DMATERIAL9 mat[256];

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_aXFile[nIdx].m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_aXFile[nIdx].m_dwNumMat; nCntMat++)
	{
		// マテリアルを保存
		mat[nCntMat] = pMat[nCntMat].MatD3D;
		mat[nCntMat].Diffuse.a = 0.5f;
		mat[nCntMat].Ambient.a = 0.5f;

		// マテリアルの設定
		pDevice->SetMaterial(&mat[nCntMat]);

		// テクスチャの設定
		pDevice->SetTexture(0, m_aXFile[nIdx].m_pTexture[nCntMat]);

		// モデル（パーツ）の描画
		m_aXFile[nIdx].m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}