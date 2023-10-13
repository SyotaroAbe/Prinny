//=========================================================
//
// メイン描画 [main.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _MAIN_H_     // このマクロ定義がされてなかったら
#define _MAIN_H_     // 2重インクルード防止のマクロ定義する

//===============================================
// マクロ定義
//===============================================
#define SCREEN_WIDTH			(1280)                            // ウインドウの幅
#define SCREEN_HEIGHT			(720)                             // ウインドウの高さ
#define FVF_VERTEX_2D			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D			(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

//===============================================
// 頂点情報[2D]の構造体を定義
//===============================================
typedef struct
{
	D3DXVECTOR3 pos;   // 頂点座標
	float rhw;         // 座標変換用関数（1.0fで固定）
	D3DCOLOR col;      // 頂点カラー
	D3DXVECTOR2 tex;   // テクスチャ座標
}VERTEX_2D;

//===============================================
// 頂点情報[3D]の構造体を定義
//===============================================
typedef struct
{
	D3DXVECTOR3 pos;   // 頂点座標
	D3DXVECTOR3 nor;   // 法線ベクトル
	D3DCOLOR col;      // 頂点カラー
	D3DXVECTOR2 tex;   // テクスチャ座標
}VERTEX_3D;


#endif