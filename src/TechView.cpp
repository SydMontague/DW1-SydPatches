#include "TechView.hpp"

#include "AtlasFont.hpp"
#include "Entity.hpp"
#include "Font.hpp"
#include "Helper.hpp"
#include "Input.hpp"
#include "Math.hpp"
#include "Partner.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dtl/unique_ptr.hpp"
#include "extern/dw1.hpp"

// TODO: refactor the shit out of this, it's way too large

namespace
{
    constexpr auto MOVE_BOX_X        = -116;
    constexpr auto MOVE_BOX_Y        = -7;
    constexpr auto MOVE_BOX_OFFSET_X = 17;
    constexpr auto MOVE_BOX_OFFSET_Y = 15;

    constexpr Pair<const char*, RenderSettings> TECHSET_SETTINGS{
        "Techset",
        {
            .x        = toRelativeX(17),
            .y        = toRelativeY(36),
            .baseClut = 1,
            .color    = TEXT_COLOR_YELLOW,
            .width    = 80,
            .height   = 14,
            .alignX   = AlignmentX::LEFT,
            .alignY   = AlignmentY::CENTER,
        },
    };

    constexpr Pair<const char*, RenderSettings> FINISHER_SETTINGS{
        "Finisher",
        {
            .x        = toRelativeX(17),
            .y        = toRelativeY(105),
            .baseClut = 1,
            .color    = TEXT_COLOR_YELLOW,
            .width    = 80,
            .height   = 14,
            .alignX   = AlignmentX::LEFT,
            .alignY   = AlignmentY::CENTER,
        },
    };

    constexpr Pair<const char*, RenderSettings> TECH_DETAILS_SETTINGS{
        "Tech Details",
        {
            .x        = toRelativeX(187),
            .y        = toRelativeY(103),
            .baseClut = 1,
            .color    = TEXT_COLOR_YELLOW,
            .width    = 80,
            .height   = 14,
            .alignX   = AlignmentX::LEFT,
            .alignY   = AlignmentY::CENTER,
        },
    };

    constexpr dtl::array<Pair<const char*, RenderSettings>, 4> LABELS_5PX_TOP = {{
        "POWER",
        {
            .x        = toRelativeX(174),
            .y        = toRelativeY(40),
            .baseClut = 1,
            .color    = TEXT_COLOR_LIGHT_GREY,
            .width    = 36,
            .height   = 10,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::CENTER,
        },
        "COST",
        {
            .x        = toRelativeX(214),
            .y        = toRelativeY(40),
            .baseClut = 1,
            .color    = TEXT_COLOR_LIGHT_GREY,
            .width    = 36,
            .height   = 10,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::CENTER,
        },
        "RANGE",
        {
            .x        = toRelativeX(254),
            .y        = toRelativeY(40),
            .baseClut = 1,
            .color    = TEXT_COLOR_LIGHT_GREY,
            .width    = 22,
            .height   = 10,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::CENTER,
        },
        "STATUS",
        {
            .x        = toRelativeX(280),
            .y        = toRelativeY(40),
            .baseClut = 1,
            .color    = TEXT_COLOR_LIGHT_GREY,
            .width    = 22,
            .height   = 10,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::CENTER,
        },
    }};

    constexpr dtl::array<Pair<const char*, RenderSettings>, 3> LABELS_5PX_MID = {{
        "POWER",
        {
            .x        = toRelativeX(174),
            .y        = toRelativeY(109),
            .baseClut = 1,
            .color    = TEXT_COLOR_LIGHT_GREY,
            .width    = 36,
            .height   = 10,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::CENTER,
        },
        "RANGE",
        {
            .x        = toRelativeX(254),
            .y        = toRelativeY(109),
            .baseClut = 1,
            .color    = TEXT_COLOR_LIGHT_GREY,
            .width    = 22,
            .height   = 10,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::CENTER,
        },
        "STATUS",
        {
            .x        = toRelativeX(280),
            .y        = toRelativeY(109),
            .baseClut = 1,
            .color    = TEXT_COLOR_LIGHT_GREY,
            .width    = 22,
            .height   = 10,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::CENTER,
        },
    }};

    constexpr dtl::array<Pair<const char*, RenderSettings>, 19> LABELS_5PX_BOTTOM = {{
        "1",
        {
            .x        = MOVE_BOX_X + MOVE_BOX_OFFSET_X * 0,
            .y        = MOVE_BOX_Y - 10,
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 12,
            .height   = 8,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::BOTTOM,
        },
        "2",
        {
            .x        = MOVE_BOX_X + MOVE_BOX_OFFSET_X * 1,
            .y        = MOVE_BOX_Y - 10,
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 12,
            .height   = 8,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::BOTTOM,
        },
        "3",
        {
            .x        = MOVE_BOX_X + MOVE_BOX_OFFSET_X * 2,
            .y        = MOVE_BOX_Y - 10,
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 12,
            .height   = 8,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::BOTTOM,
        },
        "4",
        {
            .x        = MOVE_BOX_X + MOVE_BOX_OFFSET_X * 3,
            .y        = MOVE_BOX_Y - 10,
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 12,
            .height   = 8,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::BOTTOM,
        },
        "5",
        {
            .x        = MOVE_BOX_X + MOVE_BOX_OFFSET_X * 4,
            .y        = MOVE_BOX_Y - 10,
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 12,
            .height   = 8,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::BOTTOM,
        },
        "6",
        {
            .x        = MOVE_BOX_X + MOVE_BOX_OFFSET_X * 5,
            .y        = MOVE_BOX_Y - 10,
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 12,
            .height   = 8,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::BOTTOM,
        },
        "7",
        {
            .x        = MOVE_BOX_X + MOVE_BOX_OFFSET_X * 6,
            .y        = MOVE_BOX_Y - 10,
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 12,
            .height   = 8,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::BOTTOM,
        },
        "8",
        {
            .x        = MOVE_BOX_X + MOVE_BOX_OFFSET_X * 7,
            .y        = MOVE_BOX_Y - 10,
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 12,
            .height   = 8,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::BOTTOM,
        },

        "FIRE",
        {
            .x        = MOVE_BOX_X - 28,
            .y        = MOVE_BOX_Y + MOVE_BOX_OFFSET_Y * 0,
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 25,
            .height   = 12,
            .alignX   = AlignmentX::RIGHT,
            .alignY   = AlignmentY::CENTER,
        },
        "BATTLE",
        {
            .x        = MOVE_BOX_X - 28,
            .y        = MOVE_BOX_Y + MOVE_BOX_OFFSET_Y * 1,
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 25,
            .height   = 12,
            .alignX   = AlignmentX::RIGHT,
            .alignY   = AlignmentY::CENTER,
        },
        "AIR",
        {
            .x        = MOVE_BOX_X - 28,
            .y        = MOVE_BOX_Y + MOVE_BOX_OFFSET_Y * 2,
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 25,
            .height   = 12,
            .alignX   = AlignmentX::RIGHT,
            .alignY   = AlignmentY::CENTER,
        },
        "ICE",
        {
            .x        = MOVE_BOX_X - 28,
            .y        = MOVE_BOX_Y + MOVE_BOX_OFFSET_Y * 3,
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 25,
            .height   = 12,
            .alignX   = AlignmentX::RIGHT,
            .alignY   = AlignmentY::CENTER,
        },
        "EARTH",
        {
            .x        = MOVE_BOX_X - 28,
            .y        = MOVE_BOX_Y + MOVE_BOX_OFFSET_Y * 4,
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 25,
            .height   = 12,
            .alignX   = AlignmentX::RIGHT,
            .alignY   = AlignmentY::CENTER,
        },
        "MECH",
        {
            .x        = MOVE_BOX_X - 28,
            .y        = MOVE_BOX_Y + MOVE_BOX_OFFSET_Y * 5,
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 25,
            .height   = 12,
            .alignX   = AlignmentX::RIGHT,
            .alignY   = AlignmentY::CENTER,
        },
        "FILTH",
        {
            .x        = MOVE_BOX_X - 28,
            .y        = MOVE_BOX_Y + MOVE_BOX_OFFSET_Y * 6,
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 25,
            .height   = 12,
            .alignX   = AlignmentX::RIGHT,
            .alignY   = AlignmentY::CENTER,
        },

        "POWER",
        {
            .x        = toRelativeX(187),
            .y        = toRelativeY(128),
            .baseClut = 1,
            .color    = TEXT_COLOR_LIGHT_GREY,
            .width    = 36,
            .height   = 10,
            .alignX   = AlignmentX::LEFT,
            .alignY   = AlignmentY::CENTER,
        },
        "COST",
        {
            .x        = toRelativeX(187),
            .y        = toRelativeY(150),
            .baseClut = 1,
            .color    = TEXT_COLOR_LIGHT_GREY,
            .width    = 36,
            .height   = 10,
            .alignX   = AlignmentX::LEFT,
            .alignY   = AlignmentY::CENTER,
        },
        "RANGE",
        {
            .x        = toRelativeX(187),
            .y        = toRelativeY(172),
            .baseClut = 1,
            .color    = TEXT_COLOR_LIGHT_GREY,
            .width    = 22,
            .height   = 10,
            .alignX   = AlignmentX::LEFT,
            .alignY   = AlignmentY::CENTER,
        },
        "STATUS",
        {
            .x        = toRelativeX(187),
            .y        = toRelativeY(194),
            .baseClut = 1,
            .color    = TEXT_COLOR_LIGHT_GREY,
            .width    = 22,
            .height   = 10,
            .alignX   = AlignmentX::LEFT,
            .alignY   = AlignmentY::CENTER,
        },
    }};

    constexpr Sprite typeSprites[] = {
        {.uvX = 0, .uvV = 140, .width = 0, .height = 0, .texture_page = 5, .clut = getClut(0x60, 0x1e8)},
        {.uvX = 0, .uvV = 140, .width = 12, .height = 12, .texture_page = 5, .clut = getClut(0x60, 0x1e8)},
        {.uvX = 12, .uvV = 140, .width = 12, .height = 12, .texture_page = 5, .clut = getClut(0x60, 0x1e8)},
        {.uvX = 24, .uvV = 140, .width = 12, .height = 12, .texture_page = 5, .clut = getClut(0x60, 0x1e8)},
        {.uvX = 36, .uvV = 140, .width = 12, .height = 12, .texture_page = 5, .clut = getClut(0x60, 0x1e8)},
    };

    constexpr int32_t MOVE_ROW_COUNT  = 7;
    constexpr int32_t MOVE_COL_COUNT  = 8;
    constexpr uint32_t TYPE_MAPPING[] = {0, 5, 1, 4, 2, 3, 6, 7, 8};

    constexpr uint8_t MOVE_MATRIX[MOVE_ROW_COUNT][MOVE_COL_COUNT] = {
        {0, 1, 2, 3, 4, 5, 6, 7},
        {40, 41, 42, 43, 44, 45, 46, 47},
        {8, 9, 10, 11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20, 21, 22, 23},
        {32, 33, 34, 35, 36, 37, 38, 39},
        {24, 25, 26, 27, 28, 29, 30, 31},
        {49, 50, 51, 52, 53, 54, 55, 56},
    };

    constexpr Line lines[] = {
        {.x1 = -147, .y1 = -20, .x2 = 146, .y2 = -20, .clut = 0},
        {.x1 = -147, .y1 = -19, .x2 = 146, .y2 = -19, .clut = 1},
        {.x1 = -147, .y1 = -18, .x2 = 146, .y2 = -18, .clut = 0},
    };

    constexpr Line lines2[] = {
        {.x1 = -147, .y1 = 17, .x2 = 146, .y2 = 17, .clut = 0},
        {.x1 = -147, .y1 = 18, .x2 = 146, .y2 = 18, .clut = 1},
        {.x1 = -147, .y1 = 19, .x2 = 146, .y2 = 19, .clut = 0},
    };

    constexpr Line linesSeparator[] = {
        {.x1 = 20, .y1 = -18, .x2 = 20, .y2 = 98, .clut = 0},
        {.x1 = 21, .y1 = -18, .x2 = 21, .y2 = 98, .clut = 1},
        {.x1 = 22, .y1 = -18, .x2 = 22, .y2 = 98, .clut = 0},
    };

    constexpr Sprite specialSprites[] = {
        {.uvX = 36, .uvV = 128, .width = 12, .height = 12, .texture_page = 5, .clut = 0x7a06},
        {.uvX = 48, .uvV = 128, .width = 12, .height = 12, .texture_page = 5, .clut = 0x7a06},

        {.uvX = 60, .uvV = 128, .width = 12, .height = 12, .texture_page = 5, .clut = 0x7a06},
        {.uvX = 72, .uvV = 128, .width = 12, .height = 12, .texture_page = 5, .clut = 0x7a06},

        {.uvX = 84, .uvV = 128, .width = 12, .height = 12, .texture_page = 5, .clut = 0x7a46},
        {.uvX = 96, .uvV = 128, .width = 12, .height = 12, .texture_page = 5, .clut = 0x7a46},

        {.uvX = 108, .uvV = 128, .width = 12, .height = 12, .texture_page = 5, .clut = 0x7a46},
        {.uvX = 120, .uvV = 128, .width = 12, .height = 12, .texture_page = 5, .clut = 0x7a46},

        {.uvX = 132, .uvV = 128, .width = 12, .height = 12, .texture_page = 5, .clut = 0x7a46},
        {.uvX = 144, .uvV = 128, .width = 12, .height = 12, .texture_page = 5, .clut = 0x7a46},

        {.uvX = 156, .uvV = 128, .width = 12, .height = 12, .texture_page = 5, .clut = 0x7a86},
        {.uvX = 168, .uvV = 128, .width = 12, .height = 12, .texture_page = 5, .clut = 0x7a86},

        {.uvX = 180, .uvV = 128, .width = 12, .height = 12, .texture_page = 5, .clut = 0x7a06},
        {.uvX = 192, .uvV = 128, .width = 12, .height = 12, .texture_page = 5, .clut = 0x7a06},
    };

    constexpr Sprite selectorSprite =
        {.uvX = 192, .uvV = 140, .width = 18, .height = 16, .texture_page = 5, .clut = 0x7b06};

    constexpr bool canUseMove(DigimonType type, int32_t move)
    {
        return getDigimonData(type)->moves.contains(move);
    }

    constexpr int32_t getSpriteId(int32_t moveId)
    {
        if (moveId < 8) return 0;
        if (moveId < 16) return 2;
        if (moveId < 24) return 4;
        if (moveId < 32) return 5;
        if (moveId < 40) return 3;
        if (moveId < 49) return 1;
        return 6;
    }

    constexpr bool isSameMove(int32_t moveId1, int32_t moveId2)
    {
        if (moveId1 == moveId2) return true;
        if (moveId1 == 44 && moveId2 == 48) return true;
        if (moveId1 == 48 && moveId2 == 44) return true;
        if (moveId1 == 55 && moveId2 == 57) return true;
        if (moveId1 == 57 && moveId2 == 55) return true;
        return false;
    }

    constexpr bool hasMoveEquipped(DigimonEntity* entity, int32_t moveId)
    {
        for (auto move : entity->stats.moves)
            if (isSameMove(moveId, entityGetTechFromAnim(entity, move))) return true;

        return false;
    }

    constexpr char const* getRangeString(Range range)
    {
        switch (range)
        {
            default:
            case Range::UNDEFINED: return "?";
            case Range::SHORT: return "S";
            case Range::LARGE: return "L";
            case Range::WIDE: return "W";
            case Range::SELF: return "A";
        }
    }

    constexpr char const* getRangeStringLong(Range range)
    {
        switch (range)
        {
            default:
            case Range::UNDEFINED: return "?";
            case Range::SHORT: return "short";
            case Range::LARGE: return "long";
            case Range::WIDE: return "wide";
            case Range::SELF: return "self";
        }
    }

    constexpr char const* getStatusString(Status range)
    {
        switch (range)
        {
            default:
            case Status::NONE: return "";
            case Status::POISON: return "Poison";
            case Status::CONFUSE: return "Confuse";
            case Status::STUN: return "Stun";
            case Status::FLATTEN: return "Flatten";
        }
    }

    int32_t getMoveIndex(int32_t selectedMove)
    {
        auto* para = getDigimonData(PARTNER_ENTITY.type);
        for (int32_t i = 0; i < 16; i++)
        {
            auto move = para->moves[i];
            if (selectedMove == move) return i;
            if (selectedMove == 44 && move == 48) return i;
            if (selectedMove == 55 && move == 57) return i;
        }
        return -1;
    }

    int32_t getFirstFreeSlot()
    {
        for (int32_t i = 0; i < 3; i++)
        {
            if (PARTNER_ENTITY.stats.moves[i] == 0xFF) return i;
        }

        return -1;
    }

    int32_t getEquippedSlot(uint8_t move)
    {
        for (int32_t i = 0; i < 3; i++)
        {
            auto equipped = entityGetTechFromAnim(&PARTNER_ENTITY, PARTNER_ENTITY.stats.moves[i]);
            if (move == equipped) return i;
            if (move == 44 && equipped == 48) return i;
            if (move == 55 && equipped == 57) return i;
        }

        return -1;
    }

    constexpr RenderSettings
    getSettings(const Inset& inset, AlignmentX alignX = AlignmentX::LEFT, AlignmentY alignY = AlignmentY::CENTER)
    {
        return {
            .x        = static_cast<int16_t>(inset.posX + 3),
            .y        = static_cast<int16_t>(inset.posY + 2),
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = static_cast<int16_t>(inset.width - 4),
            .height   = static_cast<int16_t>(inset.height - 2),
            .alignX   = alignX,
            .alignY   = alignY,
        };
    }

    struct MoveDetail
    {
        static constexpr auto BASE_X = toRelativeX(186);
        static constexpr auto BASE_Y = toRelativeY(106);

        static constexpr Inset nameInset   = {.posX = BASE_X, .posY = BASE_Y + 10, .width = 117, .height = 10};
        static constexpr Inset powerInset  = {.posX = BASE_X, .posY = BASE_Y + 32, .width = 45, .height = 10};
        static constexpr Inset costInset   = {.posX = BASE_X, .posY = BASE_Y + 54, .width = 45, .height = 10};
        static constexpr Inset rangeInset  = {.posX = BASE_X, .posY = BASE_Y + 76, .width = 45, .height = 10};
        static constexpr Inset statusInset = {.posX = BASE_X, .posY = BASE_Y + 98, .width = 45, .height = 10};

        static constexpr RenderSettings nameSetting   = getSettings(nameInset);
        static constexpr RenderSettings powerSetting  = getSettings(powerInset);
        static constexpr RenderSettings costSetting   = getSettings(costInset);
        static constexpr RenderSettings rangeSetting  = getSettings(rangeInset);
        static constexpr RenderSettings statusSetting = getSettings(statusInset);

        dtl::array<AtlasString, 5> strings{};
        bool isValid;

        // NOLINTNEXTLINE: dunno why it doesn't know optimize...
        __attribute__((optimize("Os"))) void setMove(int32_t moveId)
        {
            isValid = moveId != 0xFF && hasMove(moveId);
            if (isValid)
            {
                const auto* move = getMove(moveId);
                const auto& font = getAtlas7px();
                strings[0]       = font.render(MOVE_NAMES[moveId], nameSetting);
                strings[1]       = font.render(format("%d", move->mpCost * 3).data(), costSetting);
                strings[2]       = font.render(format("%d", move->power).data(), powerSetting);
                strings[3]       = font.render(getRangeStringLong(move->range), rangeSetting);
                strings[4]       = font.render(getStatusString(move->status), statusSetting);
            }
        }

        void render()
        {
            if (isValid)
                for (const auto& string : strings)
                    string.render(5);

            nameInset.render(5);
            powerInset.render(5);
            costInset.render(5);
            rangeInset.render(5);
            statusInset.render(5);
        }
    };

    struct Dimension
    {
        int32_t width;
        int32_t height;
    };

    struct MoveEntry
    {
        static constexpr RECT nameDimension{.x = 0, .y = 0, .width = 150, .height = 14};
        static constexpr RECT powerDimension{.x = 155, .y = 0, .width = 35, .height = 14};
        static constexpr RECT rangeDimension{.x = 235, .y = 0, .width = 21, .height = 14};
        static constexpr RECT statusDimension{.x = 261, .y = 0, .width = 20, .height = 14};
        static constexpr RECT costDimension{.x = 195, .y = 0, .width = 35, .height = 14};

        Inset nameInset;
        Inset powerInset;
        Inset rangeInset;
        Inset statusInset;
        Inset costInset;
        int32_t baseX;
        int32_t baseY;
        bool isFinisher;

        AtlasString nameString;
        AtlasString powerString;
        AtlasString rangeString;
        AtlasString costString;
        bool isValid;
        Status status;

        MoveEntry(int32_t baseX, int32_t baseY, bool isFinisher = false)
            : baseX(baseX)
            , baseY(baseY)
            , isFinisher(isFinisher)
        {
            nameInset = {
                .posX   = static_cast<int16_t>(nameDimension.x + baseX),
                .posY   = static_cast<int16_t>(nameDimension.y + baseY),
                .width  = nameDimension.width,
                .height = nameDimension.height,
            };
            powerInset = {
                .posX   = static_cast<int16_t>(powerDimension.x + baseX),
                .posY   = static_cast<int16_t>(powerDimension.y + baseY),
                .width  = powerDimension.width,
                .height = powerDimension.height,
            };
            rangeInset = {
                .posX   = static_cast<int16_t>(rangeDimension.x + baseX),
                .posY   = static_cast<int16_t>(rangeDimension.y + baseY),
                .width  = rangeDimension.width,
                .height = rangeDimension.height,
            };
            statusInset = {
                .posX   = static_cast<int16_t>(statusDimension.x + baseX),
                .posY   = static_cast<int16_t>(statusDimension.y + baseY),
                .width  = statusDimension.width,
                .height = statusDimension.height,
            };
            costInset = {
                .posX   = static_cast<int16_t>(costDimension.x + baseX),
                .posY   = static_cast<int16_t>(costDimension.y + baseY),
                .width  = costDimension.width,
                .height = costDimension.height,
            };
        }

        // NOLINTNEXTLINE: dunno why it doesn't know optimize...
        __attribute__((optimize("Os"))) void setMove(uint8_t moveId)
        {
            isValid = moveId != 0xFF;
            if (isValid)
            {
                const auto* move = getMove(moveId);
                const auto& font = getAtlasVanilla();
                status           = move->status;
                nameString       = font.render(MOVE_NAMES[moveId], getSettings(nameInset));
                costString =
                    font.render(format("%d", move->mpCost * 3).data(), getSettings(costInset, AlignmentX::RIGHT));
                powerString = font.render(format("%d", move->power).data(), getSettings(powerInset, AlignmentX::RIGHT));
                rangeString = font.render(getRangeString(move->range), getSettings(rangeInset, AlignmentX::CENTER));
            }
            else
                status = Status::NONE;
        }

        void render(int32_t order)
        {
            if (isValid)
            {
                if (!isFinisher) costString.render(order);
                rangeString.render(order);
                powerString.render(order);
                nameString.render(order);
            }
            typeSprites[static_cast<uint8_t>(status)].render(statusInset.posX + 5, statusInset.posY + 2, 5, 0);

            if (!isFinisher) costInset.render(order);
            statusInset.render(order);
            rangeInset.render(order);
            powerInset.render(order);
            nameInset.render(order);
        }
    };

    void renderMoveBox(int32_t row, int32_t col)
    {
        const auto moveId = MOVE_MATRIX[row][col];
        const auto posX   = MOVE_BOX_X + MOVE_BOX_OFFSET_X * col;
        const auto posY   = MOVE_BOX_Y + MOVE_BOX_OFFSET_Y * row;

        auto index = getSpriteId(moveId) * 2;
        if (PLAYTIME_FRAMES % 20 < 10) index += 1;

        if (hasMoveEquipped(&PARTNER_ENTITY, moveId)) renderBox(posX, posY, 12, 12, 200, 0, 40, 1, 5);
        if (!canUseMove(PARTNER_ENTITY.type, moveId)) renderBox(posX, posY, 12, 12, 0x68, 0x68, 0x68, 3, 5);
        if (hasMove(moveId)) specialSprites[index].render(posX, posY, 5, 0);

        renderBorderBox(posX - 1, posY - 1, 13, 13, 0x020202, 0x20202, 0x4E, 0x60, 0x6E, 5);
    }

    void renderMoveBoxes()
    {
        for (int32_t row = 0; row < 7; row++)
            for (int32_t col = 0; col < 8; col++)
                renderMoveBox(row, col);
    }
} // namespace

struct Private
{
    dtl::array<AtlasString, 5> top;
    dtl::array<AtlasString, 4> middle;
    dtl::array<AtlasString, 20> bottom;
    MoveEntry move0{toRelativeX(19), toRelativeY(50)};
    MoveEntry move1{toRelativeX(19), toRelativeY(66)};
    MoveEntry move2{toRelativeX(19), toRelativeY(82)};
    MoveEntry moveFinish{toRelativeX(19), toRelativeY(119), true};
    MoveDetail detail{};
    int8_t moveSelectedRow = 0;
    int8_t moveSelectedColumn = 0;

    uint8_t state = 0;
    // NOLINTNEXTLINE
    __attribute__((optimize("Os"))) Private();

    void render(int32_t depth);
    void tick();
    void equipMove(uint8_t selectedMove);
};

Private::Private()
{
    {
        int32_t i = 0;
        top[i++]  = getAtlasVanilla().render(TECHSET_SETTINGS.first, TECHSET_SETTINGS.second);
        for (const auto& entry : LABELS_5PX_TOP)
            top[i++] = getAtlas5px().render(entry.first, entry.second);
    }
    {
        int32_t i   = 0;
        middle[i++] = getAtlasVanilla().render(FINISHER_SETTINGS.first, FINISHER_SETTINGS.second);
        for (const auto& entry : LABELS_5PX_MID)
            middle[i++] = getAtlas5px().render(entry.first, entry.second);
    }
    {
        int32_t i   = 0;
        bottom[i++] = getAtlas7px().render(TECH_DETAILS_SETTINGS.first, TECH_DETAILS_SETTINGS.second);
        for (const auto& entry : LABELS_5PX_BOTTOM)
            bottom[i++] = getAtlas5px().render(entry.first, entry.second);
    }

    move0.setMove(entityGetTechFromAnim(&PARTNER_ENTITY, PARTNER_ENTITY.stats.moves[0]));
    move1.setMove(entityGetTechFromAnim(&PARTNER_ENTITY, PARTNER_ENTITY.stats.moves[1]));
    move2.setMove(entityGetTechFromAnim(&PARTNER_ENTITY, PARTNER_ENTITY.stats.moves[2]));
    moveFinish.setMove(entityGetTechFromAnim(&PARTNER_ENTITY, PARTNER_ENTITY.stats.moves[3]));
    detail.setMove(MOVE_MATRIX[moveSelectedRow][moveSelectedColumn]);
}

void Private::equipMove(uint8_t selectedMove)
{
    if (!hasMove(selectedMove))
    {
        playSound(0, 4);
        return;
    }

    auto moveIndex = getMoveIndex(selectedMove);
    if (moveIndex == -1)
    {
        playSound(0, 4);
        return;
    }

    auto slot = getFirstFreeSlot();
    if (slot == -1)
    {
        playSound(0, 4);
        return;
    }

    PARTNER_ENTITY.stats.moves[slot] = moveIndex + 0x2E;
    switch (slot)
    {
        case 0: move0.setMove(selectedMove); break;
        case 1: move1.setMove(selectedMove); break;
        case 2: move2.setMove(selectedMove); break;
    }
    playSound(0, 3);
}

void Private::render(int32_t depth)
{
    if (state == 0)
    {
        for (const auto& entry : middle)
            entry.render(depth);
        moveFinish.render(depth);
        renderSeperatorLines(lines2, 3, depth);
    }
    if (state == 3)
    {
        for (const auto& entry : bottom)
            entry.render(depth);
        detail.render();
        selectorSprite.render(MOVE_BOX_X + MOVE_BOX_OFFSET_X * moveSelectedColumn - 3,
                              MOVE_BOX_Y + MOVE_BOX_OFFSET_Y * moveSelectedRow - 2,
                              depth,
                              0);
        renderMoveBoxes();
        renderSeperatorLines(linesSeparator, 3, depth);
    }

    // always visible
    for (const auto& entry : top)
        entry.render(depth);
    move0.render(depth);
    move1.render(depth);
    move2.render(depth);
    renderSeperatorLines(lines, 3, depth);
}

void Private::tick()
{
    if (state == 0 && isKeyDown(InputButtons::BUTTON_CROSS))
    {
        state = 1;
        playSound(0, 3);
    }

    if (state == 5)
    {
        UI_BOX_DATA[1].finalPos.height += 39;
        if (UI_BOX_DATA[1].finalPos.height > 189) state = 0;
    }
    if (state == 2)
    {
        UI_BOX_DATA[1].finalPos.height += 39;
        if (UI_BOX_DATA[1].finalPos.height > 189) state = 3;
    }
    if (state == 1 || state == 4)
    {
        UI_BOX_DATA[1].finalPos.height -= 39;
        if (UI_BOX_DATA[1].finalPos.height < 74) state++;
    }

    if (state == 3)
    {
        if (isKeyDown(InputButtons::BUTTON_CROSS))
        {
            auto slot = getEquippedSlot(MOVE_MATRIX[moveSelectedRow][moveSelectedColumn]);
            if (slot == -1)
                equipMove(MOVE_MATRIX[moveSelectedRow][moveSelectedColumn]);
            else
            {
                switch (slot)
                {
                    case 0: move0.setMove(0xFF); break;
                    case 1: move1.setMove(0xFF); break;
                    case 2: move2.setMove(0xFF); break;
                }
                PARTNER_ENTITY.stats.moves[slot] = 0xFF;
                playSound(0, 3);
            }
        }

        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            if (hasAttackEquipped(&PARTNER_ENTITY)) state = 4;
            playSound(0, 4);
        }

        auto oldRow = moveSelectedRow;
        auto oldCol = moveSelectedColumn;
        if (isKeyDownRepeat(InputButtons::BUTTON_RIGHT)) moveSelectedColumn += 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_LEFT)) moveSelectedColumn -= 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_DOWN)) moveSelectedRow += 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_UP)) moveSelectedRow -= 1;

        moveSelectedRow    = clamp(moveSelectedRow, 0, MOVE_ROW_COUNT - 1);
        moveSelectedColumn = clamp(moveSelectedColumn, 0, MOVE_COL_COUNT - 1);

        if (moveSelectedRow != oldRow || moveSelectedColumn != oldCol)
        {
            playSound(0, 2);
            detail.setMove(MOVE_MATRIX[moveSelectedRow][moveSelectedColumn]);
        }
    }
}

TechView::TechView()
    : pimpl(dtl::make_unique<Private>())
{
}

TechView::~TechView() = default;

bool TechView::canBeClosed()
{
    return pimpl->state == 0;
}

void TechView::render(int32_t depth)
{
    pimpl->render(depth);
}

void TechView::tick()
{
    pimpl->tick();
}