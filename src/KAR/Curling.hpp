#pragma once

#include "../extern/dtl/types.hpp"

enum class CurlingWallZone : int32_t
{
    NONE                = 0,
    NEGATIVE_X_WALL     = 1,
    POSITIVE_X_WALL     = 2,
    NEGATIVE_X_DIAGONAL = 3, // z + 2*x boundary; vanilla contact flag 0.
    POSITIVE_X_DIAGONAL = 4, // z - 2*x boundary; vanilla contact flag 1.
    NEGATIVE_Z_WALL     = 5,
    POSITIVE_Z_WALL     = 6, // Reflection also requires target.vz < 1125.
};

static_assert(sizeof(CurlingWallZone) == sizeof(int32_t));

struct CurlingStone;
struct Vector;
struct SVector;

const CurlingStone& KAR_getTallyStone();

extern "C"
{
    void KAR_tickStones(int32_t instanceId);
    void KAR_checkStonesStopped();
    void KAR_updateCollisions();
    void KAR_bounceOffWall();
    void KAR_resolveStoneCollision(CurlingStone* stoneA, Vector a, CurlingStone* stoneB, Vector b);
    void KAR_collidePeggedStone(CurlingStone* pegged, Vector a, CurlingStone* mover, Vector b);
    void KAR_collideRestingStone(CurlingStone* resting, Vector a, CurlingStone* mover, Vector b);
    void KAR_collideMovingStones(CurlingStone* stoneA, Vector a, CurlingStone* stoneB, Vector b);
    CurlingWallZone KAR_getWallZone(int16_t x, int16_t z);
    int32_t KAR_distance(int32_t x, int32_t z);
    int32_t KAR_computeImpactShare(const CurlingStone* stone, Vector a, Vector b);
    void KAR_computeSeparation(Vector* out, const CurlingStone* stone, Vector a, Vector b);
    // Direction is zero for the negative-X diagonal and nonzero for the positive-X diagonal.
    bool KAR_findWallContact(Vector* out, const CurlingStone* stone, int32_t direction);
    void KAR_reflectOffDiagonal(CurlingStone* stone, int32_t direction);
    void KAR_placeAtContact(CurlingStone* stone, Vector contact);
    void KAR_rotatePoint(SVector* point, int32_t angle);
}
