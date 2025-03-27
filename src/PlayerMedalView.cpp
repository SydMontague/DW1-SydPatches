#include "Helper.hpp"
#include "Math.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"

constexpr auto DRAW_X = 704;
constexpr auto DRAW_Y = 272;

constexpr auto MEDAL_ROW_COUNT = 3;
constexpr auto MEDAL_COL_COUNT = 5;

constexpr Line lines[] = {
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

constexpr Inset inset = {
    .posX   = toRelativeX(62),
    .posY   = toRelativeY(141),
    .width  = 176,
    .height = 14,
};

constexpr Sprite selector = {
    .uvX          = 0,
    .uvV          = 0xD0,
    .width        = 24,
    .height       = 24,
    .texture_page = 24,
    .clut         = 0x7DC7,
};

constexpr Sprite medalFull = {
    .uvX          = 0xF0,
    .uvV          = 0xB0,
    .width        = 15,
    .height       = 24,
    .texture_page = 24,
    .clut         = 0x7DC7,
};

constexpr Sprite medalEmpty = {
    .uvX          = 0xF0,
    .uvV          = 0xC8,
    .width        = 15,
    .height       = 24,
    .texture_page = 24,
    .clut         = 0x7DC7,
};

static TextSprite textLabels[] = {
    {
        .font       = &vanillaFont,
        .string     = "Medal Chart",
        .uvX        = DRAW_X + 0,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = toRelativeX(14),
        .posY       = toRelativeY(34),
        .boxWidth   = 292,
        .boxHeight  = 19,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::CENTER,
        .color      = 3,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &vanillaFont,
        .string     = "\x81\xa2",
        .uvX        = DRAW_X + 0,
        .uvY        = DRAW_Y + 12,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = toRelativeX(120),
        .posY       = toRelativeY(202),
        .boxWidth   = 20,
        .boxHeight  = 16,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 15,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &vanillaFont,
        .string     = "Select",
        .uvX        = DRAW_X + 32,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = toRelativeX(140),
        .posY       = toRelativeY(202),
        .boxWidth   = 64,
        .boxHeight  = 16,
        .alignmentX = AlignmentX::LEFT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &vanillaFont,
        .string     = "\x81\x7e",
        .uvX        = DRAW_X + 8,
        .uvY        = DRAW_Y + 12,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = toRelativeX(204),
        .posY       = toRelativeY(202),
        .boxWidth   = 20,
        .boxHeight  = 16,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 7,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &vanillaFont,
        .string     = "Cancel",
        .uvX        = DRAW_X + 48,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = toRelativeX(224),
        .posY       = toRelativeY(202),
        .boxWidth   = 64,
        .boxHeight  = 16,
        .alignmentX = AlignmentX::LEFT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
};

static TextSprite title = {

    .font       = &vanillaFont,
    .string     = "",
    .uvX        = DRAW_X + 16,
    .uvY        = DRAW_Y + 12,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = toRelativeX(64),
    .posY       = toRelativeY(142),
    .boxWidth   = 176,
    .boxHeight  = 14,
    .alignmentX = AlignmentX::LEFT,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite descriptionLine = {
    .font       = &vanillaFont,
    .string     = "",
    .uvX        = DRAW_X + 0,
    .uvY        = DRAW_Y + 24,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = toRelativeX(32),
    .posY       = toRelativeY(166),
    .boxWidth   = 255,
    .boxHeight  = 16,
    .alignmentX = AlignmentX::LEFT,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
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

static void updateMedal(Medal medal)
{
    title.string           = getMedalName(medal);
    descriptionLine.string = getMedalDescription(medal);

    drawTextSprite(title);
    drawTextSprite(descriptionLine);
}

static int8_t selectedMedalRow;
static int8_t selectedMedalCol;
static Medal selectedMedal;

void renderMedalView()
{
    if (MENU_STATE == 0)
    {
        clearTextSubArea2(0, 16, 256, 240);
        for (auto& text : textLabels)
            drawTextSprite(text);
        MENU_STATE = 1;
    }

    if (MENU_STATE == 2) { selector.render(selectedMedalCol * 0x26 - 0x7E, selectedMedalRow * 24 - 0x3D, 1, 0); }

    for (int32_t i = 0; i < MEDAL_COL_COUNT; i++)
        for (int32_t j = 0; j < MEDAL_ROW_COUNT; j++)
        {
            if (!hasMedal(static_cast<Medal>(j * MEDAL_COL_COUNT + i)))
                medalEmpty.render(i * 0x26 - 0x7C, j * 24 - 0x3D, 5, 0);

            medalFull.render(i * 0x26 - 0x7A, j * 24 - 0x3D, 5, 0);
        }

    if (hasMedal(selectedMedal)) renderDigiviceMedals();

    renderSeperatorLines(lines, 12, 5);

    for (auto& text : textLabels)
        renderTextSprite(text);

    // vanilla doesn't display text for medals you haven't unlocked yet
    renderTextSprite(title);
    renderTextSprite(descriptionLine);

    inset.render(5);
}

void tickPlayerMenuMedalView()
{
    if (MENU_STATE == 2)
    {
        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            playSound(0, 4);
            MENU_STATE = 1;
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
    else if (MENU_STATE == 1)
    {
        if (isKeyDown(InputButtons::BUTTON_CROSS))
        {
            updateMedal(selectedMedal);
            MENU_STATE = 2;
            playSound(0, 3);
        }
    }
}