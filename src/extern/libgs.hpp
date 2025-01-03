#pragma once

#include "stddef.hpp"

extern "C"
{
    struct GsOT
    {
        uint32_t length;
        uint32_t* origin;
        uint32_t offset;
        uint32_t point;
        uint32_t* current;
    };

    extern void* libgs_GsGetWorkBase();
    extern void libgs_GsSetWorkBase(void* newBase);
}

static_assert(sizeof(GsOT) == 20);