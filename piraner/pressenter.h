//=========================================================
//
// PressEnter描画処理 [pressenter.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _PRESSENTER_H_     // このマクロ定義がされてなかったら
#define _PRESSENTER_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object2D.h"

//===============================================
// PressEnter描画クラス
//===============================================
class CPressEnter : public CObject2D
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CPressEnter();						// デフォルトコンストラクタ
	CPressEnter(int nPriority = 0);		// オーバーロードされたコンストラクタ
	~CPressEnter();						// デストラクタ

	static CPressEnter *Create(int nPriority = 5);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetBrightnessFade(bool bFade);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_IdxTexture;			// 使用するテクスチャの番号
	float m_fBrightness;				// 明るさの値
};

#endif