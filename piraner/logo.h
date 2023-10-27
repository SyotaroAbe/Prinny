//=========================================================
//
// タイトルロゴ描画処理 [logo.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _LOGO_H_     // このマクロ定義がされてなかったら
#define _LOGO_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object2D.h"

//===============================================
// タイトルロゴ描画クラス
//===============================================
class CLogo : public CObject2D
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CLogo();						// デフォルトコンストラクタ
	CLogo(int nPriority = 0);		// オーバーロードされたコンストラクタ
	~CLogo();						// デストラクタ

	static CLogo *Create(int nPriority = 5);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool GetEndAnim(void) { return m_bEndAnim; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_IdxTexture;			// 使用するテクスチャの番号
	bool m_bEndAnim;					// アニメーションが終了したか
};

#endif