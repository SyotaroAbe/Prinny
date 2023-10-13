//=========================================================
//
// テクスチャ管理処理 [texture.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//===============================================
// 静的メンバ変数
//===============================================
int CTexture::m_nNumAll = 0;								// テクスチャの総数
const char *CTexture::m_apFileName[MAX_TEXTURE] = {};		// テクスチャファイル名

//===============================================
// コンストラクタ
//===============================================
CTexture::CTexture()
{
	// 値をクリアする
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{
		m_apTexture[nCntTexture] = NULL;
	}
}

//===============================================
// デストラクタ
//===============================================
CTexture::~CTexture()
{

}

//===============================================
// モデルの生成（読み込み）
//===============================================
HRESULT CTexture::Load(void)
{
	// テクスチャを事前に読み込む

	return S_OK;
}

//===============================================
// テクスチャの破棄
//===============================================
void CTexture::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < m_nNumAll; nCntTexture++)
	{
		if (m_apTexture[nCntTexture] != NULL)
		{// 使用されている
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = NULL;
		}
	}
}

//===============================================
// テクスチャの破棄
//===============================================
void CTexture::Delete(int nIdx)
{
	if (nIdx >= 0)
	{
		if (m_apTexture[nIdx] != NULL)
		{// 既に読み込まれたテクスチャファイル
			m_apTexture[nIdx]->Release();
			m_apTexture[nIdx] = NULL;
		}
	}
}

//===============================================
// テクスチャの読み込み
//===============================================
int CTexture::Regist(const char *pFilename)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();   // デバイスの取得

	for (int nCntTexture = 0; nCntTexture < m_nNumAll; nCntTexture++)
	{
		if (m_apFileName[nCntTexture] == pFilename)
		{// 既に読み込まれたテクスチャファイル
			return nCntTexture;		// 同じ番号で返す
		}
	}

	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{
		if (m_apTexture[nCntTexture] == NULL)
		{// 使用されていない
			m_apFileName[nCntTexture] = pFilename;

			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pFilename,
				&m_apTexture[nCntTexture]);

			m_nNumAll++;
			return nCntTexture;
		}
	}

	return -1;
}