#pragma once
/*****************************************************************/
//
// 
// ball_camera.hiƒ{[ƒ‹ƒJƒƒ‰ˆ—j
//
//
//
//                    ‚Q‚O‚Q‚S”N‚P‚QŒ‚Q‚T“ú@»ìÒ–¼F²“¡‘ô–
//
/*****************************************************************/
#include "camera.h"

class BallCamera : public Camera
{
private:
	XMVECTOR m_Target = { 0.0f, 0.0f, 0.0f };
	float m_Length = -4.0f;
	float m_PositionHight = 2.0f;
	float m_Angle = 0.0f;

	XMVECTOR m_VecDown = { 0.0f, -1.0f, 0.0f };

public:
	BallCamera() = default;
	
	~BallCamera() {}

	void SetBall( const XMVECTOR target) { m_Target = target; }

	void Update() override;
};