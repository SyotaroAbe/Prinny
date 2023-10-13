//=========================================================
//
// テクスチャ管理処理 [texture.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _TEXTURE_H_  // このマクロ定義がされてなかったら
#define _TEXTURE_H_  // ２重インクルード防止のマクロを定義する

#include "main.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_TEXTURE		(256)		// 最大テクスチャ数

//===============================================
// テクスチャクラス
//===============================================
class CTexture
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CTexture();		// デフォルトコンストラクタ
	~CTexture();	// デストラクタ

	HRESULT Load(void);
	void Unload(void);

	void Delete(int nIdx);
	int Regist(const char *pFilename);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx) { return m_apTexture[nIdx]; }

protected:	// 派生クラスからもアクセスできる

private:	// 自分のみアクセス可能 [アクセス指定子]
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE] = {};		// テクスチャへのポインタ
	static const char *m_apFileName[MAX_TEXTURE];			// テクスチャファイル名
	static int m_nNumAll;									// テクスチャの総数
};

#endif