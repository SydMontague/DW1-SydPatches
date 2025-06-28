#include "Entity.hpp"
#include "Font.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "Sound.hpp"
#include "StatsView.hpp"
#include "TechView.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"

constexpr auto PLAYER_MENU_TAB_DRAW_X = 704;
constexpr auto PLAYER_MENU_TAB_DRAW_Y = 256;

static TextSprite statusLabel = {
    .font       = &vanillaFont,
    .string     = "Status",
    .uvX        = PLAYER_MENU_TAB_DRAW_X + 0,
    .uvY        = PLAYER_MENU_TAB_DRAW_Y + 0,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = -138,
    .posY       = -101,
    .boxWidth   = 60,
    .boxHeight  = 12,
    .alignmentX = AlignmentX::CENTER,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite techLabel = {
    .font       = &vanillaFont,
    .string     = "Tech",
    .uvX        = PLAYER_MENU_TAB_DRAW_X + 16,
    .uvY        = PLAYER_MENU_TAB_DRAW_Y + 0,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = -63,
    .posY       = -101,
    .boxWidth   = 48,
    .boxHeight  = 12,
    .alignmentX = AlignmentX::CENTER,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
};

static uint8_t digimonMenuState;

void renderDigimonMenu()
{
    if (digimonMenuState == 0)
        renderDigimonStatsView();
    else if (digimonMenuState == 1)
        renderDigimonMovesView();

    statusLabel.color = digimonMenuState != 0 ? 1 : 0;
    techLabel.color   = digimonMenuState != 1 ? 1 : 0;
    renderTextSprite(statusLabel);
    renderTextSprite(techLabel);
    renderMenuTab(-145, 76, digimonMenuState != 0);
    renderMenuTab(-70, 64, digimonMenuState != 1);
}

void tickDigimonMenu()
{
    if (MENU_STATE == 0)
    {
        clearTextSubArea2(0, 0, 256, 12);
        drawTextSprite(statusLabel);
        drawTextSprite(techLabel);
    }

    // stats menu doesn't need special interaction
    if (digimonMenuState == 0)
        tickDigimonStatsView();
    else if (digimonMenuState == 1)
        tickDigimonMenuTechs();

    if (MENU_STATE == 1)
    {
        if (isKeyDown(InputButtons::BUTTON_LEFT) && digimonMenuState >= 1)
        {
            digimonMenuState -= 1;
            MENU_STATE     = 0;
            MENU_SUB_STATE = 0;
            playSound(0, 2);
        }
        if (isKeyDown(InputButtons::BUTTON_RIGHT) && digimonMenuState < 1)
        {
            digimonMenuState += 1;
            MENU_STATE     = 0;
            MENU_SUB_STATE = 0;
            playSound(0, 2);
        }

        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            TRIANGLE_MENU_STATE = 4;
            playSound(0, 4);
        }
    }

    TAMER_ENTITY.isOnScreen   = false;
    PARTNER_ENTITY.isOnScreen = false;
}

void setDigimonMenuState(int32_t state)
{
    digimonMenuState = state;
}