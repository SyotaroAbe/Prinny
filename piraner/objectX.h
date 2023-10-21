//=========================================================
//
// オブジェクトXファイル処理 [objectX.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _OBJECTX_H_  // このマクロ定義がされてなかったら
#define _OBJECTX_H_  // ２重インクルード防止のマクロを定義する

#include "object.h"
#include "xfile.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_NAME	(256)	// 最大文字数

//===============================================
// オブジェクトXファイルクラス
//===============================================
class CObjectX : public CObject
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CObjectX();						// デフォルトコンストラクタ
	CObjectX(int nPriority = 3);	// オーバーロードされたコンストラクタ
	virtual ~CObjectX();			// デストラクタ

	// モデルの種類
	typedef enum
	{
		MODEL_NONE = 0,			// なし
		MODEL_NORMAL,			// 通常床
		MODEL_DAMAGE,			// ダメージ床
		MODEL_MAX
	}MODEL;

	static CObjectX *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL type, int nPriority = 3);
	static void Load(HWND hWnd);
	static void Script(FILE *pFile);
	static void ModelSet(FILE *pFile);

	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void DrawXFile(int nIdx, CXFile::COL col);
	static bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);
	static bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetRot(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetMove(const D3DXVECTOR3 move);
	void SetSize(D3DXVECTOR3 size);
	D3DXVECTOR3 GetSize(void);
	void SetSizeMin(D3DXVECTOR3 size);
	D3DXVECTOR3 GetSizeMin(void);
	void SetModel(MODEL type);

protected:	// 派生クラスからもアクセスできる
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_rot;		// 向き

private:	// 自分のみアクセス可能 [アクセス指定子]
	static int m_aIdxXFile[MODEL_MAX];			// 使用するXファイルの番号
	
	MODEL m_modelType;							// モデルの種類
	D3DXVECTOR3 m_vtxMax;						// モデルの最大値
	D3DXVECTOR3 m_vtxMin;						// モデルの最小値
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	int m_nIdxShadow;							// 使用する影の番号
};

#endif