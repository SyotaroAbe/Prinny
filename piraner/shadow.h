//=========================================================
//
// 影の描画処理 [shadow.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _SHADOW_H_  // このマクロ定義がされてなかったら
#define _SHADOW_H_  // ２重インクルード防止のマクロを定義する

#include "object3D.h"

//===============================================
// 影クラス
//===============================================
class CShadow : public CObject3D
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CShadow();					// デフォルトコンストラクタ
	CShadow(int nPriority = 3);	// オーバーロードされたコンストラクタ
	~CShadow();					// デストラクタ

	static CShadow *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeZ, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected:	// 派生クラスからもアクセスできる

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_nIdxTexture;	// 使用するテクスチャの番号
};

#endif