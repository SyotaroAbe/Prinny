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
#include "bullet.h"
#include "objectX.h"

//===============================================
// �}�N����`
//===============================================
#define MOVE_PLAYER			(0.45f)		// �������x
#define JUMP_PLAYER			(16.85f)	// �W�����v��
#define MOVE_GRAVITY		(0.75f)		// �d��
#define LAND_POS			(0.0f)		// �n��

#define JUMP_HIPDROP		(12.0f)		// �q�b�v�h���b�v�����̕���
#define MOVE_HIPDROP		(1.2f)		// �q�b�v�h���b�v���d��

#define JUMP_ATTACK			(0.75f)		// �󒆍U�����̕���
#define ROT_BULLET			(0.7f)		// �e�����̌���

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
	m_nStateCounter = 0;
	m_apModel[MAX_MODEL] = {};
	m_nNumModel = 0;
	m_nParticleCounter = 0;
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = STATE_NONE;
	m_bInvincible = false;
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
	m_nStateCounter = 0;
	m_apModel[MAX_MODEL] = {};
	m_nNumModel = 0;
	m_pMotion = NULL;
	m_nParticleCounter = 0;
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = STATE_NONE;
	m_bInvincible = false;
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

	// �����̐ݒ�
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

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

	// ���[�V�����̍X�V����
	if (m_pMotion != NULL)
	{
		m_pMotion->Update();
	}

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
	float fGravity = MOVE_GRAVITY;

	// �J�E���^���X�V
	m_nStateCounter--;

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
			if (m_bJump == false)
			{
				m_pMotion->Set(MOTIONTYPE_MOVE);				// �ړ����[�V�����ݒ�
			}
		}
		else if (CManager::GetInstance()->GetKeyboardInput()->GetPress(DIK_D) == true)
		{//�E�L�[�������ꂽ
			m_move.x += sinf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y)) * m_fSpeed;
			m_move.z += cosf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y)) * m_fSpeed;
			m_rotDest.y = D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetInstance()->GetCamera()->GetRot().y);
			if (m_bJump == false)
			{
				m_pMotion->Set(MOTIONTYPE_MOVE);				// �ړ����[�V�����ݒ�
			}
		}
		else if(m_bJump == false)
		{
			m_pMotion->Set(MOTIONTYPE_NEUTRAL);				// �ҋ@���[�V�����ݒ�
		}

		if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_SPACE) == true && m_bAirJump == false)
		{// SPACE�L�[�������ꂽ
			if (m_bJump == true)
			{// 2�i�W�����v
				m_bAirJump = true;
				m_pMotion->Set(MOTIONTYPE_JUMPAIR);				// �W�����v���[�V����
			}
			else
			{
				m_pMotion->Set(MOTIONTYPE_JUMP);				// �W�����v���[�V����
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
		m_pMotion->Set(MOTIONTYPE_HIPDROP);				// �q�b�v�h���b�v���[�V����
	}

	switch (m_state)
	{
	case STATE_NORMAL:		// �ʏ�
		m_fSpeed = MOVE_PLAYER;

		// �U���O����
		if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_RETURN) == true)
		{
			if (m_bJump == true && m_pMotion->GetType() != MOTIONTYPE_PREATTACK)
			{
				m_pMotion->Set(MOTIONTYPE_PREATTACK);				// �󒆍U���O����
				m_move.y = JUMP_HIPDROP;
				m_state = STATE_AIRSLASH;
			}
			else if(m_state != STATE_DASH)
			{
				//if (m_rot.y <= 0.002f && m_rot.y >= -0.002f || m_rot.y <= D3DX_PI + 0.002f && m_rot.y >= D3DX_PI - 0.002f || m_rot.y <= -D3DX_PI + 0.002f && m_rot.y >= -D3DX_PI - 0.002f)
				//{
					m_pMotion->Set(MOTIONTYPE_DASH);	// �_�b�V��
					m_state = STATE_DASH;
					m_nStateCounter = 20;				// ��ԃJ�E���^�[��ݒ�
				//}
			}
		}

		// �J�������������ɐݒ�
		m_fLenthCamera = LENTH_NORMAL;
		CManager::GetInstance()->GetCamera()->ScalingLenth(m_fLenthCamera, 0.15f);
		break;

	case STATE_DASH:		// �_�b�V��
		if (m_nStateCounter < 0)
		{
			m_state = STATE_NORMAL;
		}
		m_fSpeed = 0.9f;
		m_move.z += cosf(D3DX_PI * ROT_DOWN + (ROT_CAMERA * m_rot.y)) * m_fSpeed;

		// �G�t�F�N�g�̐���
		CEffect::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 40.0f, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f), CEffect::TYPE_NORMAL, 25, 50, 3);
		CEffect::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 40.0f, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), CEffect::TYPE_NORMAL, 20, 50, 3);
		CParticle::Create(1)->Set(D3DXVECTOR3(m_pos.x + cosf(m_rot.y) * 18.0f, m_pos.y, m_pos.z - sinf(m_rot.y) * 18.0f), CParticle::TYPE_CURVE);
		break;

	case STATE_AIRSLASH:	// �󒆍U��
		if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_RETURN) == true)
		{
			// �U�����[�V���������݂ɏo��
			if (m_pMotion->GetType() == MOTIONTYPE_PREATTACK || m_pMotion->GetType() == MOTIONTYPE_ATTACKL)
			{
				m_pMotion->Set(MOTIONTYPE_ATTACKR);	// ���[�V�����؂�ւ�
				m_move.y = JUMP_ATTACK;				// ����������

				// �e�̐���
				CBullet::Create(m_pos)->Set(m_pos, D3DXVECTOR3(0.0f, -sinf(m_rot.x + ROT_BULLET) * 10.0f, -cosf(m_rot.y + ROT_BULLET) * 10.0f));
			}
			else
			{
				m_pMotion->Set(MOTIONTYPE_ATTACKL);	// ���[�V�����؂�ւ�
				m_move.y = JUMP_ATTACK;				// ����������

				// �e�̐���
				CBullet::Create(m_pos)->Set(m_pos, D3DXVECTOR3(0.0f, -sinf(m_rot.x + ROT_BULLET) * 10.0f, -cosf(m_rot.y + ROT_BULLET) * 10.0f));
			}
		}
		
		fGravity = 0.45f;
		break;

	case STATE_HIPDROP:		// �q�b�v�h���b�v
		m_move.y -= MOVE_HIPDROP;		// �~�����x���グ��
		break;

	case STATE_LANDDROP:	// �q�b�v�h���b�v���n
		// �J�����g�k����
		if (CManager::GetInstance()->GetCamera()->ScalingLenth(m_fLenthCamera, 0.15f) == true)
		{// ������܂ŏk������
			m_state = STATE_NORMAL;
			m_fLenthCamera = LENTH_NORMAL;
		}
		break;

	case STATE_JUMPDROP:	// �q�b�v�h���b�v�W�����v
		if (m_nStateCounter < 0)
		{
			m_state = STATE_NORMAL;
		}
		break;

	case STATE_DAMAGE:		// �_���[�W
		if (m_nStateCounter < 0)
		{
			m_state = STATE_NORMAL;
			//m_bInvincible = true;
			m_nStateCounter = 200;				// ��ԃJ�E���^�[��ݒ�
		}
		break;

	case STATE_INVINCIBLE:	// ���G
		break;

	case STATE_DEATH:		// ���S
		break;
	}

	if (m_bInvincible == true)
	{
		if (m_nStateCounter < 0)
		{
			m_bInvincible = false;
		}
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
		m_move.y -= fGravity;
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
	if (m_pMotion != NULL)
	{
		m_pMotion->Update();
	}

	// �n�`�Ƃ̓����蔻��
	if (CObjectX::CollisionModel(&m_pos, &m_posOld, m_vtxMax, m_vtxMin) == true)
	{// ���n���Ă���
		SetJump(false);
	}
	else
	{
		m_bJump = true;
	}

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(" �ړ�          �FA D\n");
	CManager::GetInstance()->GetDebugProc()->Print(" �W�����v      �FSPACE\n");
	CManager::GetInstance()->GetDebugProc()->Print(" �󒆍U��      �FENTER\n");
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
			if (m_state != STATE_DAMAGE)
			{
				m_apModel[nCntModel]->Draw();
			}
			else
			{
				m_apModel[nCntModel]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}

			// ���f���̉e�̕`�揈��
			m_apModel[nCntModel]->DrawShadowmtx(m_pos.y);
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
		&& pPos->y + vtxMin.y <= m_pos.y + m_vtxMax.y && pPos->y + vtxMax.y >= m_pos.y + m_vtxMin.y)
	{// �͈͓��ɂ���
		if (pPosOld->y + vtxMax.y <= m_posOld.y + m_vtxMin.y
			&& pPos->y + vtxMax.y >= m_pos.y + m_vtxMin.y)
		{// �ォ��߂荞��
			// ��ɂ̂���
			m_pos.y = pPosOld->y - m_vtxMin.y + vtxMax.y;

			if (m_state == STATE_HIPDROP)
			{// �q�b�v�h���b�v��
				SetState(STATE_LANDDROP);
			}
			else if(m_state != STATE_LANDDROP && m_state != STATE_DAMAGE && m_state != STATE_DASH)
			{
				SetState(STATE_NORMAL);
			}
		}
		else if (pPosOld->y + vtxMin.y >= m_posOld.y + m_vtxMax.y
			&& pPos->y + vtxMin.y <= m_pos.y + m_vtxMax.y)
		{// ������߂荞��
			// ���ɖ߂�
			m_pos.y = pPosOld->y - m_vtxMax.y + vtxMin.y;
			m_move.y = 0.0f;
		}
		else if (pPosOld->z + vtxMin.z >= m_posOld.z - m_vtxMin.z
			&& pPos->z + vtxMin.z <= m_pos.z - m_vtxMin.z)
		{// ������E�ɂ߂荞��
			// �ʒu��߂�
			m_pos.z = pPosOld->z + m_vtxMin.z + vtxMin.z;
		}
		else if (pPosOld->z + vtxMax.z <= m_posOld.z + m_vtxMin.z
			&& pPos->z + vtxMax.z >= m_pos.z + m_vtxMin.z)
		{// �E���獶�ɂ߂荞��
			// �ʒu��߂�
			m_pos.z = pPosOld->z - m_vtxMin.z + vtxMax.z;
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
		if (m_state == STATE_HIPDROP)
		{// �q�b�v�h���b�v���ɓG�̓���֒��n
			SetState(STATE_JUMPDROP);
		}
		else if (m_state != STATE_DAMAGE && m_state != STATE_JUMPDROP && m_state != STATE_DASH && m_bInvincible == false)
		{// �G�ɓ�������
			SetState(STATE_DAMAGE);
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
// �T�C�Y�̐ݒ�
//===============================================
void CPlayer::SetSize(D3DXVECTOR3 size)
{
	m_vtxMax = size;
}

//===============================================
// �T�C�Y�̐ݒ�
//===============================================
void CPlayer::SetSizeMin(D3DXVECTOR3 size)
{
	m_vtxMin = size;
}

//===============================================
// ��Ԃ̐ݒ�
//===============================================
void CPlayer::SetState(EState state)
{
	if (state == STATE_LANDDROP && m_state == STATE_HIPDROP)
	{// �q�b�v�h���b�v���ɒn�`�֒��n
		m_fLenthCamera = LEMTH_HIPDROP;
		m_move.y = 0.0f;
		SetJump(false);
	}
	else if (state != STATE_LANDDROP && state != STATE_DAMAGE && state != STATE_DASH && m_state == STATE_NORMAL)
	{// �q�b�v�h���b�v���ȊO�Œn�`�֒��n
		m_move.y = 0.0f;
		SetJump(false);
	}

	if (m_state == STATE_HIPDROP && state == STATE_JUMPDROP)
	{// �q�b�v�h���b�v���ɓG�̓���֒��n
		m_nStateCounter = 2;				// ��ԃJ�E���^�[��ݒ�
		m_move.y = JUMP_HIPDROP;
		m_bAirJump = false;					// �󒆃W�����v�t���O���Z�b�g
		m_pMotion->Set(MOTIONTYPE_JUMP);	// �W�����v���[�V����

		// �p�[�e�B�N���̐���
		CParticle::Create(3)->Set(D3DXVECTOR3(m_pos.x - sinf(D3DX_PI * ROT_UP + (1.0f * m_rot.y) * 5.0f), m_pos.y,
			m_pos.z - cosf(D3DX_PI * ROT_UP + (1.0f * m_rot.y) * 5.0f)), CParticle::TYPE_ENEMY);
	}
	else if (m_state != STATE_DAMAGE && m_state != STATE_JUMPDROP && m_state != STATE_DASH && state == STATE_DAMAGE)
	{// �G�ɓ�������
		m_pMotion->Set(MOTIONTYPE_DAMAGE);
		m_nStateCounter = 30;
		m_move.z += cosf(D3DX_PI * ROT_UP + (ROT_CAMERA * m_rot.y)) * 12.0f;
	}

	m_state = state;

	switch (m_state)
	{
	case STATE_NORMAL:		// �ʏ�
		break;

	case STATE_DASH:		// �_�b�V��
		break;

	case STATE_AIRSLASH:	// �󒆍U��
		break;

	case STATE_HIPDROP:		// �q�b�v�h���b�v
		break;

	case STATE_LANDDROP:	// �q�b�v�h���b�v���n
		break;

	case STATE_JUMPDROP:	// �q�b�v�h���b�v�W�����v
		break;

	case STATE_DAMAGE:		// �_���[�W
		break;

	case STATE_INVINCIBLE:	// ���G
		break;

	case STATE_DEATH:		// ���S
		break;
	}
}

//===============================================
// ���[�V�����̐ݒ�
//===============================================
void CPlayer::SetMotion(MOTIONTYPE type)
{
	m_pMotion->Set(type);
}