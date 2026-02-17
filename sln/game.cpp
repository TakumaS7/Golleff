/*****************************************************************/
//
// 
// game.cpp（ゲーム表示）
//
//
//
//                    ２０２４年０６月１６日　製作者名：佐藤琢磨
//
/*****************************************************************/

#include "game.h"
#include "texture.h"
#include "sprite.h"
#include "cube.h"
#include "grid.h"
#include "golf_cours.h"
#include "camera.h"
#include "move_camera.h"
#include "keyboard.h"
#include "rotation_camera.h"
#include "model.h"
#include "light.h"
#include "collision.h"
#include "ball.h"
#include "ball_shot.h"
#include "billboard.h"
#include "trail.h"
#include "count.h"
#include "ball_camera.h"
#include "roulette.h"
#include "Manager.h"
#include "result.h"
#include "propeller.h"
#include "wind.h"
#include "pole.h"
#include "Audio.h"

static float g_offset = 0.0f;
static float g_angle = 0.0f;
static Camera* g_Camera[2];
static int g_CameraId = 0;
static GolfCours g_GolfCours;

static MODEL* g_pModel = nullptr;
static MODEL* g_pModel2 = nullptr;
static Ball g_Ball({ 3.5f, 5.0f, -4.0f });
//static XMVECTOR g_CubePosition{ 0.0f, -0.5f, 0.0f };

typedef enum
{
	GAME_STATE_NONE,	
	GAME_STATE_SHOT,
	GAME_STATE_BALLMOVE,
	GAME_STATE_WAIT,
	GAME_STATE_MAX
} GAME_STATE;

typedef enum
{
	SHOT_STATE_ANGLE_Y,
	SHOT_STATE_ANGLE_X,
	SHOT_STATE_ROULETTE
} SHOT_STATE;

static GAME_STATE g_State = GAME_STATE_NONE;
static unsigned int g_FrameCounter = 0;
static unsigned int g_FrameCountRegist = 0;

static Count g_Count{};
static BallCamera* g_BallCamera{};

static Roulette g_Roulette{};

static SHOT_STATE g_ShotState = SHOT_STATE_ANGLE_Y;

static Propeller g_Propeller{};

static XMVECTOR g_WindVec = { -1.0f, 0.0f, 0.0f };

static Pole g_Pole{};

static int g_AudioId[3]{}; // オーディオID

void InitGame()
{
	g_Ball.SetPosition({ 3.5f, 5.0f, -4.0f }); //初期座標
	g_Ball.OnOverlap(); //加速度０
	g_Count.SetCount(0); //カウントリセット

	InitCube();
	InitBillboard();
	InitBallShot();
	TrailInitialize();
	g_Count.InitCount();
	g_Roulette.InitRoulette();
	g_Propeller.InitPropeller();
	InitWind();
	g_Pole.InitPole();

	TextureLoad(L"asset/texture/white.png");

	g_AudioId[0] = LoadAudio("asset\\Audio\\Golouleff_game_BGM.wav"); // オーディオの読み込み
	g_AudioId[1] = LoadAudio("asset\\Audio\\Golouleff_shot_SE_01.wav"); // オーディオの読み込み
	g_AudioId[2] = LoadAudio("asset\\Audio\\Golouleff_shot_SE_02.wav"); // オーディオの読み込み

	PlayAudio(g_AudioId[0], true); // オーディオの再生　trueはループする

	g_BallCamera = new BallCamera;
	g_BallCamera->SetBall(g_Ball.GetPosition());

	g_Camera[0] = g_BallCamera;
	g_Camera[1] = new MovingCamera;
	g_Ball.ModelLoad();

	//ライトのロード
	SetDirectionalLight({ -1.0f, 0.0f, 0.0f, 0.0f }, { 0.8f, 0.7f, 0.6f, 1.0f });
	SetAmbient({ 0.3f, 0.3f, 0.7f, 0.0f });

	//打つボールをセット
	SetBallShot(&g_Ball);
}

void UninitGame()
{
	g_Ball.ModelRelease();

	g_Pole.UninitPole();
	UninitWind();
	g_Propeller.UninitPropeller();
	g_Count.UninitCount();
	TrailFinalize();
	UninitBallShot();
	UninitBillboard();
	UninitCube();

	for (int i = 0; i < 2; ++i){
		delete g_Camera[i];
	}
}

void UninitGameBGM()
{
	for (int i = 0; i < 3; ++i) {
		UnloadAudio(g_AudioId[i]); // オーディオの解放
	}
}

void UpdateGame()
{
	//風で玉が流される
	if (XMVectorGetZ(g_Ball.GetPosition()) > 8.5f &&
		XMVectorGetZ(g_Ball.GetPosition()) < 11.5f &&
		XMVectorGetY(g_Ball.GetPosition()) < 3.0f &&
		XMVectorGetX(g_Ball.GetPosition()) > -5.0f &&
		XMVectorGetX(g_Ball.GetPosition()) < 5.0f) {
		g_Ball.AddForce(g_WindVec * 0.007f);
	}

	if (g_State == GAME_STATE_NONE)
	{
		if (g_FrameCounter - g_FrameCountRegist > 60)
		{
			g_State = GAME_STATE_BALLMOVE;
		}
	}	
	else if (g_State == GAME_STATE_SHOT)
	{
		UpdateBallShot();

		if (g_ShotState == SHOT_STATE_ANGLE_Y)
		{
			YAngleRotation(); //ベクトル左右回転

			if (Keyboard_IsKeyDownTrigger(KK_SPACE)) {
				PlayAudio(g_AudioId[1], false); // オーディオの再生　trueはループする

				g_ShotState = SHOT_STATE_ANGLE_X;
			}
		}
		else if (g_ShotState == SHOT_STATE_ANGLE_X) 
		{
			XAngleRotation(); //ベクトル上下回転

			if (Keyboard_IsKeyDownTrigger(KK_SPACE)) {
				PlayAudio(g_AudioId[1], false); // オーディオの再生　trueはループする

				g_ShotState = SHOT_STATE_ROULETTE;
			}
		}
		else if (g_ShotState == SHOT_STATE_ROULETTE)
		{
			g_Roulette.RotationRoulette(); //ルーレットを回す

			if (Keyboard_IsKeyDownTrigger(KK_SPACE)){
				PlayAudio(g_AudioId[2], false); // オーディオの再生　trueはループする

				g_Count.AddCount(); //打数増やす
				g_Ball.AddForce(GetBallShotVelocity() * (g_Roulette.GetRadian() * 0.25));
				g_State = GAME_STATE_BALLMOVE;
			}
		}
	}
	else if (g_State == GAME_STATE_BALLMOVE)
	{
		SetTrailPosition(g_Ball.GetPosition());

		if (g_Ball.IsStopped())
		{
			g_State = GAME_STATE_WAIT;
			g_FrameCountRegist = g_FrameCounter;
		}
	}
	else if (g_State == GAME_STATE_WAIT)
	{
		if (g_FrameCounter - g_FrameCountRegist > 60)
		{
			g_Ball.SavePosition(g_Ball.GetPosition()); //止まった位置をセーブしとく

			g_Roulette.SetRadian(0.0f); //ルーレットの角度を初期値に戻す
			SetXYAngle(0.0f, 0.0f);

			g_ShotState = SHOT_STATE_ANGLE_Y;
			g_State = GAME_STATE_SHOT;
		}
	}

	g_FrameCounter++;

	//０，１キーで視点を切り替える
	for (int i = KK_D0; i <= KK_D1; i++)
	{
		if (Keyboard_IsKeyDownTrigger((Keyboard_Keys_tag)i))
		{
			g_CameraId = i - KK_D0;
		}
	}

	// Tキーを押した場合位置リセット
	if (Keyboard_IsKeyDownTrigger(KK_T))
	{
		g_State = GAME_STATE_NONE;
		g_Ball.SetPosition({ 3.5f, 5.0f, -4.0f });
		g_Ball.OnOverlap();
	}

	// -5.0まで落ちたら最後に止まった位置に戻る
	if (XMVectorGetY(g_Ball.GetPosition()) <= -5.0f) 
	{
		g_State = GAME_STATE_NONE;
		g_Ball.SetPosition(g_Ball.GetSavePosition());
		g_Ball.OnOverlap();
	}

	g_Ball.Update();

	AABB cube_local_aabb({ -0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f });

	for (int i = 0; i < sizeof(g_Map) / sizeof(g_Map[0]); i++)
	{
		AABB map_local_aabb({ -0.5, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f });
		AABB map_world_aabb = map_local_aabb.Translation(XMLoadFloat3(&g_Map[i]));
		g_Ball.OnHit(map_world_aabb);
	}

	g_offset += 0.05f; //動かす処理

	g_angle += 0.1f; //回転処理

	g_Camera[g_CameraId]->Update();

	g_BallCamera->SetBall(g_Ball.GetPosition());

	g_Propeller.UpdatePropeller();

	SetCount(g_Count.GetCount()); //リザルト表示用

	if (XMVectorGetZ(g_Ball.GetPosition()) > 35.5f &&
		XMVectorGetZ(g_Ball.GetPosition()) < 37.5f &&
		XMVectorGetY(g_Ball.GetPosition()) < 0.3f &&
		XMVectorGetX(g_Ball.GetPosition()) > 0.0f &&
		XMVectorGetX(g_Ball.GetPosition()) < 1.0f)
	{
		SetScene(SCENE_RESULT);
	}
}

void DrawGame()
{
	SetPixelShader3d();
	SetDepthEnable(true);

	SetViewMatrix(g_Camera[g_CameraId]->GetViewMatrix());
	SetProjectionMatrix(g_Camera[g_CameraId]->GetPerspectiveMatrix());

	g_Ball.Draw();

	g_Propeller.DrawPropeller();

	g_Pole.DrawPole();

	if (g_State == GAME_STATE_SHOT)
	{
		DrawBallShot();
	}

	{
		SetPixelShader2d();

		XMMATRIX scale = XMMatrixScaling(10.0f, 3.0f, 3.0f);
		XMMATRIX translation = XMMatrixTranslation(0.0f, 2.0f, 10.0f);

		SetWorldMatrix(scale * translation);
		DrawWind();
	}

	if (g_State == GAME_STATE_BALLMOVE)
	{
		SetPixelShader2d();
		SetTrailCameraPosition(g_Camera[1]->GetPosition());
		TrailDraw();
	}

	SetPixelShader3d();

	g_GolfCours.DrawMap();

	SetWorldViewProjection2D();
	g_Count.DrawCount(100.0f, 100.0f); //打数描画

	if (g_State == GAME_STATE_SHOT) {
		if (g_ShotState == SHOT_STATE_ROULETTE) {
			g_Roulette.DrawRoulette();
		}
	}
}
