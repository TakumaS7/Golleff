#include "main.h"
#include "renderer.h"
#include "trail.h"
#include "texture.h"
#include "camera.h"

#define TRAIL_LENGTH 30

static XMVECTOR g_Position[TRAIL_LENGTH]; //位置
static XMVECTOR g_CameraPosition = {};

static ID3D11Buffer* g_VertexBuffer = NULL;
static int g_Texture = -1;

void TrailInitialize(void)
{
	// 頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * TRAIL_LENGTH * 2;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);
	}

	// テクスチャ読込
	g_Texture = TextureLoad(L"asset\\texture\\smoke.png");

}

void TrailFinalize(void)
{
	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

void TrailUpdate(void)
{

}

void TrailDraw(void)
{
	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	ID3D11ShaderResourceView* srv = GetTexture(g_Texture);

	GetDeviceContext()->PSSetShaderResources(0, 1, &srv);

	GetDeviceContext()->Map(g_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//ここで頂点データ入力
	for (int i = 0; i < TRAIL_LENGTH - 1; i++)
	{
		XMVECTOR cameraDir, trailDir, cross, pos;

		//視線ベクトル
		cameraDir = g_Position[i] - g_CameraPosition;

		//トレイルの方向ベクトル
		trailDir = g_Position[i] - g_Position[i + 1];

		//外積
		cross = XMVector3Normalize(XMVector3Cross(cameraDir, trailDir));
		//外積の計算↓
		//cross.x = cameraDir.y * trailDir.z - cameraDir.z * trailDir.y;
		//cross.y = cameraDir.z * trailDir.x - cameraDir.x * trailDir.z;
		//cross.z = cameraDir.x * trailDir.y - cameraDir.y * trailDir.x;


		//上頂点
		pos = g_Position[i] + cross * 0.2f;

		XMStoreFloat3(&vertex[i * 2].Position, pos);
		vertex[i * 2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.75f);
		vertex[i * 2].TexCoord = XMFLOAT2(0.5f, 0.0f);
		vertex[i * 2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);


		//下頂点
		pos = g_Position[i] - cross * 0.2f;

		XMStoreFloat3(&vertex[i * 2 + 1].Position, pos);
		vertex[i * 2 + 1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.75f);
		vertex[i * 2 + 1].TexCoord = XMFLOAT2(0.5f, 1.0f);
		vertex[i * 2 + 1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}

	XMStoreFloat3(&vertex[TRAIL_LENGTH * 2 - 2].Position, g_Position[TRAIL_LENGTH - 1]);
	vertex[TRAIL_LENGTH * 2 - 2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.75f);
	vertex[TRAIL_LENGTH * 2 - 2].TexCoord = XMFLOAT2(0.5f, 0.0f);

	XMStoreFloat3(&vertex[TRAIL_LENGTH * 2 - 1].Position, g_Position[TRAIL_LENGTH - 1]);
	vertex[TRAIL_LENGTH * 2 - 1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.75f);
	vertex[TRAIL_LENGTH * 2 - 1].TexCoord = XMFLOAT2(0.5f, 1.0f);


	GetDeviceContext()->Unmap(g_VertexBuffer, 0);



	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ワールド変換行列設定
	SetWorldMatrix(XMMatrixIdentity());

	GetDeviceContext()->Draw(TRAIL_LENGTH * 2, 0);
}


void SetTrailPosition(const XMVECTOR Position)
{
	for (int i = 0; i < TRAIL_LENGTH - 1; i++)
	{
		g_Position[i] = g_Position[i + 1];
	}

	g_Position[TRAIL_LENGTH - 1] = Position;
}

void SetTrailCameraPosition(const XMVECTOR& position)
{
	g_CameraPosition = position;
}

