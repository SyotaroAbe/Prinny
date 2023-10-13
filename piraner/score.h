//=========================================================
//
// スコア処理 [score.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _SCORE_H_     // このマクロ定義がされてなかったら
#define _SCORE_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "number.h"

//===============================================
// スコアクラス
//===============================================
class CScore
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CScore();					// デフォルトコンストラクタ
	CScore(int nPriority = 5);	// オーバーロードされたコンストラクタ
	~CScore();					// デストラクタ

	static CScore *Create(int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nPriority);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(const int nScore);
	int Get(void) { return m_nScore; }
	void Add(const int nScore);

	float GetSizeX(void) { return m_fSizeX; }
	float GetSizeY(void) { return m_fSizeY; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static CNumber *m_apNumber[MAX_NUMBER];		// ナンバークラスのポインタ
	static int m_nIdxTexture;					// 使用するテクスチャの番号
	float m_fSizeX;								// 幅
	float m_fSizeY;								// 高さ
	static int m_nScore;						// スコアの値
};

#endif