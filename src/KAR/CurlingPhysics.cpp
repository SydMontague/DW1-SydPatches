#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../extern/libgte.hpp"

namespace
{
    enum class Separation
    {
        FIRST,
        SECOND,
        BOTH
    };

    [[gnu::noinline, gnu::optimize("Os")]]
    void separatePositions(Vector& first,
                           Vector& second,
                           int32_t angle,
                           int32_t distance,
                           Separation mode,
                           int32_t initialRadius = 0)
    {
        if (distance >= 151) return;
        const auto cosine      = libgte_rcos(angle);
        const auto sine        = libgte_rsin(angle);
        auto& moving           = mode == Separation::SECOND ? second : first;
        const auto start       = moving;
        const auto secondStart = second;
        for (int32_t radius = initialRadius; distance < 151; radius++) {
            const auto x = radius * cosine / 4096;
            const auto z = radius * sine / 4096;
            moving.x     = start.x + x;
            moving.z     = start.z + z;
            if (mode == Separation::BOTH) {
                second.x = secondStart.x - x;
                second.z = secondStart.z - z;
            }
            distance = KAR_distance(first.x - second.x, first.z - second.z);
        }
    }
} // namespace

extern "C"
{
    void KAR_collidePeggedStone(CurlingStone* pegged, Vector a, CurlingStone* mover, Vector b)
    {
        const int32_t reverseAngle =
            libgte_ratan2(mover->target.z - mover->position.z, mover->target.x - mover->position.x) & 0xFFF;
        const int32_t contactAngle = libgte_ratan2(b.z - a.z, b.x - a.x) & 0xFFF;
        int32_t reflectedAngle     = contactAngle * 2 - reverseAngle;
        // Vanilla only normalizes positive overflow here; negative angles remain negative.
        if (reflectedAngle >= 4096) reflectedAngle %= 4096;
        int32_t difference = contactAngle - reverseAngle;
        if (difference < 0) difference = -difference;
        const int32_t impact = 1024 - difference % 1024;
        int32_t radius       = KAR_distance(mover->position.x - b.x, mover->position.z - b.z);
        radius               = impact * radius / 1024;
        mover->position.x    = b.x;
        mover->position.z    = b.z;
        // The pegged response always tests its supplied radius once, even if the incoming endpoints are clear.
        separatePositions(mover->position, pegged->position, reflectedAngle, 0, Separation::FIRST, radius);

        const int32_t transfer = (mover->speed >> 1) * impact / 1024;
        pegged->speed          = static_cast<int16_t>(transfer);
        mover->speed           = static_cast<int16_t>(mover->speed - transfer);
        pegged->angle          = libgte_ratan2(mover->target.z - b.z, mover->target.x - b.x);
        mover->angle           = reflectedAngle;
        pegged->state          = 3;
        mover->target          = b;
    }

    [[gnu::optimize("Os")]]
    void KAR_collideRestingStone(CurlingStone* resting, Vector a, CurlingStone* mover, Vector b)
    {
        if (resting->speed == 0 && mover->speed == 0) {
            resting->position = a;
            mover->position   = b;
            return;
        }
        const int32_t share = KAR_computeImpactShare(mover, b, a);
        Vector separation;
        KAR_computeSeparation(&separation, mover, b, a);
        resting->position.x += separation.x;
        resting->position.z += separation.z;
        mover->position.x -= separation.x;
        mover->position.z -= separation.z;
        int32_t distance =
            KAR_distance(resting->position.x - mover->position.x, resting->position.z - mover->position.z);
        int32_t angle        = libgte_ratan2(separation.z, separation.x);
        const int32_t length = KAR_distance(separation.x, separation.z);
        if (length == 0)
            angle = libgte_ratan2(mover->position.z - resting->position.z, mover->position.x - resting->position.x);
        separatePositions(resting->position,
                          mover->position,
                          angle,
                          distance,
                          length != 0 ? Separation::BOTH : Separation::SECOND);
        resting->target = a;
        mover->target   = b;
        mover->speed    = static_cast<int16_t>(mover->speed - share);
        resting->speed  = static_cast<int16_t>(share);
        resting->angle  = libgte_ratan2(resting->position.z - a.z, resting->position.x - a.x) & 0xFFF;
        mover->angle    = libgte_ratan2(mover->position.z - b.z, mover->position.x - b.x) & 0xFFF;
    }

    [[gnu::optimize("Os")]]
    void KAR_collideMovingStones(CurlingStone* stoneA, Vector a, CurlingStone* stoneB, Vector b)
    {
        const int32_t shareA = KAR_computeImpactShare(stoneA, a, b);
        const int32_t shareB = KAR_computeImpactShare(stoneB, b, a);
        Vector separationA;
        Vector separationB;
        KAR_computeSeparation(&separationA, stoneB, b, a);
        KAR_computeSeparation(&separationB, stoneA, a, b);
        const int32_t dx = separationB.x - separationA.x;
        const int32_t dz = separationB.z - separationA.z;
        if (KAR_distance(dx, dz) != 0) {
            const int32_t angle = libgte_ratan2(dz, dx);
            stoneB->position.x += dx;
            stoneB->position.z += dz;
            stoneA->position.x -= dx;
            stoneA->position.z -= dz;
            int32_t distance =
                KAR_distance(stoneA->position.x - stoneB->position.x, stoneA->position.z - stoneB->position.z);
            stoneA->speed = static_cast<int16_t>(stoneA->speed + static_cast<int16_t>(shareB - shareA));
            stoneB->speed = static_cast<int16_t>(stoneB->speed + static_cast<int16_t>(shareA - shareB));
            separatePositions(stoneB->position,
                              stoneA->position,
                              angle,
                              distance,
                              stoneA->speed > stoneB->speed ? Separation::SECOND : Separation::FIRST);
            stoneA->target = a;
            stoneB->target = b;
            stoneA->angle  = libgte_ratan2(stoneA->position.z - a.z, stoneA->position.x - a.x);
            stoneB->angle  = libgte_ratan2(stoneB->position.z - b.z, stoneB->position.x - b.x);
        }
        else {
            // Vanilla cross-divides by the other weight and stops only B in this branch.
            if (stoneB->speed / stoneA->weight < 5 || stoneA->speed / stoneB->weight < 5) {
                stoneB->speed    = 0;
                stoneB->position = stoneB->target;
                stoneA->position = stoneA->target;
            }
            else {
                // Earlier collision samples can overlap after the endpoints have been separated.
                if (KAR_distance(a.x - b.x, a.z - b.z) < 150 &&
                    KAR_distance(stoneA->position.x - stoneB->position.x, stoneA->position.z - stoneB->position.z) >=
                        150)
                    return;
                stoneB->position = b;
                stoneA->position = a;
            }
        }
    }
}
