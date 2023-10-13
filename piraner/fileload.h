//=========================================================
//
// ファイル読み込み処理 [fileload.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _FILELOAD_H_     // このマクロ定義がされてなかったら
#define _FILELOAD_H_     // 2重インクルード防止のマクロ定義する

#include "motion.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_MODEL	(20)	// 最大モデル数
#define MAX_NAME	(256)	// 最大文字数

//===============================================
// ロードクラス
//===============================================
class CFileLoad
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CFileLoad();						// デフォルトコンストラクタ
	~CFileLoad();						// デストラクタ

	typedef struct
	{
		char aFileName[MAX_NAME];	// モデルファイル名
		const char *apFileName;		// モデルファイル名

		D3DXVECTOR3 pos;			// 位置
		D3DXVECTOR3 rot;			// 向き
		int aParent;				// 親
	}MODEL;

	HRESULT Init(HWND hWnd);
	void Uninit(void);

	void Name(HWND hWnd, const char* pFileName);
	void Script(FILE *pFile);
	void CharacterSet(FILE *pFile);
	void PartsSet(FILE *pFile);
	void MotionSet(FILE *pFile);
	void KeySet(FILE *pFile, int nCntKey);
	void Key(FILE *pFile, int nCntKey, int nCntModel);

	CMotion::INFO GetInfo(int nIdx) { return m_aInfo[nIdx]; }
	int GetNumModel(void) { return m_nNumModel; }
	const char *GetFileName(int nIdx) { return m_ModelInfo[nIdx].apFileName; }
	D3DXVECTOR3 GetPos(int nIdx) { return m_ModelInfo[nIdx].pos; }
	D3DXVECTOR3 GetRot(int nIdx) { return m_ModelInfo[nIdx].rot; }
	int GetParent(int nIdx) { return m_ModelInfo[nIdx].aParent; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	CMotion::INFO m_aInfo[MAX_MOTION];					// モーション情報
	int m_nNumModel;									// モデルの総数
	int m_nMotionType;									// モーションの種類
	MODEL m_ModelInfo[MAX_MODEL];						// モデル情報
};

#endif