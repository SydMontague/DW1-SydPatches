#pragma once
#include "extern/dw1.hpp"

extern "C"
{
    void createCloudFX(SVector* position);
    void addHealingParticleEffect(Entity* entity, int32_t param);
    void getDrawingOffsetCopy(int32_t* offsetX, int32_t* offsetY);
    void addScreenPolyFT4(POLY_FT4* prim, SVector* vec1, SVector* vec2, SVector* vec3, SVector* vec4);
    void renderParticleFlash(ParticleFlashData* data);
}