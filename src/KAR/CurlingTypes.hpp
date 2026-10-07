#pragma once

#include "../extern/dtl/array.hpp"
#include "../extern/libgs.hpp"

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
    uint8_t reserved10[0x04];
    Vector target;
    GsDOBJ2 object;
    uint8_t reserved34[0x62];
    int16_t selectionPhase;
    uint8_t reserved98[0x04];
    Vector position;
};

struct CurlingStoneRow
{
    uint8_t reserved00[0x08];
    dtl::array<CurlingStone, 5> stones;
};

// Existing overlay rows and resident match storage; no new ownership or allocation.
extern "C"
{
    extern dtl::array<CurlingStoneRow, 3> KAR_STONE_ROWS;
    extern int16_t KAR_SELECTED_STONE;
    extern int8_t KAR_CURRENT_PLAYER;
    extern uint32_t* KAR_MODEL_DATA;
    extern int32_t KAR_MATCH_STATE;
    extern dtl::array<int8_t, 3> KAR_PEGGED_MODEL_IDS;
}

// Vanilla passes Vector/KarPos by value across O32 argument registers and the stack.
static_assert(sizeof(void*) == 4);
static_assert(__is_standard_layout(Vector) && __is_trivially_copyable(Vector));
static_assert(sizeof(Vector) == 16 && alignof(Vector) == 4);
static_assert(__builtin_offsetof(Vector, x) == 0 && __builtin_offsetof(Vector, y) == 4);
static_assert(__builtin_offsetof(Vector, z) == 8 && __builtin_offsetof(Vector, pad) == 12);
static_assert(__is_standard_layout(SVector) && __is_trivially_copyable(SVector));
static_assert(sizeof(SVector) == 8 && alignof(SVector) == 2);
static_assert(__builtin_offsetof(SVector, x) == 0 && __builtin_offsetof(SVector, y) == 2);
static_assert(__builtin_offsetof(SVector, z) == 4 && __builtin_offsetof(SVector, pad) == 6);
static_assert(__is_standard_layout(CurlingStone) && __is_trivially_copyable(CurlingStone));
static_assert(sizeof(CurlingStone) == 0xAC && alignof(CurlingStone) == 4);
static_assert(__builtin_offsetof(CurlingStone, state) == 0x00);
static_assert(__builtin_offsetof(CurlingStone, type) == 0x04);
static_assert(__builtin_offsetof(CurlingStone, weight) == 0x06);
static_assert(__builtin_offsetof(CurlingStone, speed) == 0x08);
static_assert(__builtin_offsetof(CurlingStone, angle) == 0x0C);
static_assert(__builtin_offsetof(CurlingStone, target) == 0x14);
static_assert(__builtin_offsetof(CurlingStone, position) == 0x9C);

static_assert(__builtin_offsetof(CurlingStone, object) == 0x24);
static_assert(__builtin_offsetof(CurlingStone, selectionPhase) == 0x96);
static_assert(__is_standard_layout(GsDOBJ2) && __is_trivially_copyable(GsDOBJ2));
static_assert(sizeof(GsDOBJ2) == 0x10 && alignof(GsDOBJ2) == 4);
static_assert(__builtin_offsetof(GsDOBJ2, attribute) == 0x00);
static_assert(__builtin_offsetof(GsDOBJ2, coord2) == 0x04);
static_assert(__builtin_offsetof(GsDOBJ2, tmd) == 0x08);
static_assert(__builtin_offsetof(GsDOBJ2, id) == 0x0C);
static_assert(__is_standard_layout(CurlingStoneRow) && __is_trivially_copyable(CurlingStoneRow));
static_assert(sizeof(CurlingStoneRow) == 0x364 && alignof(CurlingStoneRow) == 4);
static_assert(__builtin_offsetof(CurlingStoneRow, stones) == 0x08);
static_assert(__is_standard_layout(decltype(CurlingStoneRow::stones)) &&
              __is_trivially_copyable(decltype(CurlingStoneRow::stones)));
static_assert(sizeof(CurlingStoneRow::stones) == 5 * sizeof(CurlingStone));
static_assert(alignof(decltype(CurlingStoneRow::stones)) == alignof(CurlingStone));
static_assert(__builtin_offsetof(decltype(CurlingStoneRow::stones), elements) == 0);
static_assert(__is_standard_layout(decltype(KAR_STONE_ROWS)) && __is_trivially_copyable(decltype(KAR_STONE_ROWS)));
static_assert(sizeof(KAR_STONE_ROWS) == 3 * sizeof(CurlingStoneRow));
static_assert(alignof(decltype(KAR_STONE_ROWS)) == alignof(CurlingStoneRow));
static_assert(__builtin_offsetof(decltype(KAR_STONE_ROWS), elements) == 0);
static_assert(__is_standard_layout(decltype(KAR_PEGGED_MODEL_IDS)) &&
              __is_trivially_copyable(decltype(KAR_PEGGED_MODEL_IDS)));
static_assert(sizeof(KAR_PEGGED_MODEL_IDS) == 3 && alignof(decltype(KAR_PEGGED_MODEL_IDS)) == 1);
static_assert(__builtin_offsetof(decltype(KAR_PEGGED_MODEL_IDS), elements) == 0);
