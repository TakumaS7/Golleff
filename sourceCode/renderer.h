/*****************************************************************/
//
// 
// renderer.h（レンダリング管理）
//
//
//
//                    ２０２４年０６月１６日　製作者名：佐藤琢磨
//
/*****************************************************************/
#pragma once

#include "main.h"

//ブレンドステート定義
enum BLENDSTATE
{
	BLENDSTATE_NONE = 0,	//ブレンドしない
	BLENDSTATE_ALFA,		//普通のαブレンド
	BLENDSTATE_ADD,			//加算合成
	BLENDSTATE_SUB,			//減算合成

	BLENDSTATE_MAX
};

//*********************************************************
// 構造体
//*********************************************************

// 頂点構造体
struct VERTEX_3D
{
	XMFLOAT3 Position; //頂点座標
	XMFLOAT3 Normal;   //法線ベクトル
	XMFLOAT4 Diffuse;  //色　XMFLOAT4 = float x, y, z, w
	XMFLOAT2 TexCoord; //テクスチャ座標 XMFLOAT2 = float x, y
};


// マテリアル構造体
struct MATERIAL
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	float		Dummy[3];//16byte境界用（パディング）
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void UninitRenderer(void);

void Clear(void);
void Present(void);

ID3D11Device *GetDevice( void );
ID3D11DeviceContext *GetDeviceContext( void );

void SetDepthEnable( bool Enable );

void SetPixelShader2d();
void SetPixelShader3d();

void SetWorldViewProjection2D(void);
void SetWorldViewProjection3D(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& perspective);
void SetWorldViewProjection3D(const XMMATRIX& world_view_projection);

void SetWorldMatrix(XMMATRIX WorldMatrix );
void SetViewMatrix(XMMATRIX ViewMatrix );
void SetProjectionMatrix(XMMATRIX ProjectionMatrix );


void SetMaterial( MATERIAL Material );

void SetViewport(FLOAT left, FLOAT top, FLOAT w, FLOAT h);

void SetUVMatrix(const XMMATRIX matrix);

void SetBlendState(BLENDSTATE blend);
