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
#include "bossBattle.h"

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
	m_aIdxXFile[MODEL_NORMAL] = CManager::GetInstance()->GetXFile()->Regist("data\\MODEL\\boxNormal000.x");
	m_aIdxXFile[MODEL_NORMALWIDE] = CManager::GetInstance()->GetXFile()->Regist("data\\MODEL\\boxNormal001.x");
	m_aIdxXFile[MODEL_DAMAGE] = CManager::GetInstance()->GetXFile()->Regist("data\\MODEL\\boxDamage.x");

	FILE *pFile = NULL;

	// ファイルを開く
	if (CManager::GetMode() == CScene::MODE_GAME)
	{
		pFile = fopen("data\\TXT\\model.txt", "r");
	}
	else if (CManager::GetMode() == CScene::MODE_BOSS)
	{
		pFile = fopen("data\\TXT\\bossBattle.txt", "r");
	}

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
		MessageBox(hWnd, "[model.txt]の読み込みに失敗！", "警告！", MB_ICONWARNING);
	}

	//// ファイル作成
	//FILE *pFile = fopen("data\\TXT\\map000.csv", "r");

	//if (pFile != NULL)
	//{// 読み込み成功
	//	char aTemp[MAX_NAME];
	//	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 700.0f, -600.0f);
	//	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//	// １行目データ読み込み
	//	fscanf(pFile, "%s\n", &aTemp[0]);

	//	// ２行目以降読み込み
	//	while (1)
	//	{
	//		int aType = -1;

	//		int result = fscanf(pFile, "%d", &aType);

	//		if (result == EOF)
	//		{// 終了判定
	//			break;
	//		}

	//		if (aType == 5)
	//		{
	//			pos.z += 100.0f;
	//			pos.y = 700.0f;
	//		}
	//		else
	//		{
	//			CObjectX::Create(pos, rot, (MODEL)aType, 3);
	//			pos.y -= 100.0f;
	//		}
	//	}

	//	// ファイル閉じる
	//	fclose(pFile);
	//}
	//else
	//{
	//	MessageBox(hWnd, "[map000.csv]の読み込みに失敗！", "警告！", MB_ICONWARNING);
	//}
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
	if (type == MODEL_DAMAGE)
	{
		pObjX->SetType(TYPE_BOXDAMAGE);
	}
	else
	{
		pObjX->SetType(TYPE_BOXNORMAL);
	}

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
	m_vtxMin = CManager::GetInstance()->GetXFile()->GetSizeMin(m_aIdxXFile[m_modelType]);
	m_vtxMax = CManager::GetInstance()->GetXFile()->GetSize(m_aIdxXFile[m_modelType]);

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
	//CGame::GetPlayer()->CollisionObjX(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);
	//CGame::GetEnemy()->CollisionObjX(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);
}

//===============================================
// 描画処理
//===============================================
void CObjectX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスの取得
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
	CManager::GetInstance()->GetXFile()->Draw(m_aIdxXFile[m_modelType], CXFile::COL_NORMAL);

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
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスの取得
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
	CManager::GetInstance()->GetXFile()->Draw(nIdx, col);
}

//===============================================
// プレイヤーとの当たり判定
//===============================================
bool CObjectX::CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bLand = false;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = CObject::GetTop(nCntPriority);		// 先頭のオブジェクトを代入
		
		while (pObject != NULL)
		{// 使用されている
			CObject *pObjectNext = pObject->GetNext();		// 次のオブジェクトを保存
			CObject::TYPE type = pObject->GetType();		// 種類を取得
			D3DXVECTOR3 pos = pObject->GetPos();			// 位置
			D3DXVECTOR3 posOld = pObject->GetPosOld();		// 前回の位置
			D3DXVECTOR3 sizeMax = pObject->GetSize();		// 最大サイズ
			D3DXVECTOR3 sizeMin = pObject->GetSizeMin();	// 最小サイズ

			if (type == CObject::TYPE_BOXNORMAL || type == CObject::TYPE_BOXDAMAGE)
			{// プレイヤー
				if (pos.x + sizeMin.x - vtxMax.x <= pPos->x && pos.x + sizeMax.x - vtxMin.x >= pPos->x
					&& pos.z + sizeMin.z <= pPos->z - vtxMin.z && pos.z + sizeMax.z >= pPos->z + vtxMin.z
					&& pos.y + sizeMin.y <= pPos->y + vtxMax.y && pos.y + sizeMax.y >= pPos->y + vtxMin.y)
				{// 範囲内にある
					CPlayer::EState state = CGame::GetPlayer()->GetState();

					if (posOld.y + sizeMax.y <= pPosOld->y + vtxMin.y
						&& pos.y + sizeMax.y >= pPos->y + vtxMin.y)
					{// 上からめり込んだ
						// 上にのせる
						if (CManager::GetMode() == CScene::MODE_GAME)
						{
							CGame::GetPlayer()->SetPos(D3DXVECTOR3(pPos->x, posOld.y - vtxMin.y + sizeMax.y, pPos->z));
							CGame::GetPlayer()->SetMove(D3DXVECTOR3(CGame::GetPlayer()->GetMove().x, 0.0f, CGame::GetPlayer()->GetMove().z));
						}
						else
						{
							CBossBattle::GetPlayer()->SetPos(D3DXVECTOR3(pPos->x, posOld.y - vtxMin.y + sizeMax.y, pPos->z));
							CBossBattle::GetPlayer()->SetMove(D3DXVECTOR3(CGame::GetPlayer()->GetMove().x, 0.0f, CGame::GetPlayer()->GetMove().z));
						}

						if (pObject->GetType() == CObject::TYPE_BOXNORMAL)
						{
							if (state == CPlayer::STATE_HIPDROP)
							{// ヒップドロップ中
								if (CManager::GetMode() == CScene::MODE_GAME)
								{
									CGame::GetPlayer()->SetState(CPlayer::STATE_LANDDROP);
								}
								else
								{
									CBossBattle::GetPlayer()->SetState(CPlayer::STATE_LANDDROP);
								}
							}
							else if (state != CPlayer::STATE_LANDDROP && state != CPlayer::STATE_DAMAGE && state != CPlayer::STATE_DASH)
							{
								if (CManager::GetMode() == CScene::MODE_GAME)
								{
									CGame::GetPlayer()->SetState(CPlayer::STATE_NORMAL);
								}
								else
								{
									CBossBattle::GetPlayer()->SetState(CPlayer::STATE_NORMAL);
								}
							}
						}
						else
						{
							if (state == CPlayer::STATE_HIPDROP)
							{// ヒップドロップ中
								if (CManager::GetMode() == CScene::MODE_GAME)
								{
									CGame::GetPlayer()->SetState(CPlayer::STATE_JUMPDROP);
								}
								else
								{
									CBossBattle::GetPlayer()->SetState(CPlayer::STATE_JUMPDROP);
								}
							}
							else if (state != CPlayer::STATE_DAMAGE && state != CPlayer::STATE_JUMPDROP && state != CPlayer::STATE_DASH)
							{// 当たった
								if (CManager::GetMode() == CScene::MODE_GAME)
								{
									CGame::GetPlayer()->SetState(CPlayer::STATE_DAMAGE);
								}
								else
								{
									CBossBattle::GetPlayer()->SetState(CPlayer::STATE_DAMAGE);
								}
							}
						}

						bLand = true;
					}
					else if (posOld.y + sizeMin.y >= pPosOld->y + vtxMax.y
						&& pos.y + sizeMin.y <= pPos->y + vtxMax.y)
					{// 下からめり込んだ
						// 下に戻す
						if (CManager::GetMode() == CScene::MODE_GAME)
						{
							CGame::GetPlayer()->SetPos(D3DXVECTOR3(pPos->x, posOld.y - vtxMax.y + sizeMin.y, pPos->z));
							CGame::GetPlayer()->SetMove(D3DXVECTOR3(CGame::GetPlayer()->GetMove().x, 0.0f, CGame::GetPlayer()->GetMove().z));
						}
						else
						{
							CBossBattle::GetPlayer()->SetPos(D3DXVECTOR3(pPos->x, posOld.y - vtxMax.y + sizeMin.y, pPos->z));
							CBossBattle::GetPlayer()->SetMove(D3DXVECTOR3(CGame::GetPlayer()->GetMove().x, 0.0f, CGame::GetPlayer()->GetMove().z));
						}

						if (pObject->GetType() == CObject::TYPE_BOXDAMAGE)
						{
							if (state != CPlayer::STATE_DAMAGE && state != CPlayer::STATE_JUMPDROP && state != CPlayer::STATE_DASH)
							{// 当たった
								if (CManager::GetMode() == CScene::MODE_GAME)
								{
									CGame::GetPlayer()->SetState(CPlayer::STATE_DAMAGE);
								}
								else
								{
									CBossBattle::GetPlayer()->SetState(CPlayer::STATE_DAMAGE);
								}
							}
						}
					}
					else if (posOld.z + sizeMin.z >= pPosOld->z - vtxMin.z
						&& pos.z + sizeMin.z <= pPos->z - vtxMin.z)
					{// 左から右にめり込んだ
						// 位置を戻す
						if (CManager::GetMode() == CScene::MODE_GAME)
						{
							CGame::GetPlayer()->SetPos(D3DXVECTOR3(pPos->x, pPos->y, posOld.z + vtxMin.z + sizeMin.z));
						}
						else
						{
							CBossBattle::GetPlayer()->SetPos(D3DXVECTOR3(pPos->x, pPos->y, posOld.z + vtxMin.z + sizeMin.z));
						}

						if (pObject->GetType() == CObject::TYPE_BOXDAMAGE)
						{
							if (state != CPlayer::STATE_DAMAGE && state != CPlayer::STATE_JUMPDROP && state != CPlayer::STATE_DASH)
							{// 当たった
								if (CManager::GetMode() == CScene::MODE_GAME)
								{
									CGame::GetPlayer()->SetState(CPlayer::STATE_DAMAGE);
								}
								else
								{
									CBossBattle::GetPlayer()->SetState(CPlayer::STATE_DAMAGE);
								}
							}
						}
					}
					else if (posOld.z + sizeMax.z <= pPosOld->z + vtxMin.z
						&& pos.z + sizeMax.z >= pPos->z + vtxMin.z)
					{// 右から左にめり込んだ
						// 位置を戻す
						if (CManager::GetMode() == CScene::MODE_GAME)
						{
							CGame::GetPlayer()->SetPos(D3DXVECTOR3(pPos->x, pPos->y, posOld.z - vtxMin.z + sizeMax.z));
						}
						else
						{
							CBossBattle::GetPlayer()->SetPos(D3DXVECTOR3(pPos->x, pPos->y, posOld.z - vtxMin.z + sizeMax.z));
						}

						if (pObject->GetType() == CObject::TYPE_BOXDAMAGE)
						{
							if (state != CPlayer::STATE_DAMAGE && state != CPlayer::STATE_JUMPDROP && state != CPlayer::STATE_DASH)
							{// 当たった
								if (CManager::GetMode() == CScene::MODE_GAME)
								{
									CGame::GetPlayer()->SetState(CPlayer::STATE_DAMAGE);
								}
								else
								{
									CBossBattle::GetPlayer()->SetState(CPlayer::STATE_DAMAGE);
								}
							}
						}
					}
				}
			}
			pObject = pObjectNext;		// 次のオブジェクトを代入
		}
	}

	return bLand;
}

//===============================================
// 敵との当たり判定
//===============================================
bool CObjectX::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pRot, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bLand = false;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = CObject::GetTop(nCntPriority);		// 先頭のオブジェクトを代入

		while (pObject != NULL)
		{// 使用されている
			CObject *pObjectNext = pObject->GetNext();		// 次のオブジェクトを保存
			CObject::TYPE type = pObject->GetType();		// 種類を取得
			D3DXVECTOR3 pos = pObject->GetPos();			// 位置
			D3DXVECTOR3 posOld = pObject->GetPosOld();		// 前回の位置
			D3DXVECTOR3 sizeMax = pObject->GetSize();		// 最大サイズ
			D3DXVECTOR3 sizeMin = pObject->GetSizeMin();	// 最小サイズ

			if (type == CObject::TYPE_BOXNORMAL || type == CObject::TYPE_BOXDAMAGE)
			{// プレイヤー
				if (pos.x + sizeMin.x - vtxMax.x <= pPos->x && pos.x + sizeMax.x - vtxMin.x >= pPos->x
					&& pos.z + sizeMin.z <= pPos->z - vtxMin.z && pos.z + sizeMax.z >= pPos->z + vtxMin.z
					&& pos.y + sizeMin.y <= pPos->y + vtxMax.y && pos.y + sizeMax.y >= pPos->y + vtxMin.y)
				{// 範囲内にある
					if (posOld.y + sizeMax.y <= pPosOld->y + vtxMin.y
						&& pos.y + sizeMax.y >= pPos->y + vtxMin.y)
					{// 上からめり込んだ
						// 上にのせる
						pPos->y = posOld.y - vtxMin.y + sizeMax.y;
						pMove->y = 0.0f;

						bLand = true;
					}
					else if (posOld.y + sizeMin.y >= pPosOld->y + vtxMax.y
						&& pos.y + sizeMin.y <= pPos->y + vtxMax.y)
					{// 下からめり込んだ
						// 下に戻す
						pPos->y = posOld.y - vtxMax.y + sizeMin.y;
						//CGame::GetPlayer()->SetMove(D3DXVECTOR3(CGame::GetPlayer()->GetMove().x, 0.0f, CGame::GetPlayer()->GetMove().z));
					}
					else if (posOld.z + sizeMin.z >= pPosOld->z - vtxMin.z
						&& pos.z + sizeMin.z <= pPos->z - vtxMin.z)
					{// 左から右にめり込んだ
						// 位置を戻す
						pPos->z = posOld.z + vtxMin.z + sizeMin.z;
						pRot->y = D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y);
					}
					else if (posOld.z + sizeMax.z <= pPosOld->z + vtxMin.z
						&& pos.z + sizeMax.z >= pPos->z + vtxMin.z)
					{// 右から左にめり込んだ
						// 位置を戻す
						pPos->z = posOld.z - vtxMin.z + sizeMax.z;
						pRot->y = D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y);
					}
				}
			}
			pObject = pObjectNext;		// 次のオブジェクトを代入
		}
	}

	return bLand;
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
void CObjectX::SetSize(D3DXVECTOR3 size)
{
	m_vtxMax = size;
}

//===============================================
// サイズ取得
//===============================================
D3DXVECTOR3 CObjectX::GetSize(void)
{
	return CManager::GetInstance()->GetXFile()->GetSize(m_modelType - 1);
}

//===============================================
// サイズ取得
//===============================================
void CObjectX::SetSizeMin(D3DXVECTOR3 size)
{
	m_vtxMin = size;
}

//===============================================
// サイズ取得
//===============================================
D3DXVECTOR3 CObjectX::GetSizeMin(void)
{
	return CManager::GetInstance()->GetXFile()->GetSizeMin(m_modelType - 1);
}