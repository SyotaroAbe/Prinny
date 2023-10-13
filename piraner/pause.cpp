//=========================================================
//
// ポーズ処理 [pause.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "input.h"
#include "title.h"
#include "sound.h"
#include "pause.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "object2D.h"
#include "fade.h"
#include "sound.h"

//===============================================
// 静的メンバ変数
//===============================================
CObject2D *CPause::m_apObject2D[TEX_MAX];			// オブジェクト2Dクラスのポインタ
const char *CPause::m_apTextureData[TEX_MAX] = {};	// テクスチャファイル名
int CPause::m_aIdxTexture[TEX_MAX] = {};			// 使用するテクスチャの番号

//===============================================
// コンストラクタ
//===============================================
CPause::CPause()
{
	// 値のクリア
	m_pVtxBuff = NULL;
	m_Menu = MENU_CONTINUE;
	m_nPosCursor = 0;
	m_EnterFade = false;
	for (int nCntObj = 0; nCntObj < TEX_MAX; nCntObj++)
	{
		m_aPos[nCntObj] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aSize[nCntObj] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CPause::CPause(int nPriority)
{
	// 値のクリア
	m_pVtxBuff = NULL;
	m_Menu = MENU_CONTINUE;
	m_nPosCursor = 0;
	m_EnterFade = false;
	for (int nCntObj = 0; nCntObj < TEX_MAX; nCntObj++)
	{
		m_aPos[nCntObj] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aSize[nCntObj] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//===============================================
// デストラクタ
//===============================================
CPause::~CPause()
{
	
}

//===============================================
// 生成処理
//===============================================
CPause *CPause::Create(int nPriority)
{
	CPause *pPause;

	// 背景の生成
	pPause = new CPause(5);

	// 初期化処理
	pPause->Init(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

	return pPause;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CPause::Init(D3DXVECTOR3 pos)
{
	// テクスチャファイル名
	const char *m_apTextureData[TEX_MAX] =
	{
		{ "data\\TEXTURE\\pause_BG.png" },			// 背景
		{ "data\\TEXTURE\\pause_Continu.png" },		// ゲームに戻る
		{ "data\\TEXTURE\\pause_Retry.png" },		// リトライ
		{ "data\\TEXTURE\\pause_Quit.png" },		// タイトルに戻る
		{ "data\\TEXTURE\\pause_Pause.png" }		// 「Pause」
	};

	// テクスチャの位置設定
	m_aPos[TEX_BG] = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	m_aPos[TEX_CONTINU] = D3DXVECTOR3(635.0f, 290.0f, 0.0f);
	m_aPos[TEX_RETRY] = D3DXVECTOR3(635.0f, 405.0f, 0.0f);
	m_aPos[TEX_QUIT] = D3DXVECTOR3(635.0f, 515.0f, 0.0f);
	m_aPos[TEX_LOGO] = D3DXVECTOR3(635.0f, 175.0f, 0.0f);

	// テクスチャのサイズ設定
	m_aSize[TEX_BG] = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	m_aSize[TEX_CONTINU] = D3DXVECTOR3(195.0f, 50.0f, 0.0f);
	m_aSize[TEX_RETRY] = D3DXVECTOR3(195.0f, 45.0f, 0.0f);
	m_aSize[TEX_QUIT] = D3DXVECTOR3(195.0f, 45.0f, 0.0f);
	m_aSize[TEX_LOGO] = D3DXVECTOR3(195.0f, 45.0f, 0.0f);

	for (int nCntObj = 0; nCntObj < TEX_MAX; nCntObj++)
	{
		m_apObject2D[nCntObj] = new CObject2D(6);

		// 種類の設定
		m_apObject2D[nCntObj]->SetType(CObject::TYPE_PAUSE);

		// サイズ設定処理
		m_apObject2D[nCntObj]->SetSize(m_aSize[nCntObj].x, m_aSize[nCntObj].y);

		// オブジェクト2Dの初期化処理
		m_apObject2D[nCntObj]->Init(m_aPos[nCntObj]);

		// テクスチャの設定
		m_aIdxTexture[nCntObj] = CManager::GetTexture()->Regist(m_apTextureData[nCntObj]);

		// テクスチャの割り当て
		m_apObject2D[nCntObj]->BindTexture(m_aIdxTexture[nCntObj]);
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CPause::Uninit(void)
{
	
}

//===============================================
// 更新処理
//===============================================
void CPause::Update(void)
{
	if (m_EnterFade == false)
	{// ENTER入力されていない
		// カーソルを動かす
		if (CManager::GetKeyboardInput()->GetTrigger(DIK_W) == true || CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_UP, 0))
		{
			m_nPosCursor--;

			if (m_nPosCursor < MENU_CONTINUE)
			{// 一周した
				m_nPosCursor = MENU_QUIT;
			}

			// サウンドの再生
			CManager::GetSound()->Play(CSound::LABEL_SE_PAUSE_CURSOR);
		}
		else if (CManager::GetKeyboardInput()->GetTrigger(DIK_S) == true || CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_DOWN, 0))
		{
			m_nPosCursor++;

			if (m_nPosCursor >= MENU_MAX)
			{// 一周した
				m_nPosCursor = MENU_CONTINUE;
			}

			// サウンドの再生
			CManager::GetSound()->Play(CSound::LABEL_SE_PAUSE_CURSOR);
		}

		for (int nCntPause = 0; nCntPause < MENU_MAX; nCntPause++)
		{
			if (nCntPause == m_nPosCursor)
			{
				// 頂点カラーの設定
				m_apObject2D[nCntPause + 1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				// 頂点カラーの設定
				m_apObject2D[nCntPause + 1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 0.1f, 0.3f));
			}
		}

		if (CManager::GetKeyboardInput()->GetTrigger(DIK_RETURN) == true || CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_A, 0))
		{// 決定キー（ENTERキー）が押された
			if (m_nPosCursor == MENU_CONTINUE)
			{
				CGame::SetEnablePause(false);

				// サウンドの再生
				CManager::GetSound()->Play(CSound::LABEL_SE_PAUSE_CANCEL);
			}
			else if (m_nPosCursor == MENU_RETRY)
			{
				m_EnterFade = true;

				// モード設定（ゲーム画面に移行）
				CRenderer::GetFade()->Set(CScene::MODE_GAME);

				// サウンドの再生
				CManager::GetSound()->Play(CSound::LABEL_SE_PAUSE_ENTER);
			}
			else if (m_nPosCursor == MENU_QUIT)
			{
				m_EnterFade = true;

				// モード設定（タイトル画面に移行）
				CRenderer::GetFade()->Set(CScene::MODE_TITLE);

				// サウンドの再生
				CManager::GetSound()->Play(CSound::LABEL_SE_PAUSE_ENTER);
			}
		}
	}
}

//===============================================
// 描画処理
//===============================================
void CPause::Draw(void)
{
	
}