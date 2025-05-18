#include "PlayerCardView.hpp"

#include "DigimonData.hpp"
#include "Files.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "extern/dw1.hpp"

constexpr auto CARD_ROW_COUNT = 6;
constexpr auto CARD_COL_COUNT = 11;
constexpr auto DRAW_X         = 704;
constexpr auto DRAW_Y         = 272;
constexpr auto CARD_BASE_X    = toRelativeX(30);
constexpr auto CARD_BASE_Y    = toRelativeY(58);
constexpr auto CARD_OFFSET_X  = 24;
constexpr auto CARD_OFFSET_Y  = 24;

static TextSprite textLabels[] = {
    {
        .font       = &vanillaFont,
        .string     = "Card List",
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

static TextSprite cardCount = {
    .font       = &vanillaFont,
    .string     = "1",
    .uvX        = 640,
    .uvY        = 496,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = 79,
    .posY       = 73,
    .boxWidth   = 44,
    .boxHeight  = 16,
    .alignmentX = AlignmentX::CENTER,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 3,
    .hasShadow  = 1,
};

constexpr Sprite selector = {
    .uvX          = 0,
    .uvV          = 0xE8,
    .width        = 24,
    .height       = 20,
    .texture_page = 24,
    .clut         = 0x7DC7,
};

static bool isCardCountDrawn = false;
static int8_t selectedCardCol;
static int8_t selectedCardRow;

static void loadCardImage(int32_t card)
{
    loadStackedTIMEntry("\\CARD\\CARD.ALL", TEXTURE_BUFFER, card * 14, 14);
}

static void renderCardImage()
{
    auto clut  = getClut(0x1C0, 0x1FF);
    auto tpage = getTPage(1, 2, 0x240, 0x100);
    renderRectPolyFT4(-75, -84, 150, 180, 0, 0, tpage, clut, 4, 0);
}

static void renderCardCount()
{
    if (!isCardCountDrawn)
    {
        uint8_t buffer[8];
        sprintf(buffer, "%d", getCardAmount(SELECTED_CARD));
        cardCount.string = reinterpret_cast<const char*>(buffer);
        drawTextSprite(cardCount);
        isCardCountDrawn = true;
    }

    renderTextSprite(cardCount);
}

void renderCardsView()
{
    if (MENU_STATE == 0)
    {
        for (auto& label : textLabels)
            drawTextSprite(label);
        MENU_STATE = 1;
    }

    if (MENU_STATE == 2)
        selector.render(CARD_BASE_X + selectedCardCol * CARD_OFFSET_X - 4,
                        CARD_BASE_Y + selectedCardRow * CARD_OFFSET_Y - 2,
                        5,
                        0);

    for (int row = 0; row < CARD_ROW_COUNT; row++)
        for (int col = 0; col < CARD_COL_COUNT; col++)
        {
            auto cardId = row * CARD_COL_COUNT + col;
            auto posX   = CARD_BASE_X + col * CARD_OFFSET_X;
            auto posY   = CARD_BASE_Y + row * CARD_OFFSET_Y;

            if (getCardAmount(cardId) > 0)
            {
                auto type = CARD_DATA[cardId].type;
                if (type != 0xFF) getDigimonSprite(static_cast<DigimonType>(type)).render(posX, posY, 5, 0, 0);
            }

            renderBorderBox(posX - 1, posY - 1, 17, 17, 0xBEBEBE, 0x3C3C3C, 0x67, 0x67, 0x67, 5);
        }

    for (auto& label : textLabels)
        renderTextSprite(label);
}

void tickPlayerMenuCardView()
{
    if (MENU_STATE == 3)
    {
        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            playSound(0, 4);
            RECT start = {
                .x      = static_cast<int16_t>(selectedCardCol * 24 - 121),
                .y      = static_cast<int16_t>(selectedCardRow * 24 - 54),
                .width  = 1,
                .height = 1,
            };
            MENU_STATE = 2;
            removeAnimatedUIBox(2, &start);
            removeAnimatedUIBox(3, &start);
        }
    }
    else if (MENU_STATE == 2)
    {
        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            playSound(0, 4);
            MENU_STATE = 1;
        }

        auto oldCard = SELECTED_CARD;
        if (isKeyDownRepeat(InputButtons::BUTTON_LEFT)) selectedCardCol -= 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_RIGHT)) selectedCardCol += 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_UP)) selectedCardRow -= 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_DOWN)) selectedCardRow += 1;
        selectedCardCol = clamp(selectedCardCol, 0, CARD_COL_COUNT - 1);
        selectedCardRow = clamp(selectedCardRow, 0, CARD_ROW_COUNT - 1);

        SELECTED_CARD = selectedCardRow * CARD_COL_COUNT + selectedCardCol;
        if (oldCard != SELECTED_CARD) playSound(0, 2);

        // vanilla doesn't check for the boxes to be available, causing the card glitch
        if (isKeyDown(InputButtons::BUTTON_CROSS) && getCardAmount(SELECTED_CARD) != 0 && isUIBoxAvailable(2) &&
            isUIBoxAvailable(3))
        {
            playSound(0, 3);
            MENU_STATE = 3;
            loadCardImage(SELECTED_CARD);
            RECT start = {
                .x      = static_cast<int16_t>(selectedCardCol * 24 - 121),
                .y      = static_cast<int16_t>(selectedCardRow * 24 - 54),
                .width  = 1,
                .height = 1,
            };
            RECT card        = {.x = -75, .y = -83, .width = 150, .height = 180};
            RECT count       = {.x = 74, .y = 69, .width = 54, .height = 24};
            isCardCountDrawn = false;
            createAnimatedUIBox(2, 1, 0, &card, &start, nullptr, renderCardImage);
            createAnimatedUIBox(3, 1, 0, &count, &start, nullptr, renderCardCount);
        }
    }
    else if (MENU_STATE == 1)
    {
        if (isKeyDown(InputButtons::BUTTON_CROSS))
        {
            MENU_STATE = 2;
            playSound(0, 3);
        }
    }
}