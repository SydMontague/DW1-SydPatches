
#include "../AtlasFont.hpp"
#include "../GameObjects.hpp"
#include "../UIBox.hpp"

namespace
{
    constexpr auto WINDOW_X      = -132;
    constexpr auto WINDOW_Y      = -27;
    constexpr auto WINDOW_WIDTH  = 264;
    constexpr auto WINDOW_HEIGHT = 54;

    constexpr RECT BOX_START{.x = -10, .y = -10, .width = 20, .height = 20};
    constexpr RECT BOX_RECT{.x = WINDOW_X, .y = WINDOW_Y, .width = WINDOW_WIDTH, .height = WINDOW_HEIGHT};

    constexpr RenderSettings LABEL1_SETTING{
        .x        = WINDOW_X,
        .y        = WINDOW_Y + WINDOW_HEIGHT - 32,
        .baseClut = 1,
        .color    = TEXT_COLOR_LIGHT_GREY,
        .width    = WINDOW_WIDTH,
        .height   = 14,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::TOP,
    };
    constexpr RenderSettings LABEL2_SETTING{
        .x        = WINDOW_X,
        .y        = WINDOW_Y + WINDOW_HEIGHT - 18,
        .baseClut = 1,
        .color    = TEXT_COLOR_LIGHT_GREY,
        .width    = WINDOW_WIDTH,
        .height   = 14,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::TOP,
    };
    constexpr RenderSettings NAME1_SETTING{
        .x        = WINDOW_X,
        .y        = WINDOW_Y + 4,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = WINDOW_WIDTH / 2 - 32,
        .height   = 14,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::BOTTOM,
    };
    constexpr RenderSettings NAME2_SETTING{
        .x        = WINDOW_X + WINDOW_WIDTH / 2 + 32,
        .y        = WINDOW_Y + 4,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = WINDOW_WIDTH / 2 - 32,
        .height   = 14,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::BOTTOM,
    };
    constexpr RenderSettings DAMAGE1_SETTING{
        .x        = WINDOW_X,
        .y        = WINDOW_Y + 26,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = WINDOW_WIDTH / 2 - 32,
        .height   = 14,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::BOTTOM,
    };
    constexpr RenderSettings DAMAGE2_SETTING{
        .x        = WINDOW_X + WINDOW_WIDTH / 2 + 32,
        .y        = WINDOW_Y + 26,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = WINDOW_WIDTH / 2 - 32,
        .height   = 14,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::BOTTOM,
    };

    dtl::array<AtlasString, 6> getStrings()
    {
        auto font = getAtlasVanilla();
        return {{
            getAtlasVanilla().render("Damage", LABEL1_SETTING),
            getAtlasVanilla().render("Done", LABEL2_SETTING),
            getAtlasVanilla().render(VS_DIGIMON_P1_PTR->name.data(), NAME1_SETTING),
            getAtlasVanilla().render(VS_DIGIMON_P2_PTR->name.data(), NAME2_SETTING),
            getAtlasVanilla().render(format("%d", VS__DAMAGE[0]).data(), DAMAGE1_SETTING),
            getAtlasVanilla().render(format("%d", VS__DAMAGE[1]).data(), DAMAGE2_SETTING),
        }}; // namespace
    }

    struct TimeoutWindow
    {
        dtl::array<AtlasString, 6> strings = getStrings();
        UIBox box{BOX_RECT, {}, true, BOX_START};

        void render(int32_t depth);
        void tick();
    };

    void TimeoutWindow::render(int32_t depth)
    {
        if (box.getState() == UIBox::State::OPENED)
            for (auto& str : strings)
                str.render(depth);
        box.render(depth);
    }

    void TimeoutWindow::tick()
    {
        box.tick();
    }

    dtl::unique_ptr<TimeoutWindow> data;

    void render(int32_t instance)
    {
        if (data) data->render(5);
    }

    void tick(int32_t instance)
    {
        if (data) data->tick();
    }
} // namespace

void VS__addTimeoutWindow()
{
    data = dtl::make_unique<TimeoutWindow>();
    addObject(ObjectID::VS_TIMEOUT_WINDOW, 0, tick, render);
}

void VS__removeTimeoutWindow()
{
    removeObject(ObjectID::VS_TIMEOUT_WINDOW, 0);
    data.reset();
}
