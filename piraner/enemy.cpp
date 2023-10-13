//=========================================================
//
// �G���� [enemy.cpp]
// Author = �����đ�Y
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
// �}�N����`
//===============================================
#define MOVE_PLAYER			(0.45f)		// �������x
#define JUMP_PLAYER			(16.85f)	// �W�����v��
#define MOVE_GRAVITY		(0.75f)		// �d��

#define JUMP_HIPDROP		(10.85f)	// �q�b�v�h���b�v�����̕���
#define MOVE_HIPDROP		(1.2f)		// �q�b�v�h���b�v���d��

#define MOVE_MINUS			(0.07f)		// �ړ��ʂ̌���
#define TURN_TIME			(1)			// �Ȃ��鎞��
#define COUNT_PARTICLE		(25)		// �p�[�e�B�N����������

//===============================================
// �R���X�g���N�^
//===============================================
CEnemy::CEnemy() : CObject(4)
{
	// �l���N���A����
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
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CEnemy::CEnemy(int nPriority) : CObject(nPriority)
{
	// �l���N���A����
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
// �f�X�g���N�^
//===============================================
CEnemy::~CEnemy()
{

}

//===============================================
// ��������
//===============================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, int nPriority)
{
	CEnemy *pEnemy;

	// �v���C���[�̐���
	pEnemy = new CEnemy(nPriority);

	// ��ނ̐ݒ�
	pEnemy->SetType(CObject::TYPE_ENEMY);
	
	// ����������
	pEnemy->Init(pos);

	return pEnemy;
}

//===============================================
// ����������
//===============================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	// ���[�V�����̐���
	m_pMotion = new CMotion;

	// ���[�V�����̏�����
	m_pMotion->Init();

	// ���f���̑���
	m_nNumModel = CManager::GetLoad()->GetNumModel();

	// �ʒu�̐ݒ�
	m_pos = pos;

	// �ړ����x�̏�����
	m_fSpeed = MOVE_PLAYER;

	const char *apModelFile[MAX_MODEL];		// ���f���t�@�C����

	// ���f���̐����i�S�p�[�c�j
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		apModelFile[nCntModel] = CManager::GetLoad()->GetFileName(nCntModel);		// �t�@�C�����擾
		pos = CManager::GetLoad()->GetPos(nCntModel);								// �ʒu�̎擾
		rot = CManager::GetLoad()->GetRot(nCntModel);								// �����̎擾

		m_apModel[nCntModel] = CModel::Create(apModelFile[nCntModel], pos, rot);	// ����
	}
	//m_apModel[0] = CModel::Create("data\\MODEL\\GarbageCan_Wood.x", pos, D3DXVECTOR3(0.0f, 0.0f , 0.0f));	// ����

	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);

	// �e���f���̐ݒ�i�S�p�[�c�j
	m_apModel[0]->SetParent(NULL);
	for (int nCntModel = 1; nCntModel < m_nNumModel; nCntModel++)
	{
		int nParent = 0;

		nParent = CManager::GetLoad()->GetParent(nCntModel);	// �e���擾

		m_apModel[nCntModel]->SetParent(m_apModel[nParent]);
	}

	// �l�̏�����
	m_vtxMin = D3DXVECTOR3(900.0f, 900.0f, 900.0f);
	m_vtxMax = D3DXVECTOR3(-900.0f, -900.0f, -900.0f);

	// �S�Ă̒��_���r���ă��f���̍ŏ��l�E�ő�l�𔲂��o��
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		// �ŏ��l�̔�r
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

		// �ő�l�̔�r
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

	// ���[�V�������̐ݒ�
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{// ���[�V���������J��Ԃ�
		m_pMotion->Set(nCntMotion);
		m_pMotion->SetInfo(CManager::GetLoad()->GetInfo(nCntMotion));
	}

	// �������[�V�����ݒ�
	m_pMotion->Set(MOTIONTYPE_NEUTRAL);

	// ������Ԑݒ�
	m_state = STATE_NORMAL;

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CEnemy::Uninit(void)
{
	// ���f���̔j��
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		if (m_apModel[nCntModel] != NULL)
		{// �g�p����Ă���
			// ���f���̏I������
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}
	}

	// ���[�V�����ւ̃|�C���^�̔j��
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	this->Release();
}

//===============================================
// �X�V����
//===============================================
void CEnemy::Update(void)
{
	// �O��̈ʒu��ۑ�
	m_posOld = m_pos;

	if (m_state == STATE_NORMAL)
	{
		//m_move.x += sinf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_fSpeed;
		//m_move.z += cosf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * m_fSpeed;
		//m_rotDest.y = D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y);
	}

	switch (m_state)
	{
	case STATE_NORMAL:		// �ʏ�
		break;

	case STATE_DASH:		// �_�b�V��
		break;

	case STATE_ATTACK:		// �󒆍U��
		break;
	}

	m_fRotDiff = m_rotDest.y - m_rot.y;	// �ړI�̌����܂ł̍���

	// �p�x�̒l�̕␳
	if (m_fRotDiff > D3DX_PI)
	{
		m_fRotDiff += -D3DX_PI * ROT_DIFF_PI;
	}
	else if (m_fRotDiff < -D3DX_PI)
	{
		m_fRotDiff += D3DX_PI * ROT_DIFF_PI;
	}

	m_rot.y += m_fRotDiff * ROT_CAMERA_DIFF;	// �p�x�̕␳

	// �p�x�̒l�̕␳
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y += -D3DX_PI * ROT_DIFF_PI;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * ROT_DIFF_PI;
	}

	// �d�͏���
	m_move.y -= MOVE_GRAVITY;

	// �ʒu���X�V
	m_pos += m_move;

	// 3D�|���S���Ƃ̓����蔻��
	//m_pos.y = CManager::GetObject3D()->CollisionVec(m_pos);

	// �ړ��ʂ��X�V�i����������j
	m_move.x += (0.0f - m_move.x) * MOVE_MINUS;
	m_move.z += (0.0f - m_move.z) * MOVE_MINUS;

	//if (m_pos.y < LAND_POS)
	//{// ���n����
	//	m_pos.y = LAND_POS;
	//	m_move.y = 0.0f;
	//	m_bJump = false;
	//	m_bAirJump = false;
	//}

	// ���[�V�����̍X�V����
	m_pMotion->Update();

	// �����蔻��
	CGame::GetPlayer()->CollisionEnemy(&m_pos, &m_posOld, m_vtxMax, m_vtxMin);

	// �f�o�b�O�\��
	CManager::GetDebugProc()->Print(" �G�̈ʒu�F�i%f, %f, %f�j\n\n", m_pos.x, m_pos.y, m_pos.z);
}

//===============================================
// �`�揈��
//===============================================
void CEnemy::Draw(void)
{
	if (CManager::GetMode() != CScene::MODE_TITLE)
	{// �^�C�g������Ȃ�
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

		// ���f���̕`��i�S�p�[�c�j
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			// ���f���̕`�揈��
			m_apModel[nCntModel]->Draw();

			// ���f���̉e�̕`�揈��
			m_apModel[nCntModel]->DrawShadowmtx();
		}
	}
}

//===============================================
// �I�u�W�F�N�gX�Ƃ̓����蔻��
//===============================================
void CEnemy::CollisionObjX(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	if (pPosOld->x + vtxMin.x - m_vtxMax.x <= m_pos.x && pPosOld->x + vtxMax.x - m_vtxMin.x >= m_pos.x
		&& pPosOld->z + vtxMin.z - m_vtxMax.z <= m_pos.z && pPosOld->z + vtxMax.z - m_vtxMin.z >= m_pos.z
		&& pPosOld->y + vtxMin.y - m_vtxMax.y <= m_pos.y && pPosOld->y + vtxMax.y - m_vtxMin.y >= m_pos.y)
	{// �͈͓��ɂ���
		if (pPosOld->y + vtxMax.y <= m_posOld.y + m_vtxMin.y
			&& pPosOld->y + vtxMax.y >= m_pos.y + m_vtxMin.y)
		{// �ォ��߂荞��
			// ��ɂ̂���
			m_pos.y = pPosOld->y - m_vtxMin.y + vtxMax.y;
			m_move.y = 0.0f;
			CGame::GetEnemy()->SetJump(false);
		}
		else if (pPosOld->y + vtxMin.y >= m_posOld.y + m_vtxMax.y
			&& pPosOld->y + vtxMin.y <= m_pos.y + m_vtxMax.y)
		{// ������߂荞��
			// ���ɖ߂�
			m_pos.y = pPosOld->y - m_vtxMax.y + vtxMin.y;
			m_move.y = 0.0f;
		}
		else if (pPosOld->z - vtxMax.z >= m_posOld.z + m_vtxMax.z
			&& pPosOld->z - vtxMax.z <= m_pos.z + m_vtxMax.z)
		{// ������E�ɂ߂荞��
			// �ʒu��߂�
			m_pos.z = pPosOld->z - m_vtxMax.z - vtxMax.z;
		}
		else if (pPosOld->z - vtxMin.z <= m_posOld.z - m_vtxMax.z
			&& pPosOld->z - vtxMin.z >= m_pos.z - m_vtxMax.z)
		{// �E���獶�ɂ߂荞��
			// �ʒu��߂�
			m_pos.z = pPosOld->z + m_vtxMax.z + vtxMax.z;
		}
	}
}

//===============================================
// �ʒu�̐ݒ�
//===============================================
void CEnemy::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===============================================
// �ړ��ʂ̐ݒ�
//===============================================
void CEnemy::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//===============================================
// �W�����v�������̐ݒ�
//===============================================
void CEnemy::SetJump(const bool bJump)
{
	m_bJump = bJump;
}

//===============================================
// ���[�V�����̐ݒ�
//===============================================
void CEnemy::SetMotion(MOTIONTYPE type)
{
	m_pMotion->Set(type);
}