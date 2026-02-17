#ifndef TRAIL_H
#define TRAIL_H

#include "renderer.h"

void TrailInitialize(void);
void TrailFinalize(void);
void TrailUpdate(void);
void TrailDraw(void);

void SetTrailPosition(const XMVECTOR Position);
void SetTrailCameraPosition(const XMVECTOR& position);

#endif //TRAIL_H