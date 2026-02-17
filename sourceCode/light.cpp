/*****************************************************************/
//
// 
// light.cpp（ライト処理）
//
//
//
//                    ２０２４年10月16日　製作者名：佐藤琢磨
//
/*****************************************************************/
#include "light.h"
#include "renderer.h"

//ライト構造体
struct Light
{
	XMFLOAT4 localLightVector;
	XMFLOAT4 lightColor;
	XMFLOAT4 ambientColor;
};

static XMVECTOR g_WorldLightVector;
static 	Light g_light;
static ID3D11Buffer* g_LightBuffer = NULL;

void InitLight(void)
{
	D3D11_BUFFER_DESC hBufferDesc = {};

	hBufferDesc.ByteWidth = sizeof(Light);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	GetDevice()->CreateBuffer(&hBufferDesc, NULL, &g_LightBuffer);
	GetDeviceContext()->PSSetConstantBuffers(3, 1, &g_LightBuffer);
}

void UninitLight(void)
{
	if (g_LightBuffer)
	{
		g_LightBuffer->Release();
	}
}

void SetDirectionalLight(const XMVECTOR& dir, const XMFLOAT4& color)
{
	//ライト構造体に情報を入れ、その情報をシェーダーに送る
	g_WorldLightVector = XMVector3Normalize(dir);
	g_light.lightColor = color;
}

void SetAmbient(const XMFLOAT4& color)
{
	g_light.ambientColor = color;
}

void SetLight(const XMMATRIX& world)
{
	XMVECTOR localLightVector = XMVector3TransformNormal(g_WorldLightVector, XMMatrixInverse(nullptr, world));
	XMStoreFloat4(&g_light.localLightVector, XMVector3Normalize(localLightVector));
	GetDeviceContext()->UpdateSubresource(g_LightBuffer, 0, NULL, &g_light, 0, 0);
}
