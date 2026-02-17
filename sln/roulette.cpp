#include "roulette.h"
#include "texture.h"
#include "sprite.h"

void Roulette::InitRoulette()
{
	m_TextureId[0] = TextureLoad(L"asset/texture/PowerRoulette.png");
	m_TextureId[1] = TextureLoad(L"asset/texture/RouletteArrow.png");
}

void Roulette::RotationRoulette()
{
	m_Radian += -0.08f;

	if (m_Radian <= -6.283184)
	{
		m_Radian = 0.0f;
	}
}

void Roulette::DrawRoulette()
{
	SetUVMatrix(XMMatrixIdentity());

	//2D•`‰æ‚È‚Ì‚Å‰œsˆ—‚ðOFF
	SetDepthEnable(false);

	SetPixelShader2d();

	SetTexture(m_TextureId[0]);

	DrawSpriteRotate(m_RoulettePos, m_RouletteSize, m_Color, m_Radian);

	SetTexture(m_TextureId[1]);

	DrawSprite(m_ArrowPos, m_ArrowSize, m_Color);
}
