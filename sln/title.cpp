/*****************************************************************/
//
// 
// title.cpp（タイトル表示）
//
//
//
//                    ２０２４年１２月２４日　製作者名：佐藤琢磨
//
/*****************************************************************/

#include "title.h"
#include "Manager.h"
#include "keyboard.h"
#include "renderer.h"
#include "sprite.h"
#include "count.h"
#include "Audio.h"

Count g_Count;
static int g_TextureId[8]{};
static int g_AudioId[3]{}; // オーディオID

static XMFLOAT2 g_BallSpeed = { 5.0f, 0.0f };
static XMFLOAT2 g_EnterTextSizeSpeed = { 0.0f, 0.0f };

static XMFLOAT3 g_TextPos = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };
static XMFLOAT3 g_BallPos = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f };

static XMFLOAT2 g_NotTextSize = { 500.0f, 500.0f };
static XMFLOAT2 g_BallSize = { 80.0f, 80.0f };
static XMFLOAT2 g_EnterTextSize = { 450.0f, 450.0f };
static XMFLOAT2 g_ExplanationSize = { 0.0f, 0.0f };

static XMFLOAT4 g_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
static XMFLOAT4 g_WhiteColor = { 0.0f, 0.0f, 0.0f, 0.5f };

static bool g_Explanation = false;

void InitTitle()
{
	g_Count.InitCount();
	g_TextureId[0] = TextureLoad(L"asset/texture/NotScoreText.png");
	g_TextureId[1] = TextureLoad(L"asset/texture/Golouleff_title_bg.png");
	g_TextureId[2] = TextureLoad(L"asset/texture/Ball.png");
	g_TextureId[3] = TextureLoad(L"asset/texture/Golouleff.png");
	g_TextureId[4] = TextureLoad(L"asset/texture/Enter_de_start.png");
	g_TextureId[5] = TextureLoad(L"asset/texture/Space_de_detumei.png");
	g_TextureId[6] = TextureLoad(L"asset/texture/white.png");
	g_TextureId[7] = TextureLoad(L"asset/texture/GolouloffExplanation.png");

	g_AudioId[0] = LoadAudio("asset\\Audio\\Golouleff_title_BGM.wav"); // オーディオの読み込み
	g_AudioId[1] = LoadAudio("asset\\Audio\\Explation_open_SE.wav"); // オーディオの読み込み
	g_AudioId[2] = LoadAudio("asset\\Audio\\Explation_close_SE.wav"); // オーディオの読み込み

	PlayAudio(g_AudioId[0], true); // オーディオの再生　trueはループする
}

void UninitTitle()
{
	for (int i = 0; i < 3; ++i) {
		UnloadAudio(g_AudioId[i]); // オーディオの解放
	}

	g_Count.UninitCount();
}

void UpdateTitle()
{
	if (!g_Explanation){
		g_ExplanationSize.x = 0.0f;
		g_ExplanationSize.y = 0.0f;

		// シーン遷移
		if (Keyboard_IsKeyDownTrigger(KK_ENTER)){
			SetScene(SCENE_GAME);
		}

		//説明表示へ移動
		if (Keyboard_IsKeyDownTrigger(KK_SPACE)) {
			PlayAudio(g_AudioId[1], false); // オーディオの再生　trueはループする
			g_Explanation = true;
		}

		// ボール動作処理
		g_BallSpeed.y += 1.0f;

		g_BallPos.x += g_BallSpeed.x;
		g_BallPos.y += g_BallSpeed.y;

		if (g_BallPos.x <= g_BallSize.x * 0.5f || g_BallPos.x >= SCREEN_WIDTH - g_BallSize.x * 0.5f) {
			g_BallSpeed.x *= -1;
		}

		if (g_BallPos.y >= 550.0f) {
			g_BallSpeed.y = -30.0f;
		}

		// エンターでスタートテキストの動作処理
		g_EnterTextSize.x += g_EnterTextSizeSpeed.x;
		g_EnterTextSize.y += g_EnterTextSizeSpeed.y;

		g_EnterTextSizeSpeed.x += -0.1f;
		g_EnterTextSizeSpeed.y += -0.1f;

		if (g_EnterTextSize.x <= 450.0f) {
			g_EnterTextSizeSpeed.x = 5.0f;
			g_EnterTextSizeSpeed.y = 5.0f;
		}
	}
	else {
		//タイトルへ戻る
		if (Keyboard_IsKeyDownTrigger(KK_SPACE)) {
			PlayAudio(g_AudioId[2], false); // オーディオの再生　trueはループする
			g_Explanation = false;
		}

		g_ExplanationSize.x += 89.6f;
		g_ExplanationSize.y += 50.4f;

		if (g_ExplanationSize.x >= SCREEN_WIDTH * 0.85f)
		{
			g_ExplanationSize.x = SCREEN_WIDTH * 0.85f;
		}

		if (g_ExplanationSize.y >= SCREEN_HEIGHT * 0.85f)
		{
			g_ExplanationSize.y = SCREEN_HEIGHT * 0.85f;
		}
	}
}

void DrawTitle()
{
	// 背景
	SetTexture(g_TextureId[1]);
	DrawSprite({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f }, {SCREEN_WIDTH, SCREEN_HEIGHT}, g_Color);

	// ボール
	SetTexture(g_TextureId[2]);
	DrawSprite(g_BallPos, g_BallSize, g_Color);
	
	// スコア
	if (g_Count.GetOldCount() == 1000) {
		SetUVMatrix(XMMatrixIdentity());

		// 2D描画なので奥行処理をOFF
		SetDepthEnable(false);

		SetPixelShader2d();

		SetTexture(g_TextureId[0]);

		DrawSprite({ g_TextPos.x, g_TextPos.y, 0.0f }, g_NotTextSize, g_Color);
	}
	else {
		g_Count.DrawOldCount(g_TextPos.x, g_TextPos.y);
	}

	// タイトル
	SetTexture(g_TextureId[3]);
	DrawSprite({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.2f, 0.0f }, { 900.0f, 900.0f }, g_Color);

	// エンターでスタートテキスト
	SetTexture(g_TextureId[4]);
	DrawSprite({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.75f, 0.0f }, g_EnterTextSize, g_Color);

	// スペースで説明表示テキスト
	SetTexture(g_TextureId[5]);
	DrawSprite({ SCREEN_WIDTH * 0.90f, SCREEN_HEIGHT * 0.75f, 0.0f }, {200.0f, 200.0f}, g_Color);

	//説明を読むとき表示
	if (g_Explanation) {
		// 説明表示の背景
		SetTexture(g_TextureId[6]);
		DrawSprite({ SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT }, g_WhiteColor);

		SetTexture(g_TextureId[7]);
		DrawSprite({ SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5f, 0.0f }, g_ExplanationSize, g_Color);
	}
}

void SetOldCount(int count)
{
	g_Count.SetOldCount(count);
}

int GetOldCount()
{
	return g_Count.GetOldCount();
}
