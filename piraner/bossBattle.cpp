//=========================================================
//
// �{�X�폈�� [bossBattle.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "bossBattle.h"
#include "object3D.h"
#include "objectX.h"
#include "meshfield.h"
#include "pause.h"
#include "input.h"
#include "renderer.h"
#include "fade.h"
#include "camera.h"
#include "debugproc.h"
#include "sound.h"
#include "player.h"
#include "boss.h"
#include "gamebg.h"
#include "clear.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CPlayer *CBossBattle::m_pPlayer = NULL;						// �v���C���[�N���X�̃|�C���^
CMeshField *CBossBattle::m_pMeshField = NULL;					// ���b�V���t�B�[���h�N���X�̃|�C���^
CObject3D *CBossBattle::m_pObject3D = NULL;					// �I�u�W�F�N�g3D�N���X�̃|�C���^
CPause *CBossBattle::m_pPause = NULL;							// �|�[�Y�N���X�̃|�C���^
CBoss *CBossBattle::m_pBoss = NULL;							// �{�X�N���X�̃|�C���^
CGameBg *CBossBattle::m_pGameBg = NULL;						// �w�i�N���X�̃|�C���^
CClear *CBossBattle::m_pClear = NULL;						// �N���A�\���N���X�̃|�C���^

bool CBossBattle::m_bPause = false;				// �|�[�Y���
bool CBossBattle::m_bStateReady = false;			// GAMSESTATE_READY���ǂ���
bool CBossBattle::m_bPauseCamera = false;			// �|�[�Y���̃J��������\��

//===============================================
// �R���X�g���N�^
//===============================================
CBossBattle::CBossBattle() : CScene()
{
	// �l�̃N���A
	m_state = STATE_NONE;
	m_nCounterState = 0;
}

//===============================================
// �f�X�g���N�^
//===============================================
CBossBattle::~CBossBattle()
{
	
}

//===============================================
// ����������
//===============================================
HRESULT CBossBattle::Init(HWND hWnd)
{
	m_bPause = false;
	m_bStateReady = true;		// �ҋ@��Ԃɂ���
	m_bPauseCamera = false;

	// �J�����̏���������
	CManager::GetInstance()->GetCamera()->Init();

	// �I�u�W�F�N�gX�t�@�C���̐���
	CObjectX::Load(hWnd);

	// �v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 330.0f, -100.0f), 4);

	// �w�i�̐���
	m_pGameBg = CGameBg::Create(D3DXVECTOR3(m_pPlayer->GetPos().x, m_pPlayer->GetPos().y, CManager::GetInstance()->GetCamera()->GetPosR().z), CGameBg::TEX_BOSS, 0);

	// �{�X�̐���
	m_pBoss = CBoss::Create(D3DXVECTOR3(50.0f, 420.0f, 900.0f), CBoss::TYPE_WALK, 4);

	// �N���A�\���̐���
	m_pClear = CClear::Create();

	// �|�[�Y�̐���
	m_pPause = CPause::Create(6);

	// �ʏ��Ԃɐݒ�
	m_state = STATE_NORMAL;
	m_nCounterState = 0;

	// �T�E���h�̍Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CBossBattle::Uninit(void)
{
	// �|�[�Y�̏I������
	m_pPause->Uninit();
	delete m_pPause;
	m_pPause = NULL;

	// �S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();
}

//===============================================
// �X�V����
//===============================================
void CBossBattle::Update(void)
{
	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_P) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_START, 0) == true)
	{// �|�[�Y����
		m_bPause = m_bPause ? false : true;		// �|�[�Y��Ԑ؂�ւ�

		if (m_bPause == true)
		{
			// �T�E���h�̍Đ�
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_PAUSE);
		}
		else
		{
			// �T�E���h�̍Đ�
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_PAUSE_CANCEL);
		}
	}

//#if _DEBUG
	if (m_bPause == true && CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_F3) == true)
	{// �|�[�Y���J��������
		m_bPauseCamera = m_bPauseCamera ? false : true;		// �|�[�Y��Ԑ؂�ւ�
	}

	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_BACKSPACE) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_BACK, 0) == true || m_pPlayer->GetPos().z > 7500.0f)
	{// BackSpace
		CRenderer::GetFade()->Set(CScene::MODE_RESULT);		// ���U���g��ʂֈړ�
	}
//#endif

	if (m_pPlayer->GetPos().y < -200.0f)
	{// ������
		GetPlayer()->SetState(CPlayer::STATE_DEATH);
		CRenderer::GetFade()->Set(CScene::MODE_BOSS);		// ���U���g��ʂֈړ�
	}

	if (m_bStateReady == false)
	{// �ҋ@��Ԃ���Ȃ�
		if (m_bPause == false)
		{// �|�[�Y��Ԃ���Ȃ�
			
		}
	}
	else if (m_bStateReady == true)
	{// �ҋ@��Ԃ̃t���O�������Ă���
		if (m_state == STATE_NORMAL)
		{
			// �ҋ@��Ԃ֐؂�ւ���
			m_state = STATE_READY;
			m_nCounterState = 120;
		}
	}

	if (m_bPause == true && m_bPauseCamera == false)
	{// �|�[�Y���
		// �|�[�Y�̍X�V����
		m_pPause->Update();
	}

	switch (m_state)
	{
	case STATE_NORMAL:  // �ʏ���
		m_nCounterState--;
		break;

	case STATE_READY:	// �J�n�ҋ@���
		if (m_bPause == false)
		{// �|�[�Y��Ԃ���Ȃ�
			m_nCounterState--;

			if (m_nCounterState <= 0)
			{
				m_state = STATE_NORMAL;				// �ʏ��Ԃɐݒ�
				m_nCounterState = 5400;
				m_bStateReady = false;
			}
		}
		break;

	case STATE_END:     // �I�����
		if (m_bPause == false)
		{// �|�[�Y��Ԃ���Ȃ�
			m_nCounterState--;

			if (m_nCounterState <= 0)
			{
				m_state = STATE_NONE;  // �������Ă��Ȃ���Ԃɐݒ�
			}
		}
		break;
	}

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(" ���݂̏�ԁF%d\n", m_state);
}

//===============================================
// �`�揈��
//===============================================
void CBossBattle::Draw(void)
{
	
}

//===============================================
// �|�[�Y��Ԃ̐ݒ�
//===============================================
void CBossBattle::SetEnablePause(const bool bPause)
{
	m_bPause = bPause;
}