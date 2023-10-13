////=========================================================
////
//// メッシュウォール描画処理 [meshwall.h]
//// Author = 阿部翔大郎
////
////=========================================================
//#ifndef _MESHWALL_H_  // このマクロ定義がされてなかったら
//#define _MESHWALL_H_  // ２重インクルード防止のマクロを定義する
//
//#include "objectMesh.h"
//
////===============================================
//// マクロ定義
////===============================================
//#define MAX_MESHWALL	(8)			// 最大壁数
//#define WALL_COLISIONX	(-280.0f)	// 壁当たり判定
//#define WALL_COLISIONZ	(4600.0f)	// 壁当たり判定
//#define WALL_SIZE		(-1800.0f)	// 壁サイズ
//
////===============================================
//// メッシュウォールクラス
////===============================================
//class CMeshwall : CObjectMesh
//{
//public:		// 誰でもアクセス可能 [アクセス指定子]
//	CMeshwall();		// デフォルトコンストラクタ
//	~CMeshwall();		// デストラクタ
//
//	// 壁の種類
//	typedef enum
//	{
//		MESHWALLTYPE_NORMAL = 0,	// 通常
//		MESHWALLTYPE_INVISIBLE,		// 透明
//		MESHWALLTYPE_MAX
//	}MESHWALLTYPE;
//
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int type);
//
//private:	// 自分のみアクセス可能 [アクセス指定子]
//	LPDIRECT3DTEXTURE9 g_pTexture = NULL;		// テクスチャへのポインタ
//	LPDIRECT3DVERTEXBUFFER9 g_pVtxBuff = NULL;	// 頂点バッファへのポインタ
//	LPDIRECT3DINDEXBUFFER9 g_pldxBuff = NULL;	// インデックスバッファへのポインタ
//
//	D3DXVECTOR3 pos;				// 位置
//	D3DXVECTOR3 rot;				// 向き
//	int nMeshwallType;				// タイプ
//	D3DXMATRIX mtxWorldMeshwall;	// ワールドマトリックス
//};
//
//#endif