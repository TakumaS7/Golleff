#pragma once
/*****************************************************************/
//
// 
// fase.h（フェード処理）
//
//
//
//                    ２０２４年０７月１８日　製作者名：佐藤琢磨
//
/*****************************************************************/

#include "main.h"
#include "sprite.h"
#include "Manager.h"

enum FADE_STATE
{
	FADE_NONE = 0,	//何もしない
	FADE_IN,		//フェードイン実行
	FADE_OUT,		//フェードアウト実行

	FADE_MAX
};

//*********************************************************
// 構造体
//*********************************************************
struct FadeObject
{
	FADE_STATE state;
	float count;
	int frame;
	XMFLOAT4 fadecolor;
	SCENE scene;
};

void InitFade();
void UpdateFade();
void UninitFade();
void DrawFade();

void SetFade(int fadeframe, XMFLOAT4 color, FADE_STATE state, SCENE scene);

FADE_STATE GetFadeState();


