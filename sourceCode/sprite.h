/*****************************************************************/
//
// 
// sprite.h（スプライト表示）
//
//
//
//                    ２０２４年０６月１６日　製作者名：佐藤琢磨
//
/*****************************************************************/

#pragma once

#include "main.h"
#include "renderer.h"

/*****************************************************************/
// プロトタイプ宣言
/*****************************************************************/
HRESULT	InitSprite();
void    UninitSprite();
void    DrawSprite(XMFLOAT3 position, XMFLOAT2 size, XMFLOAT4 color);
void	DrawSpriteRotate(XMFLOAT3 position, XMFLOAT2 size, XMFLOAT4 color, FLOAT radian);

void	DrawSpriteRotateUV(XMFLOAT3 position, XMFLOAT2 size, XMFLOAT4 color, FLOAT radian, int PtNo, int XPtnCnt, int YPtnCnt);
void	DrawSpriteRotateUV_Reverse(XMFLOAT3 position, XMFLOAT2 size, XMFLOAT4 color, FLOAT radian, int PtNo, int XPtnCnt, int YPtnCnt);

//スプライト表示　行列使用版
void    DrawSpriteLine(XMFLOAT2 size, XMFLOAT4 color);

//スクロール対応版
void    DrawSpriteScroll(XMFLOAT3 position, XMFLOAT2 size, XMFLOAT4 color);

void DrawSpriteRotateUVScroll(XMFLOAT3 position, XMFLOAT2 size, XMFLOAT4 color, FLOAT radian, int PtNo, int XPtnCnt, int YPtnCnt);
