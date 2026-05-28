#include "LabeledBorder.hpp"

#include "UIElements.hpp"
#include "extern/libgs.hpp"

void LabeledBorder::init(RECT panelRect, const char* label, RGB8 labelColor, RGB8 fill)
{
    panel     = panelRect;
    fillColor = fill;
    labelStr  = getAtlas7px().render(
        label,
        {.x = static_cast<int16_t>(panelRect.x + 8),
         .y = static_cast<int16_t>(panelRect.y - 4),
         .color = labelColor});
}

void LabeledBorder::render(int32_t depth) const
{
    RECT rect = panel;
    renderUIBoxBorder(&rect, depth);

    // Submit label first, then erase, so within OT[depth - 1] the erase ends up
    // at the list head (rendered first = behind) and label sprites trail it
    // (rendered after = on top). Flipping this hides the bottom of the label
    // under the erase rect.
    labelStr.render(depth - 1);

    const int16_t labelW = static_cast<int16_t>(labelStr.getWidth());
    const int16_t notchX = panel.x + 6;
    const int16_t notchW = labelW + 4;

    GsBOXF erase{
        .attribute = 0,
        .x         = static_cast<int16_t>(notchX - 1),
        .y         = panel.y,
        .width     = static_cast<uint16_t>(notchW + 2),
        .height    = 3,
        .r         = fillColor.red,
        .g         = fillColor.green,
        .b         = fillColor.blue,
    };
    libgs_GsSortBoxFill(&erase, ACTIVE_ORDERING_TABLE, depth - 1);
}
