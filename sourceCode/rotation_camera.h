#pragma once


#include "camera.h"

class RotationCamera : public Camera
{
private:
	XMVECTOR m_Target = {0.0f, 0.0f, 0.0f};
	float m_Length = 10.0f;
	float m_PositionHight = 5.0f;
	float m_Angle = 0.0f;

public:
	RotationCamera() = default;

	void SetTarget(const XMVECTOR& target) { m_Target = target; };

	void Update() override;
};