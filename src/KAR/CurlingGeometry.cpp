#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../Matrix.hpp"

extern "C"
{
    void KAR_rotatePoint(SVector* point, int32_t angle)
    {
        const int32_t x = point->x;
        const int32_t z = point->z;

        // Each product truncates toward zero separately before the signed-16 narrowing.
        point->x = static_cast<int16_t>(x * libgte_rcos(angle) / 4096 - z * libgte_rsin(angle) / 4096);
        point->z = static_cast<int16_t>(x * libgte_rsin(angle) / 4096 + z * libgte_rcos(angle) / 4096);
    }

    int32_t KAR_distance(int32_t diffX, int32_t diffZ)
    {
        return getDistance(diffX, 0, diffZ);
    }
}
