
#include "MenuTab.hpp"

#include "Helper.hpp"

MenuTab::MenuTab(int16_t x, int16_t y, int16_t width, bool isInactive, const char* string)
    : x(x)
    , y(y)
    , width(width)
    , isInactive(isInactive)
{
    const RenderSettings settings{
        .x        = static_cast<int16_t>(x + 7),
        .y        = static_cast<int16_t>(y + 3),
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = static_cast<int16_t>(width - 16),
        .height   = 12,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::CENTER,
    };
    label = getAtlasVanilla().render(string, settings);
}

void MenuTab::setState(bool val)
{
    isInactive = val;
    label.setColor(isInactive ? TEXT_COLOR_BLUE : TEXT_COLOR_WHITE);
}

void MenuTab::render(int32_t depth)
{
    constexpr auto CLUT = getClut(0x60, 0x1EC);
    auto height         = isInactive ? 15 : 16;

    label.render(depth);
    renderRectPolyFT4(x, y, 7, height, 0xD4, 0x8C, 5, CLUT, depth, isInactive);

    for (int32_t i = 0; i < (width - 14) / 4; i++)
        renderRectPolyFT4(x + 7 + i * 4, y, 4, height, 0xE2, 0x8C, 5, CLUT, depth, isInactive);

    renderRectPolyFT4(x + width - 9, y, 7, height, 0xDB, 0x8C, 5, CLUT, depth, isInactive);
}