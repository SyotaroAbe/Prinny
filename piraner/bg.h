//=========================================================
//
// 背景処理 [bg.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _BG_H_     // このマクロ定義がされてなかったら
#define _BG_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object2D.h"

//===============================================
// 背景クラス
//===============================================
class CBg : public CObject2D
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CBg();						// デフォルトコンストラクタ
	CBg(int nPriority = 0);		// オーバーロードされたコンストラクタ
	~CBg();						// デストラクタ

	// テクスチャの種類
	enum TYPE
	{
		TYPE_TITLE = 0,		// タイトル画面
		TYPE_RESULT,		// リザルト画面
		TYPE_MAX
	};

	static CBg *Create(int nIdx, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const bool bMove);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_aIdxTexture[TYPE_MAX];			// 使用するテクスチャの番号
	int m_nIdx;									// 何枚目の背景か
	bool m_bMove;								// テクスチャ座標を動かすか
	float m_fTexU;								// テクスチャ座標の開始位置（U値）
	float m_fTexV;								// テクスチャ座標の開始位置（V値）
};

#endif