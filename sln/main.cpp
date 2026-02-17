/*****************************************************************/
//
// 
// main.cpp（main.cpp）（メイン処理）
//
//
//
//                    ２０２４年０９月２６日　製作者名：佐藤琢磨
//
/*****************************************************************/

/*

	入力周り

	色々あるが現在マイクロソフト的に推奨している物

	コントローラー ＝＝＝＞ XInput
	キーボード ＝＝＝＞ マイクロソフトが配布している標準的な物


	１　keyboard.hと1cppを

*/


//ウィンドウの表示

#include "main.h"
#include "renderer.h"
#include "polygon.h"
#include "sprite.h"
#include "line.h"
#include "keyboard.h"
#include "Audio.h"
#include "Manager.h"
#include "texture.h"
#include "light.h"

//#include <Windows.h>

/********************************************************
* ライブラリのリンク
********************************************************/

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

/********************************************************
* マクロ定義
********************************************************/

#define CLASS_NAME "DX21 Window"
#define WINDOW_CAPTION "AT12B173-18-佐藤琢磨"

/********************************************************
* グローバル変数
********************************************************/

#ifdef _DEBUG //デバッグモード時のみ変数を作る
int g_CountFPS; //FPSカウンター
char g_DebugStr[2048] = WINDOW_CAPTION; //表示文字列設定

#endif

/********************************************************
* プロトタイプ宣言
********************************************************/

//ウィンドウプロシージャ
//コールバック関数　＝＞　他人が呼び出してくれる関数
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM IParam);

//初期化関数
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);

//終了処理
void Uninit(void);

//更新処理
void Update(void);

//描画処理
void Draw(void);

/********************************************************
* メイン関数
********************************************************/

int APIENTRY WinMain(_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nCmdShow)
{
	//乱数初期化
	srand((UINT)timeGetTime()); //デバッグ中はコメントでもよい

	UNREFERENCED_PARAMETER(hInstance);     //変数未使用時の対応
	UNREFERENCED_PARAMETER(hPrevInstance); //変数未使用時の対応
	UNREFERENCED_PARAMETER(lpCmdLine);     //変数未使用時の対応
	UNREFERENCED_PARAMETER(nCmdShow);      //変数未使用時の対応

	//フレームレート計測用変数
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	//COMコンポーネントの準備（機能を部品化して外部のプログラムから共有利用する仕組み）
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

	//ウィンドウクラスの登録（ウィンドウの仕様的な物を決めてWindowsへセットする）
	WNDCLASS wc; //構造体を準備

	ZeroMemory(&wc, sizeof(WNDCLASS)); //内容を０で初期化

	wc.lpfnWndProc = WndProc; //コールバック関数のポインター

	wc.lpszClassName = CLASS_NAME; //この仕様書の名前

	wc.hInstance = hInstance; //このアプリケーションのこと

	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //カーソルの種類

	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); //ウィンドウの背景色

	RegisterClass(&wc); //構造体をWindowsへセット

	//ウィンドウサイズの調整
	//          左上     右下
	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; //横1280　縦720
	//描画領域が960×540になるようにサイズを調整する
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, FALSE);

	//ウィンドウの作成
	HWND hWnd = CreateWindow
	(
		CLASS_NAME, //作りたいウィンドウ
	    WINDOW_CAPTION, //ウィンドウに表示するタイトル
	    WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX), //標準敵な形状のウィンドウ サイズ変更禁止
		CW_USEDEFAULT, //デフォルト設定でおまかせ
	    CW_USEDEFAULT, 
	    rc.right - rc.left, //CW_USEDEFAULT, 
		rc.bottom - rc.top, //CW_USEDEFAULT, 
		NULL,
		NULL,
		hInstance, //アプリケーションのハンドル
		NULL
	);

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, true)))
	{
		return -1; //初期化失敗
	}

	//作成したウィンドウを表示する
	ShowWindow(hWnd, nCmdShow); //引数に従って表示、または非表示

	//ウィンドウの内容を強制表示
	UpdateWindow(hWnd);

	//メッセージループ
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG)); //メッセージ構造体を作成して初期化

	//フレームレート計測初期化
	timeBeginPeriod(1); //タイマーの分解能を設定

	dwExecLastTime = dwFPSLastTime = timeGetTime(); //現在のタイマー値
	
	dwCurrentTime = dwFrameCount = 0;

	//終了メッセージが来るまでループする
	//ゲームループ
	while(1)
	{
		//メッセージの有無をチェック
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//Windowsから何かのメッセージが来ていた場合
			if (msg.message == WM_QUIT) //完全終了しましたメッセージ
			{
				break; //whileループからぬける
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg); //WndProcが呼び出される
			}
		}
		else //Windowsからメッセージが来ていない
		{
			dwCurrentTime = timeGetTime(); //現在のタイマー値を取得
			
			if ((dwCurrentTime - dwFPSLastTime) >= 1000) //1秒経過したか
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif

				dwFPSLastTime = dwCurrentTime; //現在のタイマー値を保存

				dwFrameCount = 0; //フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= ((float)1000 / 60)) // 1/60秒経過したか
			{
				dwExecLastTime = dwCurrentTime; //現在の時間を保持

#ifdef _DEBUG
				wsprintf(g_DebugStr, WINDOW_CAPTION); //配列にキャプション文字を格納

				wsprintf(&g_DebugStr[strlen(g_DebugStr)], "FPS:%d", g_CountFPS);

				SetWindowText(hWnd, g_DebugStr); //キャプション部分の書き換え
#endif
				
				Update(); //更新処理
				Draw(); //描画処理

				keycopy();

				dwFrameCount++; //フレームカウントを進める
			}
		}
	} //while

	//終了処理
	Uninit();

	//終了する
	return (int)msg.wParam;
}

/********************************************************
//ウィンドウプロシージャ
//メッセージループ内で呼び出される
********************************************************/

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
     case WM_ACTIVATEAPP:
     case WM_SYSKEYDOWN:
     case WM_KEYUP:
     case WM_SYSKEYUP:
         Keyboard_ProcessMessage(uMsg, wParam, lParam);
         break;

	case WM_KEYDOWN: //キーが押された

		if (wParam == VK_ESCAPE) //押されたのはESCキー
		{
			//ウィンドウを閉じたいリクエストをWindowsに送る
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}

		Keyboard_ProcessMessage(uMsg, wParam, lParam);

		break;

	case WM_CLOSE: //ウィンドウを閉じなさい命令

		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONWARNING) == IDOK)
		{
			//OKが押されたとき
			DestroyWindow(hWnd); //終了する手続きをWindowsへリクエスト
		}
		else
		{
			return 0; //やっぱり終わらない
		}


		break;

	case WM_DESTROY: //終了してOKですよ

		PostQuitMessage(0); //自分のメッセージに０を送る

		break;
	}

	//必要のないメッセージは適当に処理させて終了
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/********************************************************
* 初期化
********************************************************/

HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//キー入力初期化
	Keyboard_Initialize();

	//DirectX関連の初期化
	InitRenderer(hInstance, hWnd, bWindow);

	//テクスチャの初期化
	TextureInitialize();

	//ライト初期化
	InitLight();

	//サウンドの初期化
	InitAudio();

	//スプライトの初期化
	InitSprite();

	//マネージャーの初期化
	InitManager();

	return S_OK;
}

/********************************************************
* 終了処理
********************************************************/

void Uninit(void)
{
	//マネージャー終了処理
	UninitManager();

	//スプライト終了処理
	UninitSprite();

	//サウンドの終了処理
	UninitAudio();

	//ライト終了処理
	UninitLight();

	//テクスチャの終了処理
	TextureFinalize();

	//DirectXの終了処理
	UninitRenderer();
}

/********************************************************
* 更新処理
********************************************************/

void Update(void)
{
	//マネージャー更新処理
	UpdateManager();
}

/********************************************************
* 描画処理
********************************************************/

void Draw(void)
{
	//バックバッファのクリア
	Clear();
	
	//マトリクス設定
	SetWorldViewProjection2D();

	//マネージャー表示処理
	DrawManager();

	//バックバッファをフロントバッファへコピー
	Present();
}
