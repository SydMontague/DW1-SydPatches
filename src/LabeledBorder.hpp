#pragma once

#include "AtlasFont.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"

// A labeled section border: the standard UI-box border with a notch in the
// top edge where a label sits. Caller supplies the panel rect and the label
// string at init-time; render() draws border + label + notch erase at the
// requested depth (label and erase use depth-1, matching the convention used
// throughout the inventory).
struct LabeledBorder
{
    LabeledBorder() = default;

    // Bakes the label sprite at the canonical notch position (panel.x+8, panel.y-4).
    // fillColor is used to erase the top border across the notch and should match
    // whatever panel fill sits behind this border (defaults to the inventory's
    // {12, 22, 30} blue-grey).
    LabeledBorder(RECT panel,
                  const char* label,
                  RGB8 labelColor = {0xD2, 0xFF, 0xFF},
                  RGB8 fillColor  = {0x0c, 0x16, 0x1e});

    void render(int32_t depth) const;

    const RECT& bounds() const { return panel; }

private:
    RECT        panel{};
    AtlasString labelStr{};
    RGB8        fillColor{0x0c, 0x16, 0x1e};
};
