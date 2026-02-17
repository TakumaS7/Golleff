#pragma once

#include "main.h"

void InitBillboard(void);
void UninitBillboard(void);
void DrawBillboard(int textureId, const XMFLOAT2& offset, const XMVECTOR& position, const XMFLOAT2& scale, float angle, float tx, float ty, float tw, float th);
void CalculateBillboardMatrix(const XMMATRIX& mtxView);
