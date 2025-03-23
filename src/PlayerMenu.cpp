#include "Files.hpp"
#include "Font.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "PlayerInfoView.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"

/*
 * Forward declarations
 */

static void renderEvoChartDetail();

constexpr auto DIGIMON_MENU_TAB_DRAW_X = 704;
constexpr auto DIGIMON_MENU_TAB_DRAW_Y = 256;

constexpr auto CHART_COL_COUNT       = 9;
constexpr int32_t CHART_ROW_COUNT[9] = {8, 8, 9, 8, 7, 7, 7, 8, 7};
constexpr auto MEDAL_ROW_COUNT       = 3;
constexpr auto MEDAL_COL_COUNT       = 5;
constexpr auto CARD_ROW_COUNT        = 6;
constexpr auto CARD_COL_COUNT        = 11;

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
static bool isDetailRendered;

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

static void tickPlayerMenuChartView()
{
    if (MENU_STATE == 3)
    {
        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            int16_t posX = 0;
            if (CHART_SELECTED_COLUMN < 3)
                posX = CHART_SELECTED_COLUMN * 37 + 28;
            else if (CHART_SELECTED_COLUMN < 7)
                posX = (CHART_SELECTED_COLUMN - 3) * 24 + 139;
            else
                posX = (CHART_SELECTED_COLUMN - 7) * 24 + 248;
            int16_t posY = CHART_SELECTED_ROW * 19 + 43;

            RECT start = {
                .x      = static_cast<int16_t>(posX - 153),
                .y      = static_cast<int16_t>(posY - 69),
                .width  = 10,
                .height = 10,
            };
            unsetUIBoxAnimated(2, &start);
            playSound(0, 3);
            MENU_STATE = 2;
        }
    }

    else if (MENU_STATE == 2)
    {
        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            playSound(0, 4);
            MENU_STATE             = 1;
            CHART_SELECTED_DIGIMON = -1;
        }
        auto initialCol = CHART_SELECTED_COLUMN;
        auto initialRow = CHART_SELECTED_ROW;

        if (isKeyDownRepeat(InputButtons::BUTTON_RIGHT))
            CHART_SELECTED_COLUMN = min(CHART_SELECTED_COLUMN + 1, CHART_COL_COUNT - 1);
        if (isKeyDownRepeat(InputButtons::BUTTON_LEFT)) CHART_SELECTED_COLUMN = max(CHART_SELECTED_COLUMN - 1, 0);

        auto rowAmount = (CHART_SELECTED_COLUMN == 0 || CHART_SELECTED_COLUMN == 1) ? 2 : 1;
        auto rowCount  = CHART_ROW_COUNT[CHART_SELECTED_COLUMN];

        if (isKeyDownRepeat(InputButtons::BUTTON_UP)) CHART_SELECTED_ROW -= rowAmount;
        if (isKeyDownRepeat(InputButtons::BUTTON_DOWN)) CHART_SELECTED_ROW += rowAmount;

        CHART_SELECTED_ROW = clamp(CHART_SELECTED_ROW, 0, rowCount - 1);
        CHART_SELECTED_ROW -= CHART_SELECTED_ROW % rowAmount;

        if (initialCol != CHART_SELECTED_COLUMN || initialRow != CHART_SELECTED_ROW) playSound(0, 2);

        int16_t posX = 0;
        if (CHART_SELECTED_COLUMN < 3)
            posX = CHART_SELECTED_COLUMN * 37 + 28;
        else if (CHART_SELECTED_COLUMN < 7)
            posX = (CHART_SELECTED_COLUMN - 3) * 24 + 139;
        else
            posX = (CHART_SELECTED_COLUMN - 7) * 24 + 248;
        int16_t posY = CHART_SELECTED_ROW * 19 + 43;

        CHART_SELECTED_DIGIMON = getEvoChartData(posX, posY).second + 1;

        if (isKeyDown(InputButtons::BUTTON_CROSS) && hasDigimonRaised(static_cast<DigimonType>(CHART_SELECTED_DIGIMON)))
        {
            if (isUIBoxAvailable(2))
            {
                RECT full  = {.x = -150, .y = -89, .width = 300, .height = 190};
                RECT start = {
                    .x      = static_cast<int16_t>(posX - 153),
                    .y      = static_cast<int16_t>(posY - 69),
                    .width  = 10,
                    .height = 10,
                };
                createAnimatedUIBox(2, 1, 0, &full, &start, nullptr, renderEvoChartDetail);
                playSound(0, 3);
                MENU_STATE       = 3;
                isDetailRendered = false;
            }
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

// TODO improve detail view, ideas:
// jump to next Digimon from within detail view
// display special paths?
// display Digimon info
constexpr Inset detailInsets[] = {
    {.posX = 28, .posY = 42, .width = 78, .height = 130},
    {.posX = 214, .posY = 42, .width = 78, .height = 130},
    {.posX = 136, .posY = 85, .width = 48, .height = 46},
    {.posX = 129, .posY = 50, .width = 60, .height = 4},
    {.posX = 38, .posY = 187, .width = 108, .height = 4},
    {.posX = 26, .posY = 206, .width = 132, .height = 4},
};

struct ChartLine
{
    int16_t x1;
    int16_t x2;
    int16_t x3;
    int16_t x4;
    int16_t y1;
    int16_t y2;
    int16_t y3;
    int16_t y4;

    void render(uint32_t color1, uint32_t color2, int32_t layer) const
    {
        // TODO: diagonal lines don't connect neatly to one side
        renderTrianglePrimitive(color1, x1, y1 - 1, x2, y2 - 1, x3, y3 - 1, layer, 0);
        renderLinePrimitive(color1, x3, y3 - 1, x4, y4 - 1, layer, 0);

        renderTrianglePrimitive(color2, x1, y1, x2, y2, x3, y3, layer, 0);
        renderLinePrimitive(color2, x3, y3, x4, y4, layer, 0);

        renderTrianglePrimitive(color1, x1, y1 + 1, x2, y2 + 1, x3, y3 + 1, layer, 0);
        renderLinePrimitive(color1, x3, y3 + 1, x4, y4 + 1, layer, 0);
    }
};

struct ChartSprite
{
    int16_t posX;
    int16_t posY;

    void render(uint8_t texX, uint8_t texY, uint16_t clut, int32_t layer, bool spriteVisible) const
    {
        if (spriteVisible) renderRectPolyFT4(posX, posY, 16, 16, texX, texY, 24, clut, layer, 0);
        // vanilla is 18x18, resulting in a 19x19 box
        renderSpriteBox(160 + posX - 1, 120 + posY - 1, 17, 17, 0xBEBEBE, 0x3C3C3C, 0x87, 0x87, 0x87, layer);
    }
};

constexpr ChartLine fromLineEven[] = {
    {-84, -68, -37, -10, -48, -48, -17, -17},
    {-102, -74, -64, -10, -24, -24, -14, -14},
    {-102, -74, -64, -10, -1, -1, -11, -11},
    {-84, -68, -37, -10, 23, 23, -8, -8},
};

constexpr ChartLine fromLineOdd[] = {
    {-66, -52, -15, -10, -60, -60, -18, -18},
    {-84, -58, -37, -10, -36, -36, -15, -15},
    {-102, -10, -10, -10, -12, -12, -12, -12},
    {-84, -58, -37, -10, 12, 12, -9, -9},
    {-66, -52, -15, -10, 36, 36, -6, -6},
};

constexpr ChartLine toLinesOdd[] = {
    {9, 51, 64, 64, -18, -60, -60, -60},
    {9, 36, 57, 82, -15, -15, -36, -36},
    {9, 100, 100, 100, -12, -12, -12, -12},
    {9, 36, 57, 82, -9, -9, 12, 12},
    {9, 51, 64, 64, -6, 36, 36, 36},
};

constexpr ChartLine toLinesEven[] = {
    {9, 61, 64, 64, -20, -72, -72, -72},
    {9, 36, 67, 82, -17, -17, -48, -48},
    {9, 63, 73, 100, -14, -14, -24, -24},
    {9, 63, 73, 100, -11, -11, -1, -1},
    {9, 36, 67, 82, -8, -8, 23, 23},
    {9, 61, 64, 64, -5, 47, 47, 47},
};

constexpr uint16_t EVO_CLUTS[] = {0x7A07, 0x7A47, 0x7A87, 0x7AC7, 0x7B07};

constexpr RGB8 TO_COLORS[][2] = {
    {{0xFB, 0xE8, 0x02}, {0xEC, 0x9A, 0x00}},
    {{0x00, 0xB6, 0x00}, {0x00, 0x64, 0x00}},
    {{0x00, 0xAE, 0xB8}, {0x00, 0x6F, 0x8E}},
    {{0x00, 0x3C, 0xC8}, {0x00, 0x1E, 0x8C}},
    {{0xBE, 0x00, 0xA0}, {0x6E, 0x00, 0x6E}},
    {{0xBE, 0x00, 0x1E}, {0x82, 0x1E, 0x1E}},
};

constexpr ChartSprite centerSprite = {.posX = -8, .posY = -20};
constexpr ChartSprite fromOdd[]    = {{-83, -68}, {-101, -44}, {-119, -20}, {-101, 4}, {-83, 28}};
constexpr ChartSprite fromEven[]   = {{-101, -56}, {-119, -32}, {-119, -8}, {-101, 16}};
constexpr ChartSprite toOdd[]      = {{66, -69}, {84, -45}, {102, -21}, {84, 3}, {66, 27}};
constexpr ChartSprite toEven[]     = {{66, -81}, {84, -57}, {102, -33}, {102, -9}, {84, 15}, {66, 39}};

constexpr auto DETAIL_TEXT_X = 704 + 0;
constexpr auto DETAIL_TEXT_Y = 256 + 64;

constexpr char const* LEVEL_NAMES[] = {
    "Fresh",
    "In-Training",
    "Rookie",
    "Champion",
    "Ultimate",
};

static TextSprite treeText = {
    .font       = &vanillaFont,
    .string     = "Tree",
    .uvX        = DETAIL_TEXT_X + 64,
    .uvY        = DETAIL_TEXT_Y + 0,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = -20,
    .posY       = -79,
    .boxWidth   = 36,
    .boxHeight  = 12,
    .alignmentX = AlignmentX::CENTER,
    .alignmentY = AlignmentY::CENTER,
    .color      = 3,
    .layer      = 4,
    .hasShadow  = 1,
};

static TextSprite levelText = {
    .font       = &vanillaFont,
    .string     = "",
    .uvX        = DETAIL_TEXT_X + 0,
    .uvY        = DETAIL_TEXT_Y + 0,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = -100,
    .posY       = 58,
    .boxWidth   = 70,
    .boxHeight  = 12,
    .alignmentX = AlignmentX::CENTER,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 4,
    .hasShadow  = 1,
};

static TextSprite nameText = {
    .font       = &vanillaFont,
    .string     = "",
    .uvX        = DETAIL_TEXT_X + 32,
    .uvY        = DETAIL_TEXT_Y + 0,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = -130,
    .posY       = 77,
    .boxWidth   = 120,
    .boxHeight  = 12,
    .alignmentX = AlignmentX::CENTER,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 4,
    .hasShadow  = 1,
};

static void renderEvoChartDetail()
{
    if (!isDetailRendered)
    {
        auto* para       = getDigimonData(static_cast<DigimonType>(CHART_SELECTED_DIGIMON));
        nameText.string  = reinterpret_cast<const char*>(para->name);
        levelText.string = LEVEL_NAMES[static_cast<int32_t>(para->level) - 1];

        drawTextSprite(treeText);
        drawTextSprite(levelText);
        drawTextSprite(nameText);
        isDetailRendered = true;
    }

    uint32_t toCount   = 0;
    uint32_t fromCount = 0;
    auto& paths        = EVO_PATHS_DATA[CHART_SELECTED_DIGIMON];

    for (auto val : paths.from)
    {
        if (val != 0xFF) fromCount++;
    }
    for (auto val : paths.to)
    {
        if (val != 0xFF) toCount++;
    }

    auto& data = EVOCHART_BOXES[CHART_SELECTED_DIGIMON - 1];
    centerSprite.render(data.u, data.v, EVO_CLUTS[data.clut], 4, true);

    auto* toLines        = (toCount & 1) == 0 ? toLinesEven : toLinesOdd;
    auto* toSprite       = (toCount & 1) == 0 ? toEven : toOdd;
    auto toSpriteCount   = (toCount & 1) == 0 ? 6 : 5;
    auto* fromLines      = (fromCount & 1) == 0 ? fromLineEven : fromLineOdd;
    auto* fromSprite     = (fromCount & 1) == 0 ? fromEven : fromOdd;
    auto fromSpriteCount = (fromCount & 1) == 0 ? 4 : 5;

    for (int32_t i = 0; i < toSpriteCount; i++)
    {
        auto digi = paths.to[i];
        if (digi == 0xFF) continue;
        auto& data = EVOCHART_BOXES[digi - 1];
        toSprite[i].render(data.u, data.v, EVO_CLUTS[data.clut], 4, hasDigimonRaised(static_cast<DigimonType>(digi)));
        toLines[i].render(TO_COLORS[i][1].asUint32(), TO_COLORS[i][0].asUint32(), 4);
    }

    for (int32_t i = 0; i < fromSpriteCount; i++)
    {
        auto digi = paths.from[i];
        if (digi == 0xFF) continue;
        auto& data = EVOCHART_BOXES[digi - 1];
        fromSprite[i].render(data.u, data.v, EVO_CLUTS[data.clut], 4, hasDigimonRaised(static_cast<DigimonType>(digi)));
        fromLines[i].render(0x65db, 0x794e3, 4);
    }

    renderTextSprite(treeText);
    renderTextSprite(levelText);
    renderTextSprite(nameText);

    for (auto& val : detailInsets)
        renderInsetBox(val.posX, val.posY, val.width, val.height, 4);
}