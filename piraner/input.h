//=======================================================================
//
// 入力処理 [input.h]
// Author = 阿部翔大郎
//
//=======================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include "Xinput.h"

//===============================================
// ライブラリのリンク
//===============================================
#pragma comment(lib,"xinput.lib")

//=========================================================
// マクロ定義
//=========================================================
#define NUM_KEY_MAX			(256)			// キーの最大数
#define NUM_PLAYER			(4)				// プレイヤー数

//=========================================================
// 入力クラス
//=========================================================
class CInput
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CInput();				// デフォルトコンストラクタ
	virtual ~CInput();		// デストラクタ

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:	// 派生クラスからもアクセスできる [アクセス指定子]
	static LPDIRECTINPUT8 m_pInput;		
	LPDIRECTINPUTDEVICE8 m_pDevice;
};

//=========================================================
// キーボード入力クラス
//=========================================================
class CInputKeyboard : public CInput
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CInputKeyboard(); 		   // デフォルトコンストラクタ
	~CInputKeyboard();		   // デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);

private:	// 自分のみアクセス可能 [アクセス指定子]
	BYTE m_aKeyState[NUM_KEY_MAX];			// プレスの情報
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	// トリガーの情報
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];	// リリースの情報
	BYTE m_aKeyStateRepeat[NUM_KEY_MAX];	// リピートの情報
};

//=========================================================
// ゲームパッド入力クラス
//=========================================================
class CInputGamePad : public CInput
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CInputGamePad();			   // デフォルトコンストラクタ
	~CInputGamePad();			   // デストラクタ

	// 押されたボタンの種類
	typedef enum
	{
		BUTTON_UP = 0,				// 十字↑ 
		BUTTON_DOWN,				// 十字↓
		BUTTON_LEFT,				// 十字←
		BUTTON_RIGHT,				// 十字→
		BUTTON_START,				// start
		BUTTON_BACK,				// back
		BUTTON_LS,					// 左スティック押し込み
		BUTTON_RS,					// 右スティック押し込み
		BUTTON_LB,					// LB/5
		BUTTON_RB,					// RB/6
		BUTTON_10,					// ?
		BUTTON_11,					// ?
		BUTTON_A,					// A/3 
		BUTTON_B,					// B/4
		BUTTON_X,					// X/1
		BUTTON_Y,					// Y/2
		BUTTON_16,					// ?
		BUTTON_17,					// ?
		BUTTON_18,					// ?
		BUTTON_19,					// ?
		BUTTON_20,					// ?
		BUTTON_MAX
	} JOYKEY;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(JOYKEY nKey, int nPlayer);
	bool GetTrigger(JOYKEY nKey, int nPlayer);
	bool GetRelease(JOYKEY nKey, int nPlayer);
	bool GetRepeat(JOYKEY nKey, int nPlayer);
	SHORT GetJoyStickLX(int nPlayer){ return m_aState[nPlayer].Gamepad.sThumbLX; }
	SHORT GetJoyStickLY(int nPlayer){ return m_aState[nPlayer].Gamepad.sThumbLY; }
	SHORT GetJoyStickRX(int nPlayer){ return m_aState[nPlayer].Gamepad.sThumbRX; }
	SHORT GetJoyStickRY(int nPlayer){ return m_aState[nPlayer].Gamepad.sThumbRY; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	XINPUT_STATE m_aState[NUM_PLAYER];			// プレスの情報
	XINPUT_STATE m_aStateTrigger[NUM_PLAYER];	// トリガーの情報
	XINPUT_STATE m_aStateRelease[NUM_PLAYER];	// リリースの情報
	XINPUT_STATE m_aStateRepeat[NUM_PLAYER];	// リピートの情報
};

////マウス
//HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd);
//void UninitMouse(void);
//void UpdateMouse(void);
////bool GetMousePress(int nKey);
//void GetMouseDB(void);

#endif
