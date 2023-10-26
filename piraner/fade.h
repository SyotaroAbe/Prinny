//=========================================================
//
// フェード処理 [fade.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _FADE_H_  // このマクロ定義がされてなかったら
#define _FADE_H_  // ２重インクルード防止のマクロを定義する

#include "main.h"
#include "manager.h"

//===============================================
// フェードクラス
//===============================================
class CFade
{
public:		// 誰でもアクセス可能 [アクセス指定子]

	// フェードの状態
	typedef enum
	{
		STATE_NONE = 0,  // 何もしていない状態
		STATE_IN,        // フェードイン状態
		STATE_OUT,       // フェードアウト状態
		STATE_MAX
	}STATE;

	CFade();						// デフォルトコンストラクタ
	~CFade();						// デストラクタ

	static CFade *Create(CScene::MODE modeNext, int nPriority);

	HRESULT Init(CScene::MODE modeNext);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(CScene::MODE modeNext, bool bNext = true);
	STATE Get(void) { return m_state; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	STATE m_state;							// フェードの状態
	CScene::MODE m_modeNext;				// 次の画面（モード）
	D3DXCOLOR m_color;						// ポリゴン（フェード）の色
	bool m_bNext;							// 画面遷移するか
};

#endif