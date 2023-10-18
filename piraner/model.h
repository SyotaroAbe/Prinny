//=========================================================
//
// モデル描画処理 [model.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _MODEL_H_  // このマクロ定義がされてなかったら
#define _MODEL_H_  // ２重インクルード防止のマクロを定義する

#include "object.h"

//===============================================
//マクロ定義
//===============================================
#define ROT_DIFF_PI			(2.0f)	// 角度の値の補正
#define ROT_CAMERA			(1.0f)	// カメラの向きに合わせて動かす

//===============================================
// モデルクラス
//===============================================
class CModel
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CModel();						// デフォルトコンストラクタ
	virtual ~CModel();				// デストラクタ

	static CModel *Create(const char *pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	virtual HRESULT Init(const char *pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void DrawShadowmtx(void);
	
	void BindX(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat);
	void SetVtxSize(void);

	void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosOri(void) { return m_posOri; }
	void SetRot(D3DXVECTOR3 rot);
	void SetCol(D3DXCOLOR col);
	D3DXVECTOR3 GetVtxMax(void) { return m_vtxMax; }
	D3DXVECTOR3 GetVtxMin(void) { return m_vtxMin; }
	void SetParent(CModel *pModel);

protected:	// 派生クラスからもアクセスできる [アクセス指定子]

private:	// 自分のみアクセス可能 [アクセス指定子]
	LPDIRECT3DTEXTURE9 *m_apTexture;			// テクスチャへのポインタ
	int *m_apIdxTexture;						// 使用するテクスチャの番号
	LPD3DXMESH m_pMesh;							// メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER m_pBuffMat;					// マテリアルへのポインタ
	DWORD m_dwNumMat;							// マテリアルの数
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス

	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOri;	// 初期位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXCOLOR m_col;		// 色
	D3DXVECTOR3 m_vtxMax;	// モデルの最大値
	D3DXVECTOR3 m_vtxMin;	// モデルの最小値
	CModel *m_pParent;		// 親モデルのポインタ
};

#endif