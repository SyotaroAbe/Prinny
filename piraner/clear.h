//=========================================================
//
// クリア表示処理 [clear.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _CLEAR_H_  //このマクロ定義がされてなかったら
#define _CLEAR_H_  //２重インクルード防止のマクロを定義する

#include "object2D.h"

//===============================================
// クリア表示クラス
//===============================================
class CClear : public CObject2D
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CClear();						// デフォルトコンストラクタ
	CClear(int nPriority = 0);		// オーバーロードされたコンストラクタ
	~CClear();						// デストラクタ

	static CClear *Create(int nPriority = 5);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_IdxTexture;			// 使用するテクスチャの番号
	bool m_bEndAnim;					// アニメーションが終了したか
};

#endif