#pragma once
#include "model.h"

class Propeller
{
private:
	MODEL* m_pModel = nullptr;
	float m_Angle = 0.0f;

public:
	Propeller() = default;
	~Propeller() {}

	void InitPropeller();
	void UninitPropeller();
	void UpdatePropeller();
	void DrawPropeller();
};