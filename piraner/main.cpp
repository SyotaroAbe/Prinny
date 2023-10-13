//=========================================================
//
// メイン描画 [main.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "manager.h"

//===============================================
// ライブラリのリンク
//===============================================
#pragma comment(lib, "d3d9.lib")     // 描画処理に必要
#pragma comment(lib, "d3dx9.lib")    // [d3d9.lib]の拡張ライブラリ
#pragma comment(lib, "dxguid.lib")   // DirectXコンポーネント（部品）使用に必要
#pragma comment(lib, "winmm.lib")    // システム時刻取得に必要
#pragma comment(lib, "dinput8.lib")  // 入力処理に必要

//===============================================
// マクロ定義
//===============================================
#define CLASS_NAME        "WindowClass"			// ウインドウクラスの名前
#define WINDOW_NAME       "JustSeparation"		// ウインドウの名前（キャプションに表示）

//===============================================
// プロトタイプ宣言
//===============================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//===============================================
// メイン関数
//===============================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow)
{
	// 終了時にメモリリークを出力
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),               // WNDCLASSEXのメモリサイズ
		CS_CLASSDC,                       // ウインドウのスタイル
		WindowProc,                       // ウインドウプロシージャ
		0,                                // ０にする（通常は使用しない）
		0,                                // ０にする（通常は使用しない）
		hInstance,                        // インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),  // タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),      // マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),       // クライアント領域の背景色
		NULL,                             // メニューバー
		CLASS_NAME,                       // ウインドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION),  // ファイルのアイコン
	};

	HWND hWnd;											// ウインドウハンドル（識別子）
	MSG msg;											// メッセージを格納する変数
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// 画面サイズの構造体
	DWORD dwCurrentTime;								// 現在時刻
	DWORD dwExecLastTime;								// 最後に処理した時刻
	DWORD dwFrameCount;									// フレームカウント
	DWORD dwFPSLastTime;								// 最後にFPSを計測した時刻

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウを生成
	hWnd = CreateWindowEx(0,             // 拡張ウインドウスタイル
		CLASS_NAME,                      // ウインドウクラスの名前
		WINDOW_NAME,                     // ウインドウの名前
		WS_OVERLAPPEDWINDOW,             // ウインドウスタイル
		CW_USEDEFAULT,                   // ウインドウの左上X座標
		CW_USEDEFAULT,                   // ウインドウの左上Y座標
		(rect.right - rect.left),        // ウインドウの幅
		(rect.bottom - rect.top),        // ウインドウの高さ
		NULL,                            // 親ウインドウのハンドル
		NULL,                            // メニューハンドルまたは子ウインドウ
		hInstance,                       // インスタンスハンドル
		NULL);                           // ウインドウ制作データ

	CManager *pManager = NULL;
	pManager = new CManager;

	// マネージャの初期化処理
	if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
	{// 初期化処理が失敗した場合
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;                // 初期化する
	dwExecLastTime = timeGetTime();   // 現在時刻を取得（保存）
	dwFrameCount = 0;                 // 初期化する
	dwFPSLastTime = timeGetTime();    // 現在時刻を取得（保存）

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);    // ウインドウの表示状態を設定
	UpdateWindow(hWnd);            // クライアント領域を更新

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// Windowsの処理
			if (msg.message == WM_QUIT)
			{// WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				// メッセージの設定
				TranslateMessage(&msg);     // 仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);      // ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{// DirectXの処理
			dwCurrentTime = timeGetTime();   // 現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
				// FPSを計測
				int nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				pManager->SetFPS(nCountFPS);
				dwFPSLastTime = dwCurrentTime;   // FPSを計測した時刻を保存
				dwFrameCount = 0;                // フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// ６０分の１秒経過
				dwExecLastTime = dwCurrentTime;		// 処理開始の時刻（現在時刻）を保存
				dwFrameCount++;						// フレームカウントを計算

				if (pManager != NULL)
				{// メモリが使用されている
					// マネージャの更新処理
					pManager->Update();

					// マネージャの描画処理
					pManager->Draw();
				}
			}
		}
	}

	if (pManager != NULL)
	{// メモリが使用されている
		// マネージャの終了処理
		pManager->Uninit();

		// メモリを開放
		delete pManager;
		pManager = NULL;
	}

	//分解能を戻す
	timeBeginPeriod(1);

	// ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//===============================================
// ウインドウプロシージャ
//===============================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;            // 返り値を格納

	switch (uMsg)
	{
	case WM_DESTROY:   // ウインドウ破棄のメッセージ
		// WM_QUIT メッセージを送る
		PostQuitMessage(1);
		break;
	case WM_KEYDOWN:   // キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:   // [ESC]キーが押された
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				// ウインドウを破棄する（WM_DESTROYメッセージを送る）
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	case WM_CLOSE:   // 閉じるボタン押下のメッセージ
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			// ウインドウを破棄する
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;   // ０を返さないと終了してしまう
		}
		break;
	case WM_LBUTTONDOWN:   // マウス左クリックのメッセージ
						   // 対象ウインドウにフォーカスを合わせる
		SetFocus(hWnd);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);    // 既定の処理を繰り返す
}