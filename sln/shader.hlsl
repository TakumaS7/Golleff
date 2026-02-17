

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

cbuffer UVMatrixBuffer : register(b2)
{
	matrix UVMatrix;
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
	
    outTexCoord = mul(float4(inTexCoord, 0.0f, 1.0f), UVMatrix).xy;
    //outTexCoord = inTexCoord * float2(0.2f, 0.5f) + float2(0.2f, 0.0f); //画像のUVを分割する

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
    //float light = dot(normalize(inNormal.xyz), -localLightVector.xyz); //saturate → ０～１まででちょん切る
    float light = (dot(normalize(inNormal.xyz), -localLightVector.xyz) + 1.0f) * 0.5; //saturate → ０～１まででちょん切る
	
    //float light = (dot(normalize(inNormal.xyz), -localLightVector.xyz) + 1.0f) * 0.5;
	
    float3 diffuse = (lightColor.rgb * light + ambientColor.rgb) * inDiffuse.rgb;
	
    //float4 doffuse = float4(inDiffuse.rgb * light, inDiffuse.a);
	
	//float4 diffuse = float4(inDiffuse.rgb *)
	
    outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord) * float4(diffuse, inDiffuse.a);
	//outDiffuse.rgb = 1.0 - outDiffuse.rgb; //色の反転
}
