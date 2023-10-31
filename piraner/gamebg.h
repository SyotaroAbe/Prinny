//=========================================================
//
// メインモード背景処理 [gamebg.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _GAMEBG_H_     // このマクロ定義がされてなかったら
#define _GAMEBG_H_     // 2重インクルード防止のマクロ定義する

#include "objectBillboard.h"

//===============================================
// 弾クラス
//===============================================
class CGameBg : public CObjectBillboard
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CGameBg();						// デフォルトコンストラクタ
	CGameBg(int nPriority = 2);		// オーバーロードされたコンストラクタ
	~CGameBg();						// デストラクタ

	// テクスチャの種類
	enum ETex
	{
		TEX_GAME = 0,	// ゲーム
		TEX_TUTORIAL,	// チュートリアル
		TEX_BOSS,		// ボス
		TEX_MAX
	};

	static CGameBg *Create(D3DXVECTOR3 pos, ETex tex, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, ETex tex);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move);

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_nIdxTexture;			// 使用するテクスチャの番号
	ETex m_tex;							// テクスチャの種類
};

#endif