#include "PlayerInfoView.hpp"

#include "AtlasFont.hpp"
#include "Entity.hpp"
#include "Helper.hpp"
#include "UIElements.hpp"
#include "extern/dw1.hpp"

namespace
{
    constexpr auto BASE_X = toRelativeX(14);
    constexpr auto BASE_Y = toRelativeY(34);

    constexpr auto DETAIL_X = BASE_X + 105;
    constexpr auto DETAIL_Y = BASE_Y + 27;

    constexpr auto KEY_X = BASE_X + 105;
    constexpr auto KEY_Y = BASE_Y + 142;

    constexpr Inset INSETS[] = {
        {.posX = BASE_X + 70, .posY = BASE_Y + 3, .width = 120, .height = 14},

        {.posX = DETAIL_X + 67, .posY = DETAIL_Y, .width = 19, .height = 14},
        {.posX = DETAIL_X + 90, .posY = DETAIL_Y, .width = 92, .height = 14},

        {.posX = DETAIL_X + 67, .posY = DETAIL_Y + 18, .width = 32, .height = 14},
        {.posX = DETAIL_X + 67, .posY = DETAIL_Y + 36, .width = 64, .height = 14},
        {.posX = DETAIL_X + 67, .posY = DETAIL_Y + 54, .width = 64, .height = 14},
        {.posX = DETAIL_X + 67, .posY = DETAIL_Y + 72, .width = 64, .height = 14},
        {.posX = DETAIL_X + 67, .posY = DETAIL_Y + 90, .width = 32, .height = 14},

        {.posX = KEY_X + 67, .posY = KEY_Y, .width = 16, .height = 16},
        {.posX = KEY_X + 67 + 18, .posY = KEY_Y, .width = 16, .height = 16},
        {.posX = KEY_X + 67 + 36, .posY = KEY_Y, .width = 17, .height = 16},
        {.posX = KEY_X + 67 + 55, .posY = KEY_Y, .width = 17, .height = 16},

        {.posX = KEY_X + 67, .posY = KEY_Y + 20, .width = 32, .height = 14},
    };

    constexpr Line LINES[]{
        {.x1 = -45, .y1 = 50, .x2 = 146, .y2 = 50, .clut = 0},
        {.x1 = -45, .y1 = 51, .x2 = 146, .y2 = 51, .clut = 1},
        {.x1 = -45, .y1 = 52, .x2 = 146, .y2 = 52, .clut = 0},

        {.x1 = -47, .y1 = -63, .x2 = -47, .y2 = 98, .clut = 0},
        {.x1 = -46, .y1 = -63, .x2 = -46, .y2 = 98, .clut = 1},
        {.x1 = -45, .y1 = -63, .x2 = -45, .y2 = 98, .clut = 0},

        {.x1 = -147, .y1 = -65, .x2 = 146, .y2 = -65, .clut = 0},
        {.x1 = -147, .y1 = -64, .x2 = 146, .y2 = -64, .clut = 1},
        {.x1 = -147, .y1 = -63, .x2 = 146, .y2 = -63, .clut = 0},
    };

    constexpr IconSprite OLD_ROD_SPRITE = {
        .posX         = KEY_X + 67 + 1,
        .posY         = KEY_Y,
        .width        = 16,
        .height       = 16,
        .uvX          = 176,
        .uvY          = 160,
        .flag         = 0,
        .texture_page = 5,
        .clut         = 0x7bc6,
    };

    constexpr IconSprite AMAZING_ROD_SPRITE = {
        .posX         = KEY_X + 67 + 18 + 1,
        .posY         = KEY_Y,
        .width        = 16,
        .height       = 16,
        .uvX          = 192,
        .uvY          = 160,
        .flag         = 0,
        .texture_page = 5,
        .clut         = 0x7bc6,
    };

    constexpr IconSprite KEYCHAIN1_SPRITE = {
        .posX         = KEY_X + 67 + 36 + 1,
        .posY         = KEY_Y,
        .width        = 16,
        .height       = 16,
        .uvX          = 224,
        .uvY          = 160,
        .flag         = 0,
        .texture_page = 5,
        .clut         = 0x7bc6,
    };

    constexpr IconSprite KEYCHAIN2_SPRITE = {
        .posX         = KEY_X + 67 + 55 + 1,
        .posY         = KEY_Y,
        .width        = 16,
        .height       = 16,
        .uvX          = 208,
        .uvY          = 160,
        .flag         = 0,
        .texture_page = 5,
        .clut         = 0x7bc6,
    };

    constexpr dtl::array<Pair<const char*, RenderSettings>, 9> LABELS = {{
        {
            "Name",
            {
                .x        = BASE_X,
                .y        = BASE_Y + 3,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 64,
                .height   = 18,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "Level",
            {
                .x        = DETAIL_X,
                .y        = DETAIL_Y,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 64,
                .height   = 18,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "Raised",
            {
                .x        = DETAIL_X,
                .y        = DETAIL_Y + 18,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 64,
                .height   = 18,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "Bits",
            {
                .x        = DETAIL_X,
                .y        = DETAIL_Y + 36,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 64,
                .height   = 18,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "Merit",
            {
                .x        = DETAIL_X,
                .y        = DETAIL_Y + 54,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 64,
                .height   = 18,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "Playtime",
            {
                .x        = DETAIL_X,
                .y        = DETAIL_Y + 72,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 64,
                .height   = 18,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "Fish",
            {
                .x        = DETAIL_X,
                .y        = DETAIL_Y + 90,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 64,
                .height   = 18,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "Items",
            {
                .x        = KEY_X,
                .y        = KEY_Y,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 64,
                .height   = 18,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            },
        },
        {
            "Medals",
            {
                .x        = KEY_X,
                .y        = KEY_Y + 20,
                .baseClut = 1,
                .color    = TEXT_COLOR_LIGHT_YELLOW,
                .width    = 64,
                .height   = 18,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            },
        },
    }};

    constexpr RenderSettings NAME_VALUE{
        .x        = BASE_X + 72,
        .y        = BASE_Y + 3,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 118,
        .height   = 16,
        .alignX   = AlignmentX::LEFT,
        .alignY   = AlignmentY::CENTER,
    };

    constexpr RenderSettings LEVEL_VALUE{
        .x        = DETAIL_X + 69,
        .y        = DETAIL_Y,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 17,
        .height   = 16,
        .alignX   = AlignmentX::RIGHT,
        .alignY   = AlignmentY::CENTER,
    };

    constexpr RenderSettings LEVEL_NAME{
        .x        = DETAIL_X + 92,
        .y        = DETAIL_Y,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 90,
        .height   = 16,
        .alignX   = AlignmentX::LEFT,
        .alignY   = AlignmentY::CENTER,
    };

    constexpr RenderSettings RAISED_VALUE{
        .x        = DETAIL_X + 69,
        .y        = DETAIL_Y + 18,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 30,
        .height   = 16,
        .alignX   = AlignmentX::RIGHT,
        .alignY   = AlignmentY::CENTER,
    };

    constexpr RenderSettings BITS_VALUE{
        .x        = DETAIL_X + 69,
        .y        = DETAIL_Y + 36,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 62,
        .height   = 16,
        .alignX   = AlignmentX::RIGHT,
        .alignY   = AlignmentY::CENTER,
    };

    constexpr RenderSettings MERIT_VALUE{
        .x        = DETAIL_X + 69,
        .y        = DETAIL_Y + 54,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 62,
        .height   = 16,
        .alignX   = AlignmentX::RIGHT,
        .alignY   = AlignmentY::CENTER,
    };

    constexpr RenderSettings PLAYTIME_VALUE{
        .x        = DETAIL_X + 69,
        .y        = DETAIL_Y + 72,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 62,
        .height   = 16,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::CENTER,
    };

    constexpr RenderSettings FISH_VALUE{
        .x        = DETAIL_X + 69,
        .y        = DETAIL_Y + 90,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 30,
        .height   = 16,
        .alignX   = AlignmentX::RIGHT,
        .alignY   = AlignmentY::CENTER,
    };

    constexpr RenderSettings MEDAL_VALUE{
        .x        = KEY_X + 69,
        .y        = KEY_Y + 20,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 30,
        .height   = 16,
        .alignX   = AlignmentX::RIGHT,
        .alignY   = AlignmentY::CENTER,
    };

    constexpr char const* getTamerLevelName(uint8_t level)
    {
        switch (level)
        {
            default:
            case 0: return "Amateur";
            case 1: return "Novice";
            case 2: return "Beginner";
            case 3: return "Intermediate";
            case 4: return "Advanced";
            case 5: return "Veteran";
            case 6: return "Top Rate";
            case 7: return "Super";
            case 8: return "Master";
            case 9: return "Genius";
            case 10: return "Legend";
        }
    }

    int32_t getMedalCount()
    {
        int32_t medalCount = 0;
        for (int32_t i = 0; i < 20; i++)
        {
            if (hasMedal(static_cast<Medal>(i))) medalCount++;
        }

        return medalCount;
    }
} // namespace

PlayerInfoView::PlayerInfoView()
{
    const auto& font = getAtlasVanilla();
    int32_t i        = 0;
    for (const auto& entry : LABELS)
        strings[i++] = font.render(entry.first, entry.second);

    strings[i++] = font.render(getDigimonData(DigimonType::TAMER)->name, NAME_VALUE);
    strings[i++] = font.render(format("%d", TAMER_ENTITY.tamerLevel).data(), LEVEL_VALUE);
    strings[i++] = font.render(getTamerLevelName(TAMER_ENTITY.tamerLevel), LEVEL_NAME);
    strings[i++] = font.render(format("%d", MONEY).data(), BITS_VALUE);
    strings[i++] = font.render(format("%d", MERIT).data(), MERIT_VALUE);
    strings[i++] = font.render(format("%d : %02d", PLAYTIME_HOURS, PLAYTIME_MINUTES).data(), PLAYTIME_VALUE);
    strings[i++] = font.render(format("%d", PARTNER_PARA.fishCaught).data(), FISH_VALUE);
    strings[i++] = font.render(format("%d", TAMER_ENTITY.raisedCount).data(), RAISED_VALUE);
    strings[i++] = font.render(format("%d", getMedalCount()).data(), MEDAL_VALUE);
}

void PlayerInfoView::tick() {}

void PlayerInfoView::render(int32_t depth)
{
    renderDigiviceEntity(&TAMER_ENTITY, 0, 0);

    for (const auto& str : strings)
        str.render(depth);

    if (isTriggerSet(45)) OLD_ROD_SPRITE.render(5);
    if (isTriggerSet(46)) AMAZING_ROD_SPRITE.render(5);
    if (isTriggerSet(47)) KEYCHAIN1_SPRITE.render(5);
    if (isTriggerSet(48)) KEYCHAIN2_SPRITE.render(5);

    for (auto& inset : INSETS)
        inset.render(5);
    renderSeperatorLines(LINES, 9, 5);
}
