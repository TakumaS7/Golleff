#include "billboard.h"
#include "main.h"
#include "renderer.h"
#include "texture.h"

/*****************************************************************/
// グローバル変数
/*****************************************************************/
static ID3D11Buffer* g_VertexBuffer = NULL; //頂点情報
//static ID3D11Buffer* g_IndexBuffer = NULL; //インデクス情報
static XMMATRIX g_BillboardMatrix;


void InitBillboard(void)
{
	ID3D11Device* pDevice = GetDevice();

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4; //バッファサイズ　構造体サイズ * 頂点数分
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	VERTEX_3D v[] = {
		//前
		{ {-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
		{ { 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
		{ {-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },
		{ { 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
	};

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = v;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	GetDevice()->CreateBuffer(&bd, &data, &g_VertexBuffer);

	//unsigned short indices[] = {
	//	 0,  1,  2,  1,  3,  2,
	//};

	//// インデックスバッファ生成
	//bd.ByteWidth = sizeof(unsigned short) * 6; //バッファサイズ　構造体サイズ * 頂点数分
	//bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	//data.pSysMem = indices;
	//data.SysMemPitch = 0;
	//data.SysMemSlicePitch = 0;

	//GetDevice()->CreateBuffer(&bd, &data, &g_IndexBuffer);
}

void UninitBillboard(void)
{
	//if (g_IndexBuffer)
	//{
	//	g_IndexBuffer->Release();
	//	g_IndexBuffer = NULL;
	//}

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

void DrawBillboard(int textureId, const XMFLOAT2& offset, const XMVECTOR& position, const XMFLOAT2& scale, float angle, float tx, float ty, float tw, float th)
{
	SetTexture(textureId);
	SetPixelShader2d();

	XMMATRIX mtx_offset = XMMatrixTranslation(offset.x, offset.y, 0.0f); //根元を中心に回転させる
	XMMATRIX mtx_scale = XMMatrixScaling(scale.x, scale.y, 1.0f); //サイズ
	XMMATRIX mtx_translate = XMMatrixTranslationFromVector(position); //座標移動
	XMMATRIX mtx_rotation = XMMatrixRotationZ(angle); //回転

	SetWorldMatrix(mtx_rotation * mtx_offset * mtx_scale * g_BillboardMatrix * mtx_translate);

	float sx = tw / TextureGetWidth(textureId);
	float sy = th / TextureGetHeight(textureId);
	float u = tx / TextureGetWidth(textureId);
	float v = ty / TextureGetHeight(textureId);

	XMMATRIX uvs = XMMatrixScalingFromVector({ sx, sy, 1.0f });
	XMMATRIX uvt = XMMatrixTranslation(u, v, 0.0f);
	XMMATRIX uvr = XMMatrixRotationZ(angle);

	SetUVMatrix(uvs * uvt);

	//SetUVMatrix(XMMatrixIdentity());

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT voffset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &voffset);

	//インデックスバッファ設定
	//GetDeviceContext()->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); //unsigned intだとR32

	//プリミティブトポロジ設定
	//GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// ポリゴン描画
	//GetDeviceContext()->DrawIndexed(6, 0, 0); //左の6はポリゴンの表示に使用する頂点数
	GetDeviceContext()->Draw(4, 0); //左の36はポリゴンの表示に使用する頂点数
}

void CalculateBillboardMatrix(const XMMATRIX& mtxView)
{
	XMMATRIX mtx_view_inverse = XMMatrixTranspose(mtxView);
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, mtx_view_inverse);
	matrix._14 = matrix._24 = matrix._34 = 0.0f; //平行移動成分をカット

	g_BillboardMatrix = XMLoadFloat4x4(&matrix);
}
