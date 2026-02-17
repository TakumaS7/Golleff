#include "ball_shot.h"
#include "keyboard.h"
#include "texture.h"
#include "model.h"
#include "light.h"

static Ball* g_pBall = nullptr;
static XMVECTOR g_Velocity = { 0.0f, 0.0f, 0.0f };
static 	XMVECTOR g_Direction;
static XMMATRIX g_Matrix = XMMatrixIdentity();
MODEL* g_pModel = nullptr;
static float g_XAngle = 0.0f;
static float g_YAngle = 0.0f;

void InitBallShot(void)
{
	g_pModel = ::ModelLoad("asset/model/arrow.fbx", true); //ModelLoadがかぶっているため、::を付ける
}

void UninitBallShot(void)
{
	ModelRelease(g_pModel);
}

void SetBallShot(Ball* pBall)
{
	g_pBall = pBall;
}

void SetBallShotMatrix(const XMMATRIX& matrix)
{
	g_Matrix = matrix;
}

void UpdateBallShot(void)
{
	//if (Keyboard_IsKeyDown(KK_I))
	//{
	//	g_XAngle -= 0.05f;
	//}
	//else if (Keyboard_IsKeyDown(KK_K))
	//{
	//	g_XAngle += 0.05f;
	//}
	//else if (Keyboard_IsKeyDown(KK_L))
	//{
	//	g_YAngle += 0.05f;
	//}
	//else if (Keyboard_IsKeyDown(KK_J))
	//{
	//	g_YAngle -= 0.05f;
	//}

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, g_Matrix);

	matrix._41 = matrix._42 = matrix._43 = 0.0f; //平行移動成分をカット
	g_Matrix = XMMatrixRotationRollPitchYaw(g_XAngle, g_YAngle, 0.0f);

	g_Direction = XMVector3TransformNormal({ 0.0f, 0.0f, 1.0f }, g_Matrix);
	g_Velocity = XMVector3Normalize(g_Direction) * 0.08f; //打つ力のベクトル
}

void DrawBallShot(void)
{
	//モデル描画
	XMMATRIX t1 = XMMatrixTranslationFromVector(g_pBall->GetPosition());
	XMMATRIX t0 = XMMatrixTranslation(0.0f, 0.0f, 0.3f);
	XMMATRIX s = XMMatrixScaling(0.2f, 0.2f, 0.2f);

	XMMATRIX world = s * t0 * g_Matrix * t1;

	SetWorldMatrix(world);
	SetLight(world);
	ModelDraw(g_pModel);
}

const XMVECTOR& GetBallShotVelocity()
{
	return g_Velocity;
}

void XAngleRotation()
{
	g_XAngle -= 0.09f; //上下
}

void YAngleRotation()
{
	g_YAngle += 0.09f; //左右
}

void SetXYAngle(float XAngle, float YAngle)
{
	g_XAngle = XAngle;
	g_YAngle = YAngle;
}
