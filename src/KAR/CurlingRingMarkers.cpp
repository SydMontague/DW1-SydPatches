#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../Map.hpp"

namespace
{
    struct CurlingRingObject
    {
        int16_t start;
        int16_t count;
    };

    constexpr dtl::array<CurlingRingObject, 4> ringObjects = {{{25, 1}, {19, 4}, {16, 3}, {13, 3}}};
} // namespace

extern "C"
{
    [[gnu::optimize("Os")]]
    void KAR_updateRingMarkers()
    {
        uint8_t activeRings = 0;

        if (KAR_MATCH_STATE == 11) {
            for (int32_t player = 0; player < 2; player++) {
                for (const auto& stone : KAR_STONE_ROWS[player].stones) {
                    if (stone.state <= 0 || stone.speed <= 0) continue;

                    for (int32_t ring = 0; ring < 4; ring++) {
                        if (activeRings & (1 << ring)) continue;
                        const auto& zone = KAR_RING_ZONES[ring];
                        if (KAR_distance(stone.position.x + zone.dx, stone.position.z + zone.dz) < zone.radius) {
                            activeRings |= 1 << ring;
                            break;
                        }
                    }
                }
            }
        }
        else if (KAR_MATCH_STATE == 13) {
            const auto& stone  = KAR_getTallyStone();
            const int32_t ring = stone.ring & 0x0F;
            if (stone.ring != stone.previousRing && stone.state < 101 && ring != 0) activeRings |= 1 << (ring - 1);
        }

        for (int32_t ring = 0; ring < 4; ring++) {
            const auto& object = ringObjects[ring];
            setMapObjectsFlag(object.start, object.count, !(activeRings & (1 << ring)));
        }
    }
}
