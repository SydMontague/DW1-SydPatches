#pragma once

#include "dw1.hpp"
#include "stddef.hpp"

extern "C"
{
    int32_t MURD_tick(PartnerEntity* partner, bool isInitialized);
}