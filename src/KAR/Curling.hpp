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
    POSITIVE_Z_WALL     = 6, // Reflection also requires target.z < 1125.
};

static_assert(sizeof(CurlingWallZone) == sizeof(int32_t));

struct CurlingStone;
struct Vector;

const CurlingStone& KAR_getTallyStone();
void KAR_resetScoring();
void KAR_setupMatch(int32_t mode);
bool KAR_moveMatchCamera(int32_t x, int32_t z, int32_t speed);

int32_t KAR_findClearShotAngle(int32_t x, int32_t z);
int32_t KAR_aimAtStoneInRing(int32_t player, int32_t ring, int16_t* outX, int16_t* outZ);
void KAR_setOpponentShot(int32_t priority, int32_t angle, int32_t x, int32_t z);
int32_t KAR_aimAtRandomStone();
int32_t KAR_aimBankShot(const CurlingStone* stone, int32_t x, int32_t z);

void KAR_renderScene(int32_t instanceId);
void KAR_renderAimArrow();
void KAR_renderStoneCursor();
void KAR_renderScores();
void KAR_renderPowerMeter();
void KAR_renderReadyPrompt();
void KAR_renderNamePlates();
int32_t KAR_tickYesNoPrompt();
int32_t KAR_tickHintBox(int32_t page);
void KAR_handleAimScroll();
void KAR_beginAiming();
void KAR_selectPreviousStone();
void KAR_selectNextStone();
void KAR_turnAim(int32_t delta);
void KAR_beginThrow();
void KAR_updateRingMarkers();
void KAR_classifyStoneRings();
int32_t KAR_tickScoreTally();
void KAR_chooseOpponentShot();
void KAR_tickStones(int32_t instanceId);
void KAR_checkStonesStopped();
void KAR_updateCollisions();
void KAR_bounceOffWall();
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
