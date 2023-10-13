//=========================================================
//
// パーティクル処理 [particle.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _PARTICLE_H_  // このマクロ定義がされてなかったら
#define _PARTICLE_H_  // ２重インクルード防止のマクロを定義する

#include "main.h"
#include "objectBillboard.h"
#include "effect.h"

//===============================================
// パーティクルクラス
//===============================================
class CParticle : public CObjectBillboard
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CParticle();		// デフォルトコンストラクタ
	~CParticle();		// デストラクタ;

	// 種類
	typedef enum
	{
		TYPE_PLAYER = 0,	// プレイヤーの爆発
		TYPE_ENEMY,			// 敵の爆発
		TYPE_BURN,			// 燃えるゴミ
		TYPE_NONFLAMMABLE,	// 燃えないゴミ
		TYPE_RECYCLABLE,		// 資源ゴミ
		TYPE_MOVE,			// 移動
		TYPE_CURVE,			// カーブ
		TYPE_MAX
	}TYPE;

	static CParticle *Create(int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(D3DXVECTOR3 pos, TYPE type);

private:	// 自分のみアクセス可能 [アクセス指定子]
	TYPE m_type;			// 種類
	CEffect::TYPE m_tex;	// テクスチャの種類
	D3DXCOLOR m_col;		// 色
	int m_nRadius;			// 半径（大きさ）
	int m_nLife;			// 寿命
	int m_nMaxApeear;		// 一度に放出する量
	int m_nCntAppear;		// 一度に放出する量をカウント
};

#endif