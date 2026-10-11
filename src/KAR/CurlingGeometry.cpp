#include "Curling.hpp"

#include "../Matrix.hpp"

int32_t KAR_distance(int32_t diffX, int32_t diffZ)
{
    return getDistance(diffX, 0, diffZ);
}

CurlingWallZone KAR_getWallZone(int16_t x, int16_t z)
{
    const int32_t posX = x;
    const int32_t posZ = z;

    if (posZ >= 1125) return CurlingWallZone::POSITIVE_Z_WALL;

    if (posZ >= -2000) {
        if (posX < -724) return CurlingWallZone::NEGATIVE_X_WALL;
        if (posX >= 725) return CurlingWallZone::POSITIVE_X_WALL;
    }
    else {
        const int32_t doubledX = posX * 2;

        // Both X conditions include 425; vanilla tests the first diagonal before the second.
        if (posZ + doubledX < -3524 && posX < 426) return CurlingWallZone::NEGATIVE_X_DIAGONAL;
        if (posZ - doubledX < -3524 && posX >= 425) return CurlingWallZone::POSITIVE_X_DIAGONAL;
        if (posZ < -2524) return CurlingWallZone::NEGATIVE_Z_WALL;
    }

    return CurlingWallZone::NONE;
}
