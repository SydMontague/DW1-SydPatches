#include "PlayerMedalView.hpp"

#include "AtlasFont.hpp"
#include "Helper.hpp"
#include "Input.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dtl/array.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"

namespace
{
    constexpr auto MEDAL_ROW_COUNT = 3;
    constexpr auto MEDAL_COL_COUNT = 5;

    constexpr Line LINES[] = {
        {.x1 = toRelativeX(229), .y1 = toRelativeY(55), .x2 = toRelativeX(229), .y2 = toRelativeY(134), .clut = 0},
        {.x1 = toRelativeX(230), .y1 = toRelativeY(55), .x2 = toRelativeX(230), .y2 = toRelativeY(134), .clut = 1},
        {.x1 = toRelativeX(231), .y1 = toRelativeY(55), .x2 = toRelativeX(231), .y2 = toRelativeY(134), .clut = 0},

        {.x1 = toRelativeX(13), .y1 = toRelativeY(53), .x2 = toRelativeX(306), .y2 = toRelativeY(53), .clut = 0},
        {.x1 = toRelativeX(13), .y1 = toRelativeY(54), .x2 = toRelativeX(306), .y2 = toRelativeY(54), .clut = 1},
        {.x1 = toRelativeX(13), .y1 = toRelativeY(55), .x2 = toRelativeX(306), .y2 = toRelativeY(55), .clut = 0},

        {.x1 = toRelativeX(13), .y1 = toRelativeY(134), .x2 = toRelativeX(306), .y2 = toRelativeY(134), .clut = 0},
        {.x1 = toRelativeX(13), .y1 = toRelativeY(135), .x2 = toRelativeX(306), .y2 = toRelativeY(135), .clut = 1},
        {.x1 = toRelativeX(13), .y1 = toRelativeY(136), .x2 = toRelativeX(306), .y2 = toRelativeY(136), .clut = 0},

        {.x1 = toRelativeX(13), .y1 = toRelativeY(199), .x2 = toRelativeX(306), .y2 = toRelativeY(199), .clut = 0},
        {.x1 = toRelativeX(13), .y1 = toRelativeY(200), .x2 = toRelativeX(306), .y2 = toRelativeY(200), .clut = 1},
        {.x1 = toRelativeX(13), .y1 = toRelativeY(201), .x2 = toRelativeX(306), .y2 = toRelativeY(201), .clut = 0},
    };

    constexpr Inset INSET = {
        .posX   = toRelativeX(62),
        .posY   = toRelativeY(141),
        .width  = 176,
        .height = 14,
    };

    constexpr Sprite SELECTOR = {
        .uvX          = 0,
        .uvV          = 0xD0,
        .width        = 24,
        .height       = 24,
        .texture_page = 24,
        .clut         = 0x7DC7,
    };

    constexpr Sprite MEDAL_FULL = {
        .uvX          = 0xF0,
        .uvV          = 0xB0,
        .width        = 15,
        .height       = 24,
        .texture_page = 24,
        .clut         = 0x7DC7,
    };

    constexpr Sprite MEDAL_EMPTY = {
        .uvX          = 0xF0,
        .uvV          = 0xC8,
        .width        = 15,
        .height       = 24,
        .texture_page = 24,
        .clut         = 0x7DC7,
    };

    constexpr dtl::array<Pair<const char*, RenderSettings>, 5> LABELS = {{
        "Medal Chart",
        {
            .x        = toRelativeX(14),
            .y        = toRelativeY(34),
            .baseClut = 1,
            .color    = TEXT_COLOR_YELLOW,
            .width    = 292,
            .height   = 19,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::CENTER,
        },
        "\x81\xa2",
        {
            .x        = toRelativeX(120),
            .y        = toRelativeY(202),
            .baseClut = 1,
            .color    = TEXT_COLOR_X_BLUE,
            .width    = 20,
            .height   = 16,
            .alignX   = AlignmentX::RIGHT,
            .alignY   = AlignmentY::CENTER,
        },
        "Select",
        {
            .x        = toRelativeX(140),
            .y        = toRelativeY(202),
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 64,
            .height   = 16,
            .alignX   = AlignmentX::LEFT,
            .alignY   = AlignmentY::CENTER,
        },
        "\x81\x7e",
        {
            .x        = toRelativeX(204),
            .y        = toRelativeY(202),
            .baseClut = 1,
            .color    = TEXT_COLOR_GREEN,
            .width    = 20,
            .height   = 16,
            .alignX   = AlignmentX::RIGHT,
            .alignY   = AlignmentY::CENTER,
        },
        "Cancel",
        {
            .x        = toRelativeX(224),
            .y        = toRelativeY(202),
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 64,
            .height   = 16,
            .alignX   = AlignmentX::LEFT,
            .alignY   = AlignmentY::CENTER,
        },
    }};

    constexpr RenderSettings TITLE_SETTINGS{
        .x        = toRelativeX(64),
        .y        = toRelativeY(142),
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 176,
        .height   = 14,
        .alignX   = AlignmentX::LEFT,
        .alignY   = AlignmentY::CENTER,
    };
    constexpr RenderSettings DESCRIPTION_SETTINGS{
        .x        = toRelativeX(32),
        .y        = toRelativeY(166),
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 255,
        .height   = 16,
        .alignX   = AlignmentX::LEFT,
        .alignY   = AlignmentY::CENTER,
    };

    constexpr char const* getMedalName(Medal medal)
    {
        switch (medal)
        {
            case Medal::GRADE_CUP: return "Grade Cup";
            case Medal::VERSION_CUP: return "Version Cup";
            case Medal::TYPE_CUP: return "Type Cup";
            case Medal::SPECIAL_CUP: return "Special Cup";
            case Medal::WIN_100_CUPS: return "Win 100 Cups";
            case Medal::ALL_TECHS: return "Techniques Master";
            case Medal::ALL_DIGIMON: return "Digimon Master";
            case Medal::MAX_STATS: return "Max Stats";
            case Medal::PERFECT_CURLING: return "Perfect Curling";
            case Medal::CATCH_100_FISH: return "Fishing Master";
            case Medal::BEAT_THE_GAME: return "Ending";
            case Medal::RECRUIT_EVERYONE: return "Town Flourishing";
            case Medal::ALL_CARDS: return "Card Master";
            case Medal::MAX_MONEY: return "Bits Maxed";
            case Medal::PLAYTIME: return "10 Years";
            case Medal::UNUSED1:
            case Medal::UNUSED2:
            case Medal::UNUSED3:
            case Medal::UNUSED4:
            case Medal::UNUSED5:
            default: return "";
        }
    }

    constexpr char const* getMedalDescription(Medal medal)
    {
        switch (medal)
        {
            case Medal::GRADE_CUP: return "Win all letter tournaments";
            case Medal::VERSION_CUP: return "Win all version tournaments";
            case Medal::TYPE_CUP: return "Win all nature tournaments";
            case Medal::SPECIAL_CUP: return "Win all special tournaments";
            case Medal::WIN_100_CUPS: return "Win 100 tournaments";
            case Medal::ALL_TECHS: return "Master all techniques";
            case Medal::ALL_DIGIMON: return "Raise all Digimon";
            case Medal::MAX_STATS: return "Max out all Digimon stats";
            case Medal::PERFECT_CURLING: return "Achieve a perfect curling score";
            case Medal::CATCH_100_FISH: return "Catch 100 fish";
            case Medal::BEAT_THE_GAME: return "Finish the game";
            case Medal::RECRUIT_EVERYONE: return "Bring everyone back to the city";
            case Medal::ALL_CARDS: return "Collect all cards";
            case Medal::MAX_MONEY: return "Collect 999999 bits";
            case Medal::PLAYTIME: return "Stay 10 years in the digital world";
            case Medal::UNUSED1:
            case Medal::UNUSED2:
            case Medal::UNUSED3:
            case Medal::UNUSED4:
            case Medal::UNUSED5:
            default: return "";
        }
    };

    constexpr dtl::array<AtlasString, 5> getLabels()
    {
        dtl::array<AtlasString, 5> array;
        int32_t i = 0;
        for (const auto& label : LABELS)
            array[i++] = getAtlasVanilla().render(LABELS[i].first, LABELS[i].second);
        return array;
    }

    constexpr void activateMedalTexture(Medal medal)
    {
        constexpr int16_t MEDAL_TEXTURE_WIDTH  = 6;
        constexpr int16_t MEDAL_TEXTURE_HEIGHT = 32;

        RECT image = {
            .x      = static_cast<int16_t>((static_cast<int32_t>(medal) % 9) * MEDAL_TEXTURE_WIDTH + 0x206),
            .y      = static_cast<int16_t>((static_cast<int32_t>(medal) / 9) * MEDAL_TEXTURE_HEIGHT + 0x1b0),
            .width  = MEDAL_TEXTURE_WIDTH,
            .height = MEDAL_TEXTURE_HEIGHT,
        };

        libgpu_MoveImage(&image, 0x200, 0x1B0);
        libgpu_DrawSync(0);
    }
} // namespace

struct MedalView::Private
{
public:
    void tick();
    void render(int32_t depth);
    bool canBeClosed();

private:
    inline void renderMedal(int32_t depth);
    void updateMedal(Medal medal);

    dtl::array<AtlasString, 5> labels{getLabels()};
    AtlasString titleString;
    AtlasString descriptionString;
    int8_t selectedMedalRow{0};
    int8_t selectedMedalCol{0};
    int8_t state{0};
    Medal selectedMedal{0};
    SVector medalRotation{};
};

void MedalView::Private::renderMedal(int32_t depth)
{
    constexpr GsRVIEW2 MEDAL_VIEW = {
        .viewpointX      = -1050,
        .viewpointY      = 220,
        .viewpointZ      = -10000,
        .refpointX       = -1050,
        .refpointY       = 220,
        .refpointZ       = 0,
        .viewpoint_twist = 0,
        .super           = nullptr,
    };

    FRAMEBUFFER_OT[0]->length = 9;
    FRAMEBUFFER_OT[0]->origin = FRAMEBUFFER0_ORIGIN;
    FRAMEBUFFER_OT[1]->length = 9;
    FRAMEBUFFER_OT[1]->origin = FRAMEBUFFER1_ORIGIN;

    libgs_GsSetProjection(0x400);
    libgs_GsSetRefView2(&MEDAL_VIEW);
    libgs_GsClearOt(0, 1, FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER]);

    medalRotation.y += 100;
    libgte_RotMatrix(&medalRotation, &MEDAL_COORDINATES.coord);
    MEDAL_COORDINATES.flag = 0;
    drawObject(&MEDAL_OBJECT, FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER], 5);
    libgs_GsSortOt(FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
    libgs_GsSetProjection(VIEWPORT_DISTANCE);
    libgs_GsSetRefView2(&GS_VIEWPOINT);
}

bool MedalView::Private::canBeClosed()
{
    return state == 0;
}

void MedalView::Private::render(int32_t depth)
{
    if (state == 1)
    {
        SELECTOR.render(selectedMedalCol * 0x26 - 0x7E, selectedMedalRow * 24 - 0x3D, 1, 0);
    }

    for (int32_t i = 0; i < MEDAL_COL_COUNT; i++)
        for (int32_t j = 0; j < MEDAL_ROW_COUNT; j++)
        {
            if (!hasMedal(static_cast<Medal>(j * MEDAL_COL_COUNT + i)))
                MEDAL_EMPTY.render(i * 0x26 - 0x7C, j * 24 - 0x3D, 5, 0);

            MEDAL_FULL.render(i * 0x26 - 0x7A, j * 24 - 0x3D, 5, 0);
        }

    if (hasMedal(selectedMedal)) renderMedal(depth);

    renderSeperatorLines(LINES, 12, 5);

    for (auto& text : labels)
        text.render(depth);

    // vanilla doesn't display text for medals you haven't unlocked yet
    titleString.render(depth);
    descriptionString.render(depth);

    INSET.render(5);
}

void MedalView::Private::updateMedal(Medal medal)
{
    titleString       = getAtlasVanilla().render(getMedalName(medal), TITLE_SETTINGS);
    descriptionString = getAtlasVanilla().render(getMedalDescription(medal), DESCRIPTION_SETTINGS);
}

void MedalView::Private::tick()
{
    if (state == 1)
    {
        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            playSound(0, 4);
            state = 0;
        }

        auto oldMedal = selectedMedal;

        if (isKeyDownRepeat(InputButtons::BUTTON_LEFT)) selectedMedalCol -= 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_RIGHT)) selectedMedalCol += 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_UP)) selectedMedalRow -= 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_DOWN)) selectedMedalRow += 1;

        selectedMedalCol = clamp(selectedMedalCol, 0, MEDAL_COL_COUNT - 1);
        selectedMedalRow = clamp(selectedMedalRow, 0, MEDAL_ROW_COUNT - 1);

        selectedMedal = static_cast<Medal>(selectedMedalRow * MEDAL_COL_COUNT + selectedMedalCol);
        if (selectedMedal != oldMedal)
        {
            updateMedal(selectedMedal);
            if (hasMedal(selectedMedal)) activateMedalTexture(selectedMedal);
            playSound(0, 2);
        }
    }
    else if (state == 0)
    {
        if (isKeyDown(InputButtons::BUTTON_CROSS))
        {
            updateMedal(selectedMedal);
            state = 1;
            playSound(0, 3);
        }
    }
}

MedalView::MedalView()
    : impl(dtl::make_unique<Private>())
{
}

MedalView::~MedalView() = default;

bool MedalView::canBeClosed()
{
    return impl->canBeClosed();
}

void MedalView::render(int32_t depth)
{
    impl->render(depth);
}

void MedalView::tick()
{
    impl->tick();
}