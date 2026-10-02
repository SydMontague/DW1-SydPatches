#include "../Sound.hpp"
#include "../extern/dtl/array.hpp"
#include "Curling.hpp"
#include "CurlingTypes.hpp"

extern "C"
{
    [[gnu::optimize("Os")]]
    void KAR_checkStonesStopped()
    {
        if (KAR_MATCH_STATE != 11) return;

        bool moving = false;
        for (auto& row : KAR_STONE_ROWS) {
            for (auto& stone : row.stones) {
                if (stone.state <= 0) continue;
                if (stone.speed > 0) {
                    moving = true;
                    // Vanilla stops this row's scan, leaving its later speeds untouched.
                    break;
                }
                stone.speed = 0;
            }
        }
        if (!moving) KAR_MATCH_STATE = 12;

        for (auto& row : KAR_STONE_ROWS) {
            for (auto& stone : row.stones) {
                if (stone.state > 0 && stone.position.z >= 190 && stone.speed <= 0) stone.state = -101;
            }
        }
    }

    [[gnu::optimize("Os")]]
    void KAR_updateCollisions()
    {
        dtl::array<Vector, 15> current;
        dtl::array<Vector, 15> previous;
        for (auto& point : previous)
            point.y = 0;

        KAR_bounceOffWall();

        for (auto& point : current)
            point.y = 0;
        if (KAR_MATCH_STATE != 11) return;

        for (int32_t step = 0; step <= 10; step++) {
            for (int32_t player = 0; player < 3; player++) {
                for (int32_t index = 0; index < 5; index++) {
                    const auto& stone = KAR_STONE_ROWS[player].stones[index];
                    if (stone.state <= 0) continue;
                    auto& point = current[player * 5 + index];
                    point.x     = (stone.target.x * (10 - step) + stone.position.x * step) / 10;
                    point.z     = (stone.target.z * (10 - step) + stone.position.z * step) / 10;
                    point.y     = stone.target.y;
                }
            }

            bool collided = false;
            for (int32_t a = 0; a < 15; a++) {
                auto& stoneA = KAR_STONE_ROWS[a / 5].stones[a % 5];
                if (stoneA.state <= 0) continue;
                for (int32_t b = a + 1; b < 15; b++) {
                    auto& stoneB = KAR_STONE_ROWS[b / 5].stones[b % 5];
                    if (stoneB.state <= 0) continue;
                    if (KAR_distance(current[a].x - current[b].x, current[a].z - current[b].z) < 150) {
                        collided           = true;
                        const auto& points = step == 0 ? current : previous;
                        KAR_resolveStoneCollision(&stoneB, points[b], &stoneA, points[a]);
                    }
                }
            }
            // As in vanilla, temporary vector pad words are unspecified and never used for geometry.
            previous = current;
            if (collided && step == 10) step--;
        }
    }

    void KAR_resolveStoneCollision(CurlingStone* stoneA, Vector a, CurlingStone* stoneB, Vector b)
    {
        playSound2(8, static_cast<int16_t>(stoneA->speed + stoneB->speed) >= 3000 ? 1 : 2);
        if (stoneA->state == 2 || stoneA->state == 3)
            KAR_collidePeggedStone(stoneA, a, stoneB, b);
        else if (stoneB->state == 2 || stoneB->state == 3)
            KAR_collidePeggedStone(stoneB, b, stoneA, a);
        else if (stoneA->speed <= 0)
            KAR_collideRestingStone(stoneA, a, stoneB, b);
        else if (stoneB->speed <= 0)
            KAR_collideRestingStone(stoneB, b, stoneA, a);
        else
            KAR_collideMovingStones(stoneB, b, stoneA, a);
    }
}
