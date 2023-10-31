//=========================================================
//
// タイム処理 [time.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "time.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"
#include "texture.h"
#include "fade.h"

//===============================================
// 静的メンバ変数
//===============================================
CNumber *CTime::m_apNumber[MAX_NUMBER] = {};		// ナンバークラスのポインタ
int CTime::m_nIdxTexture = 0;						// 使用するテクスチャの番号
int CTime::m_nTime = 0;								// タイムの値

//===============================================
// マクロ定義
//===============================================
#define SET_TIME		(0)		// 設定タイム
#define NUM_PLACE		(4)			// タイムの桁数
#define TIME_SIZEX		(27)		// タイムの幅（半分）
#define TIME_SIZEY		(60)		// タイムの高さ（半分）

//===============================================
// コンストラクタ
//===============================================
CTime::CTime()
{
	// 値のクリア
	m_nTime = 0;
	m_dwTime = 0;
	m_dwtime = 0;
	m_bFade = false;
}

//===============================================
// デストラクタ
//===============================================
CTime::~CTime()
{

}

//===============================================
// 生成処理
//===============================================
CTime *CTime::Create(int nPriority)
{
	CTime *pTime;

	// タイムの生成
	pTime = new CTime;

	// 初期化処理
	pTime->Init(nPriority);

	return pTime;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CTime::Init(int nPriority)
{
	for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
	{
		if (m_apNumber[nCntObj] == NULL)
		{
			// 生成
			m_apNumber[nCntObj] = new CNumber(nPriority);

			// 種類の設定
			m_apNumber[nCntObj]->SetType(CNumber::TYPE_TIME);

			// 初期化
			m_apNumber[nCntObj]->Init(D3DXVECTOR3(550.0f + (nCntObj + 1) * TIME_SIZEX * 2, 50.0f, 0.0f), TIME_SIZEX, TIME_SIZEY, nPriority);
			m_apNumber[nCntObj]->SetPos(D3DXVECTOR3(550.0f + (nCntObj + 1) * TIME_SIZEX * 2, 50.0f, 0.0f), TIME_SIZEX, TIME_SIZEY);

			// テクスチャの設定
			m_nIdxTexture = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\time000.png");

			// テクスチャの割り当て
			m_apNumber[nCntObj]->BindTexture(m_nIdxTexture);
		}
	}

	// サイズを設定
	m_fSizeX = TIME_SIZEX;
	m_fSizeY = TIME_SIZEY;

	// タイム初期設定
	Set(SET_TIME);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CTime::Uninit(void)
{
	// ナンバーの終了処理
	for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
	{
		if (m_apNumber[nCntObj] != NULL)
		{
			m_apNumber[nCntObj]->Uninit();
			delete m_apNumber[nCntObj];
			m_apNumber[nCntObj] = NULL;
		}
	}
}

//===============================================
// 更新処理
//===============================================
void CTime::Update(void)
{
	m_dwtime = timeGetTime();	// 現在時刻を取得（保存）

	if ((m_dwtime - m_dwTime) >= 100)
	{// １秒経過
		m_dwTime = m_dwtime;

		if (m_nTime < 9999)
		{// のこり時間が０秒じゃない
			m_nTime++;
			Set(m_nTime);
		}
		//else if(m_nTime <= 0)
		//{
		//	if (m_bFade == false)
		//	{// フェードバグ防止
		//		CRenderer::GetFade()->Set(CScene::MODE_RESULT);		// リザルト画面へ移動
		//		m_bFade = true;
		//	}
		//}
	}

	// ナンバーの更新処理
	for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
	{
		if (m_apNumber[nCntObj] != NULL)
		{
			m_apNumber[nCntObj]->Update();
		}
	}
}

//===============================================
// 描画処理
//===============================================
void CTime::Draw(void)
{
	// ナンバーの描画処理
	for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
	{
		if (m_apNumber[nCntObj] != NULL)
		{
			m_apNumber[nCntObj]->Draw();
		}
	}
}

//===============================================
// 設定処理
//===============================================
void CTime::Set(const int nTime)
{
	int aTexU[NUM_PLACE] = {};  // 各行の数字を格納

	m_nTime = nTime;

	aTexU[0] = m_nTime % 10000 / 1000;
	aTexU[1] = m_nTime % 1000 / 100;
	aTexU[2] = m_nTime % 100 / 10;
	aTexU[3] = m_nTime % 10;

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{// 使用されている
			CNumber::TYPE type = m_apNumber[nCntScore]->GetType();

			if (type == CNumber::TYPE_TIME)
			{// タイムの場合
				// ナンバー設定
				m_apNumber[nCntScore]->Set(aTexU[nCntScore]);
			}
		}
	}
}