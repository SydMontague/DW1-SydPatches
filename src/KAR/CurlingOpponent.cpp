#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../extern/dw1.hpp"
#include "../extern/libc.hpp"
#include "../extern/libgte.hpp"

extern "C"
{
    void KAR_chooseOpponentShot()
    {
        auto& opponent = KAR_STONE_ROWS[1];
        if (KAR_OPPONENT_ENTITY == 2) {
            int32_t angle = KAR_findClearShotAngle(0, -1700);
            if (angle != -1) {
                KAR_setOpponentShot(1, angle, 0, -1700);
                return;
            }

            int16_t x = 0;
            int16_t z = 0;
            if (KAR_STONE_ROWS[0].score > opponent.score) {
                angle = KAR_aimAtStoneInRing(1, 0x14, &x, &z);
                if (angle != -1) {
                    KAR_setOpponentShot(0, angle, x, z);
                    return;
                }
            }
            angle = KAR_aimAtStoneInRing(0, 2, &x, &z);
            if (angle != -1) {
                KAR_setOpponentShot(0, angle, x, z);
                return;
            }
            angle = KAR_findClearShotAngle(-400, -2400);
            if (angle != -1) {
                // Vanilla uses the outer shot's angle with a center-X power target.
                KAR_setOpponentShot(1, angle, 0, -2400);
                return;
            }
            KAR_setOpponentShot(0, KAR_aimAtRandomStone(), 0, -2400);
            opponent.shotPower = 2400;
            return;
        }

        const auto& origin = ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY)->posData[4].posMatrix.work;
        const int16_t x    = static_cast<int16_t>(origin.t[0]);
        const int16_t z    = static_cast<int16_t>(origin.t[2]);
        if (opponent.thrown == 0) {
            int32_t angle = libgte_ratan2(-2300 - z, -400 - x);
            if (angle == KAR_findClearShotAngle(-400, -2300)) {
                KAR_setOpponentShot(1, angle, -400, -2300);
                return;
            }
            angle = libgte_ratan2(-1700 - z, -x);
            if (angle == KAR_findClearShotAngle(0, -1700)) {
                KAR_setOpponentShot(1, angle, 0, -1700);
                return;
            }
        }
        else {
            for (int32_t index = 0; index < 5; index++) {
                const auto& stone = opponent.stones[index];
                if (stone.state != -1) continue;
                if (stone.type == 3) {
                    // Keep the preselection even though successful priority selection overwrites it.
                    opponent.plannedStone = static_cast<uint16_t>(index);
                    const int32_t angle   = libgte_ratan2(-1700 - z, -x);
                    if (angle == KAR_findClearShotAngle(0, -1700))
                        KAR_setOpponentShot(1, angle, 0, -1700);
                    else
                        opponent.plannedStone = 6;
                    break;
                }
                opponent.plannedStone = 6;
            }
            if (opponent.plannedStone != 6) return;

            const auto& previous = KAR_PREVIOUS_PLAYER_STONE;
            switch (previous.type) {
                case 0:
                case 1:
                case 2:
                    if (previous.ring == 3 || previous.ring == 0) {
                        const int32_t angle = libgte_ratan2(-2300 - z, -400 - x);
                        // Vanilla still scans here; distance queries can change GTE scratch state.
                        KAR_findClearShotAngle(-400, -2300);
                        KAR_setOpponentShot(1, angle, -400, -2300);
                        return;
                    }
                    if (previous.state < 0) break;
                    if (rand() % 2 != 0) {
                        int32_t angle = libgte_ratan2(-2300 - z, -400 - x);
                        if (angle == KAR_findClearShotAngle(-400, -2300))
                            KAR_setOpponentShot(1, angle, -400, -2300);
                        else {
                            angle = libgte_ratan2(previous.position.z - z, previous.position.x - x);
                            KAR_setOpponentShot(0,
                                                angle,
                                                static_cast<int16_t>(previous.position.x),
                                                static_cast<int16_t>(previous.position.z));
                        }
                    }
                    else {
                        const int32_t angle = KAR_aimBankShot(&previous, 400, -2300);
                        KAR_setOpponentShot(0, angle, 400, -2300);
                        opponent.shotPower = 2400;
                    }
                    return;
                case 3: break;
                default: return;
            }
        }

        int32_t angle = libgte_ratan2(-1700 - z, -x);
        if (opponent.thrown % 2 != 0)
            angle += rand() % 100;
        else
            angle -= rand() % 100;
        KAR_setOpponentShot(0, angle, 0, -1700);
    }
}
