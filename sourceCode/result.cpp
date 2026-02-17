#include "result.h"
#include "texture.h"
#include "renderer.h"
#include "sprite.h"
#include "main.h"
#include "count.h"
#include "keyboard.h"
#include "Manager.h"
#include "title.h"
#include "Audio.h"

static int g_TextureId[2]{};

static XMFLOAT2 g_BgSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
static XMFLOAT2 g_EnterSize = { 500.0f, 500.0f };

static XMFLOAT4 g_BgColor = { 0.5f, 0.5f, 0.5f, 0.7f };
static XMFLOAT4 g_EnterColor = { 1.0f, 1.0f, 1.0f, 1.0f };

static Count g_Count;
static float g_SizeSpeed = 0.0f;

static int g_AudioId = -1;

void InitResult()
{
	g_Count.InitCount();
	g_TextureId[0] = TextureLoad(L"asset/texture/white.png");
	g_TextureId[1] = TextureLoad(L"asset/texture/EnterLog.png");

	g_AudioId = LoadAudio("asset\\Audio\\Golouleff_result_SE.wav"); // オーディオの読み込み

	PlayAudio(g_AudioId, false); // オーディオの再生　trueはループする
}

void UninitResult()
{
	UnloadAudio(g_AudioId); // オーディオの解放
	g_Count.UninitCount();
}

void UpdateResult()
{
	g_Count.ChangeRankResult();

	if (g_Count.CheckCount(GetOldCount(), g_Count.GetCount()))
	{
		SetOldCount(g_Count.GetCount());
	}

	//Enterテクスチャアニメーション
	if (g_EnterSize.x <= 400.0f) {
		g_SizeSpeed = 10.0f;
	}

	g_EnterSize.x += g_SizeSpeed;
	g_EnterSize.y += g_SizeSpeed;

	g_SizeSpeed -= 0.5f;

	if (Keyboard_IsKeyDownTrigger(KK_ENTER)) {
		SetScene(SCENE_TITLE);
	}
}

void DrawResult()
{
	SetWorldViewProjection2D();

	SetUVMatrix(XMMatrixIdentity());

	//2D描画なので奥行処理をOFF
	SetDepthEnable(false);

	SetPixelShader2d();

	SetTexture(g_TextureId[0]);

	DrawSprite({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f }, g_BgSize, g_BgColor);

	SetTexture(g_TextureId[1]);

	DrawSprite({ 1000.0f, 690.0f, 0.0f }, g_EnterSize, g_EnterColor);

	g_Count.SetSize({ 200.0f, 200.0f });
	g_Count.DrawCount(SCREEN_WIDTH * 0.5f, 250.f);
}

// game.cppのカウントをリザルトに持ってくる
void SetCount(int count)
{
	g_Count.SetCount(count);
}

int GetCount()
{
	return g_Count.GetCount();
}
