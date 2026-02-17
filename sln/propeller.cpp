#include "propeller.h"
#include "model.h"
#include "light.h"

void Propeller::InitPropeller()
{
	m_pModel = ModelLoad("asset/model/propeller.fbx", true);
}

void Propeller::UninitPropeller()
{
	ModelRelease(m_pModel);
}

void Propeller::UpdatePropeller()
{
	++m_Angle;

	if (m_Angle >= 360.0f) {
		m_Angle = 0.0f;
	}
}

void Propeller::DrawPropeller()
{
	XMMATRIX scale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	XMMATRIX RotationZ = XMMatrixRotationZ(XMConvertToRadians(m_Angle));
	XMMATRIX RotationY = XMMatrixRotationY(XMConvertToRadians(270.0f));
	XMMATRIX translation = XMMatrixTranslation(6.0f, 2.5f, 10.0f);

	XMMATRIX world = scale * RotationZ * RotationY * translation;

	SetWorldMatrix(world);
	SetLight(world);
	ModelDraw(m_pModel);
}
