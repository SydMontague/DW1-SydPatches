#include "Curling.hpp"
#include "CurlingTypes.hpp"

extern "C"
{
    void KAR_rotatePoint(SVector* point, int32_t angle)
    {
        const int32_t x = point->x;
        const int32_t z = point->z;

        // Each product truncates toward zero separately before the signed-16 narrowing.
        point->x = static_cast<int16_t>(KAR_multiplyLow(x, libgte_rcos(angle)) / 4096 -
                                        KAR_multiplyLow(z, libgte_rsin(angle)) / 4096);
        point->z = static_cast<int16_t>(KAR_multiplyLow(x, libgte_rsin(angle)) / 4096 +
                                        KAR_multiplyLow(z, libgte_rcos(angle)) / 4096);
    }

    int32_t KAR_distance(int32_t x, int32_t z)
    {
        const uint32_t squaredX = static_cast<uint32_t>(x) * static_cast<uint32_t>(x);
        const uint32_t squaredZ = static_cast<uint32_t>(z) * static_cast<uint32_t>(z);
        return libgte_SquareRoot0(static_cast<int32_t>(squaredX + squaredZ));
    }
}
