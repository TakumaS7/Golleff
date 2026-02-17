#pragma once
#include "ball.h"
#include "renderer.h"

void InitBallShot(void);
void UninitBallShot(void);

void SetBallShot(Ball* pBall);
void SetBallShotMatrix(const XMMATRIX& matrix);

void UpdateBallShot(void);
void DrawBallShot(void);

const XMVECTOR& GetBallShotVelocity();

void XAngleRotation();
void YAngleRotation();
void SetXYAngle(float XAngle, float YAngle);
