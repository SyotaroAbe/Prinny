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

	struct SModel
	{
		char aFileName[MAX_NAME];	// モデルファイル名
		const char *apFileName;		// モデルファイル名

		D3DXVECTOR3 pos;			// 位置
		D3DXVECTOR3 rot;			// 向き
		int aParent;				// 親
	};

	struct SFile
	{
		CMotion::INFO m_aInfo[MAX_MOTION];					// モーション情報
		int m_nNumModel;									// モデルの総数
		SModel m_ModelInfo[MAX_MODEL];						// モデル情報
	};

	enum EFile
	{
		FILE_PLAYER = 0,	// プレイヤー
		FILE_ENEMY,			// 敵
		FILE_BOSS,			// ボス
		FILE_MAX
	};

	HRESULT Init(HWND hWnd);
	void Uninit(void);

	void Name(HWND hWnd, EFile type, const char* pFileName);
	void Script(FILE *pFile, EFile type);
	void CharacterSet(FILE *pFile, EFile type);
	void PartsSet(FILE *pFile, EFile type);
	void MotionSet(FILE *pFile, EFile type);
	void KeySet(FILE *pFile, EFile type, int nCntKey);
	void Key(FILE *pFile, EFile type, int nCntKey, int nCntModel);

	CMotion::INFO GetInfo(EFile type, int nIdx) { return m_fileInfo[type].m_aInfo[nIdx]; }
	int GetNumModel(EFile type) { return m_fileInfo[type].m_nNumModel; }
	const char *GetFileName(EFile type, int nIdx) { return m_fileInfo[type].m_ModelInfo[nIdx].apFileName; }
	D3DXVECTOR3 GetPos(EFile type, int nIdx) { return m_fileInfo[type].m_ModelInfo[nIdx].pos; }
	D3DXVECTOR3 GetRot(EFile type, int nIdx) { return m_fileInfo[type].m_ModelInfo[nIdx].rot; }
	int GetParent(EFile type, int nIdx) { return m_fileInfo[type].m_ModelInfo[nIdx].aParent; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	SFile m_fileInfo[FILE_MAX];							// ファイル情報
	int m_nMotionType;									// モーションの種類
};

#endif