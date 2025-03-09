#include "Helper.hpp"
#include "Math.hpp"
#include "extern/dw1.hpp"

constexpr auto CHART_COL_COUNT       = 9;
constexpr int32_t CHART_ROW_COUNT[9] = {8, 8, 9, 8, 7, 7, 7, 8, 7};
constexpr auto MEDAL_ROW_COUNT       = 3;
constexpr auto MEDAL_COL_COUNT       = 5;
constexpr auto CARD_ROW_COUNT        = 6;
constexpr auto CARD_COL_COUNT        = 11;

static int8_t selectedCardCol;
static int8_t selectedCardRow;
static int8_t selectedMedalRow;
static int8_t selectedMedalCol;

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

        if (isKeyDown(InputButtons::BUTTON_CROSS) &&
            hasDigimonRaised(static_cast<DigimonType>(CHART_SELECTED_DIGIMON)))
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
                MENU_STATE = 3;
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
            RECT card  = {.x = -75, .y = -83, .width = 150, .height = 180};
            RECT count = {.x = 74, .y = 69, .width = 54, .height = 24};
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

extern "C"
{
    void tickPlayerMenu()
    {
        if (MENU_STATE < 2)
        {
            if (isKeyDownRepeat(InputButtons::BUTTON_LEFT) && PLAYER_MENU_STATE >= 1)
            {
                PLAYER_MENU_STATE -= 1;
                MENU_STATE     = 0;
                MENU_SUB_STATE = 0;
                playSound(0, 2);
            }
            if (isKeyDownRepeat(InputButtons::BUTTON_RIGHT) && PLAYER_MENU_STATE < 3)
            {
                PLAYER_MENU_STATE += 1;
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

        if (PLAYER_MENU_STATE == 0) { tickPlayerMenuPlayerView(); }
        else if (PLAYER_MENU_STATE == 1) { tickPlayerMenuChartView(); }
        else if (PLAYER_MENU_STATE == 2) { tickPlayerMenuMedalView(); }
        else if (PLAYER_MENU_STATE == 3) { tickPlayerMenuCardView(); }
    }
}