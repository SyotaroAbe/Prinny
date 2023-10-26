//=========================================================
//
// ファイル読み込み処理 [fileload.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "fileload.h"

//===============================================
// 静的メンバ変数
//===============================================
//CFileLoad::SFile CFileLoad::m_fileInfo[FILE_MAX] = {};							// ファイル情報

//===============================================
// コンストラクタ
//===============================================
CFileLoad::CFileLoad()
{
	// 値のクリア
	m_nMotionType = 0;
}

//===============================================
// デストラクタ
//===============================================
CFileLoad::~CFileLoad()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CFileLoad::Init(HWND hWnd)
{
	// ファイル名読み込み
	Name(hWnd, FILE_PLAYER, "data\\TXT\\player00.txt");
	Name(hWnd, FILE_ENEMY, "data\\TXT\\enemy.txt");
	Name(hWnd, FILE_BOSS, "data\\TXT\\boss.txt");

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CFileLoad::Uninit(void)
{

}

//===============================================
// ファイル名読み込み処理
//===============================================
void CFileLoad::Name(HWND hWnd, EFile type, const char* pFileName)
{
	FILE *pFile;

	// ファイルを開く
	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{// 読み込み成功
		char aStr[MAX_NAME] = {};		// 文字列読み取り

		while (1)
		{
			int nResult = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], "SCRIPT") == 0)
			{// SCRIPT情報読み込み
				Script(pFile, type);
			}
			else if (nResult == EOF)
			{// 最後まで読み込んだ
				m_nMotionType = 0;
				break;
			}
		}

		// ファイル閉じる
		fclose(pFile);
	}
	else
	{
		MessageBox(hWnd, "ファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}

//===============================================
// Script情報読み込み処理
//===============================================
void CFileLoad::Script(FILE *pFile, EFile type)
{
	char aStr[MAX_NAME] = {};		// 文字列読み取り
	int nCntModel = 0;				// モデル数をカウント

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "NUM_MODEL") == 0)
		{// モデル数読み込み
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%d", &m_fileInfo[type].m_nNumModel);
		}
		else if (strcmp(&aStr[0], "MODEL_FILENAME") == 0
			&& m_fileInfo[type].m_nNumModel > nCntModel)
		{// 親モデルのインデックスを設定
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%s", &m_fileInfo[type].m_ModelInfo[nCntModel].aFileName[0]);

			// [char] から [const char*] へ置き換え
			m_fileInfo[type].m_ModelInfo[nCntModel].apFileName = (const char*)&m_fileInfo[type].m_ModelInfo[nCntModel].aFileName[0];
			nCntModel++;
		}
		else if (strcmp(&aStr[0], "CHARACTERSET") == 0)
		{// CharacterSet情報読み込み
			CharacterSet(pFile, type);
		}
		else if (strcmp(&aStr[0], "MOTIONSET") == 0)
		{// MotionSet情報読み込み
			MotionSet(pFile, type);
			m_nMotionType++;
		}
		else if (nResult == -1 || strcmp(&aStr[0], "END_SCRIPT") == 0)
		{// 最後まで読み込んだ
			break;
		}
	}
}

//===============================================
// Character情報読み込み処理
//===============================================
void CFileLoad::CharacterSet(FILE *pFile, EFile type)
{
	char aStr[MAX_NAME] = {};		// 文字列読み取り
	int nCntModel = 0;				// モデル数をカウント
	bool bNumModel = false;

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "PARTSSET") == 0)
		{// PartsSet情報読み込み
			PartsSet(pFile, type);
			nCntModel++;
			bNumModel = true;
		}

		if (nCntModel == m_fileInfo[type].m_nNumModel
			&& bNumModel == true
			|| strcmp(&aStr[0], "END_CHARACTERSET") == 0)
		{// 読み込んだモデル数に達した
			break;
		}
	}
}

//===============================================
// PartsSet情報読み込み処理
//===============================================
void CFileLoad::PartsSet(FILE *pFile, EFile type)
{
	char aStr[MAX_NAME] = {};		// 文字列読み取り
	int nIdx;

	// 各パーツの階層構造設定
	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "INDEX") == 0)
		{// インデックス
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%d", &nIdx);
		}
		else if (strcmp(&aStr[0], "PARENT") == 0)
		{// 親モデルのインデックスを設定
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%d", &m_fileInfo[type].m_ModelInfo[nIdx].aParent);
		}
		if (strcmp(&aStr[0], "POS") == 0)
		{// 位置（オフセット）の初期位置
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%f", &m_fileInfo[type].m_ModelInfo[nIdx].pos.x);
			fscanf(pFile, "%f", &m_fileInfo[type].m_ModelInfo[nIdx].pos.y);
			fscanf(pFile, "%f", &m_fileInfo[type].m_ModelInfo[nIdx].pos.z);
		}
		else if (strcmp(&aStr[0], "ROT") == 0)
		{// 向きの初期位置
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%f", &m_fileInfo[type].m_ModelInfo[nIdx].rot.x);
			fscanf(pFile, "%f", &m_fileInfo[type].m_ModelInfo[nIdx].rot.y);
			fscanf(pFile, "%f", &m_fileInfo[type].m_ModelInfo[nIdx].rot.z);
		}
		else if (strcmp(&aStr[0], "END_PARTSSET") == 0)
		{// 読み込み終了
			break;
		}
	}
}

//===============================================
// Motion情報読み込み処理
//===============================================
void CFileLoad::MotionSet(FILE *pFile, EFile type)
{
	char aStr[MAX_NAME] = {};		// 文字列読み取り
	int nCntKey = 0;
	bool bNumKey = false;
	int nLoop = 0;

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "LOOP") == 0)
		{// ループするか
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%d", &nLoop);

			// [int] から [bool] へ置き換え
			if (nLoop == 0)
			{// ループしない
				m_fileInfo[type].m_aInfo[m_nMotionType].bLoop = false;
			}
			else
			{// ループする
				m_fileInfo[type].m_aInfo[m_nMotionType].bLoop = true;
			}
		}

		if (strcmp(&aStr[0], "NUM_KEY") == 0)
		{// キー数
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%d", &m_fileInfo[type].m_aInfo[m_nMotionType].nNumKey);
			bNumKey = true;
		}
		if (strcmp(&aStr[0], "KEYSET") == 0)
		{// KeySet情報読み込み
			KeySet(pFile, type, nCntKey);
			nCntKey++;
		}

		if (nCntKey == m_fileInfo[type].m_aInfo[m_nMotionType].nNumKey
			&& bNumKey == true
			|| strcmp(&aStr[0], "END_MOTIONSET") == 0)
		{// 読み込んだキー数に達した
			break;
		}
	}
}

//===============================================
// KeySet情報読み込み処理
//===============================================
void CFileLoad::KeySet(FILE *pFile, EFile type, int nCntKey)
{
	char aStr[MAX_NAME] = {};		// 文字列読み取り
	int nCntModel = 0;				// モデル数をカウント
	bool bFrame = false;

	while (1)
	{// キー数に達するまで繰り返す
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "FRAME") == 0)
		{// フレーム数
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%d", &m_fileInfo[type].m_aInfo[m_nMotionType].aKeyInfo[nCntKey].nFrame);
			bFrame = true;
		}
		else if (strcmp(&aStr[0], "KEY") == 0 && bFrame == true)
		{// Key情報読み込み
			Key(pFile, type, nCntKey, nCntModel);
			nCntModel++;
		}

		if (nCntModel == m_fileInfo[type].m_nNumModel
			|| strcmp(&aStr[0], "END_KEYSET") == 0)
		{// 読み込んだモデル数に達した
			break;
		}
	}
}

//===============================================
// Key情報読み込み処理
//===============================================
void CFileLoad::Key(FILE *pFile, EFile type, int nCntKey, int nCntModel)
{
	char aStr[MAX_NAME] = {};		// 文字列読み取り

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "POS") == 0)
		{// 位置
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%f", &m_fileInfo[type].m_aInfo[m_nMotionType].aKeyInfo[nCntKey].aKey[nCntModel].fPosX);
			fscanf(pFile, "%f", &m_fileInfo[type].m_aInfo[m_nMotionType].aKeyInfo[nCntKey].aKey[nCntModel].fPosY);
			fscanf(pFile, "%f", &m_fileInfo[type].m_aInfo[m_nMotionType].aKeyInfo[nCntKey].aKey[nCntModel].fPosZ);
		}
		else if (strcmp(&aStr[0], "ROT") == 0)
		{// 向き
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%f", &m_fileInfo[type].m_aInfo[m_nMotionType].aKeyInfo[nCntKey].aKey[nCntModel].fRotX);
			fscanf(pFile, "%f", &m_fileInfo[type].m_aInfo[m_nMotionType].aKeyInfo[nCntKey].aKey[nCntModel].fRotY);
			fscanf(pFile, "%f", &m_fileInfo[type].m_aInfo[m_nMotionType].aKeyInfo[nCntKey].aKey[nCntModel].fRotZ);
		}
		else if (strcmp(&aStr[0], "END_KEY") == 0)
		{// 読み込み終了
			break;
		}
	}
}