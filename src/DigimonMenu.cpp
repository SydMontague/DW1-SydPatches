#include "DigimonMenu.hpp"

#include "AtlasFont.hpp"
#include "Entity.hpp"
#include "Font.hpp"
#include "Helper.hpp"
#include "Input.hpp"
#include "Math.hpp"
#include "MenuTab.hpp"
#include "Sound.hpp"
#include "StatsView.hpp"
#include "TechView.hpp"
#include "Timestamp.hpp"
#include "UIBox.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dtl/unique_ptr.hpp"
#include "extern/dw1.hpp"

namespace
{
    constexpr auto WINDOW_X      = -150;
    constexpr auto WINDOW_Y      = -89;
    constexpr auto WINDOW_WIDTH  = 300;
    constexpr auto WINDOW_HEIGHT = 190;
    constexpr auto TAB1_WIDTH    = 76;
    constexpr auto TAB2_WIDTH    = 64;
    constexpr auto TAB1_X        = WINDOW_X + 5;
    constexpr auto TAB2_X        = TAB1_X + TAB1_WIDTH - 1;
    constexpr auto TAB_Y         = WINDOW_Y - 15;

    struct Data
    {
    public:
        Data();

        void tick();
        void render();

        // Original Digimon menu used createMenuBox features=0 → opaque fill.
        UIBox box{UIBox::Style{.semiTrans = false}};

    private:
        void updateLabelColors();

        MenuTab statusTab;
        MenuTab techTab;
        DigimonStatsView statsView;
        TechView techView;
        uint8_t state;
    };

    dtl::unique_ptr<Data> data{};

    Data::Data()
        : state(0)
        , statusTab(TAB1_X, TAB_Y, TAB1_WIDTH, false, "Status")
        , techTab(TAB2_X, TAB_Y, TAB2_WIDTH, true, "Tech")
    {
    }

    void Data::tick()
    {
        bool handleInput = true;
        if (state == 0)
        {
            // stats menu doesn't need special interaction
            statsView.tick();
        }
        else if (state == 1)
        {
            techView.tick();
            handleInput = techView.canBeClosed();
        }

        if (handleInput)
        {
            if (isKeyDown(InputButtons::BUTTON_LEFT) && state >= 1)
            {
                state -= 1;
                updateLabelColors();
                playSound(0, 2);
            }
            if (isKeyDown(InputButtons::BUTTON_RIGHT) && state < 1)
            {
                state += 1;
                updateLabelColors();
                playSound(0, 2);
            }

            if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
            {
                TRIANGLE_MENU_STATE = 4;
                playSound(0, 4);
            }
        }
    }

    void Data::render()
    {
        if (state == 0)
            statsView.render(5);
        else if (state == 1)
            techView.render(5);

        statusTab.render(5);
        techTab.render(5);
    }

    void Data::updateLabelColors()
    {
        statusTab.setState(state != 0);
        techTab.setState(state != 1);
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

void addDigimonMenu()
{
    TAMER_ENTITY.isOnScreen   = false;
    PARTNER_ENTITY.isOnScreen = false;
    data                      = dtl::make_unique<Data>();
    data->box.open({WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT}, tamerStartRect());
}

void removeDigimonMenu()
{
    if (!data) return;
    data->box.closeIfOpen();
}

bool isDigimonMenuActive()
{
    return static_cast<bool>(data);
}

bool isDigimonMenuOpened()
{
    return data && data->box.isOpen();
}

void tickDigimonMenu()
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

void renderDigimonMenu(int32_t layer)
{
    if (!data) return;
    if (data->box.isOpen()) data->render();
    data->box.render(layer);
}

int16_t digimonMenuBoxHeight()
{
    return data ? data->box.finalPos().height : 0;
}

void setDigimonMenuBoxHeight(int16_t height)
{
    if (!data) return;
    auto rect = data->box.finalPos();
    rect.height = height;
    data->box.resize(rect);
}
