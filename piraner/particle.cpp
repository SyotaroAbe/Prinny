//=========================================================
//
// パーティクル処理 [particle.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "particle.h"
#include "effect.h"

//===============================================
// マクロ定義
//===============================================
#define PARTICLE_SET	(15)		// 一度に呼ぶ量

//===============================================
// コンストラクタ
//===============================================
CParticle::CParticle() : CObjectBillboard(2)
{
	// 値をクリアする
	m_tex = CEffect::TYPE_NORMAL;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nMaxApeear = 0;
	m_nRadius = 0;
	m_nLife = 0;
	m_nCntAppear = 0;
}

//===============================================
// デストラクタ
//===============================================
CParticle::~CParticle()
{

}

//===============================================
// 生成処理
//===============================================
CParticle *CParticle::Create(int nPriority)
{
	CParticle *pParticle;

	// パーティクルの生成
	pParticle = new CParticle;

	// 初期化処理
	pParticle->Init();

	return pParticle;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CParticle::Init(void)
{
	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CParticle::Uninit(void)
{
	// オブジェクトビルボードの終了処理
	CObjectBillboard::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CParticle::Update(void)
{
	if(m_nCntAppear < m_nMaxApeear)
	{// 一度に放出する数分
		// エフェクトの生成
		CEffect::Create(m_pos, D3DXVECTOR3(m_move.x += sinf(((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % m_nRadius * m_nRadius) / 100.0f)),
											m_move.y += cosf(((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % m_nRadius * m_nRadius) / 100.0f)), 
											m_move.z += cosf(((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % m_nRadius * m_nRadius) / 100.0f))),
											m_col, m_tex, (float)m_nRadius, m_nLife, 4);

		m_nCntAppear++;		// 回数をカウントアップ
	}

	if (m_nLife <= 0)
	{// 寿命が尽きた
		Uninit();   // 自分の終了処理
	}
	else
	{
		// 体力を減らす
		m_nLife--;
	}
}

//===============================================
// 描画処理
//===============================================
void CParticle::Draw(void)
{
	
}

//===============================================
// 設定処理
//===============================================
void CParticle::Set(D3DXVECTOR3 pos, TYPE type)
{
	m_pos = pos;

	switch (type)
	{
	case TYPE_PLAYER:		// プレイヤーの爆発
		break;

	case TYPE_ENEMY:		// 敵の爆発
		m_tex = CEffect::TYPE_NORMAL;
		m_col = D3DXCOLOR(0.2f, 0.3f, 1.0f, 1.0f);
		m_nMaxApeear = 10;
		m_nRadius = 30;
		m_nLife = 300;
		break;

	case TYPE_BURN:			// 燃えるゴミ
		m_tex = CEffect::TYPE_NORMAL;
		m_col = D3DXCOLOR(1.0f, 0.3f, 0.2f, 1.0f);
		m_nMaxApeear = 10;
		m_nRadius = 20;
		m_nLife = 300;
		break;
			
	case TYPE_NONFLAMMABLE:	// 燃えないゴミ
		m_tex = CEffect::TYPE_NORMAL;
		m_col = D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f);
		m_nMaxApeear = 10;
		m_nRadius = 20;
		m_nLife = 300;
		break;

	case TYPE_RECYCLABLE:		// 資源ゴミ
		m_tex = CEffect::TYPE_NORMAL;
		m_col = D3DXCOLOR(0.5f, 0.3f, 1.0f, 1.0f);
		m_nMaxApeear = 10;
		m_nRadius = 20;
		m_nLife = 300;
		break;

	case TYPE_MOVE:			// 移動
		m_tex = CEffect::TYPE_STEAM;
		m_col = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.05f);
		m_nMaxApeear = 1;
		m_nRadius = 40;
		m_nLife = 18;
		break;

	case TYPE_CURVE:		// カーブ
		m_tex = CEffect::TYPE_FIRE;
		m_col = D3DXCOLOR(1.0f, 0.3f, 0.2f, 1.0f);
		m_nMaxApeear = 12;
		m_nRadius = 7;
		m_nLife = 30;
		break;
	}
}