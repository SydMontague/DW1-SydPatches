#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../Sound.hpp"
#include "../extern/libgs.hpp"
#include "../extern/libgte.hpp"

extern "C"
{
    [[gnu::optimize("Os")]]
    void KAR_tickStones(int32_t instanceId)
    {
        for (int32_t player = 0; player < 3; player++) {
            for (int32_t index = 0; index < 5; index++) {
                auto& stone = KAR_STONE_ROWS[player].stones[index];
                if (stone.state != 3 && stone.state != 2) stone.target = stone.position;

                if (KAR_MATCH_STATE == 5) {
                    if (index == KAR_SELECTED_STONE && player == KAR_CURRENT_PLAYER)
                        stone.selectionPhase = static_cast<int16_t>(stone.selectionPhase + 20);
                    else
                        stone.selectionPhase = 0;
                }

                if (stone.state <= 0 || stone.speed < 0) continue;

                // Travel uses the incoming speed; decay affects the next tick.
                const int16_t travel = static_cast<int16_t>(stone.speed / stone.weight);
                stone.speed          = static_cast<int16_t>(stone.speed * 98 / 100);
                if (travel < 2) stone.speed = 0;

                if (stone.state < 3) {
                    stone.position.x += travel * libgte_rcos(stone.angle) / 4096;
                    stone.position.z += travel * libgte_rsin(stone.angle) / 4096;
                }
                if (stone.state == 3) {
                    if (stone.speed > 0) {
                        const int16_t step = travel / 10;
                        stone.angle -= 2048;
                        if (stone.angle < 0) stone.angle += 4096;
                        stone.position.x = stone.target.x + step * libgte_rcos(stone.angle) / 4096;
                        stone.position.z = stone.target.z + step * libgte_rsin(stone.angle) / 4096;
                        // Vanilla changes state on the later zero-speed branch, not here.
                        if (step == 0) stone.speed = 0;
                    }
                    else {
                        stone.speed = 0;
                        stone.state = 2;
                    }
                }
                if (stone.state == 1 && travel <= 0 && stone.type == 3) {
                    stone.state = 2;
                    libgs_GsLinkObject4(KAR_MODEL_DATA + 3, &stone.object, KAR_PEGGED_MODEL_IDS[player]);
                    playSound2(8, 4);
                }
            }
        }
        KAR_updateCollisions();
    }
}
