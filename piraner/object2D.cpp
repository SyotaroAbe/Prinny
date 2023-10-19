//=========================================================
//
// オブジェクト2D処理 [object2D.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "object2D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_DIFF			(1.0f)		// 最大補正値
#define TRUE_DIFF			(3.0f)		// 補正完了値
#define MAX_BRIGHTNESS		(1.0f)		// 最大明るさ
#define MIN_BRIGHTNESS		(0.0f)		// 最小明るさ

//===============================================
// コンストラクタ（デフォルト）
//===============================================
CObject2D::CObject2D() : CObject(3)
{
	// 値をクリアする
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = 0.0f;
	m_fLength = 0.0f;
	m_fSizeX = 0.0f;
	m_fSizeY = 0.0f;
	m_fBrightness = 0.0f;
	m_bFlash = false;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	// 値をクリアする
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = 0.0f;
	m_fLength = 0.0f;
	m_fSizeX = 0.0f;
	m_fSizeY = 0.0f;
	m_fBrightness = 0.0f;
	m_bFlash = false;
}

//===============================================
// デストラクタ
//===============================================
CObject2D::~CObject2D()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CObject2D::Init(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();   // デバイスの取得
	VERTEX_2D *pVtx;													// 頂点情報へのポインタ

	// 値を代入
	m_pos = pos;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (m_pVtxBuff == NULL)
	{// 使用されていない
		return -1;
	}

	// 対角線の長さを算出する
	m_fLength = sqrtf(m_fSizeX * 2 * m_fSizeX * 2 + m_fSizeY * 2 * m_fSizeY * 2) * 0.5f;

	// 対角線の角度を算出する
	m_fAngle = atan2f(m_fSizeX * 2, m_fSizeY * 2);

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	if (GetType() == TYPE_PLAYER)
	{// プレイヤーの場合
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fSizeX, m_pos.y - m_fSizeY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y - m_fSizeY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fSizeX, m_pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y, 0.0f);
	}
	else if (GetType() == TYPE_BLOCK)
	{// ブロックの場合
		pVtx[0].pos = D3DXVECTOR3(m_pos.x,			m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x,			m_pos.y + m_fSizeY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y + m_fSizeY, 0.0f);
	}
	else
	{// その他
		pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (D3DX_PI + m_fAngle)) * m_fLength,
			m_pos.y + cosf(m_rot.y + (D3DX_PI + m_fAngle)) * m_fLength, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (D3DX_PI - m_fAngle)) * m_fLength,
			m_pos.y + cosf(m_rot.y + (D3DX_PI - m_fAngle)) * m_fLength, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (-m_fAngle)) * m_fLength,
			m_pos.y + cosf(m_rot.y + (-m_fAngle)) * m_fLength, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (m_fAngle)) * m_fLength,
			m_pos.y + cosf(m_rot.y + (m_fAngle)) * m_fLength, 0.0f);
	}

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	if (GetType() == TYPE_EXPLOSION)
	{// 爆発の場合
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.125f * 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125f + 0.125f * 0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.125f * 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f + 0.125f * 0.0f, 1.0f);
	}
	else
	{// その他
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CObject2D::Uninit(void)
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
void CObject2D::Update(void)
{
	
}

//===============================================
// 描画処理
//===============================================
void CObject2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();   // デバイスの取得

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//===============================================
// 位置の更新処理
//===============================================
void CObject2D::UpdatePos(D3DXVECTOR3 pos, float m_fSizeX, float m_fSizeY)
{
	m_pos = pos;

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	if (GetType() == TYPE_PLAYER)
	{// プレイヤー
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fSizeX, m_pos.y - m_fSizeY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y - m_fSizeY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fSizeX, m_pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y, 0.0f);
	}
	else if (GetType() == TYPE_BLOCK)
	{// ブロックの場合
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fSizeY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y + m_fSizeY, 0.0f);
	}
	else
	{
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fSizeX, m_pos.y - m_fSizeY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y - m_fSizeY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fSizeX, m_pos.y + m_fSizeY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y + m_fSizeY, 0.0f);
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===============================================
// 角度を使用した位置の更新処理
//===============================================
void CObject2D::UpdatePosAngle(D3DXVECTOR3 pos)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (D3DX_PI + m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (D3DX_PI + m_fAngle)) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (D3DX_PI - m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (D3DX_PI - m_fAngle)) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (-m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (-m_fAngle)) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (m_fAngle)) * m_fLength, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===============================================
// 色の更新処理
//===============================================
void CObject2D::SetCol(const D3DXCOLOR col)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

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
// アニメーションの更新処理
//===============================================
void CObject2D::UpdateAnim(int nPatternX, int nPatternY, int nSpeed)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	//pVtx[0].tex = D3DXVECTOR2(0.0f + 1.0f / nPatternX * m_nPatternAnim, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(0.2f + 1.0f / nPatternX * m_nPatternAnim, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.0f + 1.0f / nPatternX * m_nPatternAnim, 0.5f);
	//pVtx[3].tex = D3DXVECTOR2(0.2f + 1.0f / nPatternX * m_nPatternAnim, 0.5f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===============================================
// テクスチャ座標の更新処理
//===============================================
void CObject2D::UpdateTex(const float fTexU, const float fSubU, const float fAddU, const float fTexV, const float fSubV, const float fAddV)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の更新
	pVtx[0].tex = D3DXVECTOR2(fTexU - fSubU, fTexV - fSubV);
	pVtx[1].tex = D3DXVECTOR2(fTexU + fAddU, fTexV - fSubV);
	pVtx[2].tex = D3DXVECTOR2(fTexU - fSubU, fTexV + fAddV);
	pVtx[3].tex = D3DXVECTOR2(fTexU + fAddU, fTexV + fAddV);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===============================================
// 位置補正処理
//===============================================
bool CObject2D::RevisionPos(const D3DXVECTOR3 pos, float fMalti, bool bWidth)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	if (fMalti >= MAX_DIFF)
	{// 補正値が一定値を超えた
		fMalti = MAX_DIFF;
	}

	if (bWidth == false && pos.y + TRUE_DIFF >= m_pos.y && pos.y - TRUE_DIFF <= m_pos.y)
	{// 補正完了
		return true;
	}
	else if (bWidth == true && pos.x + TRUE_DIFF >= m_pos.x && pos.x - TRUE_DIFF <= m_pos.x)
	{// 補正完了
		return true;
	}

	// 目的のサイズまでの差分を計算
	D3DXVECTOR3 posDiff = pos - m_pos;

	// サイズの補正
	m_pos.x += posDiff.x * fMalti;
	m_pos.y += posDiff.y * fMalti;

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (D3DX_PI + m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (D3DX_PI + m_fAngle)) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (D3DX_PI - m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (D3DX_PI - m_fAngle)) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (-m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (-m_fAngle)) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (m_fAngle)) * m_fLength, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return false;
}

//===============================================
// エフェクトの描画処理
//===============================================
void CObject2D::DrawEffect(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();   // デバイスの取得

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===============================================
// 点滅処理
//===============================================
void CObject2D::Brightness(float fBrightness)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ
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
void CObject2D::BindTexture(int nIdx)
{
	m_pTexture = CManager::GetInstance()->GetTexture()->GetAddress(nIdx);
}

//===============================================
// 位置設定
//===============================================
void CObject2D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (D3DX_PI + m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (D3DX_PI + m_fAngle)) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (D3DX_PI - m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (D3DX_PI - m_fAngle)) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (-m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (-m_fAngle)) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (m_fAngle)) * m_fLength, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===============================================
// 移動量設定
//===============================================
void CObject2D::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//===============================================
// ナンバーの設定処理
//===============================================
void CObject2D::SetNumber(const int nNumber)
{
	int nTexNumber = nNumber;

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f *  nTexNumber), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f *  nTexNumber), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f *  nTexNumber), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f *  nTexNumber), 1.0f);
		
	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===============================================
// サイズの設定処理
//===============================================
void CObject2D::SetSize(const float fSizeX, const float fSizeY)
{
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;
}

//===============================================
// サイズの設定処理
//===============================================
void CObject2D::SetSize(D3DXVECTOR3 size)
{
	m_fSizeX = size.x;
	m_fSizeY = size.y;
}

//===============================================
// サイズの設定処理
//===============================================
void CObject2D::SetSizeMin(D3DXVECTOR3 size)
{
	m_fSizeX = size.x;
	m_fSizeY = size.y;
}