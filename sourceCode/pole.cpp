#include "pole.h"
#include "light.h"

void Pole::InitPole()
{
	m_pModel = ModelLoad("asset/model/pole.fbx", true);
}

void Pole::UninitPole()
{
	ModelRelease(m_pModel);
}

void Pole::DrawPole()
{
	XMMATRIX scale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	XMMATRIX translation = XMMatrixTranslation(0.5f, -0.5f, 36.5f);

	XMMATRIX world = scale * translation;

	SetWorldMatrix(world);
	SetLight(world);
	ModelDraw(m_pModel);
}
