#include "../Sound.hpp"
#include "../extern/dtl/algorithm.hpp"
#include "../extern/dtl/array.hpp"
#include "Curling.hpp"
#include "CurlingTypes.hpp"

namespace
{
    // Y and padding do not control collision decisions. Weight/type are invariant during this call.
    struct CollisionState
    {
        int32_t state;
        int32_t speed;
        int32_t angle;
        int32_t positionX;
        int32_t positionZ;
        int32_t targetX;
        int32_t targetZ;
        int32_t previousX;
        int32_t previousZ;
    };

    using CollisionCheckpoint = dtl::array<CollisionState, 15>;
    using StonePointers       = dtl::array<CurlingStone*, 15>;

    [[gnu::optimize("Os")]]
    bool checkCollisionState(const CollisionCheckpoint& checkpoint,
                             const dtl::array<Vector, 15>& previous,
                             const StonePointers& stones)
    {
        for (int32_t i = 0; i < 15; i++) {
            const auto& stone = *stones[i];
            const auto& state = checkpoint[i];
            if (state.state != stone.state || state.speed != stone.speed || state.angle != stone.angle ||
                state.positionX != stone.position.x || state.positionZ != stone.position.z ||
                state.targetX != stone.target.x || state.targetZ != stone.target.z ||
                state.previousX != previous[i].x || state.previousZ != previous[i].z)
                return false;
        }
        return true;
    }

    [[gnu::optimize("Os"), gnu::noinline]]
    void updateCollisionState(CollisionCheckpoint& checkpoint,
                              const dtl::array<Vector, 15>& previous,
                              const StonePointers& stones)
    {
        for (int32_t i = 0; i < 15; i++) {
            const auto& stone = *stones[i];
            checkpoint[i]     = {.state     = stone.state,
                                 .speed     = stone.speed,
                                 .angle     = stone.angle,
                                 .positionX = stone.position.x,
                                 .positionZ = stone.position.z,
                                 .targetX   = stone.target.x,
                                 .targetZ   = stone.target.z,
                                 .previousX = previous[i].x,
                                 .previousZ = previous[i].z};
        }
    }

    [[gnu::optimize("Os")]]
    bool coordinatesNear(int32_t a, int32_t b)
    {
        const auto distance = a < b ? static_cast<uint32_t>(b) - static_cast<uint32_t>(a)
                                    : static_cast<uint32_t>(a) - static_cast<uint32_t>(b);
        return distance <= 150;
    }

    [[gnu::optimize("Os")]]
    bool overlaps(const Vector& a, const Vector& b)
    {
        // Bound each difference before signed subtraction and the distance service's low-word squares.
        return coordinatesNear(a.x, b.x) && coordinatesNear(a.z, b.z) && KAR_distance(a.x - b.x, a.z - b.z) < 150;
    }

    [[gnu::optimize("Os")]]
    int32_t eligibilityBand(int32_t z)
    {
        if (z < -1380) return 0;
        if (z < 190) return 1;
        return 2;
    }

    [[gnu::optimize("Os")]]
    bool clearOfStones(const StonePointers& stones, const CurlingStone& moving, const Vector& point)
    {
        for (const auto* stone : stones)
            if (stone != &moving && stone->state > 0 && overlaps(point, stone->position)) return false;
        return true;
    }

    [[gnu::optimize("Os")]]
    void considerRecoveryPosition(const StonePointers& stones,
                                  CurlingStone& stone,
                                  const Vector& origin,
                                  const Vector& point,
                                  int32_t& nearest)
    {
        const auto dx       = point.x - origin.x;
        const auto dz       = point.z - origin.z;
        const auto distance = dx * dx + dz * dz;
        if (distance >= nearest || eligibilityBand(point.z) != eligibilityBand(origin.z) ||
            KAR_getWallZone(point.x, point.z) != CurlingWallZone::NONE || !clearOfStones(stones, stone, point))
            return;
        nearest          = distance;
        stone.position.x = point.x;
        stone.position.z = point.z;
    }

    [[gnu::optimize("Os")]]
    void recoverPosition(const StonePointers& stones, CurlingStone& stone)
    {
        const Vector origin = {.x = dtl::clamp(stone.position.x, -724, 724),
                               .z = dtl::clamp(stone.position.z, -2524, 1124)};

        const auto centerX = dtl::clamp(origin.x, -116, 116);
        const auto band    = eligibilityBand(origin.z);
        int32_t startZ;
        switch (band) {
            case 0: startZ = -2000; break;
            case 1: startZ = dtl::clamp(origin.z, -1076, -115) - 304; break;
            default: startZ = dtl::clamp(origin.z, 494, 820) - 304; break;
        }
        int32_t nearest = 0x7FFFFFFF;
        // Distance <150 implies true separation <151. At spacing 304, each of the other 14 centers
        // can exclude at most one of these 15 legal sites, so a free site always exists.
        for (int32_t row = 0; row < 3; row++) {
            for (int32_t column = 0; column < 5; column++) {
                const Vector point = {.x = centerX + column * 304 - 608, .z = startZ + row * 304};
                considerRecoveryPosition(stones, stone, origin, point, nearest);
            }
        }

        // Sixteen directions; the Z coordinate is the same circle table a quarter-turn later.
        static constexpr dtl::array<int16_t, 16>
            offsets{151, 140, 107, 58, 0, -58, -107, -140, -151, -140, -107, -58, 0, 58, 107, 140};
        // Compare a finite contact set with the guaranteed grid result. Every accepted point clears
        // all active centers; projections that round into a stone or cross a wall/band are rejected.
        for (const auto* other : stones) {
            if (other == &stone || other->state <= 0 || !overlaps(origin, other->position)) continue;
            const auto dx         = origin.x - other->position.x;
            const auto dz         = origin.z - other->position.z;
            const auto separation = KAR_distance(dx, dz);
            for (int32_t candidate = 0; candidate < 18; candidate++) {
                int32_t offsetX = 0;
                int32_t offsetZ = 0;
                if (candidate < 16) {
                    offsetX = offsets[candidate];
                    offsetZ = offsets[(candidate + 12) & 15];
                }
                else {
                    if (separation == 0) continue;
                    const auto radius = 151 + (candidate - 16) * 2;
                    offsetX           = dx * radius / separation;
                    offsetZ           = dz * radius / separation;
                }
                const Vector point = {.x = other->position.x + offsetX, .z = other->position.z + offsetZ};
                considerRecoveryPosition(stones, stone, origin, point, nearest);
            }
        }
    }

    [[gnu::optimize("Os")]]
    int32_t retentionPriority(const CurlingStone& stone)
    {
        if (stone.state == 2 || stone.state == 3) return 2;
        if (stone.speed <= 0) return 1;
        return 0;
    }

    [[gnu::optimize("Os")]]
    void recoverCollisions(const StonePointers& stones)
    {
        // Each move clears every other active center, so checked pairs stay clear and at most 14 stones move.
        for (int32_t a = 0; a < 15; a++) {
            auto& stoneA = *stones[a];
            if (stoneA.state <= 0) continue;
            for (int32_t b = a + 1; b < 15; b++) {
                auto& stoneB = *stones[b];
                if (stoneB.state <= 0 || !overlaps(stoneA.position, stoneB.position)) continue;
                auto* moving = retentionPriority(stoneA) < retentionPriority(stoneB) ? &stoneA : &stoneB;
                recoverPosition(stones, *moving);
                moving->target = moving->position;
                moving->speed  = 0;
                if (moving->state == 3) moving->state = 2;
            }
        }
    }
} // namespace

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
        KAR_bounceOffWall();
        if (KAR_MATCH_STATE != 11) return;

        dtl::array<Vector, 15> current{};
        // Step zero uses current; every later step follows a complete copy below.
        dtl::array<Vector, 15> previous;
        CollisionCheckpoint checkpoint;
        uint32_t power  = 1;
        uint32_t length = 0;
        StonePointers stones;
        int32_t stoneIndex = 0;
        for (auto& row : KAR_STONE_ROWS)
            for (auto& stone : row.stones)
                stones[stoneIndex++] = &stone;

        for (int32_t step = 0; step <= 10; step++) {
            for (int32_t index = 0; index < 15; index++) {
                const auto& stone = *stones[index];
                if (stone.state <= 0) continue;
                auto& point = current[index];
                point.x     = (stone.target.x * (10 - step) + stone.position.x * step) / 10;
                point.z     = (stone.target.z * (10 - step) + stone.position.z * step) / 10;
                point.y     = stone.target.y;
            }

            bool collided = false;
            for (int32_t a = 0; a < 15; a++) {
                auto& stoneA = *stones[a];
                if (stoneA.state <= 0) continue;
                for (int32_t b = a + 1; b < 15; b++) {
                    auto& stoneB = *stones[b];
                    if (stoneB.state <= 0) continue;
                    if (KAR_distance(current[a].x - current[b].x, current[a].z - current[b].z) < 150) {
                        collided           = true;
                        const auto& points = step == 0 ? current : previous;
                        KAR_resolveStoneCollision(&stoneB, points[b], &stoneA, points[a]);
                    }
                }
            }
            previous = current;
            // Brent checkpoints retain ordinary retries; only recurrence or counter exhaustion requires recovery.
            if (step == 9)
                updateCollisionState(checkpoint, previous, stones);
            else if (collided && step == 10) {
                length++;
                if (checkCollisionState(checkpoint, previous, stones) || (length == power && power == 0x80000000U)) {
                    recoverCollisions(stones);
                    return;
                }
                if (length == power) {
                    updateCollisionState(checkpoint, previous, stones);
                    power *= 2;
                    length = 0;
                }
                step--;
            }
        }
    }

    [[gnu::optimize("Os")]]
    void KAR_resolveStoneCollision(CurlingStone* stoneA, Vector a, CurlingStone* stoneB, Vector b)
    {
        playSound2(8, static_cast<int16_t>(stoneA->speed + stoneB->speed) >= 3000 ? 1 : 2);
        const auto priorityA = retentionPriority(*stoneA);
        const auto priorityB = retentionPriority(*stoneB);
        const auto priority  = dtl::max(priorityA, priorityB);
        decltype(&KAR_collideMovingStones) handler;
        switch (priority) {
            case 0: handler = KAR_collideMovingStones; break;
            case 1: handler = KAR_collideRestingStone; break;
            default: handler = KAR_collidePeggedStone; break;
        }
        // Equal pegged/resting priorities retain A; vanilla reverses the two-moving-stone case.
        const bool reverse      = priorityA < priorityB || priority == 0;
        const auto* firstPoint  = reverse ? &b : &a;
        const auto* secondPoint = reverse ? &a : &b;
        handler(reverse ? stoneB : stoneA, *firstPoint, reverse ? stoneA : stoneB, *secondPoint);
    }
}
