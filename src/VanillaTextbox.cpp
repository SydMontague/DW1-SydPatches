#include "Helper.hpp"
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    bool drawTextboxStrings(int32_t boxId, int32_t unused)
    {
        auto& box = TEXTBOX_DATA[boxId];

        if (box.writeCount == box.renderCount) return false;

        auto coords = getVRAMModeCordsNew(box.vramMode);

        auto offset = box.lineOffset + box.writeRow;
        auto* str   = TEXTBOX_LINES_PTR + offset * 0x40;

        if (coords.posX != 0) str += 0x20;

        auto stringCount = box.vramMode == VRAMMode::FRONT_FULL ? 1 : 2;
        for (auto i = 0; i < stringCount; i++) {
            auto isTerminated = drawString2(str + (0x40 * i), coords.posX, (offset + i) * 12, unused);
            box.writeRow++;
            if (isTerminated != 0) {
                box.writeCount  = 1;
                box.renderCount = 1;
                box.flipCount   = 0;
                box.registered  = 1;
                break;
            }
        }

        return true;
    }

    void clearTextboxLineCount(int32_t boxId)
    {
        auto& box = TEXTBOX_DATA[boxId];

        auto freedLines = box.lineCount;
        if (box.isDoubleBuffered) freedLines *= 2;
        box.lineCount  = 0;
        box.registered = 1;
        TEXTBOX_LINES_USED -= freedLines;
    }
}
