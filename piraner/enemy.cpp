//=========================================================
//
// 敵処理 [enemy.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "enemy.h"
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "sound.h"
#include "camera.h"
#include "object.h"
#include "particle.h"
#include "model.h"
#include "meshfield.h"
#include "motion.h"
#include "fileload.h"
#include "shadow.h"
#include "object3D.h"
#include "game.h"
#include "tutorial.h"
#include "sound.h"
#include "title.h"

//===============================================
// マクロ定義
//===============================================
#define MOVE_PLAYER			(0.45f)		// 動く速度
#define JUMP_PLAYER			(16.85f)	// ジャンプ力
#define MOVE_GRAVITY		(0.75f)		// 重力

#define JUMP_HIPDROP		(10.85f)	// ヒップドロップ初動の浮力
#define MOVE_HIPDROP		(1.2f)		// ヒップドロップ中重力

#define MOVE_MINUS			(0.07f)		// 移動量の減衰
#define TURN_TIME			(1)			// 曲がる時間
#define COUNT_PARTICLE		(25)		// パーティクル発生時間

//===============================================
// コンストラクタ
//===============================================
CEnemy::CEnemy() : CObject(4)
{
	// 値をクリアする
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld;
	m_fSpeed = 0.0f;
	m_bJump = false;
	m_apModel[MAX_MODEL] = {};
	m_nNumModel = 0;
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = STATE_NONE;
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CEnemy::CEnemy(int nPriority) : CObject(nPriority)
{
	// 値をクリアする
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld;
	m_fSpeed = 0.0f;
	m_bJump = false;
	m_apModel[MAX_MODEL] = {};
	m_nNumModel = 0;
	m_pMotion = NULL;
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = STATE_NONE;
}

//===============================================
// デストラクタ
//===============================================
CEnemy::~CEnemy()
{

}

//===============================================
// 生成処理
//===============================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, int nPriority)
{
	CEnemy *pEnemy;

	// プレイヤーの生成
	pEnemy = new CEnemy(nPriority);

	// 種類の設定
	pEnemy->SetType(CObject::TYPE_ENEMY);
	
	// 初期化処理
	pEnemy->Init(pos);

	return pEnemy;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	// モーションの生成
	m_pMotion = new CMotion;

	// モーションの初期化
	m_pMotion->Init();

	// モデルの総数
	m_nNumModel = CManager::GetLoad()->GetNumModel();

	// 位置の設定
	m_pos = pos;

	// 移動速度の初期化
	m_fSpeed = MOVE_PLAYER;

	const char *apModelFile[MAX_MODEL];		// モデルファイル名

	// モデルの生成（全パーツ）
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		apModelFile[nCntModel] = CManager::GetLoad()->GetFileName(nCntModel);		// ファイル名取得
		pos = CManager::GetLoad()->GetPos(nCntModel);								// 位置の取得
		rot = CManager::GetLoad()->GetRot(nCntModel);								// 向きの取得

		m_apModel[nCntModel] = CModel::Create(apModelFile[nCntModel], pos, rot);	// 生成
	}
	//m_apModel[0] = CModel::Create("data\\MODEL\\GarbageCan_Wood.x", pos, D3DXVECTOR3(0.0f, 0.0f , 0.0f));	// 生成

	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);

	// 親モデルの設定（全パーツ）
	m_apModel[0]->SetParent(NULL);
	for (int nCntModel = 1; nCntModel < m_nNumModel; nCntModel++)
	{
		int nParent = 0;

		nParent = CManager::GetLoad()->GetParent(nCntModel);	// 親を取得

		m_apModel[nCntModel]->SetParent(m_apModel[nParent]);
	}

	// 値の初期化
	m_vtxMin = D3DXVECTOR3(900.0f, 900.0f, 900.0f);
	m_vtxMax = D3DXVECTOR3(-900.0f, -900.0f, -900.0f);

	// 全ての頂点を比較してモデルの最小値・最大値を抜き出す
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		// 最小値の比較
		if (m_apModel[nCntModel]->GetVtxMin().x <= m_vtxMin.x)
		{
			m_vtxMin.x = m_apModel[nCntModel]->GetVtxMin().x;
		}

		if (m_apModel[nCntModel]->GetVtxMin().y <= m_vtxMin.y)
		{
			m_vtxMin.y = m_apModel[nCntModel]->GetVtxMin().y;
		}

		if (m_apModel[nCntModel]->GetVtxMin().z <= m_vtxMin.z)
		{
			m_vtxMin.z = m_apModel[nCntModel]->GetVtxMin().z;
		}

		// 最大値の比較
		if (m_apModel[nCntModel]->GetVtxMax().x >= m_vtxMax.x)
		{
			m_vtxMax.x = m_apModel[nCntModel]->GetVtxMax().x;
		}

		if (m_apModel[nCntModel]->GetVtxMax().y >= m_vtxMax.y)
		{
			m_vtxMax.y = m_apModel[nCntModel]->GetVtxMax().y;
		}

		if (m_apModel[nCntModel]->GetVtxMax().z >= m_vtxMax.z)
		{
			m_vtxMax.z = m_apModel[nCntModel]->GetVtxMax().z;
		}
	}

	// モーション情報の設定
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{// モーション数分繰り返す
		m_pMotion->Set(nCntMotion);
		m_pMotion->SetInfo(CManager::GetLoad()->GetInfo(nCntMotion));
	}

	// 初期モーション設定
	m_pMotion->Set(MOTIONTYPE_NEUTRAL);

	// 初期状態設定
	m_state = STATE_NORMAL;

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CEnemy::Uninit(void)
{
	// モデルの破棄
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// 使用されている
			// モデルの終了処理
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}
	}

	// モーションへのポインタの破棄
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	this->Release();
}

//===============================================
// 更新処理
//===============================================
void CEnemy::Update(void)
{
	// 前回の位置を保存
	m_posOld = m_pos;

	if (m_state == STATE_NORMAL)
	{
		//m_move.x += sinf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_fSpeed;
		//m_move.z += cosf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_fSpeed;
		//m_rotDest.y = D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y);
	}

	switch (m_state)
	{
	case STATE_NORMAL:		// 通常
		break;

	case STATE_DASH:		// ダッシュ
		break;

	case STATE_ATTACK:		// 空中攻撃
		break;
	}

	m_fRotDiff = m_rotDest.y - m_rot.y;	// 目的の向きまでの差分

	// 角度の値の補正
	if (m_fRotDiff > D3DX_PI)
	{
		m_fRotDiff += -D3DX_PI * ROT_DIFF_PI;
	}
	else if (m_fRotDiff < -D3DX_PI)
	{
		m_fRotDiff += D3DX_PI * ROT_DIFF_PI;
	}

	m_rot.y += m_fRotDiff * ROT_CAMERA_DIFF;	// 角度の補正

	// 角度の値の補正
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y += -D3DX_PI * ROT_DIFF_PI;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * ROT_DIFF_PI;
	}

	// 重力処理
	m_move.y -= MOVE_GRAVITY;

	// 位置を更新
	m_pos += m_move;

	// 3Dポリゴンとの当たり判定
	//m_pos.y = CManager::GetObject3D()->CollisionVec(m_pos);

	// 移動量を更新（減衰させる）
	m_move.x += (0.0f - m_move.x) * MOVE_MINUS;
	m_move.z += (0.0f - m_move.z) * MOVE_MINUS;

	//if (m_pos.y < LAND_POS)
	//{// 着地した
	//	m_pos.y = LAND_POS;
	//	m_move.y = 0.0f;
	//	m_bJump = false;
	//	m_bAirJump = false;
	//}

	// モーションの更新処理
	m_pMotion->Update();

	// 当たり判定
	CGame::GetPlayer()->CollisionEnemy(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);

	// デバッグ表示
	CManager::GetDebugProc()->Print(" 敵の位置：（%f, %f, %f）\n\n", m_pos.x, m_pos.y, m_pos.z);
}

//===============================================
// 描画処理
//===============================================
void CEnemy::Draw(void)
{
	if (CManager::GetMode() != CScene::MODE_TITLE)
	{// タイトルじゃない
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
		D3DXMATRIX mtxRot, mtxTrans;										// 計算用マトリックス

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// モデルの描画（全パーツ）
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			// モデルの描画処理
			m_apModel[nCntModel]->Draw();

			// モデルの影の描画処理
			m_apModel[nCntModel]->DrawShadowmtx();
		}
	}
}

//===============================================
// オブジェクトXとの当たり判定
//===============================================
void CEnemy::CollisionObjX(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	if (pPosOld->x + vtxMin.x - m_vtxMax.x <= m_pos.x && pPosOld->x + vtxMax.x - m_vtxMin.x >= m_pos.x
		&& pPosOld->z + vtxMin.z - m_vtxMax.z <= m_pos.z && pPosOld->z + vtxMax.z - m_vtxMin.z >= m_pos.z
		&& pPosOld->y + vtxMin.y - m_vtxMax.y <= m_pos.y && pPosOld->y + vtxMax.y - m_vtxMin.y >= m_pos.y)
	{// 範囲内にある
		if (pPosOld->y + vtxMax.y <= m_posOld.y + m_vtxMin.y
			&& pPosOld->y + vtxMax.y >= m_pos.y + m_vtxMin.y)
		{// 上からめり込んだ
			// 上にのせる
			m_pos.y = pPosOld->y - m_vtxMin.y + vtxMax.y;
			m_move.y = 0.0f;
			CGame::GetEnemy()->SetJump(false);
		}
		else if (pPosOld->y + vtxMin.y >= m_posOld.y + m_vtxMax.y
			&& pPosOld->y + vtxMin.y <= m_pos.y + m_vtxMax.y)
		{// 下からめり込んだ
			// 下に戻す
			m_pos.y = pPosOld->y - m_vtxMax.y + vtxMin.y;
			m_move.y = 0.0f;
		}
		else if (pPosOld->z - vtxMax.z >= m_posOld.z + m_vtxMax.z
			&& pPosOld->z - vtxMax.z <= m_pos.z + m_vtxMax.z)
		{// 左から右にめり込んだ
			// 位置を戻す
			m_pos.z = pPosOld->z - m_vtxMax.z - vtxMax.z;
		}
		else if (pPosOld->z - vtxMin.z <= m_posOld.z - m_vtxMax.z
			&& pPosOld->z - vtxMin.z >= m_pos.z - m_vtxMax.z)
		{// 右から左にめり込んだ
			// 位置を戻す
			m_pos.z = pPosOld->z + m_vtxMax.z + vtxMax.z;
		}
	}
}

//===============================================
// 位置の設定
//===============================================
void CEnemy::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===============================================
// 移動量の設定
//===============================================
void CEnemy::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//===============================================
// ジャンプしたかの設定
//===============================================
void CEnemy::SetJump(const bool bJump)
{
	m_bJump = bJump;
}

//===============================================
// モーションの設定
//===============================================
void CEnemy::SetMotion(MOTIONTYPE type)
{
	m_pMotion->Set(type);
}