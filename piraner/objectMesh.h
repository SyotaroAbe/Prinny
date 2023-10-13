//=========================================================
//
// オブジェクトメッシュ処理 [objectMesh.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _OBJECTMESH_H_  // このマクロ定義がされてなかったら
#define _OBJECTMESH_H_  // ２重インクルード防止のマクロを定義する

#include "object.h"

//===============================================
// オブジェクトメッシュクラス
//===============================================
class CObjectMesh : public CObject
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CObjectMesh();					// デフォルトコンストラクタ
	CObjectMesh(int nPriority = 3);	// オーバーロードされたコンストラクタ
	virtual ~CObjectMesh();			// デストラクタ

	// 色の種類
	enum COL
	{
		COL_NORMAL = 0,		// 通常
		COL_RED,			// 赤
		COL_BLUE,			// 青
		COL_GREEN,			// 緑
		COL_MAX
	};

	static CObjectMesh *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeZ, int nPriority = 3);

	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void BindTexture(int nIdx);
	float GetSizeX(void) { return m_fSizeX; }
	float GetSizeZ(void) { return m_fSizeZ; }

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; };
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetMove(const D3DXVECTOR3 move);
	void SetSize(const float fSizeX, const float fSizeZ);
	void SetCol(const COL col);
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_fSizeX, 0.0f, m_fSizeZ); }

protected:	// 派生クラスからもアクセスできる
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pldxBuff = NULL;	// インデックスバッファへのポインタ
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_posOld;						// 前回の位置
	D3DXVECTOR3 m_move;							// 移動量
	D3DXVECTOR3 m_rot;							// 向き
	int m_nCntMeshX;							// 頂点数（横）
	int m_nCntMeshY;							// 頂点数（縦）
	float m_fSizeX;								// 幅
	float m_fSizeZ;								// 奥行き

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_nIdxTexture;	// 使用するテクスチャの番号
};

#endif