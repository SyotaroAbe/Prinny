//=========================================================
//
// タイム処理 [time.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _TIME_H_     // このマクロ定義がされてなかったら
#define _TIME_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "number.h"

//===============================================
// タイムクラス
//===============================================
class CTime
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CTime();		// デフォルトコンストラクタ
	~CTime();		// デストラクタ

	static CTime *Create(int nPriority = 3);

	HRESULT Init(int nPriority);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(const int nTime);
	int Get(void) { return m_nTime; }

	float GetSizeX(void) { return m_fSizeX; }
	float GetSizeY(void) { return m_fSizeY; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static CNumber *m_apNumber[MAX_NUMBER];		// ナンバークラスのポインタ
	static int m_nIdxTexture;					// 使用するテクスチャの番号
	float m_fSizeX;								// 幅
	float m_fSizeY;								// 高さ
	static int m_nTime;							// タイムの値
	DWORD m_dwTime;								// タイムを取得
	DWORD m_dwtime;								// タイムを取得

	bool m_bFade;								// フェードに入ったか
};

#endif