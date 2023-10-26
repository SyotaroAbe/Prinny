//=========================================================
//
// フェード処理 [fade.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "fade.h"
#include "renderer.h"

//===============================================
// マクロ定義
//===============================================
#define STATE_UPDATE	(0.03f)		// フェードスピード

//===============================================
// コンストラクタ
//===============================================
CFade::CFade()
{
	// 値のクリア
	m_pVtxBuff = NULL;
	m_state = STATE_NONE;
	m_modeNext = CScene::MODE_TITLE;
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_bNext = true;
}

//===============================================
// デストラクタ
//===============================================
CFade::~CFade()
{

}

//===============================================
// 生成処理
//===============================================
CFade *CFade::Create(CScene::MODE modeNext, int nPriority)
{
	CFade *pUi;

	// 背景の生成
	pUi = new CFade;

	// 初期化処理
	pUi->Init(modeNext);

	return pUi;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CFade::Init(CScene::MODE modeNext)
{
	m_state = STATE_IN;								// フェードイン状態に
	m_modeNext = modeNext;							// 次の画面（モード）を設定
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// 黒いポリゴン（不透明）にしておく

	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();   // デバイスの取得

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D * pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CFade::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//===============================================
// 更新処理
//===============================================
void CFade::Update(void)
{
	VERTEX_2D * pVtx;   // 頂点情報へのポインタ

	if (m_state != STATE_NONE)
	{
		if (m_state == STATE_IN)
		{// フェードイン状態
			m_color.a -= STATE_UPDATE;  // ポリゴンを透明にしていく

			if (m_color.a <= 0.0f)
			{
				m_color.a = 0.0f;
				m_state = STATE_NONE;   // 何もしていない状態に
			}
		}
		else if (m_state == STATE_OUT)
		{// フェードアウト状態
			m_color.a += STATE_UPDATE;  // ポリゴンを不透明にしていく

			if (m_color.a >= 1.0f)
			{
				m_color.a = 1.0f;
				m_state = STATE_IN;

				if (m_bNext == true)
				{// 画面遷移させる
					// モードの設定（次の画面に移行）
					CManager::SetMode(m_modeNext);
				}
			}
		}

		// 頂点バッファをロックし頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_color.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_color.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_color.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_color.a);

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//===============================================
// 描画処理
//===============================================
void CFade::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();   // デバイスの取得

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//===============================================
// 設定処理
//===============================================
void CFade::Set(CScene::MODE modeNext, bool bNext)
{
	if (m_state != STATE_OUT)
	{
		m_state = STATE_OUT;							// フェードアウト状態に
		m_modeNext = modeNext;							// 次の画面（モード）を設定
		m_bNext = bNext;								// 画面遷移するか設定
		m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 黒いポリゴン（透明）にしておく
	}
}