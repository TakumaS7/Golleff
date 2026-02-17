#pragma once
/*****************************************************************/
//
// 
// polygon.cpp（ポリゴン表示）
//
//
//
//                    ２０２４年09月26日　製作者名：佐藤琢磨
//
/*****************************************************************/

#include "main.h"
#include "renderer.h"
#include "polygon.h"
#include "sprite.h"
#include "line.h"
#include "debugPrintf.h"
#include "keyboard.h"

/********************************************************
* マクロ定義
********************************************************/

#define NUM_VERTEX 100

/********************************************************
* プロトタイプ宣言
********************************************************/

/********************************************************
* グローバル変数
********************************************************/

static ID3D11Buffer

*g_VertexBuffer = NULL; //頂点情報

//テクスチャを表すオブジェクトのポインタ変数　画像1枚に１つ必要になる
static ID3D11ShaderResourceView* g_Texture = NULL;
static ID3D11ShaderResourceView* g_Texture2 = NULL;
static ID3D11ShaderResourceView* g_Texture3 = NULL;
static ID3D11ShaderResourceView* g_Texture4 = NULL;

Player Object1;
Player Object2;

/********************************************************
* 頂点管理[polygon.cpp]
* 
* Author:
*
* Date:
*
********************************************************/

VERTEX g_Vertex;

//初期化処理
HRESULT InitPolygon(void)
{
	//テクスチャの読み込み
	//Window Imaging Componentで対応する画像（bmp,png,jpg）などを読み込む
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\texture.jpg", WIC_FLAGS_NONE, &metadata, image);
	//読み込んだ画像データをDirectXへ渡してテクスチャとして管理させる
	CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &g_Texture);

	//テクスチャの読み込み
    //Window Imaging Componentで対応する画像（bmp,png,jpg）などを読み込む
	TexMetadata metadata2;
	ScratchImage image2;
	LoadFromWICFile(L"asset\\texture\\bullet000.png", WIC_FLAGS_NONE, &metadata2, image2);
	//読み込んだ画像データをDirectXへ渡してテクスチャとして管理させる
	CreateShaderResourceView(GetDevice(), image2.GetImages(), image2.GetImageCount(), metadata2, &g_Texture2);

	//テクスチャの読み込み
//Window Imaging Componentで対応する画像（bmp,png,jpg）などを読み込む
	TexMetadata metadata3;
	ScratchImage image3;
	LoadFromWICFile(L"asset\\texture\\run.png", WIC_FLAGS_NONE, &metadata3, image3);
	//読み込んだ画像データをDirectXへ渡してテクスチャとして管理させる
	CreateShaderResourceView(GetDevice(), image3.GetImages(), image3.GetImageCount(), metadata3, &g_Texture3);

	//テクスチャの読み込み
//Window Imaging Componentで対応する画像（bmp,png,jpg）などを読み込む
	TexMetadata metadata4;
	ScratchImage image4;
	LoadFromWICFile(L"asset\\texture\\mario_kinoko.png", WIC_FLAGS_NONE, &metadata4, image4);
	//読み込んだ画像データをDirectXへ渡してテクスチャとして管理させる
	CreateShaderResourceView(GetDevice(), image4.GetImages(), image4.GetImageCount(), metadata4, &g_Texture4);

	//なんか失敗した場合に警告を出す
	assert(g_Texture);



	//オブジェクト１初期化
	Object1.position = XMFLOAT3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);
	Object1.rotate = 0.0f;
	Object1.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Object1.scale = XMFLOAT2(1.0f, 1.0f);
	Object1.size = XMFLOAT2(200.0f, 200.0f);
	Object1.texID = g_Texture3;

	//オブジェクト２初期化
	Object2.position = XMFLOAT3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);
	Object2.rotate = 0.0f;
	Object2.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	Object2.scale = XMFLOAT2(1.0f, 1.0f);
	Object2.size = XMFLOAT2(100.0f, 100.0f);
	Object2.texID = g_Texture3;



	g_Vertex.posx = SCREEN_WIDTH / 2; //最初の座標
	g_Vertex.posy = SCREEN_HEIGHT / 2; //最初の座標

	g_Vertex.speed = 10.0f; //初速度
	g_Vertex.speed2 = 2.0f; //初速度（プレイヤーとキノコ）

	g_Vertex.s = 0; //スイッチ
	g_Vertex.s2 = 0; //スイッチ



	g_Vertex.posx2 = SCREEN_WIDTH / 2 - 250; //最初の座標
	g_Vertex.posy2 = SCREEN_HEIGHT / 2 - 250; //最初の座標

	g_Vertex.s3 = 0; //スイッチ
	g_Vertex.s4 = 0; //スイッチ



	g_Vertex.posx3 = SCREEN_WIDTH / 2 - 500; //最初の座標
	g_Vertex.posy3 = SCREEN_HEIGHT / 2 - 500; //最初の座標

	g_Vertex.s5 = 0; //スイッチ
	g_Vertex.s6 = 0; //スイッチ



	g_Vertex.posx4 = SCREEN_WIDTH / 2; //最初の座標（プレイヤー）
	g_Vertex.posy4 = SCREEN_HEIGHT / 2; //最初の座標（プレイヤー）

	g_Vertex.posx5 = 1600.0f; //最初の座標（キノコ）

	g_Vertex.bigsize = 200.0f; //サイズ変更

	g_Vertex.bigspeed = 200.0f; //サイズ変更スピード

	return S_OK;
}

//終了処理
void UninitPolygon(void)
{
	//テクスチャの解放
	g_Texture->Release();
}

//更新処理
void UpdatePolygon(void)
{	
	//DebugPrintf("あいうえお\n");

	//int a = 100;
	//DebugPrintf("かきくけこ %d\n", a);

	//if (g_Vertex.s == 0)
	//{
	//	g_Vertex.posx += g_Vertex.speed;

	//	if (g_Vertex.posx >= SCREEN_WIDTH - 100)
	//	{
	//		g_Vertex.s = 1;
	//	}
	//}

	//if (g_Vertex.s2 == 0)
	//{
	//	g_Vertex.posy += g_Vertex.speed;

	//	if (g_Vertex.posy >= SCREEN_HEIGHT - 100)
	//	{
	//		g_Vertex.s2 = 1;
	//	}
	//}

	//if (g_Vertex.s == 1)
	//{
	//	g_Vertex.posx -= g_Vertex.speed;

	//	if (g_Vertex.posx <= 100)
	//	{
	//		g_Vertex.s = 0;
	//	}
	//}

	//if (g_Vertex.s2 == 1)
	//{
	//	g_Vertex.posy -= g_Vertex.speed;

	//	if (g_Vertex.posy <= 100)
	//	{
	//		g_Vertex.s2 = 0;
	//	}
	//}
	//


	//if (g_Vertex.s3 == 0)
	//{
	//	g_Vertex.posx2 += g_Vertex.speed;

	//	if (g_Vertex.posx2 >= SCREEN_WIDTH - 100)
	//	{
	//		g_Vertex.s3 = 1;
	//	}
	//}

	//if (g_Vertex.s4 == 0)
	//{
	//	g_Vertex.posy2 += g_Vertex.speed;

	//	if (g_Vertex.posy2 >= SCREEN_HEIGHT - 100)
	//	{
	//		g_Vertex.s4 = 1;
	//	}
	//}

	//if (g_Vertex.s3 == 1)
	//{
	//	g_Vertex.posx2 -= g_Vertex.speed;

	//	if (g_Vertex.posx2 <= 100)
	//	{
	//		g_Vertex.s3 = 0;
	//	}
	//}

	//if (g_Vertex.s4 == 1)
	//{
	//	g_Vertex.posy2 -= g_Vertex.speed;

	//	if (g_Vertex.posy2 <= 100)
	//	{
	//		g_Vertex.s4 = 0;
	//	}
	//}



	//if (g_Vertex.s5 == 0)
	//{
	//	g_Vertex.posx3 += g_Vertex.speed;

	//	if (g_Vertex.posx3 >= SCREEN_WIDTH - 100)
	//	{
	//		g_Vertex.s5 = 1;
	//	}
	//}

	//if (g_Vertex.s6 == 0)
	//{
	//	g_Vertex.posy3 += g_Vertex.speed;

	//	if (g_Vertex.posy3 >= SCREEN_HEIGHT - 100)
	//	{
	//		g_Vertex.s6 = 1;
	//	}
	//}

	//if (g_Vertex.s5 == 1)
	//{
	//	g_Vertex.posx3 -= g_Vertex.speed;

	//	if (g_Vertex.posx3 <= 100)
	//	{
	//		g_Vertex.s5 = 0;
	//	}
	//}

	//if (g_Vertex.s6 == 1)
	//{
	//	g_Vertex.posy3 -= g_Vertex.speed;

	//	if (g_Vertex.posy3 <= 100)
	//	{
	//		g_Vertex.s6 = 0;
	//	}
	//}

	//g_Vertex.posx4 += g_Vertex.speed2; //プレイヤーが右に移動
	//g_Vertex.posx5 -= g_Vertex.speed2; //キノコが左に移動

	//if (g_Vertex.posx5 <= 910) //キノコ消去処理
	//{
	//	g_Vertex.posx5 = -50; //キノコ画面外へ移動
	//}

	//if (g_Vertex.posx5 <= -50) //キノコに当たった時のプレイヤー処理
	//{
	//	g_Vertex.bigspeed += 6.0f; //大きくなる速さ
	//}

	////
	////プレイヤーがだんだん多くなる処理
	////
	//if (g_Vertex.bigspeed >= 260) //１段階目サイズ２６０
	//{
	//	g_Vertex.bigsize = 260.0f; //サイズ変更
	//	g_Vertex.posy4 = 510.0f;   //プレイヤーの位置調整
	//	g_Vertex.speed2 = 0.0f;    //大きくなっている途中は停止する


	//	if (g_Vertex.bigspeed >= 340) //２段階目サイズ３４０
	//	{
	//		g_Vertex.bigsize = 340.0f; //サイズ変更
	//		g_Vertex.posy4 = 470.0f;   //プレイヤーの位置調整

	//		if (g_Vertex.bigspeed >= 400) //最終段階サイズ４００
	//		{
	//			g_Vertex.bigspeed = 400.0f; //サイズを固定
	//			g_Vertex.bigsize = 400.0f;  //サイズ変更
	//			g_Vertex.posy4 = 440.0f;    //プレイヤーの位置調整
	//			g_Vertex.speed2 = 2.0f;     //停止解除
	//		}
	//	}
	//}
}

//描画処理
void DrawPolygon(void)
{	
	//XMFLOAT4 col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); //ポリゴン色変え
	//SetColor(col);

	//g_Textureが表すテクスチャをポリゴン描画で使うように設定する
	//設定したテクスチャは、設定を変更しない限りずっと有効
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture);

	// マトリクス設定
	SetWorldViewProjection2D();

	//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture2);

	//	//ライン描画テスト
	//	static XMFLOAT3 spos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	static XMFLOAT3 epos = XMFLOAT3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	//	static XMFLOAT4 linecolor = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	//	DrawLine(spos, epos, linecolor);

	//	//トライアングル描画テスト
	//	static XMFLOAT3 v0 = XMFLOAT3(200.0f, 200.0f, 0.0f);
	//	static XMFLOAT3 v1 = XMFLOAT3(300.0f, 350.0f, 0.0f);
	//	static XMFLOAT3 v2 = XMFLOAT3(0.0f, 500.0f, 0.0f);
	//	static XMFLOAT4 tricolor = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	//	DrawTriangle(v0, v1, v2, tricolor);

	//	//ペンタゴン描画テスト
	//	static XMFLOAT3 u0 = XMFLOAT3(300.0f, 400.0f, 0.0f);
	//	static XMFLOAT3 u1 = XMFLOAT3(500.0f, 600.0f, 0.0f);
	//	static XMFLOAT3 u2 = XMFLOAT3(100.0f, 600.0f, 0.0f);
	//	static XMFLOAT3 u3 = XMFLOAT3(400.0f, 800.0f, 0.0f);
	//	static XMFLOAT3 u4 = XMFLOAT3(200.0f, 800.0f, 0.0f);
	//	static XMFLOAT4 pencolor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	//	DrawPentagon(u0, u1, u2, u3, u4, pencolor);

	//	//テスト表示
	//	XMFLOAT3 position = XMFLOAT3(g_Vertex.posx, g_Vertex.posy, 0.0f);
	//	XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);
	//	XMFLOAT2 size(200.0f, 200.0f);
	//	DrawSprite(position, size, color);

	//	//テスト表示
	//	XMFLOAT3 position2 = XMFLOAT3(g_Vertex.posx2, g_Vertex.posy2, 0.0f);
	//	XMFLOAT4 color2(1.0f, 1.0f, 1.0f, 1.0f);
	//	XMFLOAT2 size2(200.0f, 200.0f);
	//	DrawSprite(position2, size2, color2);

	//	//テスト表示
	//	XMFLOAT3 position3 = XMFLOAT3(g_Vertex.posx3, g_Vertex.posy3, 0.0f);
	//	XMFLOAT4 color3(1.0f, 1.0f, 1.0f, 1.0f);
	//	XMFLOAT2 size3(200.0f, 200.0f);
	//	DrawSprite(position3, size3, color3);



		//g_Textureが表すテクスチャをポリゴン描画で使うように設定する
        //設定したテクスチャは、設定を変更しない限りずっと有効
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture3);

		//XMFLOAT4 c = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//SetColor(c);

		////テスト表示
		//static XMFLOAT3 position4 = XMFLOAT3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		//static XMFLOAT4 color4 = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//static XMFLOAT2 size4 = XMFLOAT2(500.0f, 500.0f);
		//static FLOAT ptno = 0;
		//static FLOAT scx = 1.0f;
		//static FLOAT scy = 1.0f;

		//キー入力テスト
		//if (Keyboard_IsKeyDownTrigger(KK_UP))
		//{
		//	position4.y += -3.0f;
		//}

		//if (Keyboard_IsKeyDownTrigger(KK_DOWN))
		//{
		//	position4.y += 3.0f;
		//}

		{//オブジェクト１の処理
			//キー操作で移動
			if (Keyboard_IsKeyDownTrigger(KK_UP))
			{
				Object1.position.y += -5.0f;
			}

			if (Keyboard_IsKeyDownTrigger(KK_DOWN))
			{
				Object1.position.y += 5.0f;
			}

			if (Keyboard_IsKeyDownTrigger(KK_LEFT))
			{
				Object1.position.x += -5.0f;
			}

			if (Keyboard_IsKeyDownTrigger(KK_RIGHT))
			{
				Object1.position.x += 5.0f;
			}

			//平行移動行列
			XMMATRIX TranslationMatrix = XMMatrixTranslation(
				Object1.position.x,
				Object1.position.y,
				0.0f
			);

			//回転行列
			XMMATRIX RotationMatrix = XMMatrixRotationZ(
				XMConvertToRadians(Object1.rotate)
			);

			//スケーリング行列
			XMMATRIX ScalingMatrix = XMMatrixScaling(
				Object1.scale.x, Object1.scale.y, 1.0f
			);

			//ワールド行列
			XMMATRIX WorldMatrix = ScalingMatrix * RotationMatrix * TranslationMatrix;
			
			//ワールド行列をDirectXへセット
			SetWorldMatrix(WorldMatrix);
			DrawSpriteLine(Object1.size, Object1.color);
		}



		//{//オブジェクト２の処理１

		//	//平行移動行列
		//	XMMATRIX TranslationMatrix = XMMatrixTranslation(
		//		Object2.position.x,
		//		Object2.position.y,
		//		0.0f
		//	);

		//	//回転行列
		//	XMMATRIX RotationMatrix = XMMatrixRotationZ(
		//		XMConvertToRadians(Object2.rotate)
		//	);

		//	//スケーリング行列
		//	XMMATRIX ScalingMatrix = XMMatrixScaling(
		//		Object2.scale.x, Object2.scale.y, 1.0f
		//	);

		//	//ワールド行列
		//	XMMATRIX WorldMatrix = ScalingMatrix * RotationMatrix * TranslationMatrix;
		
		//ワールド行列をDirectXへセット
		//	SetWorldMatrix(WorldMatrix);
		//	DrawSprite(Object2.size, Object2.color);
		//}

		//{//オブジェクト２の処理２

		//	//OBJECT1からの表示座標の差分
		//	Object2.position = XMFLOAT3(200.0f, 0.0f, 0.0f); //右へ200.0f

	    //    //平行移動行列
		//	XMMATRIX TranslationMatrix = XMMatrixTranslation(
		//		Object2.position.x,
		//		Object2.position.y,
		//		0.0f
		//	);

		//	//Object1の表示座標の行列
		//	XMMATRIX TranslationMatrix2 = XMMatrixTranslation(
		//		Object1.position.x,
		//		Object1.position.y,
		//		0.0f
		//	);

		//	//回転行列（Z回転）角度をラジアンで与える
		//	XMMATRIX RotationMatrix = XMMatrixRotationZ(
		//		XMConvertToRadians(Object2.rotate)
		//	);

		//	//スケーリング行列（1.0が等倍）
		//	XMMATRIX ScalingMatrix = XMMatrixScaling(
		//		Object2.scale.x, Object2.scale.y, 1.0f
		//	);

		//	//ワールド行列
		//	XMMATRIX WorldMatrix = 
		//		ScalingMatrix * 
		//		RotationMatrix * 
		//		TranslationMatrix * 
		//		TranslationMatrix2;
		//	
		//	Object2.rotate += 1.5f;
		//	
		//	//ワールド行列をDirectXへセット
		//	SetWorldMatrix(WorldMatrix);
		//	DrawSprite(Object2.size, Object2.color);
		//}

		//{//オブジェクト２の処理３

		//	OBJECT1からの表示座標の差分
		//	Object2.position = XMFLOAT3(200.0f, 0.0f, 0.0f); //右へ200.0f

		//	平行移動行列
		//	XMMATRIX TranslationMatrix = XMMatrixTranslation(
		//		Object2.position.x,
		//		Object2.position.y,
		//		0.0f
		//	);

		//	Object1の表示座標の行列
		//	XMMATRIX TranslationMatrix2 = XMMatrixTranslation(
		//		Object1.position.x,
		//		Object1.position.y,
		//		0.0f
		//	);

		//	回転行列（Z回転）角度をラジアンで与える
		//	XMMATRIX RotationMatrix = XMMatrixRotationZ(
		//		XMConvertToRadians(Object2.rotate)
		//	);

		//	スケーリング行列（1.0が等倍）
		//	XMMATRIX ScalingMatrix = XMMatrixScaling(
		//		Object2.scale.x, Object2.scale.y, 1.0f
		//	);

		//	ワールド行列
		//	XMMATRIX WorldMatrix =
		//		ScalingMatrix *     //サイズ調整
		//		TranslationMatrix * //移動
		//		RotationMatrix *    //回転
		//		TranslationMatrix2; //移動

		//	Object2.rotate += 1.5f;

		//	ワールド行列をDirectXへセット
		//	SetWorldMatrix(WorldMatrix);
		//	DrawSprite(Object2.size, Object2.color);
		//}

		//{//オブジェクト２の処理４

		//	//OBJECT1からの表示座標の差分
		//	Object2.position = XMFLOAT3(200.0f, 0.0f, 0.0f); //右へ200.0f

		//	//平行移動行列
		//	XMMATRIX TranslationMatrix = XMMatrixTranslation(
		//		Object2.position.x,
		//		Object2.position.y,
		//		0.0f
		//	);

		//	//Object1の表示座標の行列
		//	XMMATRIX TranslationMatrix2 = XMMatrixTranslation(
		//		Object1.position.x,
		//		Object1.position.y,
		//		0.0f
		//	);

		//	//回転行列（Z回転）角度をラジアンで与える
		//	XMMATRIX RotationMatrix = XMMatrixRotationZ(
		//		XMConvertToRadians(Object2.rotate)
		//	);

		//	//回転行列（Z回転）角度をラジアンで与える
		//	XMMATRIX RotationMatrixReverse = XMMatrixRotationZ(
		//		XMConvertToRadians(Object2.rotate * -4)
		//	);

		//	//スケーリング行列（1.0が等倍）
		//	XMMATRIX ScalingMatrix = XMMatrixScaling(
		//		Object2.scale.x, Object2.scale.y, 1.0f
		//	);

		//	//ワールド行列
		//	XMMATRIX WorldMatrix =
		//		ScalingMatrix *         //サイズ調整
		//		RotationMatrixReverse * //回転
		//		TranslationMatrix *     //移動
		//		RotationMatrix *        //回転
		//		TranslationMatrix2;     //移動

		//	Object2.rotate += 1.5f;

		//	//ワールド行列をDirectXへセット
		//	SetWorldMatrix(WorldMatrix);
		//	DrawSprite(Object2.size, Object2.color);
		//}

		{//オブジェクト２の処理５

			//OBJECT1からの表示座標の差分
			Object2.position = XMFLOAT3(300.0f, 0.0f, 0.0f); //右へ200.0f

			//平行移動行列
			XMMATRIX TranslationMatrix = XMMatrixTranslation(
				Object2.position.x,
				Object2.position.y,
				0.0f
			);

			//Object1の表示座標の行列
			XMMATRIX TranslationMatrix2 = XMMatrixTranslation(
				Object1.position.x,
				Object1.position.y,
				0.0f
			);

			//回転の半径移動の行列
			XMMATRIX TranslationMatrix3 = XMMatrixTranslation(
				Object2.position.x - 200,
				Object2.position.y,
				0.0f
			);

			//回転行列（Z回転）角度をラジアンで与える
			XMMATRIX RotationMatrix = XMMatrixRotationZ(
				XMConvertToRadians(Object2.rotate)
			);

			//スケーリング行列（1.0が等倍）
			XMMATRIX ScalingMatrix = XMMatrixScaling(
				Object2.scale.x, Object2.scale.y, 1.0f
			);

			//ワールド行列
			XMMATRIX WorldMatrix =
				ScalingMatrix *      //サイズ調整
				TranslationMatrix3 * //移動
				RotationMatrix *     //回転
				TranslationMatrix *  //移動
				TranslationMatrix2;  //移動

			Object2.rotate += 2.5f;

			//ワールド行列をDirectXへセット
			SetWorldMatrix(WorldMatrix);
			DrawSpriteLine(Object2.size, Object2.color);
		}

		////
		////ワールド行列作成
		////
		////平行移動行列
		//XMMATRIX TranslationMatrix = XMMatrixTranslation(position4.x, position4.y, 0.0f);
		////回転行列（Ｚ回転）
		//XMMATRIX RotationMatrix = XMMatrixRotationZ(XMConvertToRadians(0.0f));
		////スケーリング行列（1.0が等倍）※注意！倍率に0.0fはダメ！（消える）
		//XMMATRIX ScalingMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		////ワールド行列
		//XMMATRIX WorldMatrix = ScalingMatrix * RotationMatrix * TranslationMatrix;

		////ワールド行列をDirectXへセット
		//SetWorldMatrix(WorldMatrix);

		//DrawSprite(size4, color4); //行列版で表示
		////DrawSpriteRotateUV(position4, size4, color4, (3.14 / 180.0 * 0), ptno, 5, 2);

		//ptno += 0.3f; //パターン番号を少しずつ増やす
		//if (ptno >= 9.0f)ptno -= 9.0f; //パターン番号をループさせる



		////g_Textureが表すテクスチャをポリゴン描画で使うように設定する
		////設定したテクスチャは、設定を変更しない限りずっと有効
		//GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture4);

		////テスト表示
		//XMFLOAT3 position5 = XMFLOAT3(g_Vertex.posx5, SCREEN_HEIGHT / 2 + 350, 0.0f);
		//XMFLOAT4 color5(1.0f, 1.0f, 1.0f, 1.0f);
		//XMFLOAT2 size5(100.0f, 100.0f);
		//DrawSprite(position5, size5, color5);
}
