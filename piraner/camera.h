//=========================================================
//
// カメラ処理 [camera.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _CAMERA_H_  // このマクロ定義がされてなかったら
#define _CAMERA_H_  // ２重インクルード防止のマクロを定義する

#include "main.h"

//===============================================
// マクロ定義
//===============================================
#define ROT_DIFF_PI			(2.0f)	// 角度の値の補正
#define ROT_CAMERA			(1.0f)	// カメラの向きに合わせて動かす
#define ROT_CAMERA_DIFF		(0.3f)	// 角度の補正係数（数が大きい程素早く変わる）

#define LENTH_NORMAL		(200.0f)	// 通常時のカメラの距離
#define LEMTH_HIPDROP		(310.0f)	// ヒップドロップ時のカメラの距離

//===============================================
// カメラクラス
//===============================================
class CCamera
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CCamera();	// デフォルトコンストラクタ
	~CCamera();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	bool ScalingLenth(const float fLenth, float fMalti);
	void Set();

	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

private:	// 自分のみアクセス可能 [アクセス指定子]
	D3DXMATRIX m_mtxView;		// ビューマトリックス
	D3DXMATRIX m_mtxProjection;	// プロジェクションマトリックス
	D3DXVECTOR3 m_posV;			// 視点
	D3DXVECTOR3 m_posVDest;		// 目的の視点
	D3DXVECTOR3 m_posR;			// 注視点
	D3DXVECTOR3 m_posRDest;		// 目的の注視点
	D3DXVECTOR3 m_vecU;			// 上方向ベクトル
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_rotDest;		// 目的の向き

	float m_fLength;			// カメラの距離
	int m_nViewMapCounter;		// マップタイムカウンタ
};

#endif