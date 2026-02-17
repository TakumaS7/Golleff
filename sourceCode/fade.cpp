/*****************************************************************/
//
// 
// fade.cpp（フェード処理）
//
//
//
//                    ２０２４年09月26日　製作者名：佐藤琢磨
//
/*****************************************************************/

#include "fade.h"
#include "Manager.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
FadeObject g_Fade;
static ID3D11ShaderResourceView* g_Texture;

//初期化処理
void InitFade()
{
	//テクスチャの読み込み
	//Window Imaging Componentで対応する画像（bmp,png,jpg）などを読み込む
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\SampleTexture\\fade.bmp", WIC_FLAGS_NONE, &metadata, image);
	//読み込んだ画像データをDirectXへ渡してテクスチャとして管理させる
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);

	//初期状態
	g_Fade.fadecolor.x = 0.0f;
	g_Fade.fadecolor.y = 0.0f;
	g_Fade.fadecolor.z = 0.0f; //色は黒
	g_Fade.fadecolor.w = 1.0f; //不透明
	g_Fade.frame = 60; //フェード処理時間（フレーム）
	g_Fade.state = FADE_NONE; //何もしない
}

//更新処理
void UpdateFade()
{

}

//終了処理
void UninitFade()
{
	if (g_Texture != NULL)
	{
		g_Texture->Release();
		g_Texture = NULL;
	}
}

//描画処理
void DrawFade()
{
	//状態による分岐
	switch (g_Fade.state)
	{
	case FADE_NONE: //何もしない
		return;

	case FADE_IN: //フェードイン
		if (g_Fade.fadecolor.w < 0.0f)
		{
			//フェードイン終了チェック
			g_Fade.fadecolor.w = 0.0f;
			g_Fade.state = FADE_NONE;
		}
		break;

	case FADE_OUT: //フェードアウト
		if (g_Fade.fadecolor.w > 1.0f)
		{
			//フェードアウト終了チェック
			g_Fade.fadecolor.w = 1.0f;

			//フェードインの準備
			XMFLOAT4 color(0.0f, 0.0f, 0.0f, 1.0f);
			SetFade(20.0f, color, FADE_IN, g_Fade.scene);
			SetScene(g_Fade.scene);
		}
		break;
	}

	//スプライト準備
	XMFLOAT3 pos = XMFLOAT3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);
	XMFLOAT2 size = XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);
	DrawSprite(pos, size, g_Fade.fadecolor);

	//α値の変更
	switch (g_Fade.state)
	{
	case FADE_IN:
		g_Fade.fadecolor.w -= (1.0f / g_Fade.frame);
		break;

	case FADE_OUT:
		g_Fade.fadecolor.w += (1.0f / g_Fade.frame);
		break;
	}
}

//フェード処理のセット
void SetFade(int fadeframe, XMFLOAT4 color, FADE_STATE st, SCENE scene)
{
	g_Fade.frame = fadeframe;
	g_Fade.fadecolor = color;
	g_Fade.state = st;
	g_Fade.scene = scene;

	if (g_Fade.state == FADE_IN)
	{
		g_Fade.fadecolor.w = 1.0f;
	}
	else
	{
		g_Fade.fadecolor.w = 0.0f;
	}
}

//フェード処理状態の取得
FADE_STATE GetFadeState()
{
	return g_Fade.state;
}
