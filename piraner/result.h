//=========================================================
//
// リザルト画面処理 [result.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _RESULT_H_     // このマクロ定義がされてなかったら
#define _RESULT_H_     // 2重インクルード防止のマクロ定義する

#include "manager.h"

//===============================================
// 前方宣言
//===============================================
class CBg;
class CRanking;
class CRankIn;

//===============================================
// リザルト画面クラス
//===============================================
class CResult : public CScene
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CResult();						// デフォルトコンストラクタ
	~CResult();						// デストラクタ

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBg *GetBg() { return m_pBg; }
	static CRanking *GetRanking() { return m_pRanking; }
	static CRankIn *GetRankIn() { return m_pRankIn; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static CBg *m_pBg;								// 背景クラスのポインタ
	static CRanking *m_pRanking;					// ランキングクラスのポインタ
	static CRankIn *m_pRankIn;						// ランクインクラスのポインタ

	int m_nTimeFade;	// 自動フェード時間
	bool m_bFade;		// フェードに入ったか
};

#endif