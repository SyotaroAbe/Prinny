//=========================================================
//
// メッシュフィールド描画処理 [meshfield.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _MESHFIELD_H_  // このマクロ定義がされてなかったら
#define _MESHFIELD_H_  // ２重インクルード防止のマクロを定義する

#include "objectMesh.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_FIELD				(36)		// 最大床数

//===============================================
// メッシュフィールドクラス
//===============================================
class CMeshField : CObjectMesh
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CMeshField();					// デフォルトコンストラクタ
	CMeshField(int nPriority = 3);	// オーバーロードされたコンストラクタ
	~CMeshField();					// デストラクタ

	static CMeshField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSizeX, float fSizeZ, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void TurnInfo(float fRot);

	int GetNumAll(void) { return m_nNumAll; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_nIdxTexture;					// 使用するテクスチャの番号
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャへのポインタ
	static TYPE m_aType[MAX_FIELD];						// 床の種類
	static int m_nNumAll;								// 総数

	D3DXVECTOR3 m_PosMin;				// 位置の最小値
	D3DXVECTOR3 m_PosMax;				// 位置の最大値

	int m_nTimeFever;				// フィーバー状態の時間
	int m_nRandFever;				// フィーバー状態のランダム
};

#endif