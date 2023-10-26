//=========================================================
//
// �I�u�W�F�N�gX�t�@�C������ [objectX.cpp]
// Author = �����đ�Y
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
// �ÓI�����o�ϐ�
//===============================================
int CObjectX::m_aIdxXFile[MODEL_MAX] = {};	// �g�p����X�t�@�C���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CObjectX::CObjectX() : CObject(3)
{
	// �l���N���A����
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
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	// �l���N���A����
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
// �f�X�g���N�^
//===============================================
CObjectX::~CObjectX()
{

}

//===============================================
// �ǂݍ��ݏ���
//===============================================
void CObjectX::Load(HWND hWnd)
{
	// ���f���ǂݍ���
	m_aIdxXFile[MODEL_NORMAL] = CManager::GetInstance()->GetXFile()->Regist("data\\MODEL\\boxNormal000.x");
	m_aIdxXFile[MODEL_NORMALWIDE] = CManager::GetInstance()->GetXFile()->Regist("data\\MODEL\\boxNormal001.x");
	m_aIdxXFile[MODEL_DAMAGE] = CManager::GetInstance()->GetXFile()->Regist("data\\MODEL\\boxDamage.x");

	FILE *pFile = NULL;

	// �t�@�C�����J��
	if (CManager::GetMode() == CScene::MODE_GAME)
	{
		pFile = fopen("data\\TXT\\model.txt", "r");
	}
	else if (CManager::GetMode() == CScene::MODE_BOSS)
	{
		pFile = fopen("data\\TXT\\bossBattle.txt", "r");
	}

	if (pFile != NULL)
	{// �ǂݍ��ݐ���
		char aStr[MAX_NAME] = {};

		while (1)
		{
			int nResult = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], "SCRIPT") == 0)
			{// SCRIPT���ǂݍ���
				Script(pFile);
				break;
			}
			else if (nResult == EOF)
			{// �Ō�܂œǂݍ���
				break;
			}
		}

		// �t�@�C������
		fclose(pFile);
	}
	else
	{
		MessageBox(hWnd, "[model.txt]�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}

	//// �t�@�C���쐬
	//FILE *pFile = fopen("data\\TXT\\map000.csv", "r");

	//if (pFile != NULL)
	//{// �ǂݍ��ݐ���
	//	char aTemp[MAX_NAME];
	//	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 700.0f, -600.0f);
	//	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//	// �P�s�ڃf�[�^�ǂݍ���
	//	fscanf(pFile, "%s\n", &aTemp[0]);

	//	// �Q�s�ڈȍ~�ǂݍ���
	//	while (1)
	//	{
	//		int aType = -1;

	//		int result = fscanf(pFile, "%d", &aType);

	//		if (result == EOF)
	//		{// �I������
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

	//	// �t�@�C������
	//	fclose(pFile);
	//}
	//else
	//{
	//	MessageBox(hWnd, "[map000.csv]�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	//}
}

//===============================================
// Script���ǂݍ��ݏ���
//===============================================
void CObjectX::Script(FILE *pFile)
{
	char aStr[MAX_NAME] = {};

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "MODELSET") == 0)
		{// ���f�����ǂݍ���
			ModelSet(pFile);
		}
		else if (nResult == -1 || strcmp(&aStr[0], "END_SCRIPT") == 0)
		{// �Ō�܂œǂݍ���
			break;
		}
	}
}

//===============================================
// Model���ǂݍ��ݏ���
//===============================================
void CObjectX::ModelSet(FILE *pFile)
{
	char aStr[MAX_NAME] = {};
	MODEL type;					// ���
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	D3DXVECTOR3 size;			// �T�C�Y

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "TYPE") == 0)
		{// ��ޓǂݍ���
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%d", &type);
		}
		else if (strcmp(&aStr[0], "POS") == 0)
		{// �ʒu�ǂݍ���
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%f", &pos.x);
			fscanf(pFile, "%f", &pos.y);
			fscanf(pFile, "%f", &pos.z);
		}
		else if (strcmp(&aStr[0], "ROT") == 0)
		{// �����ǂݍ���
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%f", &rot.x);
			fscanf(pFile, "%f", &rot.y);
			fscanf(pFile, "%f", &rot.z);
		}

		if (strcmp(&aStr[0], "END_MODELSET") == 0)
		{// �Ō�܂œǂݍ���
			CObjectX::Create(pos, rot, type, 3);
			break;
		}
	}
}

//===============================================
// ��������
//===============================================
CObjectX *CObjectX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL type, int nPriority)
{
	CObjectX *pObjX;

	// �I�u�W�F�N�g�̐���
	pObjX = new CObjectX(nPriority);

	// ��ނ̐ݒ�
	if (type == MODEL_DAMAGE)
	{
		pObjX->SetType(TYPE_BOXDAMAGE);
	}
	else
	{
		pObjX->SetType(TYPE_BOXNORMAL);
	}

	// ���f���̐ݒ�
	pObjX->SetModel(type);

	// ����������
	pObjX->Init(pos);

	// �����ݒ�
	pObjX->SetRot(rot);

	return pObjX;
}

//===============================================
// ����������
//===============================================
HRESULT CObjectX::Init(D3DXVECTOR3 pos)
{
	//// �e��ݒ�
	//m_nIdxShadow = SetShadow();

	// �ʒu�𔽉f
	m_pos = pos;

	// ���f���̍ŏ��l�E�ő�l�̎擾
	m_vtxMin = CManager::GetInstance()->GetXFile()->GetSizeMin(m_aIdxXFile[m_modelType]);
	m_vtxMax = CManager::GetInstance()->GetXFile()->GetSize(m_aIdxXFile[m_modelType]);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CObjectX::Uninit(void)
{
	this->Release();
}

//===============================================
// �X�V����
//===============================================
void CObjectX::Update(void)
{
	D3DXVECTOR3 playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �v���C���[�̈ʒu
	D3DXVECTOR3 playerPosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �v���C���[�̑O��̈ʒu
	D3DXVECTOR3 playerRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �v���C���[�̌���
	CObject *pObj = NULL;

	m_posOld = m_pos;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = CObject::GetTop(nCntPriority);		// �擪�̃I�u�W�F�N�g����

		while (pObject != NULL)
		{// �g�p����Ă���
			CObject *pObjectNext = pObject->GetNext();		// ���̃I�u�W�F�N�g��ۑ�
			CObject::TYPE type = pObject->GetType();		// ��ނ��擾

			if (type == CObject::TYPE_PLAYER)
			{// �v���C���[
				playerPos = pObject->GetPos();			// �ʒu���擾
				playerPosOld = pObject->GetPosOld();	// �O��̈ʒu���擾
				playerRot = pObject->GetRot();			// �������擾
				pObj = pObject;
			}

			pObject = pObjectNext;		// ���̃I�u�W�F�N�g����
		}
	}

	//CollisionModel(&playerPos, &playerPosOld, 0, pObj, true);

	// �����蔻��
	//CGame::GetPlayer()->CollisionObjX(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);
	//CGame::GetEnemy()->CollisionObjX(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);
}

//===============================================
// �`�揈��
//===============================================
void CObjectX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;										// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// X�t�@�C���̕`��
	CManager::GetInstance()->GetXFile()->Draw(m_aIdxXFile[m_modelType], CXFile::COL_NORMAL);

	//// ���݂̃}�e���A�����擾
	//pDevice->GetMaterial(&matDef);

	//// �}�e���A���f�[�^�ւ̃|�C���^���擾
	//pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	//{
	//	// �}�e���A���̐ݒ�
	//	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

	//	// �e�N�X�`���̐ݒ�
	//	pDevice->SetTexture(0, m_apTexture[nCntMat]);

	//	// ���f���i�p�[�c�j�̕`��
	//	m_pMesh->DrawSubset(nCntMat);
	//}

	//// �ۑ����Ă����}�e���A����߂�
	//pDevice->SetMaterial(&matDef);
}

//===============================================
// �`�揈��
//===============================================
void CObjectX::DrawXFile(int nIdx, CXFile::COL col)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;										// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// X�t�@�C���̕`��
	CManager::GetInstance()->GetXFile()->Draw(nIdx, col);
}

//===============================================
// �v���C���[�Ƃ̓����蔻��
//===============================================
bool CObjectX::CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bLand = false;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = CObject::GetTop(nCntPriority);		// �擪�̃I�u�W�F�N�g����
		
		while (pObject != NULL)
		{// �g�p����Ă���
			CObject *pObjectNext = pObject->GetNext();		// ���̃I�u�W�F�N�g��ۑ�
			CObject::TYPE type = pObject->GetType();		// ��ނ��擾
			D3DXVECTOR3 pos = pObject->GetPos();			// �ʒu
			D3DXVECTOR3 posOld = pObject->GetPosOld();		// �O��̈ʒu
			D3DXVECTOR3 sizeMax = pObject->GetSize();		// �ő�T�C�Y
			D3DXVECTOR3 sizeMin = pObject->GetSizeMin();	// �ŏ��T�C�Y

			if (type == CObject::TYPE_BOXNORMAL || type == CObject::TYPE_BOXDAMAGE)
			{// �v���C���[
				if (pos.x + sizeMin.x - vtxMax.x <= pPos->x && pos.x + sizeMax.x - vtxMin.x >= pPos->x
					&& pos.z + sizeMin.z <= pPos->z - vtxMin.z && pos.z + sizeMax.z >= pPos->z + vtxMin.z
					&& pos.y + sizeMin.y <= pPos->y + vtxMax.y && pos.y + sizeMax.y >= pPos->y + vtxMin.y)
				{// �͈͓��ɂ���
					CPlayer::EState state = CGame::GetPlayer()->GetState();

					if (posOld.y + sizeMax.y <= pPosOld->y + vtxMin.y
						&& pos.y + sizeMax.y >= pPos->y + vtxMin.y)
					{// �ォ��߂荞��
						// ��ɂ̂���
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
							{// �q�b�v�h���b�v��
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
							{// �q�b�v�h���b�v��
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
							{// ��������
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
					{// ������߂荞��
						// ���ɖ߂�
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
							{// ��������
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
					{// ������E�ɂ߂荞��
						// �ʒu��߂�
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
							{// ��������
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
					{// �E���獶�ɂ߂荞��
						// �ʒu��߂�
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
							{// ��������
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
			pObject = pObjectNext;		// ���̃I�u�W�F�N�g����
		}
	}

	return bLand;
}

//===============================================
// �G�Ƃ̓����蔻��
//===============================================
bool CObjectX::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pRot, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bLand = false;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = CObject::GetTop(nCntPriority);		// �擪�̃I�u�W�F�N�g����

		while (pObject != NULL)
		{// �g�p����Ă���
			CObject *pObjectNext = pObject->GetNext();		// ���̃I�u�W�F�N�g��ۑ�
			CObject::TYPE type = pObject->GetType();		// ��ނ��擾
			D3DXVECTOR3 pos = pObject->GetPos();			// �ʒu
			D3DXVECTOR3 posOld = pObject->GetPosOld();		// �O��̈ʒu
			D3DXVECTOR3 sizeMax = pObject->GetSize();		// �ő�T�C�Y
			D3DXVECTOR3 sizeMin = pObject->GetSizeMin();	// �ŏ��T�C�Y

			if (type == CObject::TYPE_BOXNORMAL || type == CObject::TYPE_BOXDAMAGE)
			{// �v���C���[
				if (pos.x + sizeMin.x - vtxMax.x <= pPos->x && pos.x + sizeMax.x - vtxMin.x >= pPos->x
					&& pos.z + sizeMin.z <= pPos->z - vtxMin.z && pos.z + sizeMax.z >= pPos->z + vtxMin.z
					&& pos.y + sizeMin.y <= pPos->y + vtxMax.y && pos.y + sizeMax.y >= pPos->y + vtxMin.y)
				{// �͈͓��ɂ���
					if (posOld.y + sizeMax.y <= pPosOld->y + vtxMin.y
						&& pos.y + sizeMax.y >= pPos->y + vtxMin.y)
					{// �ォ��߂荞��
						// ��ɂ̂���
						pPos->y = posOld.y - vtxMin.y + sizeMax.y;
						pMove->y = 0.0f;

						bLand = true;
					}
					else if (posOld.y + sizeMin.y >= pPosOld->y + vtxMax.y
						&& pos.y + sizeMin.y <= pPos->y + vtxMax.y)
					{// ������߂荞��
						// ���ɖ߂�
						pPos->y = posOld.y - vtxMax.y + sizeMin.y;
						//CGame::GetPlayer()->SetMove(D3DXVECTOR3(CGame::GetPlayer()->GetMove().x, 0.0f, CGame::GetPlayer()->GetMove().z));
					}
					else if (posOld.z + sizeMin.z >= pPosOld->z - vtxMin.z
						&& pos.z + sizeMin.z <= pPos->z - vtxMin.z)
					{// ������E�ɂ߂荞��
						// �ʒu��߂�
						pPos->z = posOld.z + vtxMin.z + sizeMin.z;
						pRot->y = D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y);
					}
					else if (posOld.z + sizeMax.z <= pPosOld->z + vtxMin.z
						&& pos.z + sizeMax.z >= pPos->z + vtxMin.z)
					{// �E���獶�ɂ߂荞��
						// �ʒu��߂�
						pPos->z = posOld.z - vtxMin.z + sizeMax.z;
						pRot->y = D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y);
					}
				}
			}
			pObject = pObjectNext;		// ���̃I�u�W�F�N�g����
		}
	}

	return bLand;
}

//===============================================
// ���f���^�C�v�ݒ�
//===============================================
void CObjectX::SetModel(MODEL type)
{
	m_modelType = type;
}

//===============================================
// �ʒu�ݒ�
//===============================================
void CObjectX::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===============================================
// �����ݒ�
//===============================================
void CObjectX::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//===============================================
// �ړ��ʐݒ�
//===============================================
void CObjectX::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//===============================================
// �T�C�Y�擾
//===============================================
void CObjectX::SetSize(D3DXVECTOR3 size)
{
	m_vtxMax = size;
}

//===============================================
// �T�C�Y�擾
//===============================================
D3DXVECTOR3 CObjectX::GetSize(void)
{
	return CManager::GetInstance()->GetXFile()->GetSize(m_modelType - 1);
}

//===============================================
// �T�C�Y�擾
//===============================================
void CObjectX::SetSizeMin(D3DXVECTOR3 size)
{
	m_vtxMin = size;
}

//===============================================
// �T�C�Y�擾
//===============================================
D3DXVECTOR3 CObjectX::GetSizeMin(void)
{
	return CManager::GetInstance()->GetXFile()->GetSizeMin(m_modelType - 1);
}