#include "Files.hpp"
#include "Font.hpp"
#include "Helper.hpp"
#include "Input.hpp"
#include "Math.hpp"
#include "MenuTab.hpp"
#include "PlayerCardView.hpp"
#include "PlayerChartView.hpp"
#include "PlayerInfoView.hpp"
#include "PlayerMedalView.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dtl/unique_ptr.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"

namespace
{
    constexpr auto WINDOW_X      = -150;
    constexpr auto WINDOW_Y      = -89;
    constexpr auto WINDOW_WIDTH  = 300;
    constexpr auto WINDOW_HEIGHT = 190;
    constexpr auto TAB1_WIDTH    = 72;
    constexpr auto TAB2_WIDTH    = 70;
    constexpr auto TAB3_WIDTH    = 60;
    constexpr auto TAB4_WIDTH    = 60;
    constexpr auto TAB1_X        = WINDOW_X + 5;
    constexpr auto TAB2_X        = TAB1_X + TAB1_WIDTH - 1;
    constexpr auto TAB3_X        = TAB2_X + TAB2_WIDTH - 1;
    constexpr auto TAB4_X        = TAB3_X + TAB3_WIDTH - 1;
    constexpr auto TAB_Y         = WINDOW_Y - 15;

    struct PlayerMenu
    {
        MenuTab playerTab{TAB1_X, TAB_Y, TAB1_WIDTH, false, "Player"};
        MenuTab chartTab{TAB2_X, TAB_Y, TAB2_WIDTH, true, "Chart"};
        MenuTab medalTab{TAB3_X, TAB_Y, TAB3_WIDTH, true, "Medal"};
        MenuTab cardTab{TAB4_X, TAB_Y, TAB4_WIDTH, true, "Card"};
        CardView cardView;
        MedalView medalView;
        uint8_t state{0};

        void tick();
        void render();
        void updateLabelColors();
    };

    dtl::unique_ptr<PlayerMenu> data;

    void PlayerMenu::updateLabelColors()
    {
        playerTab.setState(state != 0);
        chartTab.setState(state != 1);
        medalTab.setState(state != 2);
        cardTab.setState(state != 3);
    }

    void PlayerMenu::tick()
    {
        bool handleInput = MENU_STATE < 2;
        if (state == 0) { tickPlayerMenuPlayerView(); }
        else if (state == 1) { tickPlayerMenuChartView(); }
        else if (state == 2)
        {
            handleInput = medalView.canBeClosed();
            medalView.tick();
        }
        else if (state == 3)
        {
            handleInput = cardView.canBeClosed();
            cardView.tick();
        }

        if (handleInput)
        {
            if (isKeyDownRepeat(InputButtons::BUTTON_LEFT) && state >= 1)
            {
                state -= 1;
                updateLabelColors();
                MENU_STATE     = 0;
                MENU_SUB_STATE = 0;
                playSound(0, 2);
            }
            if (isKeyDownRepeat(InputButtons::BUTTON_RIGHT) && state < 3)
            {
                state += 1;
                updateLabelColors();
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
    }

    void PlayerMenu::render()
    {
        if (state == 0)
            renderPlayerInfoView();
        else if (state == 1)
            renderEvoChartView();
        else if (state == 2)
            medalView.render(5);
        else if (state == 3)
            cardView.render(5);

        playerTab.render(5);
        chartTab.render(5);
        medalTab.render(5);
        cardTab.render(5);
    }

    void tickPlayerMenu(int32_t instanceId)
    {
        data->tick();
    }

    void renderPlayerMenu(int32_t instanceId)
    {
        data->render();
    }
} // namespace

void addPlayerMenu()
{
    MENU_STATE                = 0;
    MENU_SUB_STATE            = 0;
    TAMER_ENTITY.isOnScreen   = false;
    PARTNER_ENTITY.isOnScreen = false;
    data                      = dtl::make_unique<PlayerMenu>();
    createMenuBox(1, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, 0, tickPlayerMenu, renderPlayerMenu);
}

void removePlayerMenu()
{
    TAMER_ENTITY.isOnScreen   = true;
    PARTNER_ENTITY.isOnScreen = true;
    closeUIBoxIfOpen(1);
    data.reset();
}