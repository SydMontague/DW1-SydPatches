#include "TechView.hpp"

#include "Entity.hpp"
#include "Font.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"

// TODO: refactor the shit out of this, it's way too large

constexpr auto DRAW_X = 704;
constexpr auto DRAW_Y = 272;

constexpr auto MOVE_BOX_BASE_X   = -116;
constexpr auto MOVE_BOX_BASE_Y   = -7;
constexpr auto MOVE_BOX_OFFSET_X = 17;
constexpr auto MOVE_BOX_OFFSET_Y = 15;

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

static char const* getRangeString(Range range)
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

static char const* getRangeStringLong(Range range)
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

static char const* getStatusString(Status range)
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

struct MoveDetail
{
    static constexpr auto BASE_X = toRelativeX(186);
    static constexpr auto BASE_Y = toRelativeY(106);

    static constexpr Inset nameInset   = {.posX = BASE_X, .posY = BASE_Y + 10, .width = 117, .height = 10};
    static constexpr Inset powerInset  = {.posX = BASE_X, .posY = BASE_Y + 32, .width = 45, .height = 10};
    static constexpr Inset costInset   = {.posX = BASE_X, .posY = BASE_Y + 54, .width = 45, .height = 10};
    static constexpr Inset rangeInset  = {.posX = BASE_X, .posY = BASE_Y + 76, .width = 45, .height = 10};
    static constexpr Inset statusInset = {.posX = BASE_X, .posY = BASE_Y + 98, .width = 45, .height = 10};

    TextSprite nameSprite = {
        .font       = &myFont7px,
        .string     = "",
        .uvX        = DRAW_X + 0,
        .uvY        = DRAW_Y + 34,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = static_cast<int16_t>(nameInset.posX + 3),
        .posY       = static_cast<int16_t>(nameInset.posY + 2),
        .boxWidth   = static_cast<uint16_t>(nameInset.width - 4),
        .boxHeight  = static_cast<uint16_t>(nameInset.height - 2),
        .alignmentX = AlignmentX::LEFT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    };

    TextSprite powerSprite = {
        .font       = &myFont7px,
        .string     = "",
        .uvX        = DRAW_X + 32,
        .uvY        = DRAW_Y + 34,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = static_cast<int16_t>(powerInset.posX + 3),
        .posY       = static_cast<int16_t>(powerInset.posY + 2),
        .boxWidth   = static_cast<uint16_t>(powerInset.width - 4),
        .boxHeight  = static_cast<uint16_t>(powerInset.height - 2),
        .alignmentX = AlignmentX::LEFT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    };

    TextSprite costSprite = {
        .font       = &myFont7px,
        .string     = "",
        .uvX        = DRAW_X + 36,
        .uvY        = DRAW_Y + 34,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = static_cast<int16_t>(costInset.posX + 3),
        .posY       = static_cast<int16_t>(costInset.posY + 2),
        .boxWidth   = static_cast<uint16_t>(costInset.width - 4),
        .boxHeight  = static_cast<uint16_t>(costInset.height - 2),
        .alignmentX = AlignmentX::LEFT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    };

    TextSprite rangeSprite = {
        .font       = &myFont7px,
        .string     = "",
        .uvX        = DRAW_X + 40,
        .uvY        = DRAW_Y + 34,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = static_cast<int16_t>(rangeInset.posX + 3),
        .posY       = static_cast<int16_t>(rangeInset.posY + 2),
        .boxWidth   = static_cast<uint16_t>(rangeInset.width - 4),
        .boxHeight  = static_cast<uint16_t>(rangeInset.height - 2),
        .alignmentX = AlignmentX::LEFT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    };

    TextSprite statusSprite = {
        .font       = &myFont7px,
        .string     = "",
        .uvX        = DRAW_X + 48,
        .uvY        = DRAW_Y + 34,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = static_cast<int16_t>(statusInset.posX + 3),
        .posY       = static_cast<int16_t>(statusInset.posY + 2),
        .boxWidth   = static_cast<uint16_t>(statusInset.width - 4),
        .boxHeight  = static_cast<uint16_t>(statusInset.height - 2),
        .alignmentX = AlignmentX::LEFT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    };

    void setMove(int32_t moveId)
    {
        if (moveId == 0xFF || !hasMove(moveId))
        {
            costSprite.string   = "";
            nameSprite.string   = "";
            powerSprite.string  = "";
            rangeSprite.string  = "";
            statusSprite.string = "";
        }
        else
        {
            auto* move        = getMove(moveId);
            nameSprite.string = reinterpret_cast<char*>(MOVE_NAMES[moveId]);

            uint8_t buffer2[8];
            sprintf(buffer2, "%d", move->mpCost * 3);
            costSprite.string = reinterpret_cast<const char*>(buffer2);
            uint8_t buffer1[8];
            sprintf(buffer1, "%d", move->power);
            powerSprite.string  = reinterpret_cast<const char*>(buffer1);
            rangeSprite.string  = getRangeStringLong(move->range);
            statusSprite.string = getStatusString(move->status);
        }

        auto* move = getMove(moveId);
        clearTextSubArea2(0, 16 + 34, 255, 0);
        drawTextSprite(nameSprite);
        drawTextSprite(powerSprite);
        drawTextSprite(costSprite);
        drawTextSprite(rangeSprite);
        drawTextSprite(statusSprite);
    }

    void render()
    {
        renderTextSprite(nameSprite);
        renderTextSprite(powerSprite);
        renderTextSprite(costSprite);
        renderTextSprite(rangeSprite);
        renderTextSprite(statusSprite);

        nameInset.render(5);
        powerInset.render(5);
        costInset.render(5);
        rangeInset.render(5);
        statusInset.render(5);
    }
};

template<int32_t drawY, int32_t offsetY> struct MoveEntry
{
    static constexpr auto BASE_X = toRelativeX(19);
    static constexpr auto BASE_Y = toRelativeY(50);

    static constexpr Inset nameInset   = {.posX = BASE_X, .posY = BASE_Y + offsetY, .width = 150, .height = 14};
    static constexpr Inset powerInset  = {.posX = BASE_X + 155, .posY = BASE_Y + offsetY, .width = 35, .height = 14};
    static constexpr Inset rangeInset  = {.posX = BASE_X + 235, .posY = BASE_Y + offsetY, .width = 21, .height = 14};
    static constexpr Inset statusInset = {.posX = BASE_X + 261, .posY = BASE_Y + offsetY, .width = 20, .height = 14};

    TextSprite nameSprite = {
        .font       = &vanillaFont,
        .string     = "",
        .uvX        = DRAW_X + 0,
        .uvY        = DRAW_Y + drawY,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = static_cast<int16_t>(nameInset.posX + 3),
        .posY       = static_cast<int16_t>(nameInset.posY + 2),
        .boxWidth   = static_cast<uint16_t>(nameInset.width - 4),
        .boxHeight  = static_cast<uint16_t>(nameInset.height - 2),
        .alignmentX = AlignmentX::LEFT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    };

    TextSprite powerSprite = {
        .font       = &vanillaFont,
        .string     = "",
        .uvX        = DRAW_X + 32,
        .uvY        = DRAW_Y + drawY,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = static_cast<int16_t>(powerInset.posX + 3),
        .posY       = static_cast<int16_t>(powerInset.posY + 2),
        .boxWidth   = static_cast<uint16_t>(powerInset.width - 4),
        .boxHeight  = static_cast<uint16_t>(powerInset.height - 2),
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    };

    TextSprite rangeSprite = {
        .font       = &vanillaFont,
        .string     = "",
        .uvX        = DRAW_X + 48,
        .uvY        = DRAW_Y + drawY,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = static_cast<int16_t>(rangeInset.posX + 3),
        .posY       = static_cast<int16_t>(rangeInset.posY + 2),
        .boxWidth   = static_cast<uint16_t>(rangeInset.width - 4),
        .boxHeight  = static_cast<uint16_t>(rangeInset.height - 2),
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    };

    Status status;

    void setMove(uint8_t moveId)
    {
        uint8_t buffer1[8];
        clearTextSubArea2(0, 16 + drawY, 256, 12);

        if (moveId == 0xFF)
        {
            nameSprite.string  = "";
            powerSprite.string = "";
            rangeSprite.string = "";
            status             = Status::NONE;
        }
        else
        {
            auto* move        = getMove(moveId);
            nameSprite.string = reinterpret_cast<char*>(MOVE_NAMES[moveId]);

            sprintf(buffer1, "%d", move->power);
            powerSprite.string = reinterpret_cast<const char*>(buffer1);
            rangeSprite.string = getRangeString(move->range);
            status             = move->status;
        }

        drawTextSprite(nameSprite);
        drawTextSprite(powerSprite);
        drawTextSprite(rangeSprite);
    }

    void render(int32_t order)
    {
        renderTextSprite(nameSprite);
        renderTextSprite(powerSprite);
        renderTextSprite(rangeSprite);

        typeSprites[static_cast<uint8_t>(status)].render(statusInset.posX + 5, statusInset.posY + 2, 5, 0);

        nameInset.render(order);
        powerInset.render(order);
        rangeInset.render(order);
        statusInset.render(order);
    }
};

template<int32_t drawY, int32_t offsetY> struct MoveRegularEntry : MoveEntry<drawY, offsetY>
{
    using Parent = MoveEntry<drawY, offsetY>;

    static constexpr Inset mpInset = {
        .posX   = Parent::BASE_X + 195,
        .posY   = Parent::BASE_Y + offsetY,
        .width  = 35,
        .height = 14,
    };

    TextSprite costSprite = {
        .font       = &vanillaFont,
        .string     = "",
        .uvX        = DRAW_X + 40,
        .uvY        = DRAW_Y + drawY,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = static_cast<int16_t>(mpInset.posX + 3),
        .posY       = static_cast<int16_t>(mpInset.posY + 2),
        .boxWidth   = static_cast<uint16_t>(mpInset.width - 4),
        .boxHeight  = static_cast<uint16_t>(mpInset.height - 2),
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    };

    void setMove(uint8_t moveId)
    {
        uint8_t buffer2[8];

        Parent::setMove(moveId);

        if (moveId == 0xFF)
            costSprite.string = "";
        else
        {
            auto* move = getMove(moveId);
            sprintf(buffer2, "%d", move->mpCost * 3);
            costSprite.string = reinterpret_cast<const char*>(buffer2);
        }

        drawTextSprite(costSprite);
    }

    void render(int32_t order)
    {
        Parent::render(order);
        renderTextSprite(costSprite);
        mpInset.render(order);
    }
};

static TextSprite techsetLabel = {
    .font       = &vanillaFont,
    .string     = "Techset",
    .uvX        = DRAW_X + 0,
    .uvY        = DRAW_Y + 0,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = toRelativeX(17),
    .posY       = toRelativeY(36),
    .boxWidth   = 80,
    .boxHeight  = 14,
    .alignmentX = AlignmentX::LEFT,
    .alignmentY = AlignmentY::CENTER,
    .color      = 3,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite finisherLabel = {
    .font       = &vanillaFont,
    .string     = "Finisher",
    .uvX        = DRAW_X + 16,
    .uvY        = DRAW_Y + 0,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = toRelativeX(17),
    .posY       = toRelativeY(105),
    .boxWidth   = 80,
    .boxHeight  = 14,
    .alignmentX = AlignmentX::LEFT,
    .alignmentY = AlignmentY::CENTER,
    .color      = 3,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite techDetailsLabel = {
    .font       = &myFont7px,
    .string     = "Tech Details",
    .uvX        = DRAW_X + 32,
    .uvY        = DRAW_Y + 6,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = toRelativeX(187),
    .posY       = toRelativeY(103),
    .boxWidth   = 80,
    .boxHeight  = 14,
    .alignmentX = AlignmentX::LEFT,
    .alignmentY = AlignmentY::CENTER,
    .color      = 3,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite matrixLabels[] = {
    {
        .font       = &myFont5px,
        .string     = "1",
        .uvX        = DRAW_X + 32,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 0,
        .posY       = MOVE_BOX_BASE_Y - 10,
        .boxWidth   = 12,
        .boxHeight  = 8,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::BOTTOM,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "2",
        .uvX        = DRAW_X + 34,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 1,
        .posY       = MOVE_BOX_BASE_Y - 10,
        .boxWidth   = 12,
        .boxHeight  = 8,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::BOTTOM,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "3",
        .uvX        = DRAW_X + 36,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 2,
        .posY       = MOVE_BOX_BASE_Y - 10,
        .boxWidth   = 12,
        .boxHeight  = 8,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::BOTTOM,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "4",
        .uvX        = DRAW_X + 38,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 3,
        .posY       = MOVE_BOX_BASE_Y - 10,
        .boxWidth   = 12,
        .boxHeight  = 8,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::BOTTOM,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "5",
        .uvX        = DRAW_X + 40,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 4,
        .posY       = MOVE_BOX_BASE_Y - 10,
        .boxWidth   = 12,
        .boxHeight  = 8,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::BOTTOM,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "6",
        .uvX        = DRAW_X + 42,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 5,
        .posY       = MOVE_BOX_BASE_Y - 10,
        .boxWidth   = 12,
        .boxHeight  = 8,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::BOTTOM,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "7",
        .uvX        = DRAW_X + 44,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 6,
        .posY       = MOVE_BOX_BASE_Y - 10,
        .boxWidth   = 12,
        .boxHeight  = 8,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::BOTTOM,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "8",
        .uvX        = DRAW_X + 46,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 7,
        .posY       = MOVE_BOX_BASE_Y - 10,
        .boxWidth   = 12,
        .boxHeight  = 8,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::BOTTOM,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },

    {
        .font       = &myFont5px,
        .string     = "FIRE",
        .uvX        = DRAW_X + 0,
        .uvY        = DRAW_Y + 16,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = MOVE_BOX_BASE_X - 28,
        .posY       = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 0,
        .boxWidth   = 25,
        .boxHeight  = 12,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "BATTLE",
        .uvX        = DRAW_X + 8,
        .uvY        = DRAW_Y + 16,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = MOVE_BOX_BASE_X - 28,
        .posY       = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 1,
        .boxWidth   = 25,
        .boxHeight  = 12,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "AIR",
        .uvX        = DRAW_X + 16,
        .uvY        = DRAW_Y + 16,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = MOVE_BOX_BASE_X - 28,
        .posY       = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 2,
        .boxWidth   = 25,
        .boxHeight  = 12,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "ICE",
        .uvX        = DRAW_X + 24,
        .uvY        = DRAW_Y + 16,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = MOVE_BOX_BASE_X - 28,
        .posY       = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 3,
        .boxWidth   = 25,
        .boxHeight  = 12,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "EARTH",
        .uvX        = DRAW_X + 32,
        .uvY        = DRAW_Y + 16,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = MOVE_BOX_BASE_X - 28,
        .posY       = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 4,
        .boxWidth   = 25,
        .boxHeight  = 12,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "MECH",
        .uvX        = DRAW_X + 40,
        .uvY        = DRAW_Y + 16,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = MOVE_BOX_BASE_X - 28,
        .posY       = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 5,
        .boxWidth   = 25,
        .boxHeight  = 12,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "FILTH",
        .uvX        = DRAW_X + 48,
        .uvY        = DRAW_Y + 16,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = MOVE_BOX_BASE_X - 28,
        .posY       = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 6,
        .boxWidth   = 25,
        .boxHeight  = 12,
        .alignmentX = AlignmentX::RIGHT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
};

static TextSprite tableLabels[] = {
    {
        .font       = &myFont5px,
        .string     = "POWER",
        .uvX        = DRAW_X + 0,
        .uvY        = DRAW_Y + 28,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = toRelativeX(174),
        .posY       = toRelativeY(40),
        .boxWidth   = 36,
        .boxHeight  = 10,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::CENTER,
        .color      = 9,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "COST",
        .uvX        = DRAW_X + 8,
        .uvY        = DRAW_Y + 28,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = toRelativeX(214),
        .posY       = toRelativeY(40),
        .boxWidth   = 36,
        .boxHeight  = 10,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::CENTER,
        .color      = 9,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "RANGE",
        .uvX        = DRAW_X + 16,
        .uvY        = DRAW_Y + 28,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = toRelativeX(254),
        .posY       = toRelativeY(40),
        .boxWidth   = 22,
        .boxHeight  = 10,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::CENTER,
        .color      = 9,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "STATUS",
        .uvX        = DRAW_X + 24,
        .uvY        = DRAW_Y + 28,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = toRelativeX(280),
        .posY       = toRelativeY(40),
        .boxWidth   = 22,
        .boxHeight  = 10,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::CENTER,
        .color      = 9,
        .layer      = 5,
        .hasShadow  = 1,
    },
};

static TextSprite tableLabels2[] = {
    {
        .font       = &myFont5px,
        .string     = "POWER",
        .uvX        = DRAW_X + 0,
        .uvY        = DRAW_Y + 28,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = toRelativeX(187),
        .posY       = toRelativeY(128),
        .boxWidth   = 36,
        .boxHeight  = 10,
        .alignmentX = AlignmentX::LEFT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 9,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "COST",
        .uvX        = DRAW_X + 8,
        .uvY        = DRAW_Y + 28,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = toRelativeX(187),
        .posY       = toRelativeY(150),
        .boxWidth   = 36,
        .boxHeight  = 10,
        .alignmentX = AlignmentX::LEFT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 9,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "RANGE",
        .uvX        = DRAW_X + 16,
        .uvY        = DRAW_Y + 28,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = toRelativeX(187),
        .posY       = toRelativeY(172),
        .boxWidth   = 22,
        .boxHeight  = 10,
        .alignmentX = AlignmentX::LEFT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 9,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &myFont5px,
        .string     = "STATUS",
        .uvX        = DRAW_X + 24,
        .uvY        = DRAW_Y + 28,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = toRelativeX(187),
        .posY       = toRelativeY(194),
        .boxWidth   = 22,
        .boxHeight  = 10,
        .alignmentX = AlignmentX::LEFT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 9,
        .layer      = 5,
        .hasShadow  = 1,
    },
};

static int8_t moveSelectedRow;
static int8_t moveSelectedColumn;
static MoveRegularEntry<64, 0> move0{};
static MoveRegularEntry<76, 16> move1{};
static MoveRegularEntry<88, 32> move2{};
static MoveEntry<100, 69> moveFinish{};
static MoveDetail detail{};

static bool canUseMove(DigimonType type, int32_t move)
{
    for (auto i : getDigimonData(type)->moves)
        if (i == move) return true;

    return false;
}

static int32_t getSpriteId(int32_t moveId)
{
    if (moveId < 8) return 0;
    if (moveId < 16) return 2;
    if (moveId < 24) return 4;
    if (moveId < 32) return 5;
    if (moveId < 40) return 3;
    if (moveId < 49) return 1;
    return 6;
}

bool isSameMove(int32_t moveId1, int32_t moveId2)
{
    if (moveId1 == moveId2) return true;
    if (moveId1 == 44 && moveId2 == 48) return true;
    if (moveId1 == 48 && moveId2 == 44) return true;
    if (moveId1 == 55 && moveId2 == 57) return true;
    if (moveId1 == 57 && moveId2 == 55) return true;
    return false;
}

static bool hasMoveEquipped(DigimonEntity* entity, int32_t moveId)
{
    for (auto move : entity->stats.moves)
        if (isSameMove(moveId, entityGetTechFromAnim(entity, move))) return true;

    return false;
}

struct MoveBox
{
    uint8_t moveId;
    int16_t posX;
    int16_t posY;

    void render() const
    {
        auto index = getSpriteId(moveId) * 2;
        if (PLAYTIME_FRAMES % 20 < 10) index += 1;

        if (hasMoveEquipped(&PARTNER_ENTITY, moveId)) renderBox(posX, posY, 12, 12, 200, 0, 40, 1, 5);
        if (!canUseMove(PARTNER_ENTITY.type, moveId)) renderBox(posX, posY, 12, 12, 0x68, 0x68, 0x68, 3, 5);
        if (hasMove(moveId)) specialSprites[index].render(posX, posY, 5, 0);

        renderBorderBox(posX - 1, posY - 1, 13, 13, 0x020202, 0x20202, 0x4E, 0x60, 0x6E, 5);
    }
};

constexpr uint8_t MOVE_MATRIX[MOVE_ROW_COUNT][MOVE_COL_COUNT] = {
    {0, 1, 2, 3, 4, 5, 6, 7},
    {40, 41, 42, 43, 44, 45, 46, 47},
    {8, 9, 10, 11, 12, 13, 14, 15},
    {16, 17, 18, 19, 20, 21, 22, 23},
    {32, 33, 34, 35, 36, 37, 38, 39},
    {24, 25, 26, 27, 28, 29, 30, 31},
    {49, 50, 51, 52, 53, 54, 55, 56},
};

constexpr MoveBox boxes[] = {
    {.moveId = 0, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 0, .posY = MOVE_BOX_BASE_Y},
    {.moveId = 1, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 1, .posY = MOVE_BOX_BASE_Y},
    {.moveId = 2, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 2, .posY = MOVE_BOX_BASE_Y},
    {.moveId = 3, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 3, .posY = MOVE_BOX_BASE_Y},
    {.moveId = 4, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 4, .posY = MOVE_BOX_BASE_Y},
    {.moveId = 5, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 5, .posY = MOVE_BOX_BASE_Y},
    {.moveId = 6, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 6, .posY = MOVE_BOX_BASE_Y},
    {.moveId = 7, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 7, .posY = MOVE_BOX_BASE_Y},

    {.moveId = 40, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 0, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 1},
    {.moveId = 41, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 1, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 1},
    {.moveId = 42, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 2, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 1},
    {.moveId = 43, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 3, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 1},
    {.moveId = 44, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 4, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 1},
    {.moveId = 45, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 5, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 1},
    {.moveId = 46, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 6, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 1},
    {.moveId = 47, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 7, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 1},

    {.moveId = 8, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 0, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 2},
    {.moveId = 9, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 1, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 2},
    {.moveId = 10, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 2, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 2},
    {.moveId = 11, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 3, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 2},
    {.moveId = 12, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 4, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 2},
    {.moveId = 13, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 5, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 2},
    {.moveId = 14, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 6, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 2},
    {.moveId = 15, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 7, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 2},

    {.moveId = 16, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 0, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 3},
    {.moveId = 17, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 1, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 3},
    {.moveId = 18, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 2, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 3},
    {.moveId = 19, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 3, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 3},
    {.moveId = 20, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 4, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 3},
    {.moveId = 21, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 5, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 3},
    {.moveId = 22, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 6, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 3},
    {.moveId = 23, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 7, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 3},

    {.moveId = 32, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 0, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 4},
    {.moveId = 33, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 1, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 4},
    {.moveId = 34, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 2, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 4},
    {.moveId = 35, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 3, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 4},
    {.moveId = 36, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 4, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 4},
    {.moveId = 37, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 5, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 4},
    {.moveId = 38, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 6, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 4},
    {.moveId = 39, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 7, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 4},

    {.moveId = 24, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 0, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 5},
    {.moveId = 25, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 1, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 5},
    {.moveId = 26, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 2, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 5},
    {.moveId = 27, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 3, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 5},
    {.moveId = 28, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 4, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 5},
    {.moveId = 29, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 5, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 5},
    {.moveId = 30, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 6, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 5},
    {.moveId = 31, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 7, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 5},

    {.moveId = 49, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 0, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 6},
    {.moveId = 50, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 1, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 6},
    {.moveId = 51, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 2, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 6},
    {.moveId = 52, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 3, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 6},
    {.moveId = 53, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 4, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 6},
    {.moveId = 54, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 5, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 6},
    {.moveId = 55, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 6, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 6},
    {.moveId = 56, .posX = MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * 7, .posY = MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * 6},
};

static int32_t getMoveIndex(int32_t selectedMove)
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

static int32_t getFirstFreeSlot()
{
    for (int32_t i = 0; i < 3; i++)
    {
        if (PARTNER_ENTITY.stats.moves[i] == 0xFF) return i;
    }

    return -1;
}

static void equipMove()
{
    auto selectedMove = MOVE_MATRIX[moveSelectedRow][moveSelectedColumn];

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

static int32_t getEquippedSlot()
{
    auto move = MOVE_MATRIX[moveSelectedRow][moveSelectedColumn];

    for (int32_t i = 0; i < 3; i++)
    {
        auto equipped = entityGetTechFromAnim(&PARTNER_ENTITY, PARTNER_ENTITY.stats.moves[i]);
        if (move == equipped) return i;
        if (move == 44 && equipped == 48) return i;
        if (move == 55 && equipped == 57) return i;
    }

    return -1;
}

void renderDigimonMovesView()
{
    if (MENU_STATE == 6)
    {
        renderTextSprite(techDetailsLabel);
        renderSeperatorLines(linesSeparator, 3, 5);
        detail.render();
        selectorSprite.render(MOVE_BOX_BASE_X + MOVE_BOX_OFFSET_X * moveSelectedColumn - 3,
                              MOVE_BOX_BASE_Y + MOVE_BOX_OFFSET_Y * moveSelectedRow - 2,
                              5,
                              0);

        for (auto& box : boxes)
            box.render();

        for (auto& label : matrixLabels)
            renderTextSprite(label);

        renderTextSprite(techDetailsLabel);

        for (auto& label : tableLabels2)
            renderTextSprite(label);
    }

    if (MENU_STATE == 5)
    {
        UI_BOX_DATA[1].finalPos.height += 39;
        if (UI_BOX_DATA[1].finalPos.height > 189) MENU_STATE = 1;
    }

    if (MENU_STATE == 3)
    {
        UI_BOX_DATA[1].finalPos.height += 39;
        if (UI_BOX_DATA[1].finalPos.height > 189) MENU_STATE = 6;
    }

    if (MENU_STATE == 2 || MENU_STATE == 4)
    {
        UI_BOX_DATA[1].finalPos.height -= 39;
        if (UI_BOX_DATA[1].finalPos.height < 74) MENU_STATE++;
    }

    if (MENU_STATE == 1)
    {
        renderTextSprite(finisherLabel);

        renderTextSprite2(tableLabels[0], 0, 69);
        renderTextSprite2(tableLabels[2], 0, 69);
        renderTextSprite2(tableLabels[3], 0, 69);

        moveFinish.render(5);
        renderSeperatorLines(lines2, 3, 5);
    }

    for (auto& label : tableLabels)
        renderTextSprite(label);

    renderTextSprite(techsetLabel);
    move0.render(5);
    move1.render(5);
    move2.render(5);
    renderSeperatorLines(lines, 3, 5);
}

void tickDigimonMenuTechs()
{
    if (MENU_STATE == 0)
    {
        clearTextSubArea2(0, 16, 256, 192);
        MENU_STATE = 1;
        drawTextSprite(techsetLabel);
        drawTextSprite(finisherLabel);
        drawTextSprite(techDetailsLabel);

        for (auto& label : tableLabels)
            drawTextSprite(label);
        for (auto& label : tableLabels2)
            drawTextSprite(label);

        for (auto& label : matrixLabels)
            drawTextSprite(label);

        move0.setMove(entityGetTechFromAnim(&PARTNER_ENTITY, PARTNER_ENTITY.stats.moves[0]));
        move1.setMove(entityGetTechFromAnim(&PARTNER_ENTITY, PARTNER_ENTITY.stats.moves[1]));
        move2.setMove(entityGetTechFromAnim(&PARTNER_ENTITY, PARTNER_ENTITY.stats.moves[2]));
        moveFinish.setMove(entityGetTechFromAnim(&PARTNER_ENTITY, PARTNER_ENTITY.stats.moves[3]));
        detail.setMove(MOVE_MATRIX[moveSelectedRow][moveSelectedColumn]);
    }

    if (MENU_STATE == 1 && isKeyDown(InputButtons::BUTTON_CROSS))
    {
        MENU_STATE = 2;
        playSound(0, 3);
    }

    if (MENU_STATE == 6)
    {
        if (isKeyDown(InputButtons::BUTTON_CROSS))
        {
            auto slot = getEquippedSlot();
            if (slot == -1)
                equipMove();
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
            if (hasAttackEquipped(&PARTNER_ENTITY)) MENU_STATE = 4;
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