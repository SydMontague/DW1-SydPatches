#include "Files.hpp"
#include "Font.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "PlayerCardView.hpp"
#include "PlayerChartView.hpp"
#include "PlayerInfoView.hpp"
#include "PlayerMedalView.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"

constexpr auto DIGIMON_MENU_TAB_DRAW_X = 704;
constexpr auto DIGIMON_MENU_TAB_DRAW_Y = 256;

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

static uint8_t playerMenuState;

void tickPlayerMenu(int32_t instanceId)
{
    if (MENU_STATE == 0)
    {
        clearTextSubArea2(0, 0, 256, 12);
        drawTextSprite(playerLabel);
        drawTextSprite(chartLabel);
        drawTextSprite(medalLabel);
        drawTextSprite(cardLabel);
    }

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

void renderPlayerMenu(int32_t instanceId)
{
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
