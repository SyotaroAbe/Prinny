//=======================================================================
//
// 入力処理 [input.cpp]
// Author = 阿部翔大郎
//
//=======================================================================
#include "input.h"

//=========================================================
// 静的メンバ変数
//=========================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//***********************************************************
// 入力クラス（親）
//***********************************************************
//=========================================================
// コンストラクタ
//=========================================================
CInput::CInput()
{
	// 値のクリア
	m_pDevice = NULL;
	m_pInput = NULL;
}

//=========================================================
// デストラクタ
//=========================================================
CInput::~CInput()
{

}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{// メモリが使用されていない
		// DirectInputオブジェクトの初期化
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CInput::Uninit(void)
{
	// 入力デバイス(キーボード)の破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();			// キーボードへのアクセス権を放棄
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// Directinputオブジェクトの破棄
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//***********************************************************
// キーボード入力クラス（子）
//***********************************************************
//=========================================================
// コンストラクタ
//=========================================================
CInputKeyboard::CInputKeyboard()
{
	// 値のクリア
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		m_aKeyState[nCntKey] = NULL;
		m_aKeyStateTrigger[nCntKey] = NULL;
		m_aKeyStateRelease[nCntKey] = NULL;
		m_aKeyStateRepeat[nCntKey] = NULL;
	}
}

//=========================================================
// デストラクタ
//=========================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=========================================================
// 初期化処理
//=========================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 入力の初期化処理
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{// 初期化処理が失敗した場合
		return -1;
	}

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 強調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//=========================================================
// 終了処理
//=========================================================
void CInputKeyboard::Uninit(void)
{
	// 入力クラスの終了処理
	CInput::Uninit();
}

//=========================================================
// 更新処理
//=========================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];			// キーボードの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			// キーボードのリピート情報を保存
			m_aKeyStateRepeat[nCntKey] = (m_aKeyState[nCntKey] & aKeyState[nCntKey]);

			// キーボードのリリース情報を保存
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] | aKeyState[nCntKey]) ^ aKeyState[nCntKey];

			// キーボードのトリガー情報を保存
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			// キーボードのプレス情報を保存
			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDevice->Acquire();				// キーボードへのアクセス権を獲得
	}
}

//=======================================================================
// プレス情報を取得
//=======================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//=======================================================================
// トリガー情報を取得
//=======================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=======================================================================
// リピート情報を取得
//=======================================================================
bool CInputKeyboard::GetRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//=======================================================================
// リリース情報を取得
//=======================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//***********************************************************
// ゲームパッド入力クラス（子）
//***********************************************************
//=========================================================
// コンストラクタ
//=========================================================
CInputGamePad::CInputGamePad()
{
	// 値のクリア
	for (int nCntKey = 0; nCntKey < NUM_PLAYER; nCntKey++)
	{
		m_aState[nCntKey] = {};
		m_aStateTrigger[nCntKey] = {};
		m_aStateRelease[nCntKey] = {};
		m_aStateRepeat[nCntKey] = {};
	}
}

//=========================================================
// デストラクタ
//=========================================================
CInputGamePad::~CInputGamePad()
{

}

//=======================================================================
// 初期化処理
//=======================================================================
HRESULT CInputGamePad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 変数宣言
	int nCntPad;

	// XInputのステート(使う状態)
	XInputEnable(true);

	// メモリの初期化(プレイヤー分)
	for (nCntPad = 0; nCntPad < NUM_PLAYER; nCntPad++)
	{
		memset(&m_aState[nCntPad], 0, sizeof(XINPUT_STATE));
		memset(&m_aStateTrigger[nCntPad], 0, sizeof(XINPUT_STATE));
		memset(&m_aStateRelease[nCntPad], 0, sizeof(XINPUT_STATE));
	}

	return S_OK;
}

//=======================================================================
// 終了処理
//=======================================================================
void CInputGamePad::Uninit(void)
{
	// XInputのステート(使わない状態)
	XInputEnable(false);
}

//=======================================================================
// 更新処理
//=======================================================================
void CInputGamePad::Update(void)
{
	// 変数宣言
	int nCntPad;

	XINPUT_STATE aGamePadState;			// ゲームパッドの入力情報

	for (nCntPad = 0; nCntPad < NUM_PLAYER; nCntPad++)
	{
		// 入力デバイスからデータを取得
		if (XInputGetState(nCntPad, &aGamePadState) == ERROR_SUCCESS)
		{
			// ゲームパッドのトリガー情報を保存
			m_aStateTrigger[nCntPad].Gamepad.wButtons =
				(m_aState[nCntPad].Gamepad.wButtons ^ aGamePadState.Gamepad.wButtons) & aGamePadState.Gamepad.wButtons;

			// リピート情報
			m_aStateRepeat[nCntPad].Gamepad.wButtons =
				(m_aState[nCntPad].Gamepad.wButtons & aGamePadState.Gamepad.wButtons);

			// リリース情報
			m_aStateRelease[nCntPad].Gamepad.wButtons =
				(m_aState[nCntPad].Gamepad.wButtons | aGamePadState.Gamepad.wButtons) ^ aGamePadState.Gamepad.wButtons;

			// ゲームパッドのプレス情報を保存
			m_aState[nCntPad] = aGamePadState;
		}
	}
}

//=======================================================================
// プレス情報取得
//=======================================================================
bool CInputGamePad::GetPress(JOYKEY nKey, int nPlayer)
{
	return (m_aState[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//=======================================================================
// トリガー情報取得
//=======================================================================
bool CInputGamePad::GetTrigger(JOYKEY nKey, int nPlayer)
{
	return (m_aStateTrigger[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//=======================================================================
// リリース情報取得
//=======================================================================
bool CInputGamePad::GetRelease(JOYKEY nKey, int nPlayer)
{
	return (m_aStateRelease[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//=======================================================================
// リピート情報取得
//=======================================================================
bool CInputGamePad::GetRepeat(JOYKEY nKey, int nPlayer)
{
	return(m_aStateRepeat[nPlayer].Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}