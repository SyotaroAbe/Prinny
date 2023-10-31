//=========================================================
//
// クリア表示処理 [clear.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "clear.h"
#include "manager.h"
#include "texture.h"
#include "boss.h"
#include "bossBattle.h"
#include "renderer.h"
#include "fade.h"
#include "time.h"

//===============================================
//マクロ定義
//===============================================
#define SPEED_MOVE	(0.08f)							// 移動速度
#define SIZE_X		(350.0f)						// 横サイズ
#define SIZE_Y		(100.0f)						// 縦サイズ

//===============================================
// 静的メンバ変数
//===============================================
int CClear::m_IdxTexture = 0;				// 使用するテクスチャの番号

//===============================================
// コンストラクタ
//===============================================
CClear::CClear() : CObject2D(0)
{
	// 値のクリア
	m_bEndAnim = false;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CClear::CClear(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_bEndAnim = false;
}

//===============================================
// デストラクタ
//===============================================
CClear::~CClear()
{

}

//===============================================
// 生成処理
//===============================================
CClear *CClear::Create(int nPriority)
{
	CClear *pUi;

	// 背景の生成
	pUi = new CClear(nPriority);

	// 種類の設定
	pUi->SetType(TYPE_BG);

	// サイズの設定
	pUi->SetSize(SIZE_X, SIZE_Y);

	// 初期化処理
	pUi->Init(D3DXVECTOR3(-SIZE_X, 350.0f, 0.0f));

	// テクスチャの割り当て
	pUi->BindTexture(m_IdxTexture);

	return pUi;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CClear::Init(D3DXVECTOR3 pos)
{
	// オブジェクト2Dの初期化処理
	CObject2D::Init(pos);

	// テクスチャの設定
	m_IdxTexture = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\GameClear.png");

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CClear::Uninit(void)
{
	// オブジェクト2Dの終了処理
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CClear::Update(void)
{
	if (CBossBattle::GetBoss()->GetLife() <= 0)
	{
		// 位置補正処理
		if (m_bEndAnim == false && CObject2D::RevisionPos(D3DXVECTOR3(620.0f, 350.0f, 0.0f), SPEED_MOVE, true) == true)
		{// 位置補正終了
			m_bEndAnim = true;		// アニメーション終了
			CRenderer::GetFade()->Set(CScene::MODE_RESULT);		// リザルト画面へ移動
			CScene::SetTime(CBossBattle::GetTime()->Get());		// 時間の設定
		}
		else if (m_bEndAnim == true)
		{
			CObject2D::RevisionPos(D3DXVECTOR3(SCREEN_WIDTH + SIZE_X, 350.0f, 0.0f), SPEED_MOVE, true);
		}
	}
}

//===============================================
// 描画処理
//===============================================
void CClear::Draw(void)
{
	// オブジェクト2Dの描画処理
	CObject2D::Draw();
}