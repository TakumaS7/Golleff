#include "ball_camera.h"
#include "keyboard.h"
#include "ball.h"

void BallCamera::Update()
{
	if (Keyboard_IsKeyDown(KK_Q))
	{
		m_Angle += 0.03f;
	}

	XMVECTOR position = { 0.0f, m_PositionHight, m_Length };
	XMMATRIX translation = XMMatrixTranslationFromVector(m_Target);
	XMMATRIX rotation = XMMatrixRotationY(m_Angle);

	position = XMVector3TransformCoord(position, rotation * translation);

	XMMATRIX view = XMMatrixLookAtLH(position, m_Target, {0.0f, 0.1f, 0.0f});
	XMMATRIX perspective = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.01f, 1000.0f);
	SetPosition(m_Target);
	SetViewMatrix(view);
	SetPerspectiveMatrix(perspective);
}
