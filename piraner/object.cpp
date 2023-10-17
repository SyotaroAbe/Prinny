//=========================================================
//
// オブジェクト処理 [object.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "object.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"
#include "game.h"

//=============================
// 静的メンバ変数 
//=============================
CObject *CObject::m_apTop[PRIORITY_MAX] = {};	// 先頭のオブジェクトへのポインタ
CObject *CObject::m_apCur[PRIORITY_MAX] = {};	// 最後尾のオブジェクトへのポインタ
int CObject::m_nNumAll = 0;						// オブジェクトの総数

//===============================================
// コンストラクタ（デフォルト）
//===============================================
CObject::CObject()
{
	m_nPriority = 3;	// 描画順を設定

	// 値をクリア
	m_pPrev = NULL;
	m_pNext = NULL;
	m_type = TYPE_NONE;
	m_pShadow = NULL;
	m_bDeath = false;

	if (m_apTop[m_nPriority] == NULL)
	{// 先頭のオブジェクトが存在しない
		m_apTop[m_nPriority] = this;		// 先頭のオブジェクトに自分を代入
	}

	if (m_apCur[m_nPriority] != NULL)
	{// 最後尾のオブジェクトが保存されている
		m_pPrev = m_apCur[m_nPriority];			// 前回の最後尾のオブジェクトを前のオブジェクトのポインタに保存
		m_pPrev->m_pNext = this;				// 前回の最後尾の次のオブジェクトに自身を保存
	}
	m_apCur[m_nPriority] = this;				// 最後尾のオブジェクトに自分を代入

	m_nNumAll++;		// 総数をカウントアップ
}

//===============================================
// コンストラクタ（オーバーロード）
//===============================================
CObject::CObject(int nPriority)
{
	m_nPriority = nPriority;	// 描画順を設定

	// 値をクリア
	m_pPrev = NULL;
	m_pNext = NULL;
	m_type = TYPE_NONE;
	m_pShadow = NULL;
	m_bDeath = false;

	if (m_apTop[m_nPriority] == NULL)
	{// 先頭のオブジェクトが存在しない
		m_apTop[m_nPriority] = this;		// 先頭のオブジェクトに自分を代入
	}

	if (m_apCur[m_nPriority] != NULL)
	{// 最後尾のオブジェクトが保存されている
		m_pPrev = m_apCur[m_nPriority];			// 前回の最後尾のオブジェクトを前のオブジェクトのポインタに保存
		m_pPrev->m_pNext = this;				// 前回の最後尾の次のオブジェクトに自身を保存
	}
	m_apCur[m_nPriority] = this;				// 最後尾のオブジェクトに自分を代入

	m_nNumAll++;		// 総数をカウントアップ
}

//===============================================
// デストラクタ
//===============================================
CObject::~CObject()
{

}

//===============================================
// 全てのオブジェクトの破棄
//===============================================
void CObject::ReleaseAll()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_apTop[nCntPriority];	// 先頭のオブジェクトを代入

		while (pObject != NULL)
		{// ポインタが使用されている
			CObject *pObjectNext = pObject->m_pNext;	// 次のオブジェクトを保存

			// 終了処理
			pObject->Uninit();

			pObject = pObjectNext;	// 次のオブジェクトを代入
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_apTop[nCntPriority];	// 先頭のオブジェクトを代入

		while (pObject != NULL)
		{// ポインタが使用されている
			CObject *pObjectNext = pObject->m_pNext;	// 次のオブジェクトを保存

			if (pObject->m_bDeath == true)
			{// 死亡フラグが立っている
				// リストから削除して破棄する
				if (m_apTop[nCntPriority] != pObject && m_apCur[nCntPriority] != pObject)
				{// 自身が先頭でも最後尾でもない
					pObject->m_pPrev->m_pNext = pObject->m_pNext;		// 前のオブジェクトの次のオブジェクトのポインタに次のオブジェクトを保存する
					pObject->m_pNext->m_pPrev = pObject->m_pPrev;		// 次のオブジェクトの前のオブジェクトのポインタに前のオブジェクトを保存する
				}
				else if (m_apTop[nCntPriority] != pObject && m_apCur[nCntPriority] == pObject)
				{// 自身が先頭ではないが最後尾である
					m_apCur[nCntPriority] = pObject->m_pPrev;		// 最後尾を前のオブジェクトに設定する
				}
				else if (m_apTop[nCntPriority] == pObject && m_apCur[nCntPriority] != pObject)
				{// 自身が先頭だが最後尾ではない
					m_apTop[nCntPriority] = pObject->m_pNext;		// 先頭を次のオブジェクトに設定する
				}

				if (pObject == m_apTop[nCntPriority])
				{// 先頭と一致
					m_apTop[nCntPriority] = NULL;
				}
				if (pObject == m_apCur[nCntPriority])
				{// 最後尾と一致
					m_apCur[nCntPriority] = NULL;
				}

				// メモリを開放
				delete pObject;
				pObject = NULL;
			}

			pObject = pObjectNext;	// 次のオブジェクトを代入
		}
	}
}

//===============================================
// リセット処理
//===============================================
void CObject::Reset()
{
	//for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	//{
	//	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	//	{
	//		if (m_apObject[nCntPriority][nCntObject] != NULL)
	//		{// 使用されている
	//			CObject *pObj;									// オブジェクトクラスのポインタ
	//			pObj = GetObject(nCntPriority, nCntObject);		// オブジェクトを取得

	//			TYPE type;						// 種類
	//			type = pObj->GetType();			// タイプを取得

	//			if (type == TYPE_ENEMY || type == TYPE_ITEM)
	//			{// 敵とアイテム
	//				// 終了処理
	//				m_apObject[nCntPriority][nCntObject]->Uninit();
	//			}
	//		}
	//	}
	//}
}

//===============================================
// 全てのオブジェクトの更新処理
//===============================================
void CObject::UpdateAll()
{
	if ((CScene::GetMode() == CScene::MODE_GAME && CGame::GetStateReady() == false) || CScene::GetMode() != CScene::MODE_GAME)
	{// 待機状態
		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			CObject *pObject = m_apTop[nCntPriority];	// 先頭のオブジェクトを代入

			while (pObject != NULL)
			{// ポインタが使用されている
				CObject *pObjectNext = pObject->m_pNext;	// 次のオブジェクトを保存

				// 更新処理
				pObject->Update();

				pObject = pObjectNext;	// 次のオブジェクトを代入
			}
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_apTop[nCntPriority];	// 先頭のオブジェクトを代入

		while (pObject != NULL)
		{// ポインタが使用されている
			CObject *pObjectNext = pObject->m_pNext;	// 次のオブジェクトを保存

			if (pObject->m_bDeath == true)
			{// 死亡フラグが立っている
				// リストから削除して破棄する
				if (m_apTop[nCntPriority] != pObject && m_apCur[nCntPriority] == pObject)
				{// 自身が先頭ではないが最後尾である
					m_apCur[nCntPriority] = pObject->m_pPrev;		// 最後尾を前のオブジェクトに設定する
					pObject->m_pPrev->m_pNext = NULL;				// 前のオブジェクトの次のオブジェクトを削除
				}
				else if (m_apTop[nCntPriority] == pObject && m_apCur[nCntPriority] != pObject)
				{// 自身が先頭だが最後尾ではない
					m_apTop[nCntPriority] = pObject->m_pNext;		// 先頭を次のオブジェクトに設定する
					pObject->m_pNext->m_pPrev = NULL;				// 次のオブジェクトの前のオブジェクトを削除
				}
				else if (m_apTop[nCntPriority] != pObject && m_apCur[nCntPriority] != pObject)
				{// 自身が先頭でも最後尾でもない
					pObject->m_pPrev->m_pNext = pObject->m_pNext;		// 前のオブジェクトの次のオブジェクトのポインタに次のオブジェクトを保存する
					pObject->m_pNext->m_pPrev = pObject->m_pPrev;		// 次のオブジェクトの前のオブジェクトのポインタに前のオブジェクトを保存する
				}

				if (pObject == m_apTop[nCntPriority])
				{// 先頭と一致
					m_apTop[nCntPriority] = NULL;
				}
				if(pObject == m_apCur[nCntPriority])
				{// 最後尾と一致
					m_apCur[nCntPriority] = NULL;
				}

				// メモリを開放
				delete pObject;
				pObject = NULL;
			}

			pObject = pObjectNext;	// 次のオブジェクトを代入
		}
	}
}

//===============================================
// 全てのオブジェクトの描画処理
//===============================================
void CObject::DrawAll()
{
	CCamera *m_pCamera = CManager::GetInstance()->GetCamera();	// カメラへのポインタを取得

	if (m_pCamera != NULL)
	{// カメラが使用されている
		// カメラの設定処理
		m_pCamera->Set();
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pObject = m_apTop[nCntPriority];	// 先頭のオブジェクトを代入

		while (pObject != NULL)
		{// ポインタが使用されている
			CObject *pObjectNext = pObject->m_pNext;	// 次のオブジェクトを保存

			if (pObject->m_type != TYPE_PAUSE || CGame::GetPauseState() == true && CGame::GetPauseCamera() == false)
			{// オブジェクトの種類がポーズじゃない もしくはポーズ状態のとき
				// 描画処理
				pObject->Draw();
			}

			pObject = pObjectNext;	// 次のオブジェクトを代入
		}
	}
}

//===============================================
// オブジェクトの破棄
//===============================================
void CObject::Release()
{
	m_bDeath = true;	// すぐに破棄せずに死亡フラグを立てる

	m_nNumAll--;	//総数をカウントダウン
}

//===============================================
// オブジェクトの種類の設定
//===============================================
void CObject::SetType(const TYPE type)
{
	m_type = type;
}