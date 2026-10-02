#include "Curling.hpp"
#include "CurlingTypes.hpp"

extern "C"
{
    int32_t KAR_computeImpactShare(const CurlingStone* stone, Vector a, Vector b)
    {
        const int32_t currentAngle = stone->angle & 0xFFF;
        const int32_t impactAngle  = libgte_ratan2(b.z - a.z, b.x - a.x) & 0xFFF;
        int32_t difference        = (currentAngle - impactAngle) & 0xFFF;

        if (difference >= 1024 && difference <= 3072) return 0;
        if (difference > 3072) difference = (4096 - difference) & 0xFFF;

        return KAR_multiplyLow(1024 - difference % 1024, stone->speed) / 1024;
    }

    void KAR_computeSeparation(Vector* out, const CurlingStone* stone, Vector a, Vector b)
    {
        const int32_t currentAngle = stone->angle & 0xFFF;
        const int32_t impactAngle  = libgte_ratan2(b.z - a.z, b.x - a.x) & 0xFFF;
        int32_t difference        = currentAngle - impactAngle;
        if (difference < 0) difference = -difference;

        int32_t x = 0;
        int32_t z = 0;
        if (difference < 1024 || difference > 3072) {
            // Unlike impact share, separation does not fold the difference across zero.
            const int32_t travel = KAR_distance(stone->position.z - a.z, stone->position.x - a.x);
            int32_t radius       = KAR_multiplyLow(1024 - difference % 1024, travel) / 1024;
            int32_t length;
            do {
                x      = KAR_multiplyLow(radius, libgte_rcos(impactAngle)) / 4096;
                z      = KAR_multiplyLow(radius, libgte_rsin(impactAngle)) / 4096;
                length = KAR_distance(x, z);
                radius++;
            } while (length == 0);
        }

        // Vanilla copies an uninitialized pad word; known callers only consume x/z.
        out->x = x;
        out->y = 0;
        out->z = z;
    }
}
