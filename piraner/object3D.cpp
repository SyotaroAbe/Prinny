//=========================================================
//
// オブジェクト3D処理 [object3D.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "object3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//===============================================
// 静的メンバ変数
//===============================================
int CObject3D::m_nIdxTexture = 0;						// 使用するテクスチャの番号

//===============================================
// コンストラクタ
//===============================================
CObject3D::CObject3D() : CObject(3)
{
	// 値をクリアする
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_mtxWorld;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fSizeX = 0.0f;
	m_fSizeZ = 0.0f;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
{
	// 値をクリアする
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_mtxWorld;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fSizeX = 0.0f;
	m_fSizeZ = 0.0f;
}

//===============================================
// デストラクタ
//===============================================
CObject3D::~CObject3D()
{

}

//===============================================
// 生成処理
//===============================================
CObject3D *CObject3D::Create(D3DXVECTOR3 pos, float m_fSizeX, float m_fSizeZ, int nPriority)
{
	CObject3D *pObject3D;

	// プレイヤーの生成
	pObject3D = new CObject3D(nPriority);

	// 種類の設定
	pObject3D->SetType(TYPE_FIELD);

	// サイズの設定
	pObject3D->SetSize(m_fSizeX, m_fSizeZ);

	// 初期化処理
	pObject3D->Init(pos);

	// テクスチャの設定
	m_nIdxTexture = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\field000.jpg");

	// テクスチャの割り当て
	pObject3D->BindTexture(m_nIdxTexture);

	return pObject3D;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CObject3D::Init(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスの取得

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D * pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fSizeX, 0.0f, m_fSizeZ);
	pVtx[1].pos = D3DXVECTOR3(m_fSizeX, 100.0f, m_fSizeZ);
	pVtx[2].pos = D3DXVECTOR3(-m_fSizeX, 100.0f, -m_fSizeZ);
	pVtx[3].pos = D3DXVECTOR3(m_fSizeX, 0.0f, -m_fSizeZ);

	// ベクトルの計算
	m_vec0 = pVtx[1].pos - pVtx[0].pos;
	m_vec1 = pVtx[2].pos - pVtx[1].pos;

	D3DXVECTOR3 Vec0;
	Vec0.x = (m_vec0.z * m_vec1.y) - (m_vec0.y * m_vec1.z);
	Vec0.y = (m_vec0.z * m_vec1.x) - (m_vec0.x * m_vec1.z);
	Vec0.z = (m_vec0.y * m_vec1.x) - (m_vec0.x * m_vec1.y);

	// 正規化する
	float fVec = sqrtf((m_vec0.x * m_vec1.x + m_vec0.y  * m_vec1.y + m_vec0.z * m_vec1.z)
		* (m_vec0.x * m_vec1.x + m_vec0.y  * m_vec1.y + m_vec0.z * m_vec1.z));

	Vec0.x /= fVec;
	Vec0.y /= fVec;
	Vec0.z /= fVec;

	// 法線ベクトルの設定
	pVtx[0].nor = Vec0;
	pVtx[1].nor = Vec0;
	pVtx[2].nor = Vec0;
	pVtx[3].nor = Vec0;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標の代入
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		m_aVtx[nCntVtx] = pVtx[nCntVtx].pos;
	}

	// 法線ベクトルの代入
	m_nor = pVtx[0].nor;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CObject3D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	this->Release();
}

//===============================================
// 更新処理
//===============================================
void CObject3D::Update(void)
{
	
}

//===============================================
// 描画処理
//===============================================
void CObject3D::Draw(void)
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

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//===============================================
// 描画処理
//===============================================
void CObject3D::DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;										// 計算用マトリックス

	// 減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

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

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// 通常の合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===============================================
// テクスチャの割り当て
//===============================================
void CObject3D::BindTexture(int nIdx)
{
	m_pTexture = CManager::GetInstance()->GetTexture()->GetAddress(nIdx);
}

//===============================================
// 位置設定
//===============================================
void CObject3D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===============================================
// 移動量設定
//===============================================
void CObject3D::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//===============================================
// サイズの設定処理
//===============================================
void CObject3D::SetSize(const float fSizeX, const float fSizeZ)
{
	m_fSizeX = fSizeX;
	m_fSizeZ = fSizeZ;
}

//===============================================
// 当たり判定
//===============================================
float CObject3D::CollisionVec(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 vec0ToPos = pos - m_aVtx[0];
	D3DXVECTOR3 vec2ToPos = pos - m_aVtx[2];
	D3DXVECTOR3 vecLine0 = m_aVtx[1] - m_aVtx[0];
	D3DXVECTOR3 vecLine1 = m_aVtx[2] - m_aVtx[0];
	D3DXVECTOR3 vecLine2 = m_aVtx[1] - m_aVtx[2];

	if ((vecLine0.z * vec0ToPos.x) - (vecLine0.x * vec0ToPos.z) > 0.0f
		&& (vecLine1.z * vec0ToPos.x) - (vecLine1.x * vec0ToPos.z) < 0.0f
		&& (vecLine2.z * vec2ToPos.x) - (vecLine2.x * vec2ToPos.z) < 0.0f)
	{// プレイヤーがポリゴンの内側にいる
		float fHeight;	// 求める高さ

		// 2つのベクトルから法線を求める
		D3DXVec3Cross(&m_nor, &m_vec0, &m_vec1);

		// 法線を正規化する
		D3DXVec3Normalize(&m_nor, &m_nor);

		if (m_nor.y != 0.0f)
		{
			fHeight = ((m_aVtx[0].x - pos.x) * -m_nor.x + (m_aVtx[0].z - pos.z) * -m_nor.z + m_aVtx[0].y) / -m_nor.y;
		}

		return fHeight;
	}

	return pos.y;
}

//===============================================
// サイズの設定処理
//===============================================
void CObject3D::SetSize(D3DXVECTOR3 size)
{
	m_fSizeX = size.x;
	m_fSizeZ = size.z;
}

//===============================================
// サイズの設定処理
//===============================================
void CObject3D::SetSizeMin(D3DXVECTOR3 size)
{
	m_fSizeX = size.x;
	m_fSizeZ = size.z;
}