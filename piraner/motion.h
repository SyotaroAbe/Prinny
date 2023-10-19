//=========================================================
//
// モーション処理 [motion.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _MOTION_H_     // このマクロ定義がされてなかったら
#define _MOTION_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_FILE	(20)	// 最大ファイル数
#define MAX_KEY		(5)		// キーの最大数
#define MAX_MOTION	(15)	// モーション最大数

//===============================================
// 前方宣言
//===============================================
class CModel;

//===============================================
// モーションクラス
//===============================================
class CMotion
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CMotion();						// デフォルトコンストラクタ
	~CMotion();						// デストラクタ

	//キーの構造体
	typedef struct
	{
		float fPosX;		// 位置X
		float fPosY;		// 位置Y
		float fPosZ;		// 位置Z
		float fRotX;		// 向きX
		float fRotY;		// 向きY
		float fRotZ;		// 向きZ
		float fPosDiffX;	// 目的の位置までの差分X
		float fPosDiffY;	// 目的の位置までの差分Y
		float fPosDiffZ;	// 目的の位置までの差分Z
		float fRotDiffX;	// 目的の向きまでの差分X
		float fRotDiffY;	// 目的の向きまでの差分Y
		float fRotDiffZ;	// 目的の向きまでの差分Z
	}KEY;

	// キー情報の構造体
	typedef struct
	{
		int nFrame;			// 再生フレーム
		KEY aKey[MAX_FILE];	// 各モデルのキー要素
	}KEY_INFO;

	// モーション情報の構造体
	typedef struct
	{
		bool bLoop;					// ループするかどうか
		int nNumKey;				// キーの総数
		KEY_INFO aKeyInfo[MAX_KEY];	// キー情報
	}INFO;

	HRESULT Init(void);
	void Uninit(void);
	void Set(int nType);
	void Update(void);
	int GetType(void) { return m_nType; }
	bool IsFinish(void) { return m_bFinish; }
	void SetInfo(INFO info);
	void SetModel(CModel **ppModel, int nNumModel);

private:	// 自分のみアクセス可能 [アクセス指定子]
	INFO m_aInfo[MAX_MOTION];	// モーション情報
	int m_nNumAll;				// モーションの総数
	int m_nType;				// モーションの種類
	bool m_bLoop;				// ループするかどうか
	int m_nNumKey;				// キーの総数
	int m_nKey;					// 現在のキーNo.
	int m_nCounter;				// モーションのカウンター
	bool m_bFinish;				// 終わったかどうか

	CModel **m_ppModel;			// モデルへのポインタ
	int m_nNumModel;			// モデルの総数
};

#endif