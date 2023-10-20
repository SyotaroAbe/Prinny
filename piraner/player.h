//=========================================================
//
// プレイヤー処理 [player.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _PLAYER_H_     // このマクロ定義がされてなかったら
#define _PLAYER_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object.h"
#include "fileload.h"

//===============================================
// 前方宣言
//===============================================
class CModel;
class CMotion;

//===============================================
// マクロ定義
//===============================================
#define ROT_LEFTUP			(-0.25f)	// 左上の角度
#define ROT_LEFT			(-0.5f)		// 左の角度
#define ROT_LEFTDOWN		(-0.75f)	// 左下の角度
#define ROT_RIGHTUP			(0.25f)		// 右上の角度
#define ROT_RIGHT			(0.5f)		// 右の角度
#define ROT_RIGHTDOWN		(0.75f)		// 右下の角度
#define ROT_UP				(0.0f)		// 上の角度
#define ROT_DOWN			(1.0f)		// 下の角度

//===============================================
// プレイヤークラス
//===============================================
class CPlayer : public CObject
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CPlayer();						// デフォルトコンストラクタ
	CPlayer(int nPriority = 3);		// オーバーロードされたコンストラクタ
	~CPlayer();						// デストラクタ

	// プレイヤーの状態
	enum EState
	{
		STATE_NONE = 0,		// なし
		STATE_NORMAL,		// 通常
		STATE_DASH,			// ダッシュ
		STATE_AIRSLASH,		// 空中攻撃
		STATE_HIPDROP,		// ヒップドロップ
		STATE_LANDDROP,		// ヒップドロップ着地
		STATE_JUMPDROP,		// ヒップドロップジャンプ
		STATE_DAMAGE,		// ダメージ
		STATE_INVINCIBLE,	// 無敵
		STATE_DEATH,		// 死亡
		STATE_MAX
	};

	// モーションの種類
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,	// 待機
		MOTIONTYPE_MOVE,		// 移動
		MOTIONTYPE_PREATTACK,	// 空中攻撃前動作
		MOTIONTYPE_JUMP,		// ジャンプ
		MOTIONTYPE_LANDING,		// 着地
		MOTIONTYPE_JUMPAIR,		// 空中ジャンプ
		MOTIONTYPE_ATTACKL,		// 空中攻撃左
		MOTIONTYPE_ATTACKR,		// 空中攻撃右
		MOTIONTYPE_HIPDROP,		// ヒップドロップ
		MOTIONTYPE_DASH,		// ダッシュ
		MOTIONTYPE_DAMAGE,		// ダメージ
		MOTIONTYPE_MAX
	};

	static CPlayer *Create(D3DXVECTOR3 pos, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void CollisionObjX(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);
	void CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetMove(const D3DXVECTOR3 move);
	D3DXVECTOR3 GetMove(void) { return m_move; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetJump(const bool bJump);
	void SetSize(D3DXVECTOR3 size);
	D3DXVECTOR3 GetSize(void) { return m_vtxMax; }
	void SetSizeMin(D3DXVECTOR3 size);
	D3DXVECTOR3 GetSizeMin(void) { return m_vtxMin; }
	void SetState(EState state);
	EState GetState(void) { return m_state; }
	void SetMotion(MOTIONTYPE type);

private:	// 自分のみアクセス可能 [アクセス指定子]
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_posOld;					// 前回の位置
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR3 m_rotDest;					// 目的の向き
	D3DXVECTOR3 m_move;						// 移動量
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3 m_vtxMax;					// モデルの最大値
	D3DXVECTOR3 m_vtxMin;					// モデルの最小値

	float m_fSpeed;							// 移動速度変更用
	bool m_bJump;							// ジャンプしたかどうか
	bool m_bAirJump;						// 2段ジャンプしたかどうか
	float m_fLenthCamera;					// カメラの距離
	float m_fRotBullet;						// 照準の向き
	float m_fRotDiff;						// 目的の向きまでの差分
	int m_nParticleCounter;					// パーティクル発生時間
	int m_nStateCounter;					// 状態管理カウンター
	bool m_bInvincible;						// 無敵状態かどうか

	EState m_state;							// 状態

	CModel *m_apModel[MAX_MODEL];			// モデル（パーツ）へのポインタ
	int m_nNumModel;						// モデル（パーツ）の総数
	CMotion *m_pMotion;						// モーションのへのポインタ
};

#endif