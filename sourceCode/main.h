/*****************************************************************/
//
// 
// main.h（メイン処理ヘッダー）
//
//
//
//                    ２０２４年０６月１６日　製作者名：佐藤琢磨
//
/*****************************************************************/
#pragma once


#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止
#include <stdio.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include "dinput.h"
#include "mmsystem.h"

#pragma warning(pop)



#include <DirectXMath.h>
using namespace DirectX;

//テクスチャサポートライブラリ
#include "DirectXTex.h"

//デバックビルドかリリースビルドでリンクするライブラリを選択する
#if _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib") //デバッグ用
#else
#pragma comment(lib, "DirectXTex_Release.lib") //リリース用
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	(1280)				// ウインドウの幅
#define SCREEN_HEIGHT	(720)				// ウインドウの高さ

