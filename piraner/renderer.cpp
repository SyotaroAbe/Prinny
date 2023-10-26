//=========================================================
//
// レンダラー処理 [renderer.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "renderer.h"
#include "object.h"
#include "debugproc.h"
#include "manager.h"
#include "game.h"
#include "fade.h"
#include "bossBattle.h"

//===============================================
// 静的メンバ変数
//===============================================
CFade * CRenderer::m_pFade = NULL;					// フェードクラスのポインタ

//===============================================
// コンストラクタ
//===============================================
CRenderer::CRenderer()
{
	// 値をクリアする
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
}

//===============================================
// デストラクタ
//===============================================
CRenderer::~CRenderer()
{

}

//===============================================
// 初期化設定
//===============================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;          // ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;   // プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}
	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));                            // パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;                         // ゲーム画面サイズ（幅）
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;                       // ゲーム画面サイズ（高さ）
	d3dpp.BackBufferFormat = d3ddm.Format;                        // バックバッファの形式
	d3dpp.BackBufferCount = 1;                                    // バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;                     // ダブルバッファの切り替え（映像パネルに同期）
	d3dpp.EnableAutoDepthStencil = TRUE;                          // デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;                  // 24ビットZバッファ8ビットステンシルバッファ
	d3dpp.Windowed = bWindow;                                     // ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;   // リフレッシュモード
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;     // インターバル

	// Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// フェードの生成
	m_pFade = CFade::Create(CScene::MODE_TITLE, 7);

	// 各種オブジェクトの初期化処理
	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CRenderer::Uninit(void)
{// 各種オブジェクトの終了処理
	if (m_pFade != NULL)
	{// 使用されている
		// フェードの終了処理
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}
	
	// Direct3Dデバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//===============================================
// 更新処理
//===============================================
void CRenderer::Update(void)
{
	if ((CManager::GetMode() == CScene::MODE_GAME && CGame::GetPauseState() == false) 
		|| (CManager::GetMode() == CScene::MODE_BOSS && CBossBattle::GetPauseState() == false) 
		|| CManager::GetMode() == CScene::MODE_TUTORIAL || CManager::GetMode() == CScene::MODE_TITLE)
	{// ポーズ状態じゃない
		// 全てのオブジェクトの更新処理
		CObject::UpdateAll();
	}

	if (m_pFade != NULL)
	{
		// フェードの更新処理
		m_pFade->Update();
	}
}

//===============================================
// 描画処理
//===============================================
void CRenderer::Draw(void)
{
	// 画面クリア（バックバッファとＺバッファのクリア）
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// 描画開始が成功した場合
		// 全てのオブジェクトの描画処理
		CObject::DrawAll();

		if (m_pFade != NULL)
		{
			// フェードの描画処理
			m_pFade->Draw();
		}

		// デバッグ表示の描画処理
		CManager::GetInstance()->GetDebugProc()->Draw();

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}