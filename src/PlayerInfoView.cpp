#include "Helper.hpp"
#include "UIElements.hpp"
#include "extern/dw1.hpp"

constexpr auto DRAW_X = 704;
constexpr auto DRAW_Y = 272;

constexpr auto BASE_X = toRelativeX(14);
constexpr auto BASE_Y = toRelativeY(34);

constexpr auto DETAIL_X = BASE_X + 105;
constexpr auto DETAIL_Y = BASE_Y + 27;

constexpr auto KEY_X = BASE_X + 105;
constexpr auto KEY_Y = BASE_Y + 142;

constexpr Inset insets[] = {
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

constexpr Line lines[]{
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

constexpr IconSprite oldRodSprite = {
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

constexpr IconSprite amazingRodSprite = {
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

constexpr IconSprite keychain1Sprite = {
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

constexpr IconSprite keychain2Sprite = {
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

static TextSprite labels[] = {
    {
        .font       = &vanillaFont,
        .string     = "Name",
        .uvX        = DRAW_X + 0,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = BASE_X + 3,
        .posY       = BASE_Y + 3,
        .boxWidth   = 64,
        .boxHeight  = 18,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 3,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &vanillaFont,
        .string     = "Level",
        .uvX        = DRAW_X + 16,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = DETAIL_X,
        .posY       = DETAIL_Y,
        .boxWidth   = 64,
        .boxHeight  = 18,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 3,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &vanillaFont,
        .string     = "Raised",
        .uvX        = DRAW_X + 32,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = DETAIL_X,
        .posY       = DETAIL_Y + 18,
        .boxWidth   = 64,
        .boxHeight  = 18,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 3,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &vanillaFont,
        .string     = "Bits",
        .uvX        = DRAW_X + 48,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = DETAIL_X,
        .posY       = DETAIL_Y + 36,
        .boxWidth   = 64,
        .boxHeight  = 18,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 3,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &vanillaFont,
        .string     = "Merit",
        .uvX        = DRAW_X + 0,
        .uvY        = DRAW_Y + 12,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = DETAIL_X,
        .posY       = DETAIL_Y + 54,
        .boxWidth   = 64,
        .boxHeight  = 18,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 3,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &vanillaFont,
        .string     = "Playtime",
        .uvX        = DRAW_X + 16,
        .uvY        = DRAW_Y + 12,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = DETAIL_X,
        .posY       = DETAIL_Y + 72,
        .boxWidth   = 64,
        .boxHeight  = 18,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 3,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &vanillaFont,
        .string     = "Fish",
        .uvX        = DRAW_X + 32,
        .uvY        = DRAW_Y + 12,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = DETAIL_X,
        .posY       = DETAIL_Y + 90,
        .boxWidth   = 64,
        .boxHeight  = 18,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 3,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &vanillaFont,
        .string     = "Items",
        .uvX        = DRAW_X + 48,
        .uvY        = DRAW_Y + 12,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = KEY_X,
        .posY       = KEY_Y,
        .boxWidth   = 64,
        .boxHeight  = 18,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 3,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &vanillaFont,
        .string     = "Medals",
        .uvX        = DRAW_X + 0,
        .uvY        = DRAW_Y + 24,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = KEY_X,
        .posY       = KEY_Y + 20,
        .boxWidth   = 64,
        .boxHeight  = 18,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 3,
        .layer      = 5,
        .hasShadow  = 1,
    },
};

static TextSprite nameText = {
    .font       = &vanillaFont,
    .string     = "",
    .uvX        = DRAW_X + 16,
    .uvY        = DRAW_Y + 24,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = BASE_X + 72,
    .posY       = BASE_Y + 3,
    .boxWidth   = 118,
    .boxHeight  = 16,
    .alignmentX = AlignmentX::LEFT,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite levelNumber = {
    .font       = &vanillaFont,
    .string     = "",
    .uvX        = DRAW_X + 0,
    .uvY        = DRAW_Y + 36,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = DETAIL_X + 69,
    .posY       = DETAIL_Y,
    .boxWidth   = 17,
    .boxHeight  = 16,
    .alignmentX = AlignmentX::RIGHT,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite levelName = {
    .font       = &vanillaFont,
    .string     = "",
    .uvX        = DRAW_X + 8,
    .uvY        = DRAW_Y + 36,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = DETAIL_X + 92,
    .posY       = DETAIL_Y,
    .boxWidth   = 90,
    .boxHeight  = 16,
    .alignmentX = AlignmentX::LEFT,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite raisedNumber = {
    .font       = &vanillaFont,
    .string     = "",
    .uvX        = DRAW_X + 48,
    .uvY        = DRAW_Y + 36,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = DETAIL_X + 69,
    .posY       = DETAIL_Y + 18,
    .boxWidth   = 30,
    .boxHeight  = 16,
    .alignmentX = AlignmentX::RIGHT,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite bitsNumber = {
    .font       = &vanillaFont,
    .string     = "",
    .uvX        = DRAW_X + 0,
    .uvY        = DRAW_Y + 48,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = DETAIL_X + 69,
    .posY       = DETAIL_Y + 36,
    .boxWidth   = 62,
    .boxHeight  = 16,
    .alignmentX = AlignmentX::RIGHT,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite meritNumber = {
    .font       = &vanillaFont,
    .string     = "",
    .uvX        = DRAW_X + 16,
    .uvY        = DRAW_Y + 48,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = DETAIL_X + 69,
    .posY       = DETAIL_Y + 54,
    .boxWidth   = 62,
    .boxHeight  = 16,
    .alignmentX = AlignmentX::RIGHT,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite playtimeNumber = {
    .font       = &vanillaFont,
    .string     = "",
    .uvX        = DRAW_X + 32,
    .uvY        = DRAW_Y + 48,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = DETAIL_X + 69,
    .posY       = DETAIL_Y + 72,
    .boxWidth   = 62,
    .boxHeight  = 16,
    .alignmentX = AlignmentX::CENTER,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite fishNumber = {
    .font       = &vanillaFont,
    .string     = "",
    .uvX        = DRAW_X + 0,
    .uvY        = DRAW_Y + 60,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = DETAIL_X + 69,
    .posY       = DETAIL_Y + 90,
    .boxWidth   = 30,
    .boxHeight  = 16,
    .alignmentX = AlignmentX::RIGHT,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite medalNumber = {
    .font       = &vanillaFont,
    .string     = "",
    .uvX        = DRAW_X + 16,
    .uvY        = DRAW_Y + 60,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = KEY_X + 69,
    .posY       = KEY_Y + 20,
    .boxWidth   = 30,
    .boxHeight  = 16,
    .alignmentX = AlignmentX::RIGHT,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
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

static int32_t getMedalCount()
{
    int32_t medalCount = 0;
    for (int32_t i = 0; i < 20; i++)
    {
        if (hasMedal(static_cast<Medal>(i))) medalCount++;
    }

    return medalCount;
}

void renderPlayerInfoView()
{
    if (MENU_STATE == 1)
    {
        renderDigiviceEntity(&TAMER_ENTITY, 0);

        renderTextSprite(nameText);
        renderTextSprite(levelNumber);
        renderTextSprite(levelName);
        renderTextSprite(raisedNumber);
        renderTextSprite(bitsNumber);
        renderTextSprite(meritNumber);
        renderTextSprite(playtimeNumber);
        renderTextSprite(fishNumber);
        renderTextSprite(medalNumber);

        if (isTriggerSet(45)) oldRodSprite.render(5);
        if (isTriggerSet(46)) amazingRodSprite.render(5);
        if (isTriggerSet(47)) keychain1Sprite.render(5);
        if (isTriggerSet(48)) keychain2Sprite.render(5);

        for (auto& label : labels)
            renderTextSprite(label);
        for (auto& inset : insets)
            inset.render(5);
        renderSeperatorLines(lines, 9, 5);
    }

    if (MENU_STATE == 0)
    {
        DIGIVICE_ENTITY_VIEW.refpointX = 0;
        for (auto& label : labels)
            drawTextSprite(label);

        nameText.string = reinterpret_cast<const char*>(getDigimonData(DigimonType::TAMER)->name);

        uint8_t buffer[8];
        sprintf(buffer, "%d", TAMER_ENTITY.tamerLevel);
        levelNumber.string = reinterpret_cast<const char*>(buffer);
        levelName.string   = getTamerLevelName(TAMER_ENTITY.tamerLevel);

        uint8_t buffer2[12];
        sprintf(buffer2, "%d", MONEY);
        bitsNumber.string = reinterpret_cast<const char*>(buffer2);

        uint8_t buffer3[12];
        sprintf(buffer3, "%d", MERIT);
        meritNumber.string = reinterpret_cast<const char*>(buffer3);

        uint8_t buffer4[12];
        sprintf(buffer4, "%d : %02d", PLAYTIME_HOURS, PLAYTIME_MINUTES);
        playtimeNumber.string = reinterpret_cast<const char*>(buffer4);

        uint8_t buffer5[8];
        sprintf(buffer5, "%d", PARTNER_PARA.fishCaught);
        fishNumber.string = reinterpret_cast<const char*>(buffer5);

        uint8_t buffer7[8];
        sprintf(buffer7, "%d", TAMER_ENTITY.raisedCount);
        raisedNumber.string = reinterpret_cast<const char*>(buffer7);

        uint8_t buffer6[8];
        sprintf(buffer6, "%d", getMedalCount());
        medalNumber.string = reinterpret_cast<const char*>(buffer6);

        drawTextSprite(nameText);
        drawTextSprite(levelNumber);
        drawTextSprite(levelName);
        drawTextSprite(raisedNumber);
        drawTextSprite(bitsNumber);
        drawTextSprite(meritNumber);
        drawTextSprite(playtimeNumber);
        drawTextSprite(fishNumber);
        drawTextSprite(medalNumber);

        MENU_STATE = 1;
    }
}