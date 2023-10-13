//=========================================================
// 
// メッシュフィールド描画処理 [meshfield.cpp]
// Author = 阿部翔大郎
// 
//=========================================================
#include "main.h"
#include "meshfield.h"
#include "texture.h"
#include "input.h"
#include "debugproc.h"
#include "game.h"
#include "tutorial.h"
#include "title.h"
#include "renderer.h"

//===============================================
// マクロ定義
//===============================================
#define FIELD_COUNTX			(6)			// 床の配置数（X）
#define FIELD_COUNTY			(6)			// 床の配置数（Y）
#define MIN_FIELDPOS			(9900.0f)	// 床最小位置初期値
#define MAX_FIELDPOS			(-9900.0f)	// 床最大位置初期値

#define TURN_ROT_DIFF			(0.001f)	// 曲がる角度の差分
#define TURN_LENTH				(0.25f)		// 曲がれる範囲
#define TURN_DIFF				(50.0f)		// 曲がれる差分

#define LENTH_FAR				(240.0f)	// 道の中心からの距離（遠い）
#define LENTH_GABAGE			(180.0f)	// 道の中心からの距離（中）
#define LENTH_NEAR				(150.0f)	// 道の中心からの距離（近い）
#define GABAGE_POSITION			(280.0f)	// ゴミの置かれる位置
#define DISTANCE_HEIGHT			(100.0f)	// ゴミ同士の縦間隔
#define DISTANCE_WIDTH			(220.0f)		// ゴミ同士の横間隔

// TUTORIAL
#define DELETE_POSITION			(650.0f)	// 床を破棄する位置

//===============================================
// 静的メンバ変数
//===============================================
int CMeshField::m_nIdxTexture = NULL;				// 使用するテクスチャの番号
LPDIRECT3DTEXTURE9 CMeshField::m_pTexture = NULL;	// テクスチャへのポインタ
CMeshField::TYPE CMeshField::m_aType[MAX_FIELD] = {};		// 床の種類
int CMeshField::m_nNumAll = 0;								// 総数

//===============================================
// コンストラクタ
//===============================================
CMeshField::CMeshField() : CObjectMesh(3)
{
	//値をクリアする
	m_nTimeFever = 0;
	m_nRandFever = 0;

	m_nNumAll++;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CMeshField::CMeshField(int nPriority) : CObjectMesh(nPriority)
{
	//値をクリアする
	m_nTimeFever = 0;
	m_nRandFever = 0;

	m_nNumAll++;
}

//===============================================
// デストラクタ
//===============================================
CMeshField::~CMeshField()
{
	m_nNumAll--;
}

//===============================================
// 生成処理
//===============================================
CMeshField *CMeshField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSizeX, float fSizeZ, int nPriority)
{
	CMeshField *pObjectMesh;

	// メッシュフィールドの生成
	pObjectMesh = new CMeshField(nPriority);

	// 種類の設定
	pObjectMesh->SetType(TYPE_FIELD);

	// サイズの設定
	pObjectMesh->SetSize(fSizeX, fSizeZ);

	// 初期化処理
	pObjectMesh->Init(pos);

	// 設定処理
	pObjectMesh->Set(pos, rot);

	// テクスチャの割り当て
	pObjectMesh->BindTexture(m_nIdxTexture);

	return pObjectMesh;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CMeshField::Init(D3DXVECTOR3 pos)
{
	// オブジェクトメッシュの初期化処理
	CObjectMesh::Init(pos);

	m_PosMin = D3DXVECTOR3(MIN_FIELDPOS, MIN_FIELDPOS, MIN_FIELDPOS);
	m_PosMax = D3DXVECTOR3(MAX_FIELDPOS, MAX_FIELDPOS, MAX_FIELDPOS);
	
	// 床の位置の最小値を比較
	if (m_pos.x <= m_PosMin.x)
	{// 最小値を更新
		m_PosMin.x = m_pos.x;
	}

	if (m_pos.z <= m_PosMin.z)
	{// 最小値を更新
		m_PosMin.z = m_pos.z;
	}

	// 床の位置の最大値を比較
	if (m_pos.x >= m_PosMax.x)
	{// 最大値を更新
		m_PosMax.x = m_pos.x;
	}

	if (m_pos.z >= m_PosMax.z)
	{// 最大値を更新
		m_PosMax.z = m_pos.z;
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CMeshField::Uninit(void)
{
	// オブジェクトメッシュの終了処理
	CObjectMesh::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CMeshField::Update(void)
{
	
}

//===============================================
// 描画処理
//===============================================
void CMeshField::Draw(void)
{
	// オブジェクトメッシュの描画処理
	CObjectMesh::Draw();
}

//===============================================
// 設定処理
//===============================================
void CMeshField::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;

	srand(timeGetTime());										// randの値を初期化
	int nNumXRand = 1;											// randの値を取得（数X）
	int nNumYRand = rand() % 2 + 1;								// randの値を取得（数Y）

	if (CManager::GetMode() == CScene::MODE_GAME)
	{// ゲームモード
		
	}
}