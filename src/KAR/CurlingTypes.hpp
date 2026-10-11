#pragma once

#include "../extern/dtl/array.hpp"
#include "../extern/libgs.hpp"

// KAR owns stone storage; only the embedded libgs/libgte types have an external layout contract.
struct CurlingStone
{
    int32_t state;
    int8_t type;
    int16_t weight;
    int16_t speed;
    int32_t angle;
    int8_t ring;
    int8_t previousRing;
    Vector target;
    GsDOBJ2 object;
    GsCOORDINATE2 coordinate;
    SVector rotation;
    Vector position;
};

struct CurlingStoneRow
{
    int8_t score;
    int8_t thrown;
    int16_t shotPower;
    uint16_t plannedStone;
    dtl::array<CurlingStone, 5> stones;
};

// Whole-stone snapshots use ordinary assignment, including their GS binding.
static_assert(__is_trivially_copyable(CurlingStone));

struct CurlingRingZone
{
    int16_t dx;
    int16_t dz;
    int16_t radius;
};

extern const dtl::array<CurlingRingZone, 4> KAR_RING_ZONES;

enum class CurlingMatchState : int32_t
{
    INITIALIZE           = 0,
    PROMPT               = 1,
    REVEAL_DEMO_STONES   = 2,
    READY                = 3,
    SETUP_DEMO_SHOT      = 4,
    SELECT_STONE         = 5,
    AIM                  = 6,
    FOCUS_THROW_CAMERA   = 7,
    SELECT_POWER         = 8,
    START_THROW          = 9,
    WAIT_FOR_RELEASE     = 10,
    STONES_MOVING        = 11,
    CLASSIFY_RINGS       = 12,
    TALLY_SCORE          = 13,
    FOCUS_DEMO_RESULT    = 14,
    RESET_TURN           = 15,
    RETURN_CAMERA        = 16,
    CHOOSE_OPPONENT_SHOT = 17,
    WAIT_FOR_HINT_BOX    = 18,
    SHOW_HINT            = 19,
    FINISHED             = 20,
};

static_assert(sizeof(CurlingMatchState) == sizeof(int32_t));

// One definition in the owning KAR source; these are not fixed-address imports.
constexpr uint16_t KAR_OBJECT_INSTANCE = 15;
// KARRING.TMD is 27,828 bytes; readFile transfers fourteen complete 2,048-byte sectors.
constexpr uint32_t KAR_MODEL_BUFFER_WORDS = 14 * 2048 / sizeof(uint32_t);
// TODO internalize access to these KAR-owned rows.
extern dtl::array<CurlingStoneRow, 3> KAR_STONE_ROWS;
extern int16_t KAR_SELECTED_STONE;
extern int8_t KAR_CURRENT_PLAYER;
extern dtl::array<uint32_t, KAR_MODEL_BUFFER_WORDS> KAR_MODEL_BUFFER;
extern CurlingMatchState KAR_MATCH_STATE;
extern uint8_t KAR_OPPONENT_ENTITY;
extern int8_t KAR_HINT_STATE;
extern CurlingStone KAR_PREVIOUS_PLAYER_STONE;

extern uint16_t KAR_ANIMATION_TIMER;
extern uint8_t KAR_READY_COUNTER;
extern int16_t KAR_THROW_POWER;
extern int32_t KAR_AIM_ANGLE;
extern int16_t KAR_AIM_VELOCITY;
extern dtl::array<GsOT, 2> KAR_ORDERING_TABLES;
