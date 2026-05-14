#include "AtlasFont.hpp"
#include "Entity.hpp"
#include "Font.hpp"
#include "Helper.hpp"
#include "Input.hpp"
#include "Math.hpp"
#include "Sound.hpp"
#include "StatsView.hpp"
#include "TechView.hpp"
#include "Timestamp.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"

namespace
{
    constexpr RenderSettings STATUS_SETTINGS{
        .x        = -138,
        .y        = -101,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 60,
        .height   = 12,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::CENTER,
    };
    constexpr RenderSettings TECH_SETTINGS{
        .x        = -63,
        .y        = -101,
        .baseClut = 1,
        .color    = TEXT_COLOR_BLUE,
        .width    = 48,
        .height   = 12,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::CENTER,
    };

    struct Data
    {
        AtlasString statusLabel;
        AtlasString techLabel;

        Data()
        {
            statusLabel = getAtlasVanilla().render("Status", STATUS_SETTINGS);
            techLabel   = getAtlasVanilla().render("Tech", TECH_SETTINGS);
        }
    };

    dtl::unique_ptr<Data> data{};
    uint8_t digimonMenuState;

    void renderDigimonMenu(int32_t instanceId)
    {
        if (digimonMenuState == 0)
            renderDigimonStatsView();
        else if (digimonMenuState == 1)
            renderDigimonMovesView();

        data->statusLabel.render(5);
        data->techLabel.render(5);
        renderMenuTab(-145, 76, digimonMenuState != 0);
        renderMenuTab(-70, 64, digimonMenuState != 1);
    }

    void updateLabelColors()
    {
        if (digimonMenuState == 0)
        {
            data->statusLabel.setColor(TEXT_COLOR_WHITE);
            data->techLabel.setColor(TEXT_COLOR_BLUE);
        }
        else
        {
            data->statusLabel.setColor(TEXT_COLOR_BLUE);
            data->techLabel.setColor(TEXT_COLOR_WHITE);
        }
    }

    void tickDigimonMenu(int32_t instanceId)
    {
        // stats menu doesn't need special interaction
        if (digimonMenuState == 0)
            tickDigimonStatsView();
        else if (digimonMenuState == 1)
            tickDigimonMenuTechs();

        if (isKeyDown(InputButtons::BUTTON_LEFT) && digimonMenuState >= 1)
        {
            digimonMenuState -= 1;
            updateLabelColors();
            MENU_STATE     = 0;
            MENU_SUB_STATE = 0;
            playSound(0, 2);
        }
        if (isKeyDown(InputButtons::BUTTON_RIGHT) && digimonMenuState < 1)
        {
            digimonMenuState += 1;
            updateLabelColors();
            MENU_STATE     = 0;
            MENU_SUB_STATE = 0;
            playSound(0, 2);
        }

        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            TRIANGLE_MENU_STATE = 4;
            playSound(0, 4);
        }

        TAMER_ENTITY.isOnScreen   = false;
        PARTNER_ENTITY.isOnScreen = false;
    }
} // namespace

void addDigimonMenu()
{
    MENU_STATE     = 0;
    MENU_SUB_STATE = 0;
    data           = dtl::make_unique<Data>();
    createMenuBox(1, -150, -89, 300, 190, 0, tickDigimonMenu, renderDigimonMenu);
}

void removeDigimonMenu()
{
    TAMER_ENTITY.isOnScreen   = true;
    PARTNER_ENTITY.isOnScreen = true;
    closeUIBoxIfOpen(1);
    data.release();
}