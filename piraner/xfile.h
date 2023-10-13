//=========================================================
//
// Xファイル管理処理 [xfile.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _XFILE_H_  // このマクロ定義がされてなかったら
#define _XFILE_H_  // ２重インクルード防止のマクロを定義する

#include "main.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_XFILE		(256)		// 最大Xファイル数

//===============================================
// Xファイルクラス
//===============================================
class CXFile
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CXFile();	// デフォルトコンストラクタ
	~CXFile();	// デストラクタ

	// Xファイル構造体
	struct XFile
	{
		LPDIRECT3DTEXTURE9 *m_pTexture;			// テクスチャへのポインタ
		int *m_pIdxTexture;						// 使用するテクスチャの番号
		LPD3DXMESH m_pMesh;						// メッシュ（頂点情報）へのポインタ
		LPD3DXBUFFER m_pBuffMat;				// マテリアルへのポインタ
		DWORD m_dwNumMat;						// マテリアルの数
		D3DXVECTOR3 m_vtxMin, m_vtxMax;			// モデルの最小値・最大値
	};

	typedef enum
	{
		COL_NORMAL = 0,		// 通常
		COL_RED,			// 赤
		COL_GREEN,			// 緑
		COL_BLUE,			// 青
		COL_YELLOW,			// 黄
		COL_ORANGE,			// 橙
		COL_LIGHTBLUE,		// 水色
		COL_YELLOWGREEN,	// 黄緑
		COL_MAX
	}COL;

	HRESULT Load(void);
	void Unload(void);

	int Regist(const char *pFilename);
	void BindX(int nIdx);
	void SetVtxSize(int nIdx);
	void Draw(int nIdx, COL col);
	void CleannessDraw(int nIdx);

	D3DXVECTOR3 GetSize(int nIdx) { return m_aXFile[nIdx].m_vtxMax; }
	D3DXVECTOR3 GetSizeMin(int nIdx) { return m_aXFile[nIdx].m_vtxMin; }

protected:	// 派生クラスからもアクセスできる

private:	// 自分のみアクセス可能 [アクセス指定子]
	static const char *m_apFileName[MAX_XFILE];			// Xファイルファイル名
	static int m_nNumAll;								// Xファイルの総数
	
	int m_nIdx;											// モデルの番号
	XFile m_aXFile[MAX_XFILE];							// Xファイルの情報
	COL m_col;											// 色の情報
};

#endif