#include "StatsView.hpp"

#include "AtlasFont.hpp"
#include "Entity.hpp"
#include "Font.hpp"
#include "GameData.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "Timestamp.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "constants.hpp"
#include "extern/dtl/type_traits.hpp"
#include "extern/dw1.hpp"
#include "extern/libc.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"

namespace
{
    namespace SpriteIndex
    {
        enum SpriteIndex
        {
            TYPE,
            SPECIAL1,
            SPECIAL2,
            SPECIAL3,
            HAPPINESS,
            DISCIPLINE,
            LIFE1,
            LIFE2,
            LIFE3,
            SICK,
            INJURY,
            HUNGER,
            SLEEP,
            UNHAPPY,
            TIRED,
            POOP,
            HAPPINESS_BAR1,
            HAPPINESS_BAR2,
            DISCIPLINE_BAR1,
            DISCIPLINE_BAR2,
        };
    } // namespace SpriteIndex

    constexpr int32_t STATS_OFFSET_X         = toRelativeX(187);
    constexpr int32_t STATS_OFFSET_Y         = toRelativeY(108);
    constexpr int32_t PROFILE_OFFSET_X       = toRelativeX(20);
    constexpr int32_t PROFILE_OFFSET_Y       = toRelativeY(37);
    constexpr int32_t CONDITION_OFFSET_X     = toRelativeX(20);
    constexpr int32_t CONDITION_OFFSET_Y     = toRelativeY(108);
    constexpr uint16_t CONDITION_OFFSETS[16] = {14, 15, 16, 17, 18, 19, 20, 21, 21, 20, 19, 18, 17, 16, 15, 14};

    constexpr Line lines[] = {
        {.x1 = 18, .y1 = -15, .x2 = 18, .y2 = 98, .clut = 0},
        {.x1 = 19, .y1 = -16, .x2 = 19, .y2 = 98, .clut = 1},
        {.x1 = 20, .y1 = -15, .x2 = 20, .y2 = 98, .clut = 0},
        {.x1 = -146, .y1 = -18, .x2 = 146, .y2 = -18, .clut = 0},
        {.x1 = -147, .y1 = -17, .x2 = 147, .y2 = -17, .clut = 1},
        {.x1 = -146, .y1 = -16, .x2 = 146, .y2 = -16, .clut = 0},
    };

    constexpr Inset insets[] = {
        //{PROFILE_OFFSET_X + 190, PROFILE_OFFSET_Y + 14, 86, 14}, // Active/Type
        //{PROFILE_OFFSET_X + 190, PROFILE_OFFSET_Y + 30, 86, 14}, // Special

        {PROFILE_OFFSET_X + 32, PROFILE_OFFSET_Y + 14, 155, 14}, // Name
        {PROFILE_OFFSET_X + 32, PROFILE_OFFSET_Y + 30, 155, 14}, // Digimon
        {PROFILE_OFFSET_X + 32, PROFILE_OFFSET_Y + 46, 28, 14},  // Age
        {PROFILE_OFFSET_X + 104, PROFILE_OFFSET_Y + 46, 28, 14}, // Weight
        {PROFILE_OFFSET_X + 176, PROFILE_OFFSET_Y + 46, 28, 14}, // Battle
        {PROFILE_OFFSET_X + 248, PROFILE_OFFSET_Y + 46, 28, 14}, // Care

        {STATS_OFFSET_X + 32, STATS_OFFSET_Y + 22, 80, 5}, // HP
        {STATS_OFFSET_X + 32, STATS_OFFSET_Y + 37, 80, 5}, // MP
        {STATS_OFFSET_X + 32, STATS_OFFSET_Y + 52, 45, 5}, // Off
        {STATS_OFFSET_X + 32, STATS_OFFSET_Y + 67, 45, 5}, // Def
        {STATS_OFFSET_X + 32, STATS_OFFSET_Y + 82, 45, 5}, // Speed
        {STATS_OFFSET_X + 32, STATS_OFFSET_Y + 97, 45, 5}, // Brains

        {CONDITION_OFFSET_X + 60, CONDITION_OFFSET_Y + 93, 51, 8},
        {CONDITION_OFFSET_X + 60, CONDITION_OFFSET_Y + 79, 51, 8},
        {CONDITION_OFFSET_X + 60, CONDITION_OFFSET_Y + 65, 51, 8},
        {CONDITION_OFFSET_X + 60, CONDITION_OFFSET_Y + 51, 51, 8},
    };

    constexpr dtl::array<Pair<const char*, RenderSettings>, 20> LABELS_7PX = {{
        {
            "NAME",
            {
                .x        = PROFILE_OFFSET_X,
                .y        = PROFILE_OFFSET_Y + 15,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_BLUE,
                .width    = 30,
                .height   = 14,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "DIGI",
            {
                .x        = PROFILE_OFFSET_X,
                .y        = PROFILE_OFFSET_Y + 31,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_BLUE,
                .width    = 30,
                .height   = 14,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "AGE",
            {
                .x        = PROFILE_OFFSET_X,
                .y        = PROFILE_OFFSET_Y + 47,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_BLUE,
                .width    = 30,
                .height   = 14,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "WEIGHT",
            {
                .x        = PROFILE_OFFSET_X + 62,
                .y        = PROFILE_OFFSET_Y + 47,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_BLUE,
                .width    = 40,
                .height   = 14,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "BATTLE",
            {
                .x        = PROFILE_OFFSET_X + 134,
                .y        = PROFILE_OFFSET_Y + 47,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_BLUE,
                .width    = 40,
                .height   = 14,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "MISTAKE",
            {
                .x        = PROFILE_OFFSET_X + 206,
                .y        = PROFILE_OFFSET_Y + 47,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_BLUE,
                .width    = 40,
                .height   = 14,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "LIVES",
            {
                .x        = CONDITION_OFFSET_X + 14,
                .y        = CONDITION_OFFSET_Y + 34,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 44,
                .height   = 12,
                .alignX   = AlignmentX::LEFT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "HAPPINESS",
            {
                .x        = CONDITION_OFFSET_X + 14,
                .y        = CONDITION_OFFSET_Y + 50,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 44,
                .height   = 12,
                .alignX   = AlignmentX::LEFT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "DISCIPLINE",
            {
                .x        = CONDITION_OFFSET_X + 14,
                .y        = CONDITION_OFFSET_Y + 64,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 44,
                .height   = 12,
                .alignX   = AlignmentX::LEFT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "TIREDNESS",
            {
                .x        = CONDITION_OFFSET_X + 14,
                .y        = CONDITION_OFFSET_Y + 78,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 44,
                .height   = 12,
                .alignX   = AlignmentX::LEFT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "CURSE",
            {
                .x        = CONDITION_OFFSET_X + 14,
                .y        = CONDITION_OFFSET_Y + 92,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 44,
                .height   = 12,
                .alignX   = AlignmentX::LEFT,
                .alignY   = AlignmentY::CENTER,
            },
        },

        {
            "HP",
            {
                .x        = STATS_OFFSET_X + 14,
                .y        = STATS_OFFSET_Y + 14,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 18,
                .height   = 14,
                .alignX   = AlignmentX::LEFT,
                .alignY   = AlignmentY::BOTTOM,
            },
        },
        {
            "MP",
            {
                .x        = STATS_OFFSET_X + 14,
                .y        = STATS_OFFSET_Y + 29,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 18,
                .height   = 14,
                .alignX   = AlignmentX::LEFT,
                .alignY   = AlignmentY::BOTTOM,
            },
        },
        {
            "OFF",
            {
                .x        = STATS_OFFSET_X + 14,
                .y        = STATS_OFFSET_Y + 44,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 18,
                .height   = 14,
                .alignX   = AlignmentX::LEFT,
                .alignY   = AlignmentY::BOTTOM,
            },
        },
        {
            "DEF",
            {
                .x        = STATS_OFFSET_X + 14,
                .y        = STATS_OFFSET_Y + 59,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 18,
                .height   = 14,
                .alignX   = AlignmentX::LEFT,
                .alignY   = AlignmentY::BOTTOM,
            },
        },
        {
            "SPD",
            {
                .x        = STATS_OFFSET_X + 14,
                .y        = STATS_OFFSET_Y + 74,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 18,
                .height   = 14,
                .alignX   = AlignmentX::LEFT,
                .alignY   = AlignmentY::BOTTOM,
            },
        },
        {
            "BRN",
            {
                .x        = STATS_OFFSET_X + 14,
                .y        = STATS_OFFSET_Y + 89,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 18,
                .height   = 14,
                .alignX   = AlignmentX::LEFT,
                .alignY   = AlignmentY::BOTTOM,
            },
        },

        {
            "PROFILE",
            {
                .x        = PROFILE_OFFSET_X + 1,
                .y        = PROFILE_OFFSET_Y + 1,
                .baseClut = 2,
                .color    = TEXT_COLOR_GREY,
                .width    = 51,
                .height   = 10,
                .alignX   = AlignmentX::CENTER,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "CONDITION",
            {
                .x        = CONDITION_OFFSET_X + 1,
                .y        = CONDITION_OFFSET_Y + 1,
                .baseClut = 2,
                .color    = TEXT_COLOR_GREY,
                .width    = 51,
                .height   = 10,
                .alignX   = AlignmentX::CENTER,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "PARAMETER",
            {
                .x        = STATS_OFFSET_X + 1,
                .y        = STATS_OFFSET_Y + 1,
                .baseClut = 2,
                .color    = TEXT_COLOR_GREY,
                .width    = 51,
                .height   = 10,
                .alignX   = AlignmentX::CENTER,
                .alignY   = AlignmentY::CENTER,
            },
        },
    }};

    constexpr dtl::array<Pair<const char*, RenderSettings>, 3> LABELS_5PX = {{
        {
            "SPECIAL",
            {
                .x        = PROFILE_OFFSET_X + 192,
                .y        = PROFILE_OFFSET_Y + 31,
                .baseClut = 1,
                .color    = TEXT_COLOR_GREEN,
                .width    = 32,
                .height   = 14,
                .alignX   = AlignmentX::LEFT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "TYPE",
            {
                .x        = PROFILE_OFFSET_X + 259,
                .y        = PROFILE_OFFSET_Y,
                .baseClut = 1,
                .color    = TEXT_COLOR_YELLOW,
                .width    = 20,
                .height   = 14,
                .alignX   = AlignmentX::CENTER,
                .alignY   = AlignmentY::BOTTOM,
            },
        },
        {
            "ACTIVE",
            {
                .x        = PROFILE_OFFSET_X + 192,
                .y        = PROFILE_OFFSET_Y,
                .baseClut = 1,
                .color    = TEXT_COLOR_PINK,
                .width    = 64,
                .height   = 14,
                .alignX   = AlignmentX::CENTER,
                .alignY   = AlignmentY::BOTTOM,
            },
        },
    }};

    constexpr RenderSettings VALUE_NAME{
        .x        = PROFILE_OFFSET_X + 34,
        .y        = PROFILE_OFFSET_Y + 16,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 152,
        .height   = 12,
        .alignX   = AlignmentX::LEFT,
        .alignY   = AlignmentY::CENTER,
    };
    constexpr RenderSettings VALUE_DIGIMON{
        .x        = PROFILE_OFFSET_X + 34,
        .y        = PROFILE_OFFSET_Y + 32,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 152,
        .height   = 12,
        .alignX   = AlignmentX::LEFT,
        .alignY   = AlignmentY::CENTER,
    };
    constexpr RenderSettings VALUE_AGE{
        .x        = PROFILE_OFFSET_X + 34,
        .y        = PROFILE_OFFSET_Y + 48,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 25,
        .height   = 12,
        .alignX   = AlignmentX::RIGHT,
        .alignY   = AlignmentY::CENTER,
    };
    constexpr RenderSettings VALUE_WEIGHT{
        .x        = PROFILE_OFFSET_X + 106,
        .y        = PROFILE_OFFSET_Y + 48,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 25,
        .height   = 12,
        .alignX   = AlignmentX::RIGHT,
        .alignY   = AlignmentY::CENTER,
    };
    constexpr RenderSettings VALUE_BATTLES{
        .x        = PROFILE_OFFSET_X + 178,
        .y        = PROFILE_OFFSET_Y + 48,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 25,
        .height   = 12,
        .alignX   = AlignmentX::RIGHT,
        .alignY   = AlignmentY::CENTER,
    };
    constexpr RenderSettings VALUE_CARE{
        .x        = PROFILE_OFFSET_X + 250,
        .y        = PROFILE_OFFSET_Y + 48,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 25,
        .height   = 12,
        .alignX   = AlignmentX::RIGHT,
        .alignY   = AlignmentY::CENTER,
    };
    constexpr RenderSettings VALUE_ACTIVE{
        .x        = PROFILE_OFFSET_X + 192,
        .y        = PROFILE_OFFSET_Y + 16,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 64,
        .height   = 12,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::CENTER,
    };

    constexpr RenderSettings VALUE_HP{
        .x        = STATS_OFFSET_X + 34,
        .y        = STATS_OFFSET_Y + 15,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 77,
        .height   = 8,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::CENTER,
    };
    constexpr RenderSettings VALUE_MP{
        .x        = STATS_OFFSET_X + 34,
        .y        = STATS_OFFSET_Y + 30,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 77,
        .height   = 8,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::CENTER,
    };
    constexpr RenderSettings VALUE_OFF{
        .x        = STATS_OFFSET_X + 34,
        .y        = STATS_OFFSET_Y + 45,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 42,
        .height   = 8,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::CENTER,
    };
    constexpr RenderSettings VALUE_DEF{
        .x        = STATS_OFFSET_X + 34,
        .y        = STATS_OFFSET_Y + 60,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 42,
        .height   = 8,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::CENTER,
    };
    constexpr RenderSettings VALUE_SPD{
        .x        = STATS_OFFSET_X + 34,
        .y        = STATS_OFFSET_Y + 75,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 42,
        .height   = 8,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::CENTER,
    };
    constexpr RenderSettings VALUE_BRN{
        .x        = STATS_OFFSET_X + 34,
        .y        = STATS_OFFSET_Y + 90,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 42,
        .height   = 8,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::CENTER,
    };

    constexpr RenderSettings VALUE_HAPPINESS{
        .x        = CONDITION_OFFSET_X + 62,
        .y        = CONDITION_OFFSET_Y + 49,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 48,
        .height   = 6,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::BOTTOM,
    };
    constexpr RenderSettings VALUE_DISCIPLINE{
        .x        = CONDITION_OFFSET_X + 62,
        .y        = CONDITION_OFFSET_Y + 63,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 48,
        .height   = 6,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::BOTTOM,
    };
    constexpr RenderSettings VALUE_TIREDNESS{
        .x        = CONDITION_OFFSET_X + 62,
        .y        = CONDITION_OFFSET_Y + 77,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 48,
        .height   = 6,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::BOTTOM,
    };
    constexpr RenderSettings VALUE_CURSE{
        .x        = CONDITION_OFFSET_X + 62,
        .y        = CONDITION_OFFSET_Y + 91,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 48,
        .height   = 6,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::BOTTOM,
    };

    IconSprite sprites[] = {
        // type
        {PROFILE_OFFSET_X + 263, PROFILE_OFFSET_Y + 16, 12, 12, 0, 128, 0, 5, getClut(0x60, 0x1e8 + 0)},
        // special 0
        {PROFILE_OFFSET_X + 224, PROFILE_OFFSET_Y + 31, 12, 12, 36, 128, 1, 5, getClut(0x60, 0x1e8 + 0)},
        // special 1
        {PROFILE_OFFSET_X + 240, PROFILE_OFFSET_Y + 31, 12, 12, 36, 128, 1, 5, getClut(0x60, 0x1e8 + 0)},
        // special 2
        {PROFILE_OFFSET_X + 256, PROFILE_OFFSET_Y + 31, 12, 12, 36, 128, 1, 5, getClut(0x60, 0x1e8 + 0)},
        // happiness
        {CONDITION_OFFSET_X, CONDITION_OFFSET_Y + 50, 11, 12, 0, 244, 0, 5, getClut(0x60, 0x1e8 + 10)},
        // discipline
        {CONDITION_OFFSET_X, CONDITION_OFFSET_Y + 64, 11, 12, 22, 244, 0, 5, getClut(0x60, 0x1e8 + 10)},
        // heart 1
        {CONDITION_OFFSET_X + 63, CONDITION_OFFSET_Y + 35, 12, 11, 232, 140, 0, 5, getClut(0x60, 0x1e8 + 6)},
        // heart 2
        {CONDITION_OFFSET_X + 80, CONDITION_OFFSET_Y + 35, 12, 11, 232, 140, 0, 5, getClut(0x60, 0x1e8 + 6)},
        // heart 3
        {CONDITION_OFFSET_X + 97, CONDITION_OFFSET_Y + 35, 12, 11, 232, 140, 0, 5, getClut(0x60, 0x1e8 + 6)},
        // sick
        {CONDITION_OFFSET_X + 14, CONDITION_OFFSET_Y + 12, 12, 12, 48, 140, 0, 5, getClut(0x60, 0x1e8 + 2)},
        // injury
        {CONDITION_OFFSET_X + 28, CONDITION_OFFSET_Y + 12, 12, 12, 60, 140, 0, 5, getClut(0x60, 0x1e8 + 0)},
        // hunger
        {CONDITION_OFFSET_X + 42, CONDITION_OFFSET_Y + 12, 12, 12, 72, 140, 0, 5, getClut(0x60, 0x1e8 + 0)},
        // sleep
        {CONDITION_OFFSET_X + 56, CONDITION_OFFSET_Y + 12, 12, 12, 84, 140, 0, 5, getClut(0x60, 0x1e8 + 0)},
        // unhappy
        {CONDITION_OFFSET_X + 70, CONDITION_OFFSET_Y + 12, 12, 12, 96, 140, 0, 5, getClut(0x60, 0x1e8 + 2)},
        // tired
        {CONDITION_OFFSET_X + 84, CONDITION_OFFSET_Y + 12, 12, 12, 108, 140, 0, 5, getClut(0x60, 0x1e8 + 1)},
        // poop
        {CONDITION_OFFSET_X + 98, CONDITION_OFFSET_Y + 12, 12, 12, 180, 128, 0, 5, getClut(0x60, 0x1e8 + 0)},
        // happiness 1
        {CONDITION_OFFSET_X + 62, CONDITION_OFFSET_Y + 53, 48, 5, 24, 240, 0, 24, getClut(0x70, 0x1f6)},
        // happiness 2
        {CONDITION_OFFSET_X + 62, CONDITION_OFFSET_Y + 53, 48, 5, 74, 240, 0, 24, getClut(0x70, 0x1f5)},
        // discipline 1
        {CONDITION_OFFSET_X + 62, CONDITION_OFFSET_Y + 67, 48, 5, 24, 240, 0, 24, getClut(0x70, 0x1f6)},
        // discipline 2
        {CONDITION_OFFSET_X + 62, CONDITION_OFFSET_Y + 67, 48, 5, 74, 240, 0, 24, getClut(0x70, 0x1f5)},

        {PROFILE_OFFSET_X, PROFILE_OFFSET_Y, 53, 11, 0, 203, 0, 5, getClut(0x60, 0x1e8 + 8)},
        {CONDITION_OFFSET_X, CONDITION_OFFSET_Y, 53, 11, 0, 203, 0, 5, getClut(0x60, 0x1e8 + 8)},
        {STATS_OFFSET_X, STATS_OFFSET_Y, 53, 11, 0, 203, 0, 5, getClut(0x60, 0x1e8 + 8)},
        // stat icons got moved from 147, 186 to 59, 255, to make space for atlas fonts
        {STATS_OFFSET_X, STATS_OFFSET_Y + 18, 11, 11, 59 + 0, 225, 0, 5, getClut(0x60, 0x1e8 + 11)},
        {STATS_OFFSET_X, STATS_OFFSET_Y + 33, 11, 11, 59 + 11, 225, 0, 5, getClut(0x60, 0x1e8 + 11)},
        {STATS_OFFSET_X, STATS_OFFSET_Y + 48, 11, 11, 59 + 22, 225, 0, 5, getClut(0x60, 0x1e8 + 11)},
        {STATS_OFFSET_X, STATS_OFFSET_Y + 63, 11, 11, 59 + 33, 225, 0, 5, getClut(0x60, 0x1e8 + 11)},
        {STATS_OFFSET_X, STATS_OFFSET_Y + 78, 11, 11, 59 + 44, 225, 0, 5, getClut(0x60, 0x1e8 + 11)},
        {STATS_OFFSET_X, STATS_OFFSET_Y + 93, 10, 11, 59 + 55, 225, 0, 5, getClut(0x60, 0x1e8 + 11)},
        {CONDITION_OFFSET_X + 2, CONDITION_OFFSET_Y + 78, 8, 11, 54, 203, 0, 5, getClut(0x60, 0x1e8 + 1)},
        {CONDITION_OFFSET_X + 1, CONDITION_OFFSET_Y + 92, 9, 12, 44, 244, 0, 5, getClut(0x60, 0x1e8 + 10)},
    };

    template<typename... Args>
    AtlasString buildString(const AtlasFont& font, const RenderSettings& settings, const char* format, Args... args)
    {
        uint8_t buffer[256];
        sprintf(buffer, format, args...);
        return font.render(buffer, settings);
    }
} // namespace

DigimonStatsView::DigimonStatsView()
{
    auto* para      = getDigimonData(PARTNER_ENTITY.type);
    auto* raisePara = getRaiseData(PARTNER_ENTITY.type);

    sprites[SpriteIndex::TYPE].uvX = static_cast<uint32_t>(para->type) * 12 - 12;
    initSpecialSprite(sprites[SpriteIndex::SPECIAL1], para->special[0]);
    initSpecialSprite(sprites[SpriteIndex::SPECIAL2], para->special[1]);
    initSpecialSprite(sprites[SpriteIndex::SPECIAL3], para->special[2]);
    sprites[SpriteIndex::HAPPINESS].uvX  = PARTNER_PARA.happiness < HAPPINESS_MID ? 11 : 0;
    sprites[SpriteIndex::DISCIPLINE].uvX = PARTNER_PARA.discipline < DISCIPLINE_MID ? 33 : 22;

    sprites[SpriteIndex::LIFE1].uvX = PARTNER_ENTITY.lives > 0 ? 232 : 244;
    sprites[SpriteIndex::LIFE2].uvX = PARTNER_ENTITY.lives > 1 ? 232 : 244;
    sprites[SpriteIndex::LIFE3].uvX = PARTNER_ENTITY.lives > 2 ? 232 : 244;

    sprites[SpriteIndex::SICK].height    = PARTNER_PARA.condition.isSick ? 12 : 0;
    sprites[SpriteIndex::INJURY].height  = PARTNER_PARA.condition.isInjured ? 12 : 0;
    sprites[SpriteIndex::HUNGER].height  = PARTNER_PARA.condition.isHungry ? 12 : 0;
    sprites[SpriteIndex::SLEEP].height   = PARTNER_PARA.condition.isSleepy ? 12 : 0;
    sprites[SpriteIndex::UNHAPPY].height = PARTNER_PARA.condition.isUnhappy ? 12 : 0;
    sprites[SpriteIndex::TIRED].height   = PARTNER_PARA.condition.isTired ? 12 : 0;
    sprites[SpriteIndex::POOP].height    = PARTNER_PARA.condition.isPoopy ? 12 : 0;

    // clang-format off
    auto happiness1  = clamp((PARTNER_PARA.happiness - HAPPINESS_MIN) * 100 / (HAPPINESS_MAX - HAPPINESS_MID), 0, 100);
    auto happiness2  = clamp((PARTNER_PARA.happiness - HAPPINESS_MID) * 100 / (HAPPINESS_MAX - HAPPINESS_MID), 0, 100);
    auto discipline1 = clamp((PARTNER_PARA.discipline - DISCIPLINE_MIN) * 100 / (DISCIPLINE_MAX - DISCIPLINE_MID), 0, 100);
    auto discipline2 = clamp((PARTNER_PARA.discipline - DISCIPLINE_MID) * 100 / (DISCIPLINE_MAX - DISCIPLINE_MID), 0, 100);
    // clang-format on

    sprites[SpriteIndex::HAPPINESS_BAR1].width  = happiness2 * 48 / 100;
    sprites[SpriteIndex::HAPPINESS_BAR2].width  = happiness1 * 48 / 100;
    sprites[SpriteIndex::DISCIPLINE_BAR1].width = discipline2 * 48 / 100;
    sprites[SpriteIndex::DISCIPLINE_BAR2].width = discipline1 * 48 / 100;

    int32_t index = 0;
    for (const auto& entry : LABELS_7PX)
        labels[index++] = getAtlas7px().render(entry.first, entry.second);
    for (const auto& entry : LABELS_5PX)
        labels[index++] = getAtlas5px().render(entry.first, entry.second);

    auto& pattern = SLEEP_PATTERN[raisePara->sleepCycle];

    labels[index++] = buildString(getAtlasVanilla(), VALUE_DIGIMON, "%s", para->name);
    labels[index++] = buildString(getAtlasVanilla(), VALUE_NAME, "%s", PARTNER_ENTITY.name);
    labels[index++] = buildString(getAtlasVanilla(), VALUE_AGE, "%d", PARTNER_PARA.age);
    labels[index++] = buildString(getAtlasVanilla(), VALUE_WEIGHT, "%d", PARTNER_PARA.weight);
    labels[index++] = buildString(getAtlasVanilla(), VALUE_BATTLES, "%d", PARTNER_PARA.battles);
    labels[index++] = buildString(getAtlasVanilla(), VALUE_CARE, "%d", PARTNER_PARA.careMistakes);
    labels[index++] = buildString(getAtlas7px(),
                                  VALUE_ACTIVE,
                                  "%02d:%02d - %02d:%02d",
                                  pattern.wakeupHour,
                                  pattern.wakeupMinute,
                                  pattern.sleepyHour,
                                  pattern.sleepyMinute);
    labels[index++] =
        buildString(getAtlas7px(), VALUE_HP, "%d  /  %d", PARTNER_ENTITY.stats.currentHP, PARTNER_ENTITY.stats.hp);
    labels[index++] =
        buildString(getAtlas7px(), VALUE_MP, "%d  /  %d", PARTNER_ENTITY.stats.currentMP, PARTNER_ENTITY.stats.mp);
    labels[index++] = buildString(getAtlas7px(), VALUE_OFF, "%d", PARTNER_ENTITY.stats.off);
    labels[index++] = buildString(getAtlas7px(), VALUE_DEF, "%d", PARTNER_ENTITY.stats.def);
    labels[index++] = buildString(getAtlas7px(), VALUE_SPD, "%d", PARTNER_ENTITY.stats.speed);
    labels[index++] = buildString(getAtlas7px(), VALUE_BRN, "%d", PARTNER_ENTITY.stats.brain);
    labels[index++] = buildString(getAtlas5px(), VALUE_HAPPINESS, "%d", PARTNER_PARA.happiness);
    labels[index++] = buildString(getAtlas5px(), VALUE_DISCIPLINE, "%d", PARTNER_PARA.discipline);
    labels[index++] = buildString(getAtlas5px(), VALUE_TIREDNESS, "%d", PARTNER_PARA.tiredness);
    labels[index++] = buildString(getAtlas5px(), VALUE_CURSE, "%d", PARTNER_PARA.virusBar);
}

void DigimonStatsView::tick()
{
    MENU_STATE = 1;
}

void DigimonStatsView::render()
{
    sprites[SpriteIndex::SICK].posY    = CONDITION_OFFSET_Y + CONDITION_OFFSETS[PLAYTIME_FRAMES % 16];
    sprites[SpriteIndex::INJURY].posY  = CONDITION_OFFSET_Y + CONDITION_OFFSETS[PLAYTIME_FRAMES % 16];
    sprites[SpriteIndex::HUNGER].posY  = CONDITION_OFFSET_Y + CONDITION_OFFSETS[PLAYTIME_FRAMES % 16];
    sprites[SpriteIndex::SLEEP].posY   = CONDITION_OFFSET_Y + CONDITION_OFFSETS[PLAYTIME_FRAMES % 16];
    sprites[SpriteIndex::UNHAPPY].posY = CONDITION_OFFSET_Y + CONDITION_OFFSETS[PLAYTIME_FRAMES % 16];
    sprites[SpriteIndex::TIRED].posY   = CONDITION_OFFSET_Y + CONDITION_OFFSETS[PLAYTIME_FRAMES % 16];
    sprites[SpriteIndex::POOP].posY    = CONDITION_OFFSET_Y + CONDITION_OFFSETS[PLAYTIME_FRAMES % 16];

    for (const auto& entry : labels)
        entry.render(5);

    for (auto& entry : sprites)
    {
        int16_t offsetX = 0;
        if ((entry.flag & 1) == 1 && PLAYTIME_FRAMES % 10 < 5) offsetX = entry.width;

        // clang-format off
        renderRectPolyFT4(entry.posX, entry.posY, entry.width, entry.height, entry.uvX + offsetX, entry.uvY, entry.texture_page, entry.clut, 5, 0);
    }

    renderDigimonStatsBar(PARTNER_ENTITY.stats.hp, HP_MAX, 77, STATS_OFFSET_X + 34, STATS_OFFSET_Y + 24);
    renderDigimonStatsBar(PARTNER_ENTITY.stats.mp, MP_MAX, 77, STATS_OFFSET_X + 34, STATS_OFFSET_Y + 39);
    renderDigimonStatsBar(PARTNER_ENTITY.stats.off, OFF_MAX, 42, STATS_OFFSET_X + 34, STATS_OFFSET_Y + 54);
    renderDigimonStatsBar(PARTNER_ENTITY.stats.def, DEF_MAX, 42, STATS_OFFSET_X + 34, STATS_OFFSET_Y + 69);
    renderDigimonStatsBar(PARTNER_ENTITY.stats.speed, SPEED_MAX, 42, STATS_OFFSET_X + 34, STATS_OFFSET_Y + 84);
    renderDigimonStatsBar(PARTNER_ENTITY.stats.brain, BRAIN_MAX, 42, STATS_OFFSET_X + 34, STATS_OFFSET_Y + 99);
    renderBox(CONDITION_OFFSET_X + 62, CONDITION_OFFSET_Y + 81, (PARTNER_PARA.tiredness * 48 / (TIREDNESS_MAX - TIREDNESS_MIN)), 5, 50, 150, 255, 0, 5);
    renderBox(CONDITION_OFFSET_X + 62, CONDITION_OFFSET_Y + 95, PARTNER_PARA.virusBar * 48 / (VIRUS_MAX - VIRUS_MIN), 5, 200, 200, 60, 0, 5);
    // clang-format on

    // profile insets
    for (auto& inset : insets)
        inset.render(5);

    renderSeperatorLines(lines, 6, 5);

    renderDigiviceEntity(&PARTNER_ENTITY, 1, -350);
}
