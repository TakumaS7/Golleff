/*****************************************************************/
//
// 
// polygon.h（ポリゴン表示）
//
//
//
//                    ２０２４年０６月１６日　製作者名：佐藤琢磨
//
/*****************************************************************/
#pragma once 
#include "main.h" 
#include "renderer.h" 
//***************************************************************************** 
// マクロ定義 
//***************************************************************************** 
 
 

//***************************************************************************** 
// プロトタイプ宣言 
//***************************************************************************** 

//構造体
struct VERTEX
{
	float posx = 0.0f;
	float posy = 0.0f;

	float speed = 0.0f;
	float speed2 = 0.0f;

	int s = 0;
	int s2 = 0;


	float posx2 = 0.0f;
	float posy2 = 0.0f;

	int s3 = 0;
	int s4 = 0;


	float posx3 = 0.0f;
	float posy3 = 0.0f;

	int s5 = 0;
	int s6 = 0;


	float posx4 = 0.0f;
	float posy4 = 0.0f;

	float posx5 = 0.0f;

	float bigsize = 0.0f;
	float bigspeed = 0.0f;
};

HRESULT InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void DrawPolygon(void);



class Player
{
public:
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT2 size;
	XMFLOAT2 scale;
	float rotate;
	ID3D11ShaderResourceView* texID;
};