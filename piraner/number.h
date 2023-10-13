//=========================================================
//
// ナンバー処理 [number.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _NUMBER_H_     // このマクロ定義がされてなかったら
#define _NUMBER_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object.h"

//===============================================
// 前方宣言
//===============================================
class CObject2D;

//===============================================
// マクロ定義
//===============================================
#define MAX_NUMBER		(1024)		// 最大ナンバー数

#define PATTERNANIM_X		(8)									// アニメ横パターン数
#define PATTERNANIM_Y		(1)									// アニメ縦パターン数
#define PATTERNANIM_ALL		(PATTERNANIM_X * PATTERNANIM_Y)		// アニメ縦パターン数
#define SPEED_ANIM			(10)								// アニメーションスピード

//===============================================
// ナンバークラス
//===============================================
class CNumber
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CNumber();					// デフォルトコンストラクタ
	CNumber(int nPriority = 5);	// オーバーロードされたコンストラクタ
	virtual ~CNumber();			// デストラクタ

	// ナンバーの種類
	typedef enum
	{
		TYPE_NONE = 0,	// なし
		TYPE_TIME,		// タイム
		TYPE_SCORE,		// スコア
		TYPE_MAX
	}TYPE;

	virtual HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nPriority);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetType(const TYPE type);
	TYPE GetType(void) { return m_type; }

	void BindTexture(int nIdx);
	void Set(const int nNumber);
	void SetPos(const D3DXVECTOR3 pos, float fSizeX, float fSizeY);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetCol(const D3DXCOLOR col);

protected:	// 派生クラスからもアクセスできる [アクセス指定子]
	D3DXVECTOR3 m_pos;					// 位置
	int m_nCounterAnim;					// アニメーションカウンター
	int m_nPatternAnim;					// アニメーションパターンNo.

private:	// 自分のみアクセス可能 [アクセス指定子]
	CObject2D *m_pObject2D;			// オブジェクト2Dクラスのポインタ
	TYPE m_type;					// ナンバーの種類
};

#endif