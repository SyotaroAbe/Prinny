//=========================================================
//
// オブジェクト2D処理 [object2D.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _OBJECT2D_H_     // このマクロ定義がされてなかったら
#define _OBJECT2D_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object.h"

//===============================================
// オブジェクト2Dクラス
//===============================================
class CObject2D : public CObject
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CObject2D();					// デフォルトコンストラクタ
	CObject2D(int nPriority = 3);	// オーバーロードされたコンストラクタ
	virtual ~CObject2D();			// デストラクタ

	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void UpdatePos(D3DXVECTOR3 pos, float fSizeX, float fSizeY);
	void UpdatePosAngle(D3DXVECTOR3 pos);
	void UpdateAnim(int nPatternX, int nPatternY, int nSpeed);
	void UpdateTex(const float fTexU, const float fSubU, const float fAddU, const float fTexV, const float fSubV, const float fAddV);
	bool RevisionPos(const D3DXVECTOR3 pos, float fMalti, bool bWidth = false);
	void DrawEffect(D3DXCOLOR col);
	void Brightness(float fBrightness);

	void BindTexture(int nIdx);
	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	virtual D3DXVECTOR3 GetPosOld(void) { return m_posOld; };
	void SetMove(const D3DXVECTOR3 move);
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetCol(const D3DXCOLOR col);
	void SetNumber(const int nNumber);
	void SetSize(const float fSizeX, const float fSizeY);
	float GetSizeX(void) { return m_fSizeX; }
	float GetSizeY(void) { return m_fSizeY; }
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_fSizeX, m_fSizeY, 0.0f); }

protected:	// 派生クラスからもアクセスできる [アクセス指定子]
	LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ

	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_rot;		// 向き
	float m_fLength;		// 対角線の長さ
	float m_fAngle;			// 対角線の角度
	float m_fSizeX;			// 幅
	float m_fSizeY;			// 高さ

private:	// 自分のみアクセス可能 [アクセス指定子]
	float m_fBrightness;				// 明るさの値
	bool m_bFlash;						// 明るくするか暗くするか
};

#endif