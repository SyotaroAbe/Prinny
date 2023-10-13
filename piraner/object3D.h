//=========================================================
//
// オブジェクト3D処理 [object3D.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _OBJECT3D_H_  // このマクロ定義がされてなかったら
#define _OBJECT3D_H_  // ２重インクルード防止のマクロを定義する

#include "object.h"

//===============================================
// オブジェクト3Dクラス
//===============================================
class CObject3D : public CObject
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CObject3D();					// デフォルトコンストラクタ
	CObject3D(int nPriority = 3);	// オーバーロードされたコンストラクタ
	virtual ~CObject3D();			// デストラクタ

	static CObject3D *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeZ, int nPriority = 3);

	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void DrawShadow(void);

	void BindTexture(int nIdx);
	float GetSizeX(void) { return m_fSizeX; }
	float GetSizeZ(void) { return m_fSizeZ; }

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; };
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetMove(const D3DXVECTOR3 move);
	void SetSize(const float fSizeX, const float fSizeZ);
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_fSizeX, 0.0f, m_fSizeZ); }
	float CollisionVec(D3DXVECTOR3 pos);

protected:	// 派生クラスからもアクセスできる [アクセス指定子]
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_posOld;						// 前回の位置
	D3DXVECTOR3 m_move;							// 移動量
	D3DXVECTOR3 m_rot;							// 向き
	float m_fSizeX;								// 幅
	float m_fSizeZ;								// 奥行き

	D3DXVECTOR3 m_aVtx[4];							// 頂点座標
	D3DXVECTOR3 m_nor;							// 法線ベクトル
	D3DXVECTOR3 m_vec0, m_vec1;

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_nIdxTexture;	// 使用するテクスチャの番号
};

#endif