#pragma once

#include "../extern/dtl/types.hpp"

enum class CurlingWallZone : int32_t
{
    NONE                = 0,
    NEGATIVE_X_WALL     = 1,
    POSITIVE_X_WALL     = 2,
    NEGATIVE_X_DIAGONAL = 3, // z + 2*x boundary; vanilla contact flag 0.
    POSITIVE_X_DIAGONAL = 4, // z - 2*x boundary; vanilla contact flag 1.
    NEGATIVE_Z_WALL     = 5,
    POSITIVE_Z_WALL     = 6, // Reflection also requires target.vz < 1125.
};

static_assert(sizeof(CurlingWallZone) == sizeof(int32_t));

extern "C"
{
    CurlingWallZone KAR_getWallZone(int16_t x, int16_t z);
}
