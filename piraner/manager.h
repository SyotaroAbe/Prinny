//=========================================================
//
// マネージャ処理 [manager.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _MANAGER_H_     // このマクロ定義がされてなかったら
#define _MANAGER_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"

//===============================================
// 前方宣言
//===============================================
class CRenderer;
class CInputKeyboard;
class CInputGamePad;
class CDebugProc;
class CSound;
class CCamera;
class CLight;
class CTexture;
class CObjectX;
class CXFile;
class CFade;
class CFileLoad;

//===============================================
// シーンクラス
//===============================================
class CScene
{
public:		// 誰でもアクセス可能[アクセス指定子]
	
	// 画面モードの種類
	enum MODE
	{
		MODE_TITLE,		// タイトル画面
		MODE_TUTORIAL,	// チュートリアル画面
		MODE_GAME,		// ゲーム画面
		MODE_RESULT,	// リザルト画面
		MODE_MAX
	};

	CScene();				// デフォルトコンストラクタ
	virtual ~CScene();		// デストラクタ
	
	static CScene *Create(HWND hWnd, MODE mode);
	virtual HRESULT Init(HWND hWnd) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void SetMode(MODE mode);
	static MODE GetMode(void) { return m_mode; }
	static MODE GetModeOld(void) { return m_modeOld; }

private:	// 自分のみアクセス可能[アクセス指定子]
	static MODE m_mode;		// 現在の画面モード
	static MODE m_modeOld;	// 前回の画面モード
};

//===============================================
// マネージャクラス
//===============================================
class CManager
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CManager();		// デフォルトコンストラクタ
	~CManager();	// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Reset(void);
	void SetFPS(int nCountFPS);
	HWND GetHWND(void) { return m_hWnd; }

	static void SetMode(CScene::MODE mode);
	static CScene::MODE GetMode(void);

	static CManager *GetManager() { return m_pManager; }
	static CRenderer *GetRenderer() { return m_pRenderer; }
	static CInputKeyboard *GetKeyboardInput() { return m_pKeyboardInput; }
	static CInputGamePad *GetInputGamePad() { return m_pInputGamePad; }
	static CDebugProc *GetDebugProc() { return m_pDebugProc; }
	static CSound *GetSound() { return m_pSound; }
	static CCamera *GetCamera() { return m_pCamera; }
	static CLight *GetLight() { return m_pLight; }
	static CTexture *GetTexture() { return m_pTexture; }
	static CXFile *GetXFile() { return m_pXFile; }
	static CFileLoad *GetLoad() { return m_pFileLoad; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static CManager *m_pManager;					// マネージャクラスのポインタ
	static CRenderer *m_pRenderer;					// レンダラークラスのポインタ
	static CInputKeyboard *m_pKeyboardInput;		// キーボード入力クラスのポインタ
	static CInputGamePad *m_pInputGamePad;			// ゲームパッド入力クラスのポインタ
	static CDebugProc *m_pDebugProc;				// デバッグ表示クラスのポインタ
	static CSound *m_pSound;						// サウンドクラスのポインタ
	static CCamera *m_pCamera;						// カメラクラスのポインタ
	static CLight *m_pLight;						// ライトクラスのポインタ
	static CTexture *m_pTexture;					// テクスチャクラスのポインタ
	static CXFile *m_pXFile;						// Xファイルクラスのポインタ
	static CScene *m_pScene;						// シーンクラスのポインタ
	static CFileLoad *m_pFileLoad;					// ファイルロードクラスのポインタ

	static HWND m_hWnd;						// ウインドウ保存用
	static CScene::MODE m_mode;				// 現在の画面モード

	int m_nCountFPS;						// FPSをカウント
	bool m_bEdit;							// エディット切り替え
};

#endif