#pragma once
#include "extern/dtl/types.hpp"
#include "extern/dw1.hpp"

void initializeParticleFX();
extern "C"
{
    void createParticleFX(uint32_t color, int32_t mode, SVector* position, Entity* entity, int16_t cloudDelay);
}