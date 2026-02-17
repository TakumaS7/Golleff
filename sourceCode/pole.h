#pragma once
#include "model.h"

class Pole
{
private:
	MODEL* m_pModel = nullptr;

public:
	Pole() = default;
	~Pole() {}

	void InitPole();
	void UninitPole();
	void DrawPole();
};