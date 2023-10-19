//=========================================================
//
// オブジェクト処理 [object.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _OBJECT_H_     // このマクロ定義がされてなかったら
#define _OBJECT_H_     // 2重インクルード防止のマクロ定義する

#include "main.h"

//===============================================
// 前方宣言
//===============================================
class CShadow;

//===============================================
// マクロ定義
//===============================================
#define MAX_OBJECT		(2048)		// 最大オブジェクト数
#define PRIORITY_MAX	(8)			// 優先順位の総数

//===============================================
// オブジェクトクラス
//===============================================
class CObject
{
public:		// 誰でもアクセス可能 [アクセス指定子]
	CObject();						// デフォルトコンストラクタ
	CObject(int nPriority = 3);		// オーバーロードされたコンストラクタ
	virtual ~CObject();				// デストラクタ

	// オブジェクトの種類
	typedef enum
	{
		TYPE_NONE = 0,	// なし
		TYPE_PLAYER,	// プレイヤー
		TYPE_ENEMY,		// 敵
		TYPE_BLOCK,		// ブロック
		TYPE_BULLET,	// 弾
		TYPE_EXPLOSION,	// 爆発
		TYPE_ITEM,		// アイテム
		TYPE_BG,		// 背景
		TYPE_EFFECT,	// エフェクト
		TYPE_FIELD,		// 床
		TYPE_WALL,		// 壁
		TYPE_BILLBOARD,	// ビルボード
		TYPE_TARGET,	// ターゲット
		TYPE_ROCKON,	// ロックオン
		TYPE_XFAIL,		// Xファイル
		TYPE_GARBAGE,	// ごみ
		TYPE_DUMPSTER,	// ゴミステーション
		TYPE_SHADOW,	// 影
		TYPE_SCENE,		// シーン
		TYPE_PAUSE,		// ポーズ
		TYPE_RANKIN,	// ランクイン
		TYPE_MAX
	}TYPE;

	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void Reset(void);
	static void UpdateAll(void);
	static void DrawAll(void);

	static CObject *GetTop(int nPriority) { return m_apTop[nPriority]; }
	CObject *GetNext(void) { return m_pNext; }
	void SetType(const TYPE type);
	TYPE GetType(void) { return m_type; }

	virtual void SetPos(const D3DXVECTOR3 pos) = 0;
	virtual D3DXVECTOR3 GetPos(void) = 0;
	virtual D3DXVECTOR3 GetPosOld(void) = 0;
	virtual void SetMove(const D3DXVECTOR3 move) = 0;
	virtual D3DXVECTOR3 GetRot(void) = 0;
	virtual void SetSize(D3DXVECTOR3 size) = 0;
	virtual D3DXVECTOR3 GetSize(void) = 0;
	virtual void SetSizeMin(D3DXVECTOR3 size) = 0;
	virtual D3DXVECTOR3 GetSizeMin(void) = 0;

protected:	// 派生クラスからもアクセスできる [アクセス指定子]
	void Release(void);

	CShadow *m_pShadow;		// 影へのポインタ

private:	// 自分のみアクセス可能 [アクセス指定子]
	static CObject *m_apTop[PRIORITY_MAX];			// 先頭のオブジェクトへのポインタ
	static CObject *m_apCur[PRIORITY_MAX];			// 最後尾のオブジェクトへのポインタ
	CObject *m_pPrev;								// 前のオブジェクトへのポインタ
	CObject *m_pNext;								// 次のオブジェクトへのポインタ

	TYPE m_type;				// オブジェクトの種類
	static int m_nNumAll;		// オブジェクトの総数
	int m_nPriority;			// 優先順位の位置
	bool m_bDeath;				// 死亡フラグ
};

#endif