//=========================================================
//
// タイトル画面処理 [title.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _TITLE_H_     // このマクロ定義がされてなかったら
#define _TITLE_H_     // 2重インクルード防止のマクロ定義する

#include "manager.h"

//===============================================
// 前方宣言
//===============================================
class CBg;
class CFade;
class CLogo;
class CPressEnter;

//===============================================
// タイトル画面クラス
//===============================================
class CTitle : public CScene
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CTitle();						// デフォルトコンストラクタ
	~CTitle();						// デストラクタ

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetFade(void) { return m_bFade; }

	static CBg *GetBg() { return m_pBg; }
	static CLogo *GetLogo() { return m_pLogo; }
	static CPressEnter *GetPressEnter() { return m_pPressEnter; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static CBg *m_pBg;						// 背景クラスのポインタ
	static CLogo *m_pLogo;					// タイトルロゴ描画クラスのポインタ
	static CPressEnter *m_pPressEnter;		// PressEnter描画クラスのポインタ

	int m_nTimeFade;	// 自動フェード時間
	bool m_bFade;		// フェードに入ったか
};

#endif