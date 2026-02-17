

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer ConstantBuffer : register( b0 )
{
	matrix WorldViewProjection;
}

// マテリアルバッファ
cbuffer MaterialBuffer : register( b1 )
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	float3		Dummy;//16bit境界用
}

cbuffer colBuffer : register(b2)
{
	float4 col;
}

cbuffer lightBuffer : register(b3)
{
    float4 localLightVector;	//ライトの方向
    float4 lightColor;			//ライトの色
    float4 ambientColor;		//環境光
}

//=============================================================================
// 頂点シェーダ
//=============================================================================
void VertexShaderPolygon( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outNormal		: NORMAL0,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float4 outDiffuse		: COLOR0 )
{
	outPosition = mul( inPosition, WorldViewProjection );

	outNormal = inNormal;
	outTexCoord = inTexCoord;

    outDiffuse = inDiffuse * Diffuse;

	//outDiffuse = col; //頂点の色を強制的に変更 <<<<<これは削除しておく
}



//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture : register( t0 );
SamplerState	g_SamplerState : register( s0 );


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void PixelShaderPolygon( in  float4 inPosition		: POSITION0,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,

						 out float4 outDiffuse		: SV_Target )
{		
	outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord) * inDiffuse;

	//αテスト
    //clip(outDiffuse.a - 0.01f);
	
    if (outDiffuse.a <= 0.0f)
    {
        discard; //ピクセルシェーダーはポリゴンを書かない
    }
}
