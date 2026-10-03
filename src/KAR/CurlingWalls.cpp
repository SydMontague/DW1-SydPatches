#include "Curling.hpp"
#include "CurlingTypes.hpp"

extern "C"
{
    void KAR_findWallContact(Vector* out, const CurlingStone* stone, int32_t direction)
    {
        const int32_t reverseAngle  = (stone->angle + 2048) & 0xFFF;
        const int32_t diagonalSlope = direction == 0 ? 2 : -2;
        int32_t step                = 0;
        int32_t x;
        int32_t z;
        int32_t side;
        do {
            x = stone->position.x + KAR_multiplyLow(step, libgte_rcos(reverseAngle)) / 4096;
            z = stone->position.z + KAR_multiplyLow(step, libgte_rsin(reverseAngle)) / 4096;
            step++;
            const uint32_t diagonal = static_cast<uint32_t>(KAR_multiplyLow(x, diagonalSlope));
            // Preserve the low-word result of vanilla's add/subtract even when doubled x is INT_MIN.
            side = static_cast<int32_t>(static_cast<uint32_t>(z + 3525) + diagonal);
        } while (side <= 0);

        // The reverse ray must cross this half-plane; retain vanilla's uncapped search.
        // Vanilla's uninitialized y/pad copies are not observed by the known callers.
        out->x = x;
        out->z = z;
    }

    void KAR_reflectOffDiagonal(CurlingStone* stone, int32_t direction)
    {
        const int32_t reverseAngle = (stone->angle + 2048) & 0xFFF;
        const int32_t wallAngle    = libgte_ratan2(1, direction == 0 ? 2 : -2);
        stone->angle              = KAR_multiplyLow(wallAngle, 2) - reverseAngle;
    }

    void KAR_placeAtContact(CurlingStone* stone, Vector contact)
    {
        const int32_t contactDistance = KAR_distance(contact.x - stone->target.x, contact.z - stone->target.z);
        const int32_t remaining       =
            KAR_distance(stone->target.z - stone->position.z, stone->target.x - stone->position.x) - contactDistance;

        stone->position.x = contact.x + KAR_multiplyLow(remaining, libgte_rcos(stone->angle)) / 4096;
        stone->position.z = contact.z + KAR_multiplyLow(remaining, libgte_rsin(stone->angle)) / 4096;
    }
}
