#pragma once

#include "AtlasFont.hpp"
#include "UIBox.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"

// A UIBox with a label notched into its top border. Renders exactly like a
// UIBox (fill + border, plus the open/close slide animation), then—once fully
// open—draws the label and erases the border behind it. The notch erase reuses
// the box's own fill color, so it can't drift out of sync with the panel fill.
struct LabeledUIBox : UIBox
{
    LabeledUIBox() = default;

    // Bakes the label sprite at the canonical notch position (final.x+8, final.y-4).
    LabeledUIBox(RECT final,
                 const char* label,
                 Style style               = {},
                 RGB8 labelColor           = {0xD2, 0xFF, 0xFF},
                 dtl::optional<RECT> start = {});

    void render(int32_t depth);

private:
    AtlasString labelStr{};
};
