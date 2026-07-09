#include "LabeledUIBox.hpp"

#include "UIElements.hpp"
#include "extern/libgs.hpp"

LabeledUIBox::LabeledUIBox(RECT final, const char* label, Style style, RGB8 labelColor, dtl::optional<RECT> start)
    : UIBox(final, style, start)
    , labelStr(getAtlas7px().render(
          label,
          {.x = static_cast<int16_t>(final.x + 8), .y = static_cast<int16_t>(final.y - 4), .color = labelColor}))
{
}

void LabeledUIBox::render(int32_t depth)
{
    // Border + fill (or the slide animation while opening/closing).
    UIBox::render(depth);

    // The label only belongs on the static, fully-open panel.
    if (!isOpen()) return;

    const RECT& panel = finalPos();

    // Submit label first, then erase, so within OT[depth - 1] the erase ends up
    // at the list head (rendered first = behind) and label sprites trail it
    // (rendered after = on top). Flipping this hides the bottom of the label
    // under the erase rect. depth - 1 sits in front of the border drawn above.
    labelStr.render(depth - 1);

    const int16_t labelW = static_cast<int16_t>(labelStr.getWidth());
    const int16_t notchX = panel.x + 6;
    const int16_t notchW = labelW + 4;
    const RGB8 fill      = fillColor();

    GsBOXF erase{
        .attribute = 0,
        .x         = static_cast<int16_t>(notchX - 1),
        .y         = panel.y,
        .width     = static_cast<uint16_t>(notchW + 2),
        .height    = 3,
        .r         = fill.red,
        .g         = fill.green,
        .b         = fill.blue,
    };
    libgs_GsSortBoxFill(&erase, ACTIVE_ORDERING_TABLE, depth - 1);
}
