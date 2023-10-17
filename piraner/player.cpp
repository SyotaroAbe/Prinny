//=========================================================
//
// �v���C���[���� [player.cpp]
// Author = �����đ�Y
//
//=========================================================
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
#define LAND_POS			(0.0f)		// �n��

#define JUMP_HIPDROP		(10.85f)	// �q�b�v�h���b�v�����̕���
#define MOVE_HIPDROP		(1.2f)		// �q�b�v�h���b�v���d��

#define MOVE_MINUS			(0.07f)		// �ړ��ʂ̌���
#define TURN_TIME			(1)			// �Ȃ��鎞��
#define COUNT_PARTICLE		(25)		// �p�[�e�B�N����������

//===============================================
// �R���X�g���N�^
//===============================================
CPlayer::CPlayer() : CObject(4)
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
	m_bAirJump = false;
	m_fLenthCamera = 0.0f;
	m_fRotBullet = 0.0f;
	m_nTurnCounter = 0;
	m_apModel[MAX_MODEL] = {};
	m_nNumModel = 0;
	m_nParticleCounter = 0;
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = STATE_NONE;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CPlayer::CPlayer(int nPriority) : CObject(nPriority)
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
	m_bAirJump = false;
	m_fLenthCamera = 0.0f;
	m_fRotBullet = 0.0f;
	m_nTurnCounter = 0;
	m_apModel[MAX_MODEL] = {};
	m_nNumModel = 0;
	m_pMotion = NULL;
	m_nParticleCounter = 0;
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = STATE_NONE;
}

//===============================================
// �f�X�g���N�^
//===============================================
CPlayer::~CPlayer()
{

}

//===============================================
// ��������
//===============================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, int nPriority)
{
	CPlayer *pPlayer;

	// �v���C���[�̐���
	pPlayer = new CPlayer(nPriority);

	// ��ނ̐ݒ�
	pPlayer->SetType(CObject::TYPE_PLAYER);
	
	// ����������
	pPlayer->Init(pos);

	return pPlayer;
}

//===============================================
// ����������
//===============================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	// ���[�V�����̐���
	m_pMotion = new CMotion;

	// ���[�V�����̏�����
	m_pMotion->Init();

	// ���f���̑���
	m_nNumModel = CManager::GetInstance()->GetLoad()->GetNumModel();

	// �ʒu�̐ݒ�
	m_pos = pos;

	// �ړ����x�̏�����
	m_fSpeed = MOVE_PLAYER;

	// �p�[�e�B�N�������h�~
	m_nParticleCounter = COUNT_PARTICLE;

	const char *apModelFile[MAX_MODEL];		// ���f���t�@�C����

	// ���f���̐����i�S�p�[�c�j
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		apModelFile[nCntModel] = CManager::GetInstance()->GetLoad()->GetFileName(nCntModel);		// �t�@�C�����擾
		pos = CManager::GetInstance()->GetLoad()->GetPos(nCntModel);								// �ʒu�̎擾
		rot = CManager::GetInstance()->GetLoad()->GetRot(nCntModel);								// �����̎擾

		m_apModel[nCntModel] = CModel::Create(apModelFile[nCntModel], pos, rot);	// ����
	}
	//m_apModel[0] = CModel::Create("data\\MODEL\\GarbageCan_Wood.x", pos, D3DXVECTOR3(0.0f, 0.0f , 0.0f));	// ����

	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);

	// �e���f���̐ݒ�i�S�p�[�c�j
	m_apModel[0]->SetParent(NULL);
	for (int nCntModel = 1; nCntModel < m_nNumModel; nCntModel++)
	{
		int nParent = 0;

		nParent = CManager::GetInstance()->GetLoad()->GetParent(nCntModel);	// �e���擾

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
		m_pMotion->SetInfo(CManager::GetInstance()->GetLoad()->GetInfo(nCntMotion));
	}

	// �������[�V�����ݒ�
	m_pMotion->Set(MOTIONTYPE_NEUTRAL);

	// ������Ԑݒ�
	m_state = STATE_NORMAL;
	m_fLenthCamera = LENTH_NORMAL;

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CPlayer::Uninit(void)
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
void CPlayer::Update(void)
{
	// �O��̈ʒu��ۑ�
	m_posOld = m_pos;

	if (m_nParticleCounter < COUNT_PARTICLE && CManager::GetMode() != CScene::MODE_TITLE)
	{// �p�[�e�B�N���������ԂɒB���Ă��Ȃ�
		// �p�[�e�B�N���̐���
		CParticle::Create(1)->Set(D3DXVECTOR3(m_pos.x + cosf(m_rot.y) * 18.0f, m_pos.y, m_pos.z - sinf(m_rot.y) * 18.0f), CParticle::TYPE_CURVE);
		CParticle::Create(1)->Set(D3DXVECTOR3(m_pos.x - cosf(m_rot.y) * 18.0f, m_pos.y, m_pos.z + sinf(m_rot.y) * 18.0f), CParticle::TYPE_CURVE);
		m_nParticleCounter++;											// �������Ԃ��J�E���g�A�b�v
	}

	if (m_state == STATE_NORMAL)
	{
		if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_A) == true)
		{//���L�[�������ꂽ
			m_move.x += sinf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y)) * m_fSpeed;
			m_move.z += cosf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y)) * m_fSpeed;
			m_rotDest.y = D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y);
		}
		else if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_D) == true)
		{//�E�L�[�������ꂽ
			m_move.x += sinf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y)) * m_fSpeed;
			m_move.z += cosf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y)) * m_fSpeed;
			m_rotDest.y = D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y);
		}

		if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_SPACE) == true && m_bAirJump == false)
		{// SPACE�L�[�������ꂽ
			if (m_bJump == true)
			{// 2�i�W�����v
				m_bAirJump = true;
			}

			m_move.y = JUMP_PLAYER;
			m_bJump = true;
		}
	}

	// �q�b�v�h���b�v
	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_S) == true && m_bJump == true && m_state != STATE_HIPDROP)
	{// S�L�[�������ꂽ
		m_state = STATE_HIPDROP;
		m_move.y = JUMP_HIPDROP;
	}

	switch (m_state)
	{
	case STATE_NORMAL:		// �ʏ�
		CManager::GetInstance()->GetCamera()->ScalingLenth(m_fLenthCamera, 0.1f);
		break;

	case STATE_DASH:		// �_�b�V��
		break;

	case STATE_AIRSLASH:	// �󒆍U��
		break;

	case STATE_HIPDROP:		// �q�b�v�h���b�v
		m_move.y -= MOVE_HIPDROP;		// �~�����x���グ��
		break;

	case STATE_LANDDROP:	// �q�b�v�h���b�v���n
		// �J�����g�k����
		if (CManager::GetInstance()->GetCamera()->ScalingLenth(m_fLenthCamera, 0.1f) == true)
		{// ������܂ŏk������
			m_state = STATE_NORMAL;
			m_fLenthCamera = LENTH_NORMAL;
		}
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
	if (m_state != STATE_HIPDROP)
	{
		m_move.y -= MOVE_GRAVITY;
	}

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

	if (m_pos.y < -100.0f)
	{// ������
		
	}

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(" �ړ�          �FA D\n");
	CManager::GetInstance()->GetDebugProc()->Print(" �W�����v      �FSPACE\n");
	CManager::GetInstance()->GetDebugProc()->Print(" �q�b�v�h���b�v�FS\n");
	CManager::GetInstance()->GetDebugProc()->Print(" ���U���g��    �FBACKSPACE\n\n");

	CManager::GetInstance()->GetDebugProc()->Print(" �v���C���[�̈ʒu�F�i%f, %f, %f�j\n", m_pos.x, m_pos.y, m_pos.z);
	CManager::GetInstance()->GetDebugProc()->Print(" �v���C���[�̈ړ����x�F%f\n", m_fSpeed);
	CManager::GetInstance()->GetDebugProc()->Print(" �v���C���[�̌����F%f\n\n", m_rot.y);
	
	CManager::GetInstance()->GetDebugProc()->Print(" �J�����̌����F%f\n", CManager::GetInstance()->GetCamera()->GetRot().y);
	CManager::GetInstance()->GetDebugProc()->Print(" ���_�F�i%f, %f, %f�j\n", CManager::GetInstance()->GetCamera()->GetPosV().x, CManager::GetInstance()->GetCamera()->GetPosV().y, CManager::GetInstance()->GetCamera()->GetPosV().z);
	CManager::GetInstance()->GetDebugProc()->Print(" �����_�F�i%f, %f, %f�j\n\n", CManager::GetInstance()->GetCamera()->GetPosR().x, CManager::GetInstance()->GetCamera()->GetPosR().y, CManager::GetInstance()->GetCamera()->GetPosR().z);
}

//===============================================
// �`�揈��
//===============================================
void CPlayer::Draw(void)
{
	if (CManager::GetMode() != CScene::MODE_TITLE)
	{// �^�C�g������Ȃ�
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
void CPlayer::CollisionObjX(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	if (pPos->x + vtxMin.x - m_vtxMax.x <= m_pos.x && pPos->x + vtxMax.x - m_vtxMin.x >= m_pos.x
		&& pPos->z + vtxMin.z <= m_pos.z - m_vtxMin.z && pPos->z + vtxMax.z >= m_pos.z + m_vtxMin.z
		&& pPos->y + vtxMin.y - m_vtxMax.y <= m_pos.y && pPos->y + vtxMax.y - m_vtxMin.y >= m_pos.y)
	{// �͈͓��ɂ���
		if (pPosOld->y + vtxMax.y <= m_posOld.y + m_vtxMin.y
			&& pPos->y + vtxMax.y >= m_pos.y + m_vtxMin.y)
		{// �ォ��߂荞��
			// ��ɂ̂���
			m_pos.y = pPosOld->y - m_vtxMin.y + vtxMax.y;
			m_move.y = 0.0f;
			CGame::GetPlayer()->SetJump(false);

			if (m_state == STATE_HIPDROP)
			{// �q�b�v�h���b�v��
				m_state = STATE_LANDDROP;
				m_fLenthCamera = LEMTH_HIPDROP;
			}
		}
		else if (pPosOld->y + vtxMin.y >= m_posOld.y + m_vtxMax.y
			&& pPos->y + vtxMin.y <= m_pos.y + m_vtxMax.y)
		{// ������߂荞��
			// ���ɖ߂�
			m_pos.y = pPosOld->y - m_vtxMax.y + vtxMin.y;
			m_move.y = 0.0f;
		}
		else if (pPosOld->z - vtxMax.z >= m_posOld.z + m_vtxMax.z
			&& pPos->z - vtxMax.z <= m_pos.z + m_vtxMax.z)
		{// ������E�ɂ߂荞��
			// �ʒu��߂�
			m_pos.z = pPosOld->z - m_vtxMax.z - vtxMax.z;
		}
		else if (pPosOld->z - vtxMin.z <= m_posOld.z - m_vtxMax.z
			&& pPos->z - vtxMin.z >= m_pos.z - m_vtxMax.z)
		{// �E���獶�ɂ߂荞��
			// �ʒu��߂�
			m_pos.z = pPosOld->z + m_vtxMax.z + vtxMax.z;
		}
	}
}

//===============================================
// �G�Ƃ̓����蔻��
//===============================================
void CPlayer::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	if (pPosOld->x + vtxMin.x - m_vtxMax.x <= m_pos.x && pPosOld->x + vtxMax.x - m_vtxMin.x >= m_pos.x
		&& pPosOld->z + vtxMin.z - m_vtxMax.z <= m_pos.z && pPosOld->z + vtxMax.z - m_vtxMin.z >= m_pos.z
		&& pPosOld->y + vtxMin.y - m_vtxMax.y <= m_pos.y && pPosOld->y + vtxMax.y - m_vtxMin.y >= m_pos.y)
	{// �͈͓��ɂ���
		if (pPosOld->y + vtxMax.y <= m_posOld.y + m_vtxMin.y
			&& pPosOld->y + vtxMax.y >= m_pos.y + m_vtxMin.y)
		{// �ォ��߂荞��
			if (m_state == STATE_HIPDROP)
			{// �q�b�v�h���b�v��
				m_state = STATE_NORMAL;
				m_move.y = JUMP_HIPDROP;
				m_bAirJump = false;
			}
			else
			{
				// �_���[�W
			}
		}
		else
		{
			// �_���[�W
		}
	}
}

//===============================================
// �ʒu�̐ݒ�
//===============================================
void CPlayer::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===============================================
// �ړ��ʂ̐ݒ�
//===============================================
void CPlayer::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//===============================================
// �W�����v�������̐ݒ�
//===============================================
void CPlayer::SetJump(const bool bJump)
{
	m_bJump = bJump;
	m_bAirJump = bJump;
}

//===============================================
// ���[�V�����̐ݒ�
//===============================================
void CPlayer::SetMotion(MOTIONTYPE type)
{
	m_pMotion->Set(type);
}