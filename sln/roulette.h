#pragma once
#include "main.h"

class Roulette
{
private:
	XMFLOAT3 m_RoulettePos	= { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 , 0.0f};
	XMFLOAT3 m_ArrowPos		= { SCREEN_WIDTH / 2, 135.0f , 0.0f};
	int m_TextureId[2]{};
	XMFLOAT2 m_RouletteSize = { 450.0f, 450.0f };
	XMFLOAT2 m_ArrowSize	= { 90.0f, 90.0f };
	XMFLOAT4 m_Color		= { 1.0f, 1.0f, 1.0f, 1.0f };
	float m_Radian = 0.0f;

public:
	Roulette() = default;
	~Roulette() {}

	void InitRoulette();
	void RotationRoulette();
	void DrawRoulette();

	void SetRadian(float radian) {
		m_Radian = radian;
	}

	float GetRadian(){
		return fabsf(m_Radian); //äpìxÇÃê‚ëŒílÇï‘Ç∑
	}
};