#pragma once

#include "dw1.hpp"
#include "stddef.hpp"

extern "C"
{
    int32_t DoOA_tick(PartnerEntity* partner, void* data, bool isInitialized);
    int32_t DoOA_getSequenceState(int32_t val1, int32_t isInitialized);
}