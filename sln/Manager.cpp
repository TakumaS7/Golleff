/*****************************************************************/
//
// 
// Manager.cpp（マネージャー処理）
//
//
//
//                    ２０２４年09月26日　製作者名：佐藤琢磨
//
/*****************************************************************/

#include "main.h"
#include "Manager.h"

#include "fade.h"
#include "game.h"
#include "title.h"
#include "result.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static SCENE g_Scene = SCENE_NONE; //現在のシーン変数

//初期化
void InitManager()
{
	InitFade();

	//開始シーンのセット
	SetScene(SCENE_TITLE); //ゲームシーンからスタート
}

//終了処理
void UninitManager()
{
	UninitFade();

	SetScene(SCENE_NONE);
}

//更新処理
void UpdateManager()
{
	UpdateFade();

	//シーンごとの更新処理
	switch (g_Scene)
	{
	case SCENE_NONE: //何もしない
		break;

	case SCENE_TITLE: //タイトル
		UpdateTitle();
		break;

	case SCENE_GAME: //ゲーム
		UpdateGame();
		break;

	case SCENE_RESULT: //リザルト
		UpdateResult();
		break;
	}
}

//表示処理
void DrawManager()
{
	//シーンごとの表示処理
	switch (g_Scene)
	{
	case SCENE_NONE: //何もしない
		break;

	case SCENE_TITLE: //タイトル
		DrawTitle();
		break;

	case SCENE_GAME: //ゲーム
		DrawGame();
		break;

	case SCENE_RESULT: //リザルト
		DrawGame();
		DrawResult();
		break;
	}

	DrawFade();
}

//シーンの切り替え
void SetScene(SCENE Scene)
{
	//現シーンの終了
	switch (g_Scene)
	{
	case SCENE_NONE: //何もしない
		break;

	case SCENE_TITLE: //タイトル
		UninitTitle();
		break;

	case SCENE_GAME: //ゲーム
		UninitGameBGM();
		break;

	case SCENE_RESULT: //リザルト
		UninitGame();
		UninitResult();
		break;
	}

	g_Scene = Scene; //次シーンをセット

	//次シーンの初期化
	switch (g_Scene)
	{
	case SCENE_NONE: //何もしない
		break;

	case SCENE_TITLE: //タイトル
		InitTitle();
		break;

	case SCENE_GAME: //ゲーム
		InitGame();
		break;

	case SCENE_RESULT: //リザルト
		InitResult();
		break;
	}
}

