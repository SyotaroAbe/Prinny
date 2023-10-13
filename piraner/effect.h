//=========================================================
//
// エフェクト処理 [effect.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _EFFECT_H_  // このマクロ定義がされてなかったら
#define _EFFECT_H_  // ２重インクルード防止のマクロを定義する

#include "main.h"
#include "objectBillboard.h"

//===============================================
// エフェクトクラス
//===============================================
class CEffect : public CObjectBillboard
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CEffect();						// デフォルトコンストラクタ
	CEffect(int nPriority = 2);		// オーバーロードされたコンストラクタ
	~CEffect();						// デストラクタ;

	enum TYPE
	{
		TYPE_NORMAL = 0,	// 通常
		TYPE_STEAM,			// 煙
		TYPE_FIRE,			// 炎
		TYPE_MAX
	};

	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, TYPE type, float fRadios, int nLife, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXCOLOR col, const float fRadios, const int nLife);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_aIdxTexture[TYPE_MAX];	// 使用するテクスチャの番号
	static int m_nNumAll;				// 総数
	TYPE m_type;						// 種類
	D3DXCOLOR m_col;					// 色
	float m_nRadius;					// 半径（大きさ）
	int m_nLife;						// 寿命
};

#endif