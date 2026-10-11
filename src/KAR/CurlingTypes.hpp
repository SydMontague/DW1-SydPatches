#pragma once

#include "../extern/dtl/array.hpp"
#include "../extern/libgs.hpp"

// Vector arguments and libgs data use the PS1 O32 layout.
static_assert(sizeof(void*) == 4 && sizeof(Vector) == 16 && sizeof(SVector) == 8);

// Non-owning view of a stone in the loaded KAR overlay. Unused vanilla fields stay opaque.
struct CurlingStone
{
    int32_t state;
    int8_t type;
    uint8_t reserved05;
    int16_t weight;
    int16_t speed;
    uint8_t reserved0A[0x02];
    int32_t angle;
    int8_t ring;
    int8_t previousRing;
    uint8_t reserved12[0x02];
    Vector target;
    GsDOBJ2 object;
    GsCOORDINATE2 coordinate;
    uint8_t reserved84[0x10];
    int16_t rotationX;
    int16_t selectionPhase;
    int16_t rotationZ;
    int16_t reserved9A;
    Vector position;
};

// The loaded row storage and libgs calls share these binary layouts.
static_assert(__is_standard_layout(CurlingStone) && __is_trivially_copyable(CurlingStone));
static_assert(sizeof(CurlingStone) == 0xAC && alignof(CurlingStone) == 4);
static_assert(__builtin_offsetof(CurlingStone, target) == 0x14);
static_assert(__builtin_offsetof(CurlingStone, object) == 0x24);
static_assert(__builtin_offsetof(CurlingStone, coordinate) == 0x34);
static_assert(__builtin_offsetof(CurlingStone, rotationX) == 0x94);
static_assert(__builtin_offsetof(CurlingStone, position) == 0x9C);

struct CurlingStoneRow
{
    int8_t score;
    int8_t thrown;
    int16_t shotPower;
    uint16_t plannedStone;
    uint8_t reserved06[0x02];
    dtl::array<CurlingStone, 5> stones;
};

static_assert(__is_standard_layout(CurlingStoneRow) && __is_trivially_copyable(CurlingStoneRow));
static_assert(sizeof(CurlingStoneRow) == 0x364 && __builtin_offsetof(CurlingStoneRow, stones) == 8);

struct CurlingRingZone
{
    int16_t dx;
    int16_t dz;
    int16_t radius;
};

extern const dtl::array<CurlingRingZone, 4> KAR_RING_ZONES;

// Existing overlay rows and resident match storage preserve their entry/re-entry lifetime.
extern dtl::array<CurlingStoneRow, 3> KAR_STONE_ROWS;
extern int16_t KAR_SELECTED_STONE;
extern int8_t KAR_CURRENT_PLAYER;
extern uint32_t* KAR_MODEL_DATA;
extern int32_t KAR_MATCH_STATE;
extern uint8_t KAR_OPPONENT_ENTITY;
extern int8_t KAR_HINT_STATE;
extern CurlingStone KAR_PREVIOUS_PLAYER_STONE;

extern uint16_t KAR_ANIMATION_TIMER;
extern uint8_t KAR_READY_COUNTER;
extern int16_t KAR_THROW_POWER;
extern int32_t KAR_AIM_ANGLE;
extern int16_t KAR_AIM_VELOCITY;
extern dtl::array<GsOT, 2> KAR_ORDERING_TABLES;
