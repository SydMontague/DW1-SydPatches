#pragma once
#include "extern/libgs.hpp"

extern "C"
{
    void calculatePosition(GsCOORDINATE2* coord, Matrix* matrix);
    int32_t getDistance(int32_t diffX, int32_t diffY, int32_t diffZ);
}
