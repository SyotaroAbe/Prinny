//=========================================================
//
// モデル描画処理 [model.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "objectX.h"

//===============================================
// マクロ定義
//===============================================
#define MIN_VTX				(900.0f)	// vtx最小値初期値
#define MAX_VTX				(-900.0f)	// vtx最大値初期値

//===============================================
// コンストラクタ
//===============================================
CModel::CModel()
{
	//値をクリアする
	m_apTexture = NULL;
	m_apIdxTexture = 0;
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_dwNumMat = 0;
	m_mtxWorld;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOri = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_vtxMax = D3DXVECTOR3(MIN_VTX, MIN_VTX, MIN_VTX);
	m_vtxMax = D3DXVECTOR3(MAX_VTX, MAX_VTX, MAX_VTX);
	m_pParent = NULL;
}

//===============================================
// デストラクタ
//===============================================
CModel::~CModel()
{

}

//===============================================
// 生成処理
//===============================================
CModel *CModel::Create(const char *pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CModel *pModel;

	// プレイヤーの生成
	pModel = new CModel();

	// 初期化処理
	pModel->Init(pFilename, pos, rot);

	return pModel;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CModel::Init(const char *pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();   // デバイスの取得

	// Ｘファイルに読み込み
	D3DXLoadMeshFromX(pFilename,
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&m_pBuffMat,
						NULL,
						&m_dwNumMat,
						&m_pMesh);

	// Xファイルに割り当て
	BindX(m_pMesh, m_pBuffMat, m_dwNumMat);

	// 位置の設定
	m_pos = pos;
	m_posOri = pos;

	// 向きの設定
	m_rot = rot;

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CModel::Uninit(void)
{
	// メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアルの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	if (m_apTexture != NULL)
	{// 使用されている
		// メモリの開放
		delete[] m_apTexture;
		m_apTexture = NULL;
	}

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (m_apIdxTexture[nCntMat] != NULL)
		{// テクスチャファイルが存在する
			// テクスチャの破棄
			CManager::GetInstance()->GetTexture()->Delete(m_apIdxTexture[nCntMat]);
		}
	}

	if (m_apIdxTexture != NULL)
	{// 使用されている
		// メモリの開放
		delete[] m_apIdxTexture;
		m_apIdxTexture = NULL;
	}
}

//===============================================
// 更新処理
//===============================================
void CModel::Update(void)
{
	
}

//===============================================
// 描画処理
//===============================================
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;														// 計算用マトリックス
	D3DXMATRIX mtxParent;																// 親のマトリックス
	//D3DXMATRIX mtxPlayer = CManager::GetPlayer()->GetMtxWorld();						// プレイヤーのマトリックス
	D3DMATERIAL9 matDef;																// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;																	// マテリアルデータ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// パーツの「親マトリックス」を設定
	if (m_pParent != NULL)
	{// 親モデルがある場合
		mtxParent = m_pParent->m_mtxWorld;
	}
	else
	{// 親モデルがない場合
		//mtxParent = mtxPlayer;
		// 現在（最新）のマトリックスを取得する [ = プレイヤーのマトリックス ]
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, m_apTexture[nCntMat]);

		// モデル（パーツ）の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//===============================================
// 影の描画処理
//===============================================
void CModel::DrawShadowmtx(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX mtxShadow;		// シャドウマトリックス
	D3DLIGHT9 light;			// ライト情報
	D3DXVECTOR4 posLight;		// ライトの位置
	D3DXVECTOR3 pos, normal;	// 平面上の任意の点、法線ベクトル
	D3DXPLANE plane;			// 平面情報
	D3DMATERIAL9 matDef;		// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;			// マテリアルデータ

	// ライトの位置を設定
	pDevice->GetLight(0, &light);
	posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

	// 平面情報を作成
	pos = D3DXVECTOR3(m_pos.x, 0.5f, m_pos.z);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	// シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	// シャドウマトリックスの作成
	D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// シャドウマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// モデルの色を保存
		D3DXCOLOR DiffuseModel = pMat[nCntMat].MatD3D.Diffuse;

		// モデルのカラー変更
		pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, m_apTexture[nCntMat]);

		// モデル（パーツ）の描画
		m_pMesh->DrawSubset(nCntMat);

		// 保存していた色を戻す
		pMat[nCntMat].MatD3D.Diffuse = DiffuseModel;
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//===============================================
// Xファイルの割り当て
//===============================================
void CModel::BindX(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat)
{
	m_pMesh = pMesh;
	m_pBuffMat = pBuffMat;
	m_dwNumMat = dwNumMat;

	// Vtxサイズ設定
	SetVtxSize();

	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATERIAL *pMat;																	// マテリアルへのポインタ

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	m_apTexture = new LPDIRECT3DTEXTURE9[m_dwNumMat];
	m_apIdxTexture = new int[m_dwNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// テクスチャファイルが存在する
			// テクスチャの設定
			m_apIdxTexture[nCntMat] = CManager::GetInstance()->GetTexture()->Regist(pMat[nCntMat].pTextureFilename);

			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&m_apTexture[nCntMat]);

			//m_apTexture[nCntMat] = CManager::GetTexture()->GetAddress(m_apIdxTexture[nCntMat]);
		}
		else
		{// 存在しない
			m_apIdxTexture[nCntMat] = NULL;
			m_apTexture[nCntMat] = NULL;
		}
	}
}

//===============================================
// Vtxサイズ設定
//===============================================
void CModel::SetVtxSize(void)
{
	int nNumVtx;		// 頂点数
	DWORD dwSizeFVF;	// 頂点フォーマットのサイズ
	BYTE * pVtxBuff;	// 頂点バッファへのポインタ

	// 頂点数を取得
	nNumVtx = m_pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//頂点座標の代入

		// 全ての頂点を比較してモデルの最小値・最大値を抜き出す
		// 最小値の比較
		if (vtx.x <= m_vtxMin.x)
		{
			m_vtxMin.x = vtx.x;
		}

		if (vtx.y <= m_vtxMin.y)
		{
			m_vtxMin.y = vtx.y;
		}

		if (vtx.z <= m_vtxMin.z)
		{
			m_vtxMin.z = vtx.z;
		}

		// 最大値の比較
		if (vtx.x >= m_vtxMax.x)
		{
			m_vtxMax.x = vtx.x;
		}

		if (vtx.y >= m_vtxMax.y)
		{
			m_vtxMax.y = vtx.y;
		}

		if (vtx.z >= m_vtxMax.z)
		{
			m_vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();
}

//===============================================
// 位置設定
//===============================================
void CModel::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===============================================
// 向き設定
//===============================================
void CModel::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//===============================================
// 色設定
//===============================================
void CModel::SetCol(D3DXCOLOR col)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;														// 計算用マトリックス
	D3DXMATRIX mtxParent;																// 親のマトリックス
	D3DMATERIAL9 matDef;																// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;																	// マテリアルデータ

	m_col = col;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// パーツの「親マトリックス」を設定
	if (m_pParent != NULL)
	{// 親モデルがある場合
		mtxParent = m_pParent->m_mtxWorld;
	}
	else
	{// 親モデルがない場合
		// 現在（最新）のマトリックスを取得する [ = プレイヤーのマトリックス ]
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// モデルの色を保存
		D3DXCOLOR DiffuseModel = pMat[nCntMat].MatD3D.Diffuse;

		// モデルのカラー変更
		pMat[nCntMat].MatD3D.Diffuse = m_col;

		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, m_apTexture[nCntMat]);

		// モデル（パーツ）の描画
		m_pMesh->DrawSubset(nCntMat);

		// 保存していた色を戻す
		pMat[nCntMat].MatD3D.Diffuse = DiffuseModel;
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//===============================================
// 親設定
//===============================================
void CModel::SetParent(CModel *pModel)
{
	m_pParent = pModel;
}