#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../Sound.hpp"
#include "../extern/dtl/algorithm.hpp"

namespace
{
    [[gnu::optimize("Os")]]
    bool clampToCourt(Vector& point)
    {
        const auto z       = dtl::clamp(point.z, -2524, 1124);
        const auto limit   = z >= -2000 ? 724 : (z + 3524) / 2;
        const auto x       = dtl::clamp(point.x, -limit, limit);
        const bool changed = point.x != x || point.z != z;
        point.x            = x;
        point.z            = z;
        return changed;
    }

    [[gnu::optimize("Os")]]
    void bounceOffDiagonal(CurlingStone& stone, int32_t direction)
    {
        const auto incomingAngle  = stone.angle;
        const auto slope          = direction == 0 ? 2 : -2;
        const auto incomingNormal = libgte_rsin(incomingAngle) + slope * libgte_rcos(incomingAngle);
        Vector contact;
        if (KAR_findWallContact(&contact, &stone, direction)) {
            KAR_reflectOffDiagonal(&stone, direction);
            KAR_placeAtContact(&stone, contact);
        }
        else if (incomingNormal < 0) {
            KAR_reflectOffDiagonal(&stone, direction);
        }

        if (clampToCourt(stone.position)) {
            if (incomingNormal >= 0)
                stone.angle = incomingAngle;
            else if (libgte_rsin(stone.angle) + slope * libgte_rcos(stone.angle) < 0)
                // At the quantized tangent, vanilla's rounded reflection can
                // still point outside. One angle unit is the first inward angle.
                stone.angle += direction == 0 ? 1 : -1;
        }
        clampToCourt(stone.target);
    }
} // namespace

extern "C" [[gnu::optimize("Os")]]
void KAR_bounceOffWall()
{
    for (auto& row : KAR_STONE_ROWS) {
        for (auto& stone : row.stones) {
            if (stone.state <= 0) continue;
            if (stone.speed == 0) {
                // The front edge intentionally admits stones that are later
                // removed by checkStonesStopped. Recover only the closed court.
                if (stone.position.z < 1125) {
                    clampToCourt(stone.position);
                    clampToCourt(stone.target);
                }
                continue;
            }
            if (stone.position.x < -32768 || stone.position.x > 32767 || stone.position.z < -32768 ||
                stone.position.z > 32767) {
                clampToCourt(stone.position);
                clampToCourt(stone.target);
                continue;
            }
            const auto zone = KAR_getWallZone(stone.position.x, stone.position.z);
            if (zone > CurlingWallZone::NONE && zone < CurlingWallZone::POSITIVE_Z_WALL) playSound2(8, 3);
            switch (zone) {
                case CurlingWallZone::NEGATIVE_X_WALL:
                    stone.position.x -= static_cast<int16_t>(stone.position.x + 725) * 2;
                    stone.angle = 2048 - stone.angle;
                    break;
                case CurlingWallZone::POSITIVE_X_WALL:
                    stone.angle = 6144 - stone.angle;
                    stone.position.x -= static_cast<int16_t>(stone.position.x - 725) * 2;
                    break;
                case CurlingWallZone::NEGATIVE_X_DIAGONAL: bounceOffDiagonal(stone, 0); break;
                case CurlingWallZone::POSITIVE_X_DIAGONAL: bounceOffDiagonal(stone, 1); break;
                case CurlingWallZone::NEGATIVE_Z_WALL:
                    stone.angle = -stone.angle;
                    stone.position.z += static_cast<int16_t>(-2525 - stone.position.z) * 2;
                    break;
                case CurlingWallZone::POSITIVE_Z_WALL:
                    if (stone.target.z < 1125) {
                        playSound2(8, 3);
                        stone.angle = -stone.angle;
                        stone.position.z -= static_cast<int16_t>(stone.position.z - 1125) * 2;
                    }
                    break;
                case CurlingWallZone::NONE: break;
            }
        }
    }
}
