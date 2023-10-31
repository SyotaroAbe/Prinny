//=========================================================
//
// エフェクト処理 [effect.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_EFFECT		(2048)		// エフェクトの最大数
#define SUB_RADIOS		(0.5f)		// 半径の減算

//===============================================
// 静的メンバ変数
//===============================================
int CEffect::m_aIdxTexture[TYPE_MAX] = {};		// 使用するテクスチャの番号
int CEffect::m_nNumAll = 0;						// 総数

//===============================================
// コンストラクタ
//===============================================
CEffect::CEffect() : CObjectBillboard(1)
{
	// 値をクリアする
	m_type = TYPE_NORMAL;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nRadius = 0.0f;
	m_nLife = 0;

	m_nNumAll++;	// 総数をカウントアップ
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CEffect::CEffect(int nPriority) : CObjectBillboard(nPriority)
{
	// 値をクリアする
	m_type = TYPE_NORMAL;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nRadius = 0.0f;
	m_nLife = 0;

	m_nNumAll++;	// 総数をカウントアップ
}

//===============================================
// デストラクタ
//===============================================
CEffect::~CEffect()
{
	m_nNumAll--;	// 総数をカウントダウン
}

//===============================================
// 生成処理
//===============================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, TYPE type, float fRadios, int nLife, int nPriority)
{
	CEffect *pEffect = NULL;	// エフェクトクラスのポインタ

	if (MAX_EFFECT > m_nNumAll)
	{// 総数に達していない
		// エフェクトの生成
		pEffect = new CEffect(nPriority);

		// 種類の設定
		pEffect->SetType(TYPE_EFFECT);

		// 初期化処理
		pEffect->Init(pos);

		// 設定処理
		pEffect->Set(pos, move, col, fRadios, nLife);

		// テクスチャの割り当て
		pEffect->BindTexture(m_aIdxTexture[TYPE_NORMAL]);
		pEffect->BindTexture(m_aIdxTexture[TYPE_STEAM]);
		pEffect->BindTexture(m_aIdxTexture[TYPE_FIRE]);
	}

	return pEffect;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CEffect::Init(D3DXVECTOR3 pos)
{
	// サイズの設定
	SetSize(15.0f, 15.0f);

	// オブジェクトビルボードのサイズ
	CObjectBillboard::Init(pos);

	// テクスチャの設定
	m_aIdxTexture[TYPE_NORMAL] = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\effect000.jpg");
	m_aIdxTexture[TYPE_STEAM] = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\eff_steam_000.png");
	m_aIdxTexture[TYPE_FIRE] = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\eff_Glow_001.png");

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CEffect::Uninit(void)
{
	// オブジェクトビルボードの終了処理
	CObjectBillboard::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CEffect::Update(void)
{
	if (m_nLife <= 0 || m_nRadius <= 0.0f)
	{// 弾の位置が画面外に出た場合または寿命が尽きた
		Uninit();				// 自分の終了処理
	}
	else
	{
		// 位置を更新
		m_pos += m_move;

		// 頂点座標の更新処理
		CObjectBillboard::UpdatePos(m_pos, m_nRadius, m_nRadius);

		// 体力を減らす
		m_nLife--;
		m_nRadius -= SUB_RADIOS;
	}
}

//===============================================
// 描画処理
//===============================================
void CEffect::Draw(void)
{
	// オブジェクトビルボードの描画処理
	CObjectBillboard::DrawEffect();
}

//===============================================
// 設定処理
//===============================================
void CEffect::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXCOLOR col, const float fRadios, const int nLife)
{
	// 値を代入
	m_pos = pos;
	m_move = move;
	m_col = col;
	m_nRadius = fRadios;
	m_nLife = nLife;

	// 色の設定処理
	CObjectBillboard::SetCol(m_col);
}