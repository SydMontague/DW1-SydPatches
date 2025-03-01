#include "Font.hpp"
#include "GameObjects.hpp"
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    constexpr RECT menuTextArea{.x = 0, .y = 232, .width = 24, .height = 12};
    constexpr uint8_t YEAR_DAY_STRING[]  = "YearDay";
    constexpr uint8_t PLAYER_MENU_TABS[] = "Player   Chart    Med. Card Fish";

    void tickTriangleMenu(int32_t instanceId)
    {
        switch (TRIANGLE_MENU_STATE)
        {
            case 0:
            {
                createMenuBox(0, -66, -80, 132, HAS_FISHING_ROD ? 149 : 110, 2, tickGameMenu, renderGameMenu);
                if (UI_BOX_DATA[0].frame == 4) TRIANGLE_MENU_STATE = 0xFFFFFFFF;

                RECT rect = menuTextArea;
                clearTextSubArea(&rect);
                // vanilla is at Y 232, but this caused garbage data overwrites
                // TODO move this draw into the menu box tick/render
                drawStringNew(&vanillaFont, YEAR_DAY_STRING, 704, 216 + 256);
                break;
            }
            case 1:
            {
                closeUIBoxIfOpen(0);
                removeObject(ObjectID::GAME_MENU, 0);
                break;
            }
            case 2:
            {
                closeUIBoxIfOpen(0);
                if (UI_BOX_DATA[0].frame == 0)
                {
                    initializeInventoryObject();
                    TRIANGLE_MENU_STATE = 0xFFFFFFFF;
                }
                break;
            }
            case 3:
            {
                closeUIBoxIfOpen(0);
                if (UI_BOX_DATA[0].frame == 0)
                {
                    createMenuBox(1, -150, -89, 300, 190, 0, tickDigimonMenu, renderDigimonMenu);
                    // TODO move this draw into the menu box tick/render
                    clearTextArea();
                    drawStringNew(&vanillaFont, PLAYER_MENU_TABS, 704, 256);
                    MENU_STATE          = 0;
                    MENU_SUB_STATE      = 0;
                    TRIANGLE_MENU_STATE = 0xFFFFFFFF;
                    DIGIMON_MENU_STATE  = 0;
                }
                break;
            }
            case 4:
            {
                TAMER_ENTITY.isOnScreen   = true;
                PARTNER_ENTITY.isOnScreen = true;
                closeUIBoxIfOpen(1);
                if (UI_BOX_DATA[1].frame == 0) TRIANGLE_MENU_STATE = 0;
                break;
            }
            case 5:
            {
                closeUIBoxIfOpen(0);
                if (UI_BOX_DATA[0].frame == 0)
                {
                    createMenuBox(1, -150, -89, 300, 190, 0, tickPlayerMenu, renderPlayerMenu);
                    // TODO move this draw into the menu box tick/render
                    clearTextArea();
                    drawStringNew(&vanillaFont, PLAYER_MENU_TABS, 704, 256);
                    MENU_STATE          = 0;
                    MENU_SUB_STATE      = 0;
                    TRIANGLE_MENU_STATE = 0xFFFFFFFF;
                    PLAYER_MENU_STATE   = 0;
                }
                break;
            }
            case 6:
            {
                TAMER_ENTITY.isOnScreen   = true;
                PARTNER_ENTITY.isOnScreen = true;
                closeUIBoxIfOpen(1);
                if (UI_BOX_DATA[1].frame == 0) TRIANGLE_MENU_STATE = 0;
                break;
            }
        }
    }

    void addGameMenu()
    {
        HAS_FISHING_ROD = hasFishingRod();
        if (HAS_FISHING_ROD)
        {
            MENU_OPTION_COUNT       = 8;
            MENU_OPTIONS[6].enabled = true;
            MENU_POINTER.selection  = 7;
        }
        else
        {
            MENU_OPTION_COUNT       = 7;
            MENU_OPTIONS[6].enabled = false;
            MENU_POINTER.selection  = 1;
        }
        MENU_OPTIONS[6].enabled = PARTNER_PARA.condition.isSleepy;
        TRIANGLE_MENU_STATE     = 0;
        addObject(ObjectID::GAME_MENU, 0, tickTriangleMenu, nullptr);
    }
}