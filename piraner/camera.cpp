//=========================================================
//
// カメラ処理 [camera.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "game.h"
#include "debugproc.h"

//===============================================
// マクロ定義
//===============================================
#define MOVE_CAMERA			(2.0f)		// 移動量
#define RAD_CAMERA			(60.0f)		// 基本軸からの回転角度
#define SLANTING			(0.25f)		// 斜めの角度
#define MOVE_X				(0.5f)		// X方向への移動
#define PI_SLASH			(100.0f)	// IPを割る数
#define LENGTH_CAMERA		(-300.0f)	// 視点の距離
#define HIGHT_CAMERA		(250.0f)	// カメラの高さ
#define MOVEV_MINUS			(0.3f)		// 視点の移動量の減衰
#define MOVER_MINUS			(0.2f)		// 注視点の移動量の減衰
#define POSR_DEST			(-300.0f)	// 注視点の少し前に設定
#define CAMERA_ROT_SPEED	(0.7f)		// カメラがプレイヤーの背後に回り込むまでのスピード
#define CAMERA_ROT_MOVE		(70)		// 向きが変わるタイミング
#define POS_DIFF			(0.15f)		// 位置の値の補正
#define MAPVIEW_TIME		(80)		// 上空カメラになっている時間
#define TRUE_LENTH			(3.0f)		// 補正完了値

//===============================================
// コンストラクタ
//===============================================
CCamera::CCamera()
{
	// 値のクリア
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
	m_nViewMapCounter = 0;
}

//===============================================
// デストラクタ
//===============================================
CCamera::~CCamera()
{
	
}

//===============================================
// 初期化処理
//===============================================
HRESULT CCamera::Init(void)
{
	// 値を初期化
	m_posV = D3DXVECTOR3(0.0f, HIGHT_CAMERA, 300.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, -400.0f);

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_fLength = LENTH_NORMAL;
	m_nViewMapCounter = MAPVIEW_TIME;

	D3DXVECTOR3 playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 playerRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// カメラの追従
	// 目的の視点・注視点を設定
	m_posRDest.x = playerPos.x + sinf(playerRot.y) * POSR_DEST;
	m_posRDest.y = playerPos.y;
	m_posRDest.z = playerPos.z + cosf(playerRot.y) * POSR_DEST;
	m_posVDest.x = playerPos.x + sinf(m_rot.y) * LENGTH_CAMERA;
	m_posVDest.y = playerPos.y;
	m_posVDest.z = playerPos.z + cosf(m_rot.y) * LENGTH_CAMERA;

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CCamera::Uninit(void)
{
	
}

//===============================================
// 更新処理
//===============================================
void CCamera::Update(void)
{
	D3DXVECTOR3 playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// プレイヤーの位置
	D3DXVECTOR3 playerPosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// プレイヤーの前回の位置
	D3DXVECTOR3 playerRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// プレイヤーの向き

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = CObject::GetTop(nCntPriority);		// 先頭のオブジェクトを代入

		while (pObject != NULL)
		{// 使用されている
			CObject *pObjectNext = pObject->GetNext();		// 次のオブジェクトを保存
			CObject::TYPE type = pObject->GetType();		// 種類を取得

			if (type == CObject::TYPE_PLAYER)
			{// プレイヤー
				playerPos = pObject->GetPos();			// 位置を取得
				playerPosOld = pObject->GetPosOld();	// 前回の位置を取得
				playerRot = pObject->GetRot();			// 向きを取得
			}

			pObject = pObjectNext;		// 次のオブジェクトを代入
		}
	}

	// カメラの追従
	// 目的の視点・注視点を設定
	m_posRDest.x = playerPos.x;
	m_posRDest.y = playerPos.y + 400.0f;
	m_posRDest.z = playerPos.z;
	m_posVDest.x = playerPos.x;
	m_posVDest.y = playerPos.y + 400.0f;
	m_posVDest.z = playerPos.z;

	// 移動量を更新（減衰させる）
	m_posV.x += (m_posVDest.x - m_posV.x) * MOVEV_MINUS;
	m_posV.y += (m_posVDest.y - m_posV.y) * MOVEV_MINUS;
	m_posV.z += (m_posVDest.z - m_posV.z) * MOVEV_MINUS;
	m_posR.x += (m_posRDest.x - m_posR.x) * MOVER_MINUS;
	m_posR.y += (m_posVDest.y - m_posV.y) * MOVEV_MINUS;
	m_posR.z += (m_posRDest.z - m_posR.z) * MOVER_MINUS;

	//注視点と一緒に視点も動かす
	m_posV.x = m_posV.x + m_fLength;
	m_posV.z = m_posV.z;
}

//===============================================
// 拡縮処理
//===============================================
bool CCamera::ScalingLenth(const float fLenth, float fMalti)
{
	// 目的のサイズまでの差分を計算
	float fLenthDiff = fLenth - m_fLength;

	// サイズの補正
	m_fLength += fLenthDiff * fMalti;

	if (m_fLength <= fLenth + TRUE_LENTH && m_fLength >= fLenth - TRUE_LENTH)
	{
		return true;
	}

	return false;
}

//===============================================
// 設定処理
//===============================================
void CCamera::Set(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		8000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
						&m_posV,
						&m_posR,
						&m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}