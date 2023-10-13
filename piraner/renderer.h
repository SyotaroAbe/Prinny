//=========================================================
//
// レンダラー処理 [renderer.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _RENDERER_H_     // このマクロ定義がされてなかったら
#define _RENDERER_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"

//===============================================
// 前方宣言
//===============================================
class CFade;

//===============================================
// レンダラークラス
//===============================================
class CRenderer
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CRenderer();		// デフォルトコンストラクタ
	~CRenderer();		// デストラクタ

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	static CFade *GetFade() { return m_pFade; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	LPDIRECT3D9 m_pD3D;						// Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;			// Direct3Dデバイスへのポインタ

	static CFade *m_pFade;							// フェードクラスのポインタ
};

#endif