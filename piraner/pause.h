//=========================================================
//
// ポーズ処理 [pause.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _PAUSE_H_  // このマクロ定義がされてなかったら
#define _PAUSE_H_  // ２重インクルード防止のマクロを定義する

#include "main.h"

//===============================================
// 前方宣言
//===============================================
class CObject2D;

//===============================================
// マクロ定義
//===============================================

//===============================================
// ポーズクラス
//===============================================
class CPause
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CPause();					// デフォルトコンストラクタ
	CPause(int nPriority = 5);	// オーバーロードされたコンストラクタ
	~CPause();					// デストラクタ;

	// ゲームの状態
	enum MENU
	{
		MENU_CONTINUE = 0,	// ゲームに戻る
		MENU_RETRY,			// ゲームをやり直す
		MENU_QUIT,			// タイトル画面に戻る
		MENU_MAX
	};

	// テクスチャの種類
	enum TEX
	{
		TEX_BG = 0,		// 背景
		TEX_CONTINU,	// ゲームに戻る
		TEX_RETRY,		// リトライ
		TEX_QUIT,		// タイトルに戻る
		TEX_LOGO,		// 「Pause」
		TEX_MAX
	};

	static CPause *Create(int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static CObject2D *m_apObject2D[TEX_MAX];		// オブジェクト2Dクラスのポインタ
	static int m_aIdxTexture[TEX_MAX];				// 使用するテクスチャの番号
	static const char *m_apTextureData[TEX_MAX];	// テクスチャファイル名

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// 頂点バッファへのポインタ
	D3DXVECTOR3 m_aPos[TEX_MAX];						// 位置
	D3DXVECTOR3 m_aSize[TEX_MAX];						// サイズ
	MENU m_Menu;										// ポーズメニュー
	int m_nPosCursor;									// カーソルの位置
	bool m_EnterFade;									// 遷移中コマンド防止
};

#endif