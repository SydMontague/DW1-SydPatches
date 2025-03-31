#pragma once
#include "extern/dw1.hpp"

extern "C"
{
    void createCloudFX(SVector* position);
    void addHealingParticleEffect(Entity* entity, int32_t param);
}