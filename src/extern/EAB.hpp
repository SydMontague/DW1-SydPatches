#pragma once

#include "dw1.hpp"
#include "stddef.hpp"

extern "C"
{
    int32_t EaB_tick(Entity* partner, bool isInitialized);
    void EAB_startBuildup(Entity* entity);

    extern uint8_t EAB_MODEL_BUFFER;
}
