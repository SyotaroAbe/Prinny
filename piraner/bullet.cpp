//=========================================================
//
// 弾処理 [bullet.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "enemy.h"
#include "effect.h"
#include "particle.h"
#include "sound.h"
#include "score.h"
#include "objectBillboard.h"
#include "texture.h"
#include "game.h"

//===============================================
// マクロ定義
//===============================================
#define BULLET_SIZEX		(8.0f)			// 幅（半分）
#define BULLET_SIZEY		(8.0f)			// 高さ（半分）
#define MOVE_BULLET			(5.0f)			// 動く速度
#define BULLET_LIFE			(120)			// ライフ

//===============================================
// 静的メンバ変数
//===============================================
int CBullet::m_nIdxTexture = 0;						// 使用するテクスチャの番号
int CBullet::m_nNumAll = 0;							// 総数

//===============================================
// コンストラクタ
//===============================================
CBullet::CBullet() : CObjectBillboard(2)
{
	// 値をクリア
	m_nLife = 0;

	m_nNumAll++;	// 総数をインクリメント
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CBullet::CBullet(int nPriority) : CObjectBillboard(nPriority)
{
	// 値をクリア
	m_nLife = 0;

	m_nNumAll++;	// 総数をインクリメント
}

//===============================================
// デストラクタ
//===============================================
CBullet::~CBullet()
{
	m_nNumAll--;	// 総数をデクリメント
}

//===============================================
// 生成処理
//===============================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, int nPriority)
{
	CBullet *pBullet;

	// 弾の生成
	pBullet = new CBullet(2);

	// 種類の設定
	pBullet->SetType(TYPE_BULLET);

	// サイズの設定
	pBullet->SetSize(BULLET_SIZEX, BULLET_SIZEY);

	// 初期化処理
	pBullet->Init(pos);

	// テクスチャの割り当て
	pBullet->BindTexture(m_nIdxTexture);

	return pBullet;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CBullet::Init(D3DXVECTOR3 pos)
{
	// オブジェクト2Dの初期化処理
	CObjectBillboard::Init(pos);

	// テクスチャの設定
	m_nIdxTexture = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\bullet000.png");

	// オブジェクト2Dの初期化処理
	CObjectBillboard::SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	// 体力の設定
	m_nLife = BULLET_LIFE;

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CBullet::Uninit(void)
{
	// オブジェクト2Dの終了処理
	CObjectBillboard::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CBullet::Update(void)
{
	//敵との当たり判定
	if (CollisionEnemy(m_pos) == false)
	{//弾が当たってない
		if (m_nLife <= 0)
		{//弾の位置が画面外に出た場合
			//CExplosion::Create(m_pos, 3)->SetPos(m_pos);	//爆発の生成
			Uninit();				//自分の終了処理

			////サウンドの再生
			//CManager::GetSound()->Play(CSound::LABEL_SE_EXPLOSION);
		}
		else
		{
			// 前回の位置を保存
			m_posOld = m_pos;

			//位置を更新
			m_pos.x += m_move.x;
			m_pos.y += m_move.y;
			m_pos.z += m_move.z;

			m_nLife--;	//体力を減らす

			// エフェクトの生成
			CParticle::Create(1)->Set(D3DXVECTOR3(m_pos.x + cosf(m_rot.y) * 18.0f, m_pos.y, m_pos.z - sinf(m_rot.y) * 18.0f), CParticle::TYPE_CURVE);
			CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f), CEffect::TYPE_NORMAL, 25, 25, 3);
			CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), CEffect::TYPE_NORMAL, 15, 25, 3);
		}
	}
	else
	{// 弾が当たった
		//CExplosion::Create(3)->SetPos(m_pos);	//爆発の生成
		Uninit();				//自分の終了処理
	}
}

//===============================================
// 描画処理
//===============================================
void CBullet::Draw(void)
{
	CObjectBillboard::Draw();
}

//===============================================
// 設定処理
//===============================================
void CBullet::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move)
{
	m_pos = pos;
	m_move = move;

	//// サウンドの再生
	//CManager::GetSound()->Play(CSound::LABEL_SE_SHOT);
}

//===============================================
// 敵との当たり判定
//===============================================
bool CBullet::CollisionEnemy(D3DXVECTOR3 pos)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = CObject::GetTop(nCntPriority);		// 先頭のオブジェクトを代入

		while(pObject != NULL)
		{// 使用されている
			CObject *pObjectNext = pObject->GetNext();		// 次のオブジェクト
			TYPE type = pObject->GetType();					// 種類を取得

			if (type == TYPE_ENEMY || type == TYPE_BOXNORMAL || type == TYPE_BOXDAMAGE)
			{// 敵、地形
				D3DXVECTOR3 ObjPos = pObject->GetPos();			// 位置を取得
				D3DXVECTOR3 ObjSize = pObject->GetSize();		// サイズを取得
				D3DXVECTOR3 ObjSizeMin = pObject->GetSizeMin();	// サイズを取得

				if (pos.x >= ObjPos.x + ObjSizeMin.x && pos.x <= ObjPos.x + ObjSize.x
					&& pos.z + BULLET_SIZEX >= ObjPos.z + ObjSizeMin.z && pos.z - BULLET_SIZEX <= ObjPos.z + ObjSize.z
					&& pos.y + BULLET_SIZEX >= ObjPos.y + ObjSizeMin.y && pos.y - BULLET_SIZEX <= ObjPos.y + ObjSize.y)
				{// 敵と弾が当たった

					//// サウンドの再生
					//CManager::GetSound()->Play(CSound::LABEL_SE_EXPLOSION_HIT);

					if (type == TYPE_ENEMY)
					{
						CParticle::Create(1)->Set(pObject->GetPos(), CParticle::TYPE_ENEMY);	// パーティクルの生成
						pObject->Uninit();														// 敵の終了処理
						//CGame::GetScore()->Add(ENEMY_SCORE);									// スコア加算
					}
					else
					{
						CParticle::Create(1)->Set(pos, CParticle::TYPE_BULLET);	// パーティクルの生成
					}
					return true;
				}
			}
			pObject = pObjectNext;		// 次のオブジェクトを代入
		}
	}
	return false;
}