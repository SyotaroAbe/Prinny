//=========================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "title.h"
#include "bg.h"
#include "input.h"
#include "renderer.h"
#include "fade.h"
#include "sound.h"
#include "meshfield.h"
#include "manager.h"
#include "camera.h"

//===============================================
// �}�N����`
//===============================================
#define TIME_FADERANKING		(800)			// �����t�F�[�h����

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CBg *CTitle::m_pBg = NULL;						// �w�i�N���X�̃|�C���^

//===============================================
// �R���X�g���N�^
//===============================================
CTitle::CTitle() : CScene()
{
	// �l�̃N���A
	m_nTimeFade = 0;
	m_bFade = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CTitle::~CTitle()
{
	
}

//===============================================
// ����������
//===============================================
HRESULT CTitle::Init(HWND hWnd)
{
	// �J�����̏���������
	CManager::GetCamera()->Init();

	//// �w�i�̐���
	//m_pBg = CBg::Create(CBg::TYPE_TITLE, 6);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CTitle::Uninit(void)
{
	// �S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();
}

//===============================================
// �X�V����
//===============================================
void CTitle::Update(void)
{
	m_nTimeFade++;		// ���Ԃ��J�E���g

	if (/*m_pLogo->GetEndAnim() == true &&*/ (CManager::GetKeyboardInput()->GetTrigger(DIK_RETURN) == true
		|| CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_A, 0) == true))
	{
		if (m_bFade == false)
		{// �t�F�[�h�o�O�h�~
			CRenderer::GetFade()->Set(CScene::MODE_TUTORIAL);	// �Q�[����ʂֈڍs
			m_bFade = true;

			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_TITLE_ENTER);
		}
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_Q) == true || CManager::GetKeyboardInput()->GetPress(DIK_Z) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_LB, 0) || CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_LEFT, 0)
		|| CManager::GetKeyboardInput()->GetPress(DIK_E) == true || CManager::GetKeyboardInput()->GetPress(DIK_C) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_RB, 0) || CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_RIGHT, 0))
	{// �v���C���[�i�J�����j�𑀍쒆
		m_nTimeFade = 0;		//	�J�E���g�����Z�b�g
	}
	else if(m_nTimeFade > TIME_FADERANKING)
	{// ��莞�Ԍo��
		if (m_bFade == false)
		{// �t�F�[�h�o�O�h�~
			CRenderer::GetFade()->Set(CScene::MODE_RESULT);	// �����L���O��ʂֈڍs
			m_bFade = true;
		}
	}
}

//===============================================
// �`�揈��
//===============================================
void CTitle::Draw(void)
{
	
}