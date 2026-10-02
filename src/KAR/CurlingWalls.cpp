#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../Math.hpp"

namespace
{
    constexpr bool hasValidCoordinates(const Vector& point)
    {
        return point.x >= -32768 && point.x <= 32767 && point.z >= -32768 && point.z <= 32767;
    }
} // namespace

extern "C"
{
    [[gnu::optimize("Os")]]
    bool KAR_findWallContact(Vector* out, const CurlingStone* stone, int32_t direction)
    {
        // The wall classifier takes signed-16 coordinates. Reject corrupt full-width
        // positions before doing arithmetic; the caller repairs their court position.
        if (!hasValidCoordinates(stone->position) || !hasValidCoordinates(stone->target)) {
            *out = {};
            return false;
        }

        const int32_t reverseAngle  = ((stone->angle & 0xFFF) + 2048) & 0xFFF;
        const int32_t diagonalSlope = direction == 0 ? 2 : -2;
        const int32_t cosine        = libgte_rcos(reverseAngle);
        const int32_t sine          = libgte_rsin(reverseAngle);
        const int32_t side          = stone->position.z + 3525 + diagonalSlope * stone->position.x;
        const int32_t normal        = sine + diagonalSlope * cosine;
        // A normal tick retraces its target within Manhattan displacement + 2:
        // |sin| + |cos| >= 4096, with less than one unit lost per component.
        const int32_t last = abs(stone->position.x - stone->target.x) + abs(stone->position.z - stone->target.z) + 2;
        // Separate truncations perturb the continuous side by less than 3.
        // Near the boundary, even a reverse ray pointing outward can hit.
        if (normal < 0 && side <= -3) {
            *out = {};
            return false;
        }
        for (int32_t i = 0; i <= last; i++) {
            const auto x = stone->position.x + i * cosine / 4096;
            const auto z = stone->position.z + i * sine / 4096;
            if (z + 3525 + diagonalSlope * x > 0) {
                *out = {.x = x, .z = z};
                return true;
            }
        }
        *out = {};
        return false;
    }

    [[gnu::optimize("Os")]]
    void KAR_reflectOffDiagonal(CurlingStone* stone, int32_t direction)
    {
        const int32_t reverseAngle = ((stone->angle & 0xFFF) + 2048) & 0xFFF;
        const int32_t wallAngle    = libgte_ratan2(1, direction == 0 ? 2 : -2);
        stone->angle               = wallAngle * 2 - reverseAngle;
    }

    [[gnu::optimize("Os")]]
    void KAR_placeAtContact(CurlingStone* stone, Vector contact)
    {
        const int32_t contactDistance = KAR_distance(contact.x - stone->target.x, contact.z - stone->target.z);
        const int32_t remaining =
            KAR_distance(stone->target.z - stone->position.z, stone->target.x - stone->position.x) - contactDistance;

        stone->position.x = contact.x + remaining * libgte_rcos(stone->angle) / 4096;
        stone->position.z = contact.z + remaining * libgte_rsin(stone->angle) / 4096;
    }
}
