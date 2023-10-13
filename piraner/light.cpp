//=========================================================
// 
// ライト処理 [light.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "main.h"
#include "light.h"
#include "manager.h"
#include "renderer.h"

//===============================================
// コンストラクタ
//===============================================
CLight::CLight()
{
	for (int nCntLight = 0; nCntLight < MAX_LGHT; nCntLight++)
	{// ライトの最大数分繰り返す
		// 情報をクリアする
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));
	}
}

//===============================================
// デストラクタ
//===============================================
CLight::~CLight()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	D3DXVECTOR3 vecDir;													// 設定用方向ベクトル

	for (int nCntLight = 0; nCntLight < MAX_LGHT; nCntLight++)
	{// ライトの最大数分繰り返す
		// 情報をクリアする
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));

		// 種類を設定
		m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		// 拡散光を設定
		m_aLight[nCntLight].Diffuse = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	}

	// 方向を設定
	vecDir = D3DXVECTOR3(0.22f, -0.87f, -0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);		// ベクトルを正規化する
	m_aLight[0].Direction = vecDir;
	m_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 拡散光を設定

	vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);		// ベクトルを正規化する
	m_aLight[1].Direction = vecDir;
	m_aLight[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);	// 拡散光を設定

	vecDir = D3DXVECTOR3(-0.79f, -0.11f, 0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);		// ベクトルを正規化する
	m_aLight[2].Direction = vecDir;
	m_aLight[2].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);	// 拡散光を設定

	for (int nCntLight = 0; nCntLight < MAX_LGHT; nCntLight++)
	{// ライトの最大数分繰り返す
		// ライトを設定する
		pDevice->SetLight(nCntLight, &m_aLight[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CLight::Uninit(void)
{
	
}

//===============================================
// 更新処理
//===============================================
void CLight::Update(void)
{

}