////=========================================================
////
//// メッシュウォール描画処理 [meshwall.cpp]
//// Author = 阿部翔大郎
////
////=========================================================
//#include "main.h"
//#include "meshwall.h"
//#include "bullet.h"
//#include "camera.h"
//
////===============================================
//// コンストラクタ
////===============================================
//CMeshwall::CMeshwall() : CObjectMesh(1)
//{
//	//値をクリアする
//	
//}
//
////===============================================
//// コンストラクタ（オーバーロード）
////===============================================
//CMeshwall::CMeshwall(int nPriority) : CObjectMesh(nPriority)
//{
//	// 値をクリアする
//	
//}
//
////===============================================
//// デストラクタ
////===============================================
//CMeshwall::~CMeshwall()
//{
//
//}
//
////===============================================
//// 初期化処理
////===============================================
//HRESULT CMeshwall::Init(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();   // デバイスの取得
//
//	// テクスチャの読み込み
//	D3DXCreateTextureFromFile(pDevice,
//		"data\\TEXTURE\\Audience001.png",
//		&g_pTexture);
//
//	// 変数の初期化
//	g_aMeshWall[nCntMeshWall].pos = D3DXVECTOR3(0.0f, 0.0f, 200.0f);
//	g_aMeshWall[nCntMeshWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//
//	// 頂点バッファの生成
//	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 9 * MAX_MESHWALL,
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_3D,
//		D3DPOOL_MANAGED,
//		&g_pVtxBuff,
//		NULL);
//
//	VERTEX_3D * pVtx;   // 頂点情報へのポインタ
//
//	// 頂点バッファをロックし頂点情報へのポインタを取得
//	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	// 頂点座標の設定
//	pVtx[0].pos = D3DXVECTOR3(-1800.0f, 800.0f, 0.0f);
//	pVtx[1].pos = D3DXVECTOR3(0.0f, 800.0f, 0.0f);
//	pVtx[2].pos = D3DXVECTOR3(1800.0f, 800.0f, 0.0f);
//	pVtx[3].pos = D3DXVECTOR3(-1800.0f, 400.0f, 0.0f);
//	pVtx[4].pos = D3DXVECTOR3(0.0f, 400.0f, 0.0f);
//	pVtx[5].pos = D3DXVECTOR3(1800.0f, 400.0f, 0.0f);
//	pVtx[6].pos = D3DXVECTOR3(-1800.0f, 0.0f, 0.0f);
//	pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	pVtx[8].pos = D3DXVECTOR3(1800.0f, 0.0f, 0.0f);
//
//	// 法線ベクトルの設定
//	for (nCntMesh = 0; nCntMesh < 9; nCntMesh++)
//	{
//		pVtx[nCntMesh].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//	}
//
//	// 頂点カラーの設定
//	for (nCntMesh = 0; nCntMesh < 9; nCntMesh++)
//	{
//		pVtx[nCntMesh].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//	}
//
//	// テクスチャ座標の設定
//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
//	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
//	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
//	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
//	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
//	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
//	pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
//	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//	// 頂点バッファをアンロックする
//	g_pVtxBuff->Unlock();
//
//	// インデックスバッファの生成
//	pDevice->CreateIndexBuffer(sizeof(WORD) * 14 * MAX_MESHWALL,
//		D3DUSAGE_WRITEONLY,
//		D3DFMT_INDEX16,
//		D3DPOOL_MANAGED,
//		&g_pldxBuff,
//		NULL);
//
//	WORD * pldx;	// インデックス情報へのポインタ
//
//	// インデックスバッファをロックし、頂点番号データへのポインタを取得
//	g_pldxBuff->Lock(0, 0, (void**)&pldx, 0);
//
//	// 頂点番号データの設定
//	pldx[0] = 3;
//	pldx[1] = 0;
//	pldx[2] = 4;
//	pldx[3] = 1;
//	pldx[4] = 5;
//	pldx[5] = 2;
//	pldx[6] = 2;
//	pldx[7] = 6;
//	pldx[8] = 6;
//	pldx[9] = 3;
//	pldx[10] = 7;
//	pldx[11] = 4;
//	pldx[12] = 8;
//	pldx[13] = 5;
//
//	// インデックスバッファをアンロックする
//	g_pldxBuff->Unlock();
//
//	Set(D3DXVECTOR3(-1800.0f, 0.0f, -2600.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), MESHWALLTYPE_NORMAL);
//	Set(D3DXVECTOR3(-1800.0f, 0.0f, 1000.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), MESHWALLTYPE_NORMAL);
//	Set(D3DXVECTOR3(-1800.0f, 0.0f, 4600.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), MESHWALLTYPE_NORMAL);
//	Set(D3DXVECTOR3(-1800.0f, 0.0f, 8200.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), MESHWALLTYPE_NORMAL);
//	Set(D3DXVECTOR3(1300.0f, 0.0f, -2600.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), MESHWALLTYPE_NORMAL);
//	Set(D3DXVECTOR3(1300.0f, 0.0f, 1000.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), MESHWALLTYPE_NORMAL);
//	Set(D3DXVECTOR3(1300.0f, 0.0f, 4600.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), MESHWALLTYPE_NORMAL);
//	Set(D3DXVECTOR3(1300.0f, 0.0f, 8200.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), MESHWALLTYPE_NORMAL);
//}
//
////===============================================
//// 終了処理
////===============================================
//void CMeshwall::Uninit(void)
//{
//	// テクスチャの破棄
//	if (g_pTexture != NULL)
//	{
//		g_pTexture->Release();
//		g_pTexture = NULL;
//	}
//
//	// 頂点バッファの破棄
//	if (g_pVtxBuff != NULL)
//	{
//		g_pVtxBuff->Release();
//		g_pVtxBuff = NULL;
//	}
//
//	// インデックスバッファの破棄
//	if (g_pldxBuff != NULL)
//	{
//		g_pldxBuff->Release();
//		g_pldxBuff = NULL;
//	}
//}
//
////===============================================
//// 更新処理
////===============================================
//void CMeshwall::Update(void)
//{
//
//}
//
////===============================================
//// 描画処理
////===============================================
//void CMeshwall::Draw(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
//	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
//
//	// ワールドマトリックスの初期化
//	D3DXMatrixIdentity(&g_aMeshWall[nCntMeshWall].mtxWorld);
//
//	// 向きを反映
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshWall[nCntMeshWall].rot.y, g_aMeshWall[nCntMeshWall].rot.x, g_aMeshWall[nCntMeshWall].rot.z);
//	D3DXMatrixMultiply(&g_aMeshWall[nCntMeshWall].mtxWorld, &g_aMeshWall[nCntMeshWall].mtxWorld, &mtxRot);
//
//	// 位置を反映
//	D3DXMatrixTranslation(&mtxTrans, g_aMeshWall[nCntMeshWall].pos.x, g_aMeshWall[nCntMeshWall].pos.y, g_aMeshWall[nCntMeshWall].pos.z);
//	D3DXMatrixMultiply(&g_aMeshWall[nCntMeshWall].mtxWorld, &g_aMeshWall[nCntMeshWall].mtxWorld, &mtxTrans);
//
//	// ワールドマトリックスの設定
//	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshWall[nCntMeshWall].mtxWorld);
//
//	// 頂点バッファをデータストリームに設定
//	pDevice->SetStreamSource(0, g_pVtxBuff, 0, sizeof(VERTEX_3D));
//	pDevice->SetIndices(g_pldxBuff);
//
//	// 頂点フォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// テクスチャの設定
//	pDevice->SetTexture(0, g_pTexture);
//
//	// ポリゴンの描画
//	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
//		0,
//		0,
//		9,
//		0,
//		12);
//}
//
////===============================================
//// 設定処理
////===============================================
//void CMeshwall::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int type)
//{
//	VERTEX_3D * pVtx;   // 頂点情報へのポインタ
//
//	// 頂点バッファをロックし頂点情報へのポインタを取得
//	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	g_aMeshWall[nCntMeshWall].pos = pos;
//	g_aMeshWall[nCntMeshWall].rot = rot;
//	g_aMeshWall[nCntMeshWall].nType = type;
//
//	// GetWall(g_aMeshWall[0].pos + pVtx[0].pos, g_aMeshWall[0].pos + pVtx[8].pos, 0);
//
//	// GetWall(D3DXVECTOR3(g_aMeshWall[nCntMeshWall].pos.x + sinf(g_aMeshWall[nCntMeshWall].rot.y + (ROT_CAMERA * pCamera->rot.y)) * pVtx[0].pos.x,
//	// 					g_aMeshWall[nCntMeshWall].pos.y,
//	// 					g_aMeshWall[nCntMeshWall].pos.z + sinf(g_aMeshWall[nCntMeshWall].rot.y + (ROT_CAMERA * pCamera->rot.y)) * pVtx[0].pos.z),
//	// 		D3DXVECTOR3(g_aMeshWall[nCntMeshWall].pos.x + cosf(g_aMeshWall[nCntMeshWall].rot.y + (ROT_CAMERA * pCamera->rot.y)) * pVtx[8].pos.x,
//	// 					g_aMeshWall[nCntMeshWall].pos.y,
//	// 					g_aMeshWall[nCntMeshWall].pos.z + cosf(g_aMeshWall[nCntMeshWall].rot.y + (ROT_CAMERA * pCamera->rot.y)) * pVtx[8].pos.z),
//	// 					nCntMeshWall);
//
//	if (g_aMeshWall[nCntMeshWall].nType == MESHWALLTYPE_NORMAL)
//	{
//		// 頂点カラーの設定
//		for (nCntMesh = 0; nCntMesh < 9; nCntMesh++)
//		{
//			pVtx[nCntMesh].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		}
//
//		// テクスチャ座標の設定
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
//		pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
//		pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
//		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
//		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
//		pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//		break;
//	}
//	else if (g_aMeshWall[nCntMeshWall].nType == MESHWALLTYPE_INVISIBLE)
//	{
//		// 頂点カラーの設定
//		for (nCntMesh = 0; nCntMesh < 9; nCntMesh++)
//		{
//			pVtx[nCntMesh].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
//		}
//
//		// テクスチャ座標の設定
//		pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);
//		pVtx[1].tex = D3DXVECTOR2(0.5f, 1.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.5f);
//		pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
//		pVtx[5].tex = D3DXVECTOR2(0.0f, 0.5f);
//		pVtx[6].tex = D3DXVECTOR2(1.0f, 0.0f);
//		pVtx[7].tex = D3DXVECTOR2(0.5f, 0.0f);
//		pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
//
//		break;
//	}
//
//	// 頂点バッファをアンロックする
//	g_pVtxBuff->Unlock();
//}