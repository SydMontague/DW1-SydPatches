#pragma once

#include "../extern/libgte.hpp"

// Non-owning view of a stone in the loaded KAR overlay. Unused vanilla fields stay opaque.
struct CurlingStone
{
    int32_t state;
    uint8_t reserved04[0x04];
    int16_t speed;
    uint8_t reserved0A[0x02];
    int32_t angle;
    uint8_t reserved10[0x04];
    Vector target;
    uint8_t reserved24[0x78];
    Vector position;
};

struct CurlingStoneRow
{
    uint8_t reserved00[0x08];
    CurlingStone stones[5];
};

extern "C" CurlingStoneRow KAR_STONE_ROWS[3];
extern "C" int32_t KAR_MATCH_STATE;

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
static_assert(__builtin_offsetof(CurlingStone, speed) == 0x08);
static_assert(__builtin_offsetof(CurlingStone, angle) == 0x0C);
static_assert(__builtin_offsetof(CurlingStone, target) == 0x14);
static_assert(__builtin_offsetof(CurlingStone, position) == 0x9C);
static_assert(sizeof(CurlingStoneRow) == 0x364 && alignof(CurlingStoneRow) == 4);
static_assert(__builtin_offsetof(CurlingStoneRow, stones) == 0x08);
