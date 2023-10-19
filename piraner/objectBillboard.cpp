//=========================================================
//
// ビルボード処理 [objectBillboard.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "objectBillboard.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "sound.h"
#include "texture.h"

//===============================================
// マクロ定義
//===============================================
#define BILLBOARD_LIFE		(150)		// ビルボードの体力
#define MAX_DIFF			(1.0f)		// 最大補正値
#define TRUE_DIFF			(3.0f)		// 補正完了値

//===============================================
// コンストラクタ
//===============================================
CObjectBillboard::CObjectBillboard() : CObject(4)
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
	m_fBrightness = 0.0f;
	m_bFlash = false;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CObjectBillboard::CObjectBillboard(int nPriority) : CObject(nPriority)
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
	m_fBrightness = 0.0f;
	m_bFlash = false;
}

//===============================================
// デストラクタ
//===============================================
CObjectBillboard::~CObjectBillboard()
{

}

//===============================================
// 生成処理
//===============================================
CObjectBillboard *CObjectBillboard::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeZ, int nPriority)
{
	CObjectBillboard *pObjBillboard;

	// オブジェクトビルボードの生成
	pObjBillboard = new CObjectBillboard(nPriority);

	// 種類の設定
	pObjBillboard->SetType(TYPE_BILLBOARD);

	// サイズの設定
	pObjBillboard->SetSize(fSizeX, fSizeZ);

	// 初期化処理
	pObjBillboard->Init(pos);

	return pObjBillboard;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CObjectBillboard::Init(D3DXVECTOR3 pos)
{
	// 位置を設定
	m_pos = pos;

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
	pVtx[0].pos = D3DXVECTOR3(-m_fSizeX, m_fSizeZ, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fSizeX, m_fSizeZ, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fSizeX, -m_fSizeZ, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fSizeX, -m_fSizeZ, 0.0f);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CObjectBillboard::Uninit(void)
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
void CObjectBillboard::Update(void)
{
	
}

//===============================================
// 描画処理
//===============================================
void CObjectBillboard::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX mtxTrans;												// 計算用マトリックス
	D3DXMATRIX mtxView;													// ビューマトリックス取得用

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	// 逆行列を求める
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

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

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//===============================================
// エフェクトの描画処理
//===============================================
void CObjectBillboard::DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX mtxTrans;												// 計算用マトリックス
	D3DXMATRIX mtxView;													// ビューマトリックス取得用

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	// 逆行列を求める
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

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

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//===============================================
// 位置の更新処理
//===============================================
void CObjectBillboard::UpdatePos(D3DXVECTOR3 pos, float fSizeX, float fSizeZ)
{
	m_pos = pos;
	m_fSizeX = fSizeX;
	m_fSizeZ = fSizeZ;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fSizeX, m_fSizeZ, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fSizeX, m_fSizeZ, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fSizeX, -m_fSizeZ, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fSizeX, -m_fSizeZ, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===============================================
// 位置補正処理
//===============================================
void CObjectBillboard::RevisionPos(const D3DXVECTOR3 pos, float fMalti)
{
	if (fMalti > MAX_DIFF)
	{// 補正値が一定値を超えた
		fMalti = MAX_DIFF;
	}

	// 目的のサイズまでの差分を計算
	D3DXVECTOR3 posDiff = pos - m_pos;

	// サイズの補正
	m_pos.x += posDiff.x * fMalti;
	m_pos.z += posDiff.z * fMalti;
}

//===============================================
// 拡大縮小処理
//===============================================
bool CObjectBillboard::ScalingSize(const float fSize, float fMalti)
{
	// 目的のサイズまでの差分を計算
	float fSizeDiff = fSize - m_fSizeX;

	// サイズの補正
	m_fSizeX += fSizeDiff * fMalti;
	m_fSizeZ = m_fSizeX;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fSizeX, m_fSizeZ, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fSizeX, m_fSizeZ, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fSizeX, -m_fSizeZ, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fSizeX, -m_fSizeZ, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	if (m_fSizeX <= fSize + TRUE_DIFF && m_fSizeX >= fSize - TRUE_DIFF)
	{
		return true;
	}

	return false;
}

//===============================================
// 点滅処理
//===============================================
void CObjectBillboard::Brightness(float fBrightness)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ
	D3DXCOLOR colDiff = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	if (m_bFlash == false)
	{
		m_fBrightness -= fBrightness;	// 明るさを減算
	}
	else
	{
		m_fBrightness += fBrightness;	// 明るさを加算
	}

	if (m_fBrightness > MAX_DIFF)
	{// 値が一定値を超えた
		m_fBrightness = MAX_DIFF;
		m_bFlash = false;
	}
	else if (m_fBrightness < 0.0f)
	{// 値が一定値を超えた
		m_fBrightness = 0.0f;
		m_bFlash = true;
	}

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(m_fBrightness, m_fBrightness, m_fBrightness, m_fBrightness);
	pVtx[1].col = D3DXCOLOR(m_fBrightness, m_fBrightness, m_fBrightness, m_fBrightness);
	pVtx[2].col = D3DXCOLOR(m_fBrightness, m_fBrightness, m_fBrightness, m_fBrightness);
	pVtx[3].col = D3DXCOLOR(m_fBrightness, m_fBrightness, m_fBrightness, m_fBrightness);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===============================================
// テクスチャの割り当て
//===============================================
void CObjectBillboard::BindTexture(int nIdx)
{
	m_pTexture = CManager::GetInstance()->GetTexture()->GetAddress(nIdx);
}

//===============================================
// 位置設定
//===============================================
void CObjectBillboard::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===============================================
// 移動量設定
//===============================================
void CObjectBillboard::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//===============================================
// 色の設定処理
//===============================================
void CObjectBillboard::SetCol(D3DXCOLOR col)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===============================================
// サイズの設定処理
//===============================================
void CObjectBillboard::SetSize(const float fSizeX, const float fSizeZ)
{
	m_fSizeX = fSizeX;
	m_fSizeZ = fSizeZ;
}

//===============================================
// サイズの設定処理
//===============================================
void CObjectBillboard::SetSize(D3DXVECTOR3 size)
{
	m_fSizeX = size.x;
	m_fSizeZ = size.z;
}

//===============================================
// サイズの設定処理
//===============================================
void CObjectBillboard::SetSizeMin(D3DXVECTOR3 size)
{
	m_fSizeX = size.x;
	m_fSizeZ = size.z;
}