#include "move_camera.h"
#include "keyboard.h"

void MovingCamera::Update()
{
	//è„à⁄ìÆ
	if (Keyboard_IsKeyDown(KK_E))
	{
		SetPosition(GetPosition() + m_VecUp * 0.1f);
	}

	//â∫à⁄ìÆ
	if (Keyboard_IsKeyDown(KK_R))
	{
		SetPosition(GetPosition() + m_VecDown * 0.1f);
	}

	//ëOà⁄ìÆ
	if (Keyboard_IsKeyDown(KK_W))
	{
		//XMFLOAT3 vec;
		//XMStoreFloat3(&vec, m_VecFront); //XMFLOAT3 <- XMVECTOR
		//vec.y = 0.0f;
		//XMVECTOR v = XMVector3Normalize(XMLoadFloat3(&vec)); //XMFLOAT3 -> XMVECTOR

		XMVECTOR v = XMVector3Normalize(XMVectorSetY(m_VecFront, 0.0f));

		SetPosition(GetPosition() + v * 0.1f);
	}

	//å„ÇÎà⁄ìÆ
	if (Keyboard_IsKeyDown(KK_S))
	{
		//XMFLOAT3 vec;
		//XMStoreFloat3(&vec, m_VecFront); //XMFLOAT3 <- XMVECTOR
		//vec.y = 0.0f;
		//XMVECTOR v = XMVector3Normalize(XMLoadFloat3(&vec)); //XMFLOAT3 -> XMVECTOR

		XMVECTOR v = XMVector3Normalize(XMVectorSetY(m_VecFront, 0.0f));

		SetPosition(GetPosition() + v * -0.1f);
	}

	//âEà⁄ìÆ
	if (Keyboard_IsKeyDown(KK_D))
	{
		SetPosition(GetPosition() + m_VecRight * 0.1f);
	}

	//ç∂à⁄ìÆ
	if (Keyboard_IsKeyDown(KK_A))
	{
		SetPosition(GetPosition() + m_VecRight * -0.1f);
	}

	//è„âÒì]
	if (Keyboard_IsKeyDown(KK_UP))
	{
		XMMATRIX rotation = XMMatrixRotationAxis(m_VecRight, XMConvertToRadians(-1));
		m_VecFront = XMVector3TransformNormal(m_VecFront, rotation);
		m_VecRight = XMVector3Cross(m_VecUp, m_VecFront);
	}
	
	//â∫âÒì]
	if (Keyboard_IsKeyDown(KK_DOWN))
	{
		XMMATRIX rotation = XMMatrixRotationAxis(m_VecRight, XMConvertToRadians(1)); //m_VecRightÇé≤Ç…1ìxÇ∏Ç¬âÒÇÈ
		m_VecFront = XMVector3TransformNormal(m_VecFront, rotation);
		m_VecRight = XMVector3Cross(m_VecUp, m_VecFront);
	}

	//âEâÒì]
	if (Keyboard_IsKeyDown(KK_RIGHT))
	{
		XMMATRIX rotation = XMMatrixRotationY(XMConvertToRadians(1));
		m_VecFront = XMVector3TransformNormal(m_VecFront, rotation);
		m_VecRight = XMVector3Cross(m_VecUp, m_VecFront);
	}

	//ç∂âÒì]
	if (Keyboard_IsKeyDown(KK_LEFT))
	{
		XMMATRIX rotation = XMMatrixRotationY(XMConvertToRadians(-1));
		m_VecFront = XMVector3TransformNormal(m_VecFront, rotation);
		m_VecRight = XMVector3Cross(m_VecUp, m_VecFront);
	}

	m_VecFront = XMVector3Normalize(m_VecFront);
	m_VecRight = XMVector3Normalize(m_VecRight);
	m_VecUp = XMVector3Normalize(m_VecUp);
	m_VecDown = XMVector3Normalize(m_VecDown);

	XMMATRIX view = XMMatrixLookAtLH(GetPosition(), GetPosition() + m_VecFront, m_VecUp);
	XMMATRIX perspective = XMMatrixPerspectiveFovLH(XMConvertToRadians(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.01f, 1000.0f);

	SetViewMatrix(view);
	SetPerspectiveMatrix(perspective);
}
