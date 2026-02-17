/*****************************************************************/
//
// 
// cube.cpp（キューブ処理）
//
//
//
//                    ２０２４年10月16日　製作者名：佐藤琢磨
//
/*****************************************************************/

#include "grid.h"
#include "main.h"
#include "renderer.h"
#include "texture.h"

/*****************************************************************/
// グローバル変数
/*****************************************************************/
static ID3D11Buffer* g_VertexBuffer = NULL;

static constexpr int GRID_NUM_X = 10; //Xの辺の長さ
static constexpr int GRID_NUM_Z = 10; //Zの辺の長さ
static constexpr int GRID_LINE_NUM_X = GRID_NUM_X + 1; //マスに必要な線の数
static constexpr int GRID_LINE_NUM_Z = GRID_NUM_Z + 1; //マスに必要な線の数
static constexpr int GRID_VERTEX_COUNT = (GRID_LINE_NUM_X * 2) + (GRID_LINE_NUM_Z * 2); //必要な頂点数

static int g_TextureId = -1;

void InitGrid(void)
{
	ID3D11Device* pDevice = GetDevice();

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * GRID_VERTEX_COUNT; //バッファサイズ　構造体サイズ * 頂点数分
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	//頂点の作成
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	static constexpr float start_x = GRID_NUM_X * -0.5f;
	static constexpr float    end_x = GRID_NUM_X * 0.5f;
	static constexpr float start_z = GRID_NUM_Z * -0.5f;
	static constexpr float    end_z = GRID_NUM_Z * 0.5f;

	int i = 0;

	for (float x = start_x; x <= end_x; x += 1.0f)
	{
		vertex[i++] = { {x, 0.0f, start_z}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} };
		vertex[i++] = { {x, 0.0f, end_z}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} };
	}

	for (float z = start_z; z <= end_z; z += 1.0f)
	{
		vertex[i++] = { {start_x, 0.0f, z}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} };
		vertex[i++] = { {end_x, 0.0f, z}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} };
	}

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	g_TextureId = TextureLoad(L"asset/SampleTexture/fade.bmp");
}

void UninitGrid(void)
{
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

void DrawGrid(void)
{
	ID3D11ShaderResourceView* srv = GetTexture(g_TextureId);
	GetDeviceContext()->PSSetShaderResources(0, 1, &srv);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	SetWorldMatrix(XMMatrixIdentity());

	// ポリゴン描画
	GetDeviceContext()->Draw(GRID_VERTEX_COUNT, 0); //12はポリゴンの表示に使用する頂点数
}
