#include "Files.hpp"
#include "Font.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "PlayerChartView.hpp"
#include "PlayerInfoView.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"

constexpr auto DIGIMON_MENU_TAB_DRAW_X = 704;
constexpr auto DIGIMON_MENU_TAB_DRAW_Y = 256;

constexpr auto MEDAL_ROW_COUNT = 3;
constexpr auto MEDAL_COL_COUNT = 5;
constexpr auto CARD_ROW_COUNT  = 6;
constexpr auto CARD_COL_COUNT  = 11;

static TextSprite playerLabel = {
    .font       = &vanillaFont,
    .string     = "Player",
    .uvX        = DIGIMON_MENU_TAB_DRAW_X + 0,
    .uvY        = DIGIMON_MENU_TAB_DRAW_Y + 0,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = -138,
    .posY       = -101,
    .boxWidth   = 56,
    .boxHeight  = 12,
    .alignmentX = AlignmentX::CENTER,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite chartLabel = {
    .font       = &vanillaFont,
    .string     = "Chart",
    .uvX        = DIGIMON_MENU_TAB_DRAW_X + 16,
    .uvY        = DIGIMON_MENU_TAB_DRAW_Y + 0,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = -66,
    .posY       = -101,
    .boxWidth   = 54,
    .boxHeight  = 12,
    .alignmentX = AlignmentX::CENTER,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite medalLabel = {
    .font       = &vanillaFont,
    .string     = "Medal",
    .uvX        = DIGIMON_MENU_TAB_DRAW_X + 32,
    .uvY        = DIGIMON_MENU_TAB_DRAW_Y + 0,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = 3,
    .posY       = -101,
    .boxWidth   = 44,
    .boxHeight  = 12,
    .alignmentX = AlignmentX::CENTER,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite cardLabel = {
    .font       = &vanillaFont,
    .string     = "Card",
    .uvX        = DIGIMON_MENU_TAB_DRAW_X + 48,
    .uvY        = DIGIMON_MENU_TAB_DRAW_Y + 0,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = 62,
    .posY       = -101,
    .boxWidth   = 44,
    .boxHeight  = 12,
    .alignmentX = AlignmentX::CENTER,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
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

static bool isCardCountDrawn = false;
static int8_t selectedCardCol;
static int8_t selectedCardRow;
static int8_t selectedMedalRow;
static int8_t selectedMedalCol;
static uint8_t playerMenuState;

static Pair<EvoChartBoxData, int32_t> getEvoChartData(uint16_t posX, uint16_t posY)
{
    int32_t i = 0;
    for (auto& data : EVOCHART_BOXES)
    {
        if (data.posX == posX + 2 && data.posY == posY) return {data, i};
        i++;
    }

    return {{}, -1};
}

static void tickPlayerMenuPlayerView() {}

static void tickPlayerMenuMedalView()
{
    if (MENU_STATE == 2 || MENU_STATE == 3)
    {
        if (MENU_STATE == 2 && isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            playSound(0, 4);
            MENU_STATE = 1;
        }

        auto oldMedal = MEDAL_SELECTOR_INDEX;

        if (isKeyDownRepeat(InputButtons::BUTTON_LEFT)) selectedMedalCol -= 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_RIGHT)) selectedMedalCol += 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_UP)) selectedMedalRow -= 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_DOWN)) selectedMedalRow += 1;

        selectedMedalCol = clamp(selectedMedalCol, 0, MEDAL_COL_COUNT - 1);
        selectedMedalRow = clamp(selectedMedalRow, 0, MEDAL_ROW_COUNT - 1);

        MEDAL_SELECTOR_INDEX = selectedMedalRow * MEDAL_COL_COUNT + selectedMedalCol;
        if (MEDAL_SELECTOR_INDEX != oldMedal)
        {
            if (hasMedal(static_cast<Medal>(MEDAL_SELECTOR_INDEX)))
            {
                activateMedalTexture(MEDAL_SELECTOR_INDEX);
                MENU_STATE = 3;
            }
            playSound(0, 2);
        }
        SELECTED_MEDAL = MEDAL_SELECTOR_INDEX;
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

static void loadCardImage(int32_t card)
{
    loadStackedTIMEntry("\\CARD\\CARD.ALL", TEXTURE_BUFFER, card * 14, 14);
}

static void renderCardImage()
{
    auto clut  = getClut(0x1C0, 0x1FF);
    auto tpage = libgpu_GetTPage(1, 2, 0x240, 0x100);
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

static void tickPlayerMenuCardView()
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
            unsetUIBoxAnimated(2, &start);
            unsetUIBoxAnimated(3, &start);
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

void tickPlayerMenu()
{
    if (MENU_STATE < 2)
    {
        if (isKeyDownRepeat(InputButtons::BUTTON_LEFT) && playerMenuState >= 1)
        {
            playerMenuState -= 1;
            MENU_STATE     = 0;
            MENU_SUB_STATE = 0;
            playSound(0, 2);
        }
        if (isKeyDownRepeat(InputButtons::BUTTON_RIGHT) && playerMenuState < 3)
        {
            playerMenuState += 1;
            MENU_STATE     = 0;
            MENU_SUB_STATE = 0;
            playSound(0, 2);
        }

        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            TRIANGLE_MENU_STATE = 6;
            playSound(0, 4);
        }
    }

    if (playerMenuState == 0) { tickPlayerMenuPlayerView(); }
    else if (playerMenuState == 1) { tickPlayerMenuChartView(); }
    else if (playerMenuState == 2) { tickPlayerMenuMedalView(); }
    else if (playerMenuState == 3) { tickPlayerMenuCardView(); }
}

void renderPlayerMenu()
{
    if (MENU_STATE == 0)
    {
        clearTextSubArea2(0, 0, 256, 12);
        drawTextSprite(playerLabel);
        drawTextSprite(chartLabel);
        drawTextSprite(medalLabel);
        drawTextSprite(cardLabel);
    }

    if (playerMenuState == 0)
        renderPlayerInfoView();
    else if (playerMenuState == 1)
        renderEvoChartView();
    else if (playerMenuState == 2)
        renderMedalView();
    else if (playerMenuState == 3)
        renderCardsView();

    playerLabel.color = playerMenuState != 0 ? 1 : 0;
    chartLabel.color  = playerMenuState != 1 ? 1 : 0;
    medalLabel.color  = playerMenuState != 2 ? 1 : 0;
    cardLabel.color   = playerMenuState != 3 ? 1 : 0;

    renderTextSprite(playerLabel);
    renderTextSprite(chartLabel);
    renderTextSprite(medalLabel);
    renderTextSprite(cardLabel);

    renderMenuTab(-145, 72, playerMenuState != 0);
    renderMenuTab(-74, 70, playerMenuState != 1);
    renderMenuTab(-5, 60, playerMenuState != 2);
    renderMenuTab(54, 60, playerMenuState != 3);
}

void setPlayerMenuState(uint32_t state)
{
    playerMenuState = state;
}

/*
 * Evo Chart Detail
 */
