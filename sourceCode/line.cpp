/*****************************************************************/
//
// 
// line.cpp（ライン表示）
//
//
//
//                    ２０２４年09月26日　製作者名：佐藤琢磨
//
/*****************************************************************/

#include "line.h"

/*****************************************************************/
// マクロ定義
/*****************************************************************/

#define NUM_LINEVERTEX (2)

/*****************************************************************/
// プロトタイプ宣言
/*****************************************************************/
void SetVertex();


/*****************************************************************/
// グローバル変数
/*****************************************************************/

static ID3D11Buffer* g_VertexBuffer = NULL;

/*****************************************************************/
// 初期化
/*****************************************************************/

HRESULT InitLine()
{
	ID3D11Device* pDevice = GetDevice();

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_LINEVERTEX; //バッファサイズ　構造体サイズ * 頂点数分
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	return S_OK;
}

/*****************************************************************/
// 終了処理
/*****************************************************************/

void    UninitLine()
{
	// 頂点バッファの解放
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();

		g_VertexBuffer = NULL;
	}
}

/*****************************************************************/
// 描画処理
/*****************************************************************/

void    DrawLine(XMFLOAT3 sposition, XMFLOAT3 eposition, XMFLOAT4 color)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//左上
	vertex[0].Position = sposition;
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	//右上
	vertex[1].Position = eposition;
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST); //ライン描画設定

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// ポリゴン描画
	GetDeviceContext()->Draw(2, 0); //2はポリゴンの表示に使用する頂点数
}


