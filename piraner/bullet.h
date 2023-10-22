//=========================================================
//
// 弾処理 [bullet.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _BULLET_H_     // このマクロ定義がされてなかったら
#define _BULLET_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object2D.h"
#include "objectBillboard.h"

//===============================================
// 弾クラス
//===============================================
class CBullet : public CObjectBillboard
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CBullet();						// デフォルトコンストラクタ
	CBullet(int nPriority = 2);		// オーバーロードされたコンストラクタ
	~CBullet();						// デストラクタ

	static CBullet *Create(D3DXVECTOR3 pos, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move);

	bool CollisionEnemy(D3DXVECTOR3 pos);

	static int GetNumAll(void){ return m_nNumAll; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_nIdxTexture;			// 使用するテクスチャの番号
	int m_nLife;						// 体力
	static int m_nNumAll;				// 弾の総数
};

#endif