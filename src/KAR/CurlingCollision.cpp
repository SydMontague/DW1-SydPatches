#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../Math.hpp"

extern "C"
{
    int32_t KAR_computeImpactShare(const CurlingStone* stone, Vector a, Vector b)
    {
        const int32_t currentAngle = stone->angle & 0xFFF;
        const int32_t impactAngle  = libgte_ratan2(b.z - a.z, b.x - a.x) & 0xFFF;
        int32_t difference         = (currentAngle - impactAngle) & 0xFFF;

        if (difference >= 1024 && difference <= 3072) return 0;
        if (difference > 3072) difference = (4096 - difference) & 0xFFF;

        return (1024 - difference % 1024) * stone->speed / 1024;
    }

    void KAR_computeSeparation(Vector* out, const CurlingStone* stone, Vector a, Vector b)
    {
        const int32_t currentAngle = stone->angle & 0xFFF;
        const int32_t impactAngle  = libgte_ratan2(b.z - a.z, b.x - a.x) & 0xFFF;
        const int32_t difference   = abs(currentAngle - impactAngle);

        if (difference >= 1024 && difference <= 3072) {
            *out = {};
            return;
        }

        // Unlike impact share, separation does not fold the difference across zero.
        const int32_t travel = KAR_distance(stone->position.z - a.z, stone->position.x - a.x);
        const int32_t radius = (1024 - difference % 1024) * travel / 1024;
        for (int32_t i = radius;; i++) {
            const auto x = i * libgte_rcos(impactAngle) / 4096;
            const auto z = i * libgte_rsin(impactAngle) / 4096;
            if (KAR_distance(x, z) != 0) {
                *out = {.x = x, .z = z};
                return;
            }
        }
    }
}
