#pragma once

#include "dw1.hpp"
#include "stddef.hpp"

extern "C"
{
    int32_t DoOA_tick(PartnerEntity* partner, void* data, bool isInitialized);
}