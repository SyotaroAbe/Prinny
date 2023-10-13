//=========================================================
//
// プリコンパイル処理 [precompile.h]
// Author = 阿部翔大郎
//
//=========================================================
#ifndef _PRECOMPILE_H_     // このマクロ定義がされてなかったら
#define _PRECOMPILE_H_     // 2重インクルード防止のマクロ定義する

#include <windows.h>
#include "d3dx9.h"							// 描画処理に必要
#define DIRECTINPUT_VERSION		(0x0800)	// ビルド時の警告対処用マクロ
#include "dinput.h"							// 入力処理に必要
#include "xaudio2.h"						// サウンド処理に必要
#include <stdio.h>

#define _CRTDBG_MAP_ALLOC	// メモリリーク出力用マクロ
#include <crtdbg.h>			// メモリリーク検出に必要

#endif