//=========================================================
//
// 敵処理 [enemy.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _ENEMY_H_     // このマクロ定義がされてなかったら
#define _ENEMY_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"
#include "object.h"
#include "fileload.h"

//===============================================
// 前方宣言
//===============================================
class CModel;
class CMotion;

//===============================================
// 敵クラス
//===============================================
class CEnemy : public CObject
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CEnemy();						// デフォルトコンストラクタ
	CEnemy(int nPriority = 3);		// オーバーロードされたコンストラクタ
	~CEnemy();						// デストラクタ

	// プレイヤーの状態
	enum EState
	{
		STATE_NONE = 0,		// なし
		STATE_NORMAL,		// 通常
		STATE_MOVERIGHT,	// 右移動
		STATE_MOVELEFT,		// 左移動
		STATE_DASH,			// ダッシュ
		STATE_ATTACK,		// 攻撃
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
		MOTIONTYPE_ACTION,		// アクション
		MOTIONTYPE_JUMP,		// ジャンプ
		MOTIONTYPE_LANDING,		// 着地
		MOTIONTYPE_MAX
	};

	static CEnemy *Create(D3DXVECTOR3 pos, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void CollisionObjX(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetMove(const D3DXVECTOR3 move);
	D3DXVECTOR3 GetMove(void) { return m_move; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetJump(const bool bJump);
	D3DXVECTOR3 GetSize(void) { return m_vtxMax; }
	D3DXVECTOR3 GetSizeMin(void) { return m_vtxMin; }
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
	float m_fRotDiff;						// 目的の向きまでの差分

	EState m_state;							// 状態

	CModel *m_apModel[MAX_MODEL];			// モデル（パーツ）へのポインタ
	int m_nNumModel;						// モデル（パーツ）の総数
	CMotion *m_pMotion;						// モーションのへのポインタ
};

#endif