//=========================================================
//
// オブジェクトXファイル処理 [objectX.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "objectX.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "xfile.h"
#include "game.h"
#include "player.h"
#include "enemy.h"

//===============================================
// 静的メンバ変数
//===============================================
int CObjectX::m_aIdxXFile[MODEL_MAX] = {};	// 使用するXファイルの番号

//===============================================
// コンストラクタ
//===============================================
CObjectX::CObjectX() : CObject(3)
{
	// 値をクリアする
	m_modelType = MODEL_NONE;
	m_mtxWorld;
	m_nIdxShadow = -1;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	// 値をクリアする
	m_modelType = MODEL_NONE;
	m_mtxWorld;
	m_nIdxShadow = -1;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//===============================================
// デストラクタ
//===============================================
CObjectX::~CObjectX()
{

}

//===============================================
// 読み込み処理
//===============================================
void CObjectX::Load(HWND hWnd)
{
	// モデル読み込み
	m_aIdxXFile[MODEL_HOUSE00] = CManager::GetXFile()->Regist("data\\MODEL\\Dumpster.x");

	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data\\TXT\\model.txt", "r");

	if (pFile != NULL)
	{// 読み込み成功
		char aStr[MAX_NAME] = {};

		while (1)
		{
			int nResult = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], "SCRIPT") == 0)
			{// SCRIPT情報読み込み
				Script(pFile);
				break;
			}
			else if (nResult == EOF)
			{// 最後まで読み込んだ
				break;
			}
		}

		// ファイル閉じる
		fclose(pFile);
	}
	else
	{
		MessageBox(hWnd, "ファイルの読み込みに失敗！", "警告！", MB_ICONWARNING);
	}
}

//===============================================
// Script情報読み込み処理
//===============================================
void CObjectX::Script(FILE *pFile)
{
	char aStr[MAX_NAME] = {};

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "MODELSET") == 0)
		{// モデル情報読み込み
			ModelSet(pFile);
		}
		else if (nResult == -1 || strcmp(&aStr[0], "END_SCRIPT") == 0)
		{// 最後まで読み込んだ
			break;
		}
	}
}

//===============================================
// Model情報読み込み処理
//===============================================
void CObjectX::ModelSet(FILE *pFile)
{
	char aStr[MAX_NAME] = {};
	MODEL type;					// 種類
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	D3DXVECTOR3 size;			// サイズ

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "TYPE") == 0)
		{// 種類読み込み
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%d", &type);
		}
		else if (strcmp(&aStr[0], "POS") == 0)
		{// 位置読み込み
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%f", &pos.x);
			fscanf(pFile, "%f", &pos.y);
			fscanf(pFile, "%f", &pos.z);
		}
		else if (strcmp(&aStr[0], "ROT") == 0)
		{// 向き読み込み
			fscanf(pFile, "%s", &aStr[0]);	// (=)読み込み
			fscanf(pFile, "%f", &rot.x);
			fscanf(pFile, "%f", &rot.y);
			fscanf(pFile, "%f", &rot.z);
		}

		if (strcmp(&aStr[0], "END_MODELSET") == 0)
		{// 最後まで読み込んだ
			CObjectX::Create(pos, rot, type, 3);
			break;
		}
	}
}

//===============================================
// 生成処理
//===============================================
CObjectX *CObjectX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL type, int nPriority)
{
	CObjectX *pObjX;

	// オブジェクトの生成
	pObjX = new CObjectX(nPriority);

	// 種類の設定
	pObjX->SetType(TYPE_XFAIL);

	// モデルの設定
	pObjX->SetModel(type);

	// 初期化処理
	pObjX->Init(pos);

	// 向き設定
	pObjX->SetRot(rot);

	return pObjX;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CObjectX::Init(D3DXVECTOR3 pos)
{
	//// 影を設定
	//m_nIdxShadow = SetShadow();

	// 位置を反映
	m_pos = pos;

	// モデルの最小値・最大値の取得
	m_vtxMin = CManager::GetXFile()->GetSizeMin(m_aIdxXFile[m_modelType]);
	m_vtxMax = CManager::GetXFile()->GetSize(m_aIdxXFile[m_modelType]);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CObjectX::Uninit(void)
{
	this->Release();
}

//===============================================
// 更新処理
//===============================================
void CObjectX::Update(void)
{
	D3DXVECTOR3 playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// プレイヤーの位置
	D3DXVECTOR3 playerPosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// プレイヤーの前回の位置
	D3DXVECTOR3 playerRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// プレイヤーの向き
	CObject *pObj = NULL;

	m_posOld = m_pos;

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
				pObj = pObject;
			}

			pObject = pObjectNext;		// 次のオブジェクトを代入
		}
	}

	//CollisionModel(&playerPos, &playerPosOld, 0, pObj, true);

	// 当たり判定
	CGame::GetPlayer()->CollisionObjX(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);
	CGame::GetEnemy()->CollisionObjX(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);
}

//===============================================
// 描画処理
//===============================================
void CObjectX::Draw(void)
{
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

	// Xファイルの描画
	CManager::GetXFile()->Draw(m_aIdxXFile[m_modelType], CXFile::COL_NORMAL);

	//// 現在のマテリアルを取得
	//pDevice->GetMaterial(&matDef);

	//// マテリアルデータへのポインタを取得
	//pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	//{
	//	// マテリアルの設定
	//	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

	//	// テクスチャの設定
	//	pDevice->SetTexture(0, m_apTexture[nCntMat]);

	//	// モデル（パーツ）の描画
	//	m_pMesh->DrawSubset(nCntMat);
	//}

	//// 保存していたマテリアルを戻す
	//pDevice->SetMaterial(&matDef);
}

//===============================================
// 描画処理
//===============================================
void CObjectX::DrawXFile(int nIdx, CXFile::COL col)
{
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

	// Xファイルの描画
	CManager::GetXFile()->Draw(nIdx, col);
}

//===============================================
// プレイヤーとの当たり判定
//===============================================
void CObjectX::CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, int nIdx, CObject *pObj, bool bPlayer)
{
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(900.0f, 900.0f, 900.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(-900.0f, -900.0f, -900.0f);
	D3DXVECTOR3 movePlayer = CGame::GetPlayer()->GetMove();

	if (bPlayer == false)
	{
		vtxMax = CManager::GetXFile()->GetSize(nIdx);
		vtxMin = CManager::GetXFile()->GetSizeMin(nIdx);
	}
	else
	{
		vtxMax = CGame::GetPlayer()->GetSize();
		vtxMin = CGame::GetPlayer()->GetSizeMin();
	}

	if (m_pos.x + m_vtxMin.x - vtxMax.x <= pPos->x && m_pos.x + m_vtxMax.x - vtxMin.x >= pPos->x
		&& m_pos.z + m_vtxMin.z - vtxMax.z <= pPos->z && m_pos.z + m_vtxMax.z - vtxMin.z >= pPos->z
		&& m_pos.y + m_vtxMin.y - vtxMax.y <= pPos->y && m_pos.y + m_vtxMax.y - vtxMin.y >= pPos->y)
	{// 範囲内にある
		if (m_posOld.y + m_vtxMax.y <= pPosOld->y + vtxMin.y
			&& m_pos.y + m_vtxMax.y >= pPos->y + vtxMin.y)
		{// 上からめり込んだ
			// 上にのせる
			pObj->SetPos(D3DXVECTOR3(pPos->x, m_pos.y - vtxMin.y + m_vtxMax.y, pPos->z));
			pObj->SetMove(D3DXVECTOR3(movePlayer.x, 0.0f, movePlayer.z));
			CGame::GetPlayer()->SetJump(false);
		}
		else if (m_posOld.y + m_vtxMin.y >= pPosOld->y + vtxMax.y
				&& m_pos.y + m_vtxMin.y <= pPos->y + vtxMax.y)
		{// 下からめり込んだ
			// 下に戻す
			pObj->SetPos(D3DXVECTOR3(pPos->x, m_pos.y - vtxMax.y + m_vtxMin.y, pPos->z));
			pObj->SetMove(D3DXVECTOR3(movePlayer.x, 0.0f, movePlayer.z));
		}
		else if (m_posOld.z - m_vtxMax.z >= pPosOld->z + vtxMax.z
				&& m_pos.z - m_vtxMax.z <= pPos->z + vtxMax.z)
		{// 左から右にめり込んだ
			// 位置を戻す
			pObj->SetPos(D3DXVECTOR3(pPos->x, pPos->y, m_pos.z - vtxMax.z - m_vtxMax.z));
		}
		else if (m_posOld.z - m_vtxMin.z <= pPosOld->z - vtxMax.z
				&& m_pos.z - m_vtxMin.z >= pPos->z - vtxMax.z)
		{// 右から左にめり込んだ
			// 位置を戻す
			pObj->SetPos(D3DXVECTOR3(pPos->x, pPos->y, m_pos.z + vtxMax.z + m_vtxMax.z));
		}
	}
}

//===============================================
// モデルタイプ設定
//===============================================
void CObjectX::SetModel(MODEL type)
{
	m_modelType = type;
}

//===============================================
// 位置設定
//===============================================
void CObjectX::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===============================================
// 向き設定
//===============================================
void CObjectX::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//===============================================
// 移動量設定
//===============================================
void CObjectX::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//===============================================
// サイズ取得
//===============================================
D3DXVECTOR3 CObjectX::GetSize(void)
{
	return CManager::GetXFile()->GetSize(0);
}