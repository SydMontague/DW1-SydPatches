#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../extern/dtl/array.hpp"
#include "../extern/dw1.hpp"
#include "../extern/libc.hpp"
#include "../extern/libgte.hpp"

namespace
{
    constexpr dtl::array<dtl::array<int32_t, 4>, 2> SHOT_PRIORITIES = {{{1, 0, 2, 3}, {3, 2, 0, 1}}};

    [[gnu::optimize("Os")]]
    int32_t findUnusedStoneOfType(int32_t type)
    {
        for (int32_t index = 0; index < 5; index++) {
            const auto& stone = KAR_STONE_ROWS[1].stones[index];
            if (stone.state == -1 && stone.type == type) return index;
        }
        return -1;
    }

    [[gnu::optimize("Os")]]
    int32_t computeThrowPower(int32_t weight, int32_t x, int32_t z)
    {
        const uint32_t distance = static_cast<uint32_t>(KAR_distance(400 - x, 1400 - z));
        uint32_t accumulated    = 0;
        uint32_t current        = static_cast<uint32_t>(weight);
        uint32_t value          = static_cast<uint32_t>(weight) * 100;
        while (accumulated < distance) {
            accumulated += current / static_cast<uint32_t>(weight);
            value   = value * 100 / 98;
            current = value / 100;
        }
        current -= KAR_OPPONENT_ENTITY == 2 ? 600 : 500;
        return static_cast<int32_t>(current);
    }

} // namespace

[[gnu::optimize("Os")]]
int32_t KAR_findClearShotAngle(int32_t x, int32_t z)
{
    const auto* position = ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY)->posData;
    int16_t currentX     = static_cast<int16_t>(position->location.x);
    int16_t currentZ     = static_cast<int16_t>(position->location.z);
    if (KAR_OPPONENT_ENTITY != 2) {
        currentX = static_cast<int16_t>(position[4].posMatrix.work.t[0]);
        currentZ = static_cast<int16_t>(position[4].posMatrix.work.t[2]);
    }
    const int32_t angle = libgte_ratan2(z - currentZ, x - currentX);
    if (z >= currentZ) return angle;

    // The table-based trig functions have no persistent side effects; the scan's step is invariant.
    const int32_t step = KAR_OPPONENT_ENTITY == 2 ? 30 : 10;
    const int16_t dx   = static_cast<int16_t>(step * libgte_rcos(angle) / 4096);
    const int16_t dz   = static_cast<int16_t>(step * libgte_rsin(angle) / 4096);
    do {
        currentX = static_cast<int16_t>(currentX + dx);
        currentZ = static_cast<int16_t>(currentZ + dz);
        for (const auto& row : KAR_STONE_ROWS) {
            for (const auto& stone : row.stones) {
                if (stone.state > 0 && KAR_distance(currentX - stone.position.x, currentZ - stone.position.z) < 151) {
                    if (KAR_OPPONENT_ENTITY == 2) return -1;
                    return libgte_ratan2(stone.position.z - z, stone.position.x - x);
                }
            }
        }
    } while (z < currentZ);
    return angle;
}

[[gnu::optimize("Os")]]
int32_t KAR_aimAtStoneInRing(int32_t player, int32_t ring, int16_t* outX, int16_t* outZ)
{
    for (const auto& stone : KAR_STONE_ROWS[player].stones) {
        if (stone.state < 2 && stone.ring == ring) {
            const int32_t angle =
                KAR_findClearShotAngle(static_cast<int16_t>(stone.position.x), static_cast<int16_t>(stone.position.z));
            *outX = static_cast<int16_t>(stone.position.x);
            *outZ = static_cast<int16_t>(stone.position.z);
            return angle;
        }
    }
    return -1;
}

void KAR_setOpponentShot(int32_t priority, int32_t angle, int32_t x, int32_t z)
{
    for (const auto type : SHOT_PRIORITIES[priority]) {
        const int32_t index = findUnusedStoneOfType(static_cast<int8_t>(type));
        if (index < 0) continue;

        auto& row        = KAR_STONE_ROWS[1];
        row.plannedStone = static_cast<uint16_t>(index);
        auto& stone      = row.stones[index];
        stone.angle      = angle;
        row.shotPower    = static_cast<int16_t>(computeThrowPower(stone.weight, x, z));
        return;
    }
    // The match dispatcher excludes exhaustion; invalid direct calls leave the shot unchanged.
}

[[gnu::optimize("Os")]]
int32_t KAR_aimAtRandomStone()
{
    if (KAR_PREVIOUS_PLAYER_STONE.state < 0) return libgte_ratan2(-3100, -400);

    dtl::array<const CurlingStone*, 5> candidates;
    uint8_t count = 0;
    for (const auto& stone : KAR_STONE_ROWS[0].stones) {
        if (stone.state == 1) candidates[count++] = &stone;
    }

    int16_t x = 0;
    int16_t z = -1700;
    if (count != 0) {
        const uint8_t index = static_cast<uint8_t>(rand() % count);
        x                   = static_cast<int16_t>(candidates[index]->position.x);
        z                   = static_cast<int16_t>(candidates[index]->position.z);
    }
    return libgte_ratan2(z - 1400, x - 400);
}

int32_t KAR_aimBankShot(const CurlingStone* stone, int32_t x, int32_t z)
{
    const int32_t angle   = libgte_ratan2(stone->position.z - z, stone->position.x - x);
    int32_t offsetX       = stone->position.x + libgte_rcos(angle) * 150 / 4096;
    const int32_t offsetZ = stone->position.z + libgte_rsin(angle) * 150 / 4096;
    offsetX               = -725 - (offsetX + 725);
    const auto& origin    = ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY)->posData[4].posMatrix.work;
    return libgte_ratan2(offsetZ - origin.t[2], offsetX - origin.t[0]);
}
