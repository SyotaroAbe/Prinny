//=========================================================
//
// モーション処理 [motion.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "motion.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "corecrt_wstdio.h"
#include <stdio.h>

//===============================================
// コンストラクタ
//===============================================
CMotion::CMotion()
{
	// 値をクリアする
	m_aInfo[MAX_MOTION];
	m_nNumAll = 0;
	m_nType = 0;
	m_bLoop = false;
	m_nNumKey = 0;
	m_nKey = 0;
	m_nCounter = 0;
	m_bFinish = false;
	m_ppModel = NULL;
	m_nNumModel = 0;
}

//===============================================
// デストラクタ
//===============================================
CMotion::~CMotion()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CMotion::Init(void)
{

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CMotion::Uninit(void)
{

}

//===============================================
// 設定処理
//===============================================
void CMotion::Set(int nType)
{
	if (m_nType != nType)
	{// 種類が変更された
		m_nType = nType;		// 種類を設定

		// 値を初期化
		m_nKey = 0;
		m_nCounter = 0;
		m_bFinish = false;
	}
}

//===============================================
// 更新処理
//===============================================
void CMotion::Update(void)
{
	D3DXVECTOR3 posDest, rotDest;	// 目的の位置、目的の向き
	D3DXVECTOR3 posOri;				// 初期位置
	int nNextKey = 0;				// 次のキー

	if (m_aInfo[m_nType].bLoop == true || m_nKey < m_aInfo[m_nType].nNumKey)
	{// ループするもしくはキーの総数に達していない
		// 次のキーを現在のキーの1つ後に設定する
		nNextKey = (m_nKey + 1) % m_aInfo[m_nType].nNumKey;

		// 全モデル（パーツ）の更新
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			posOri = m_ppModel[nCntModel]->GetPosOri();		// 初期位置を取得

			// キー情報から位置・向きを算出
			posDest.x = (m_aInfo[m_nType].aKeyInfo[nNextKey].aKey[nCntModel].fPosX
						- m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosX);
			posDest.y = m_aInfo[m_nType].aKeyInfo[nNextKey].aKey[nCntModel].fPosY
						- m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosY;
			posDest.z = m_aInfo[m_nType].aKeyInfo[nNextKey].aKey[nCntModel].fPosZ
						- m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosZ;
			rotDest.x = m_aInfo[m_nType].aKeyInfo[nNextKey].aKey[nCntModel].fRotX
						- m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotX;
			rotDest.y = m_aInfo[m_nType].aKeyInfo[nNextKey].aKey[nCntModel].fRotY
						- m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotY;
			rotDest.z = m_aInfo[m_nType].aKeyInfo[nNextKey].aKey[nCntModel].fRotZ
						- m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotZ;

			// パーツの位置・向きを設定
			D3DXVECTOR3 pos = posOri + D3DXVECTOR3(
				m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosX + posDest.x
				* ((float)m_nCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame),
				m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosY + posDest.y
				* ((float)m_nCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame),
				m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosZ + posDest.z
				* ((float)m_nCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame));
			D3DXVECTOR3 rot = D3DXVECTOR3(
				m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotX + rotDest.x
				* ((float)m_nCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame),
				m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotY + rotDest.y
				* ((float)m_nCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame),
				m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotZ + rotDest.z
				* ((float)m_nCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame));

			// モデルに値を設定
			m_ppModel[nCntModel]->SetPos(pos);
			m_ppModel[nCntModel]->SetRot(rot);
		}
		m_nCounter++;	// モーションカウンターを進める

		if (m_nCounter >= m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame)
		{// モーションカウンターが再生フレームに達した
			m_nKey++;				// 現在のキーを1つ進める
			m_nCounter = 0;			// カウンターを初期化する
		}

		if (m_nKey >= m_aInfo[m_nType].nNumKey)
		{// キーNo.が総数以上
			if (m_aInfo[m_nType].bLoop == true)
			{// ループするモーション中
				m_nKey = 0;				// 現在のキーを初期化する
			}
			else
			{// ループしない
				m_bFinish = true;
			}
		}
	}
}

//===============================================
// 情報設定処理
//===============================================
void CMotion::SetInfo(INFO info)
{
	m_aInfo[m_nType] = info;
}

//===============================================
// モデル設定処理
//===============================================
void CMotion::SetModel(CModel **ppModel, int nNumModel)
{
	m_ppModel = ppModel;
	m_nNumModel = nNumModel;
}