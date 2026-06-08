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
#include "PlayerMenu.hpp"
#include "Sound.hpp"
#include "UIBox.hpp"
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

    struct Data
    {
        // Original Player menu used createMenuBox features=0 → opaque fill.
        UIBox box;
        MenuTab playerTab{TAB1_X, TAB_Y, TAB1_WIDTH, false, "Player"};
        MenuTab chartTab{TAB2_X, TAB_Y, TAB2_WIDTH, true, "Chart"};
        MenuTab medalTab{TAB3_X, TAB_Y, TAB3_WIDTH, true, "Medal"};
        MenuTab cardTab{TAB4_X, TAB_Y, TAB4_WIDTH, true, "Card"};
        PlayerInfoView infoView;
        ChartView chartView;
        CardView cardView;
        MedalView medalView;
        uint8_t state{0};

        void tick();
        void render();
        void updateLabelColors();
    };

    dtl::unique_ptr<Data> data;

    void Data::updateLabelColors()
    {
        playerTab.setState(state != 0);
        chartTab.setState(state != 1);
        medalTab.setState(state != 2);
        cardTab.setState(state != 3);
    }

    void Data::tick()
    {
        bool handleInput = true;
        if (state == 0) { infoView.tick(); }
        else if (state == 1)
        {
            handleInput = chartView.canBeClosed();
            chartView.tick();
        }
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
                playSound(0, 2);
            }
            if (isKeyDownRepeat(InputButtons::BUTTON_RIGHT) && state < 3)
            {
                state += 1;
                updateLabelColors();
                playSound(0, 2);
            }

            if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
            {
                TRIANGLE_MENU_STATE = 6;
                playSound(0, 4);
            }
        }
    }

    void Data::render()
    {
        if (state == 0)
            infoView.render(5);
        else if (state == 1)
            chartView.render(5);
        else if (state == 2)
            medalView.render(5);
        else if (state == 3)
            cardView.render(5);

        playerTab.render(5);
        chartTab.render(5);
        medalTab.render(5);
        cardTab.render(5);
    }

    RECT tamerStartRect()
    {
        ScreenPos pos = getScreenPosition(TAMER_ENTITY, 1);
        return {
            .x      = static_cast<int16_t>(pos.screenX - 5),
            .y      = static_cast<int16_t>(pos.screenY - 5),
            .width  = 10,
            .height = 10,
        };
    }
} // namespace

void addPlayerMenu()
{
    TAMER_ENTITY.isOnScreen   = false;
    PARTNER_ENTITY.isOnScreen = false;
    data                      = dtl::make_unique<Data>();
    data->box = UIBox({WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT},
                      {.fill = UIBox::Fill::OPAQUE},
                      tamerStartRect());
}

void removePlayerMenu()
{
    if (!data) return;
    data->box.closeIfOpen();
}

bool isPlayerMenuActive()
{
    return static_cast<bool>(data);
}

bool isPlayerMenuOpened()
{
    return data && data->box.isOpen();
}

void tickPlayerMenu()
{
    if (!data) return;
    data->box.tick();
    if (data->box.isClosed())
    {
        TAMER_ENTITY.isOnScreen   = true;
        PARTNER_ENTITY.isOnScreen = true;
        data.reset();
        return;
    }
    if (data->box.isOpen()) data->tick();
}

void renderPlayerMenu(int32_t layer)
{
    if (!data) return;
    if (data->box.isOpen()) data->render();
    data->box.render(layer);
}