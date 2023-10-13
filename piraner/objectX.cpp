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
	m_aIdxXFile[MODEL_HOUSE00] = CManager::GetXFile()->Regist("data\\MODEL\\Dumpster.x");

	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data\\TXT\\model.txt", "r");

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
		MessageBox(hWnd, "�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
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
	pObjX->SetType(TYPE_XFAIL);

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
	m_vtxMin = CManager::GetXFile()->GetSizeMin(m_aIdxXFile[m_modelType]);
	m_vtxMax = CManager::GetXFile()->GetSize(m_aIdxXFile[m_modelType]);

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
	CGame::GetPlayer()->CollisionObjX(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);
	CGame::GetEnemy()->CollisionObjX(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);
}

//===============================================
// �`�揈��
//===============================================
void CObjectX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
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
	CManager::GetXFile()->Draw(m_aIdxXFile[m_modelType], CXFile::COL_NORMAL);

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
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
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
	CManager::GetXFile()->Draw(nIdx, col);
}

//===============================================
// �v���C���[�Ƃ̓����蔻��
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
	{// �͈͓��ɂ���
		if (m_posOld.y + m_vtxMax.y <= pPosOld->y + vtxMin.y
			&& m_pos.y + m_vtxMax.y >= pPos->y + vtxMin.y)
		{// �ォ��߂荞��
			// ��ɂ̂���
			pObj->SetPos(D3DXVECTOR3(pPos->x, m_pos.y - vtxMin.y + m_vtxMax.y, pPos->z));
			pObj->SetMove(D3DXVECTOR3(movePlayer.x, 0.0f, movePlayer.z));
			CGame::GetPlayer()->SetJump(false);
		}
		else if (m_posOld.y + m_vtxMin.y >= pPosOld->y + vtxMax.y
				&& m_pos.y + m_vtxMin.y <= pPos->y + vtxMax.y)
		{// ������߂荞��
			// ���ɖ߂�
			pObj->SetPos(D3DXVECTOR3(pPos->x, m_pos.y - vtxMax.y + m_vtxMin.y, pPos->z));
			pObj->SetMove(D3DXVECTOR3(movePlayer.x, 0.0f, movePlayer.z));
		}
		else if (m_posOld.z - m_vtxMax.z >= pPosOld->z + vtxMax.z
				&& m_pos.z - m_vtxMax.z <= pPos->z + vtxMax.z)
		{// ������E�ɂ߂荞��
			// �ʒu��߂�
			pObj->SetPos(D3DXVECTOR3(pPos->x, pPos->y, m_pos.z - vtxMax.z - m_vtxMax.z));
		}
		else if (m_posOld.z - m_vtxMin.z <= pPosOld->z - vtxMax.z
				&& m_pos.z - m_vtxMin.z >= pPos->z - vtxMax.z)
		{// �E���獶�ɂ߂荞��
			// �ʒu��߂�
			pObj->SetPos(D3DXVECTOR3(pPos->x, pPos->y, m_pos.z + vtxMax.z + m_vtxMax.z));
		}
	}
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
D3DXVECTOR3 CObjectX::GetSize(void)
{
	return CManager::GetXFile()->GetSize(0);
}