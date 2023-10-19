//=========================================================
//
// ビルボード処理 [objectBillboard.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _OBJECTBILLBOARD_H_  // このマクロ定義がされてなかったら
#define _OBJECTBILLBOARD_H_  // ２重インクルード防止のマクロを定義する

#include "object3D.h"

//===============================================
// マクロ定義
//===============================================
#define SIZE_DIFF			(0.4f)		// サイズの値の補正

//===============================================
// ビルボードクラス
//===============================================
class CObjectBillboard : public CObject
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CObjectBillboard();						// デフォルトコンストラクタ
	CObjectBillboard(int nPriority = 3);	// オーバーロードされたコンストラクタ
	~CObjectBillboard();					// デストラクタ

	static CObjectBillboard *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeZ, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void UpdatePos(D3DXVECTOR3 pos, float fSizeX, float fSizeY);
	void RevisionPos(const D3DXVECTOR3 pos, const float fMalti = 1.0f);
	bool ScalingSize(const float fSize, float fMalti = SIZE_DIFF);
	void DrawEffect(void);
	void Brightness(float fBrightness);

	void BindTexture(int nIdx);
	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; };
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetMove(const D3DXVECTOR3 move);
	void SetCol(const D3DXCOLOR col);
	void SetSize(const float fSizeX, const float fSizeZ);
	void SetSize(D3DXVECTOR3 size);
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_fSizeX, 0.0f, m_fSizeZ); }
	void SetSizeMin(D3DXVECTOR3 size);
	D3DXVECTOR3 GetSizeMin(void) { return D3DXVECTOR3(m_fSizeX, 0.0f, m_fSizeZ); }

protected:	// 派生クラスからもアクセスできる [アクセス指定子]
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_posOld;						// 前回の位置
	D3DXVECTOR3 m_move;							// 移動量
	D3DXVECTOR3 m_rot;							// 向き
	float m_fSizeX;								// 幅
	float m_fSizeZ;								// 高さ

private:	// 自分のみアクセス可能 [アクセス指定子]
	float m_fBrightness;				// 明るさの値
	bool m_bFlash;						// 明るくするか暗くするか
};

#endif