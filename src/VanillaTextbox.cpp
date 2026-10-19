#include "Helper.hpp"
#include "Input.hpp"
#include "Math.hpp"
#include "Script.hpp"
#include "Sound.hpp"
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

    uint16_t tickSelectionDialogue()
    {
        if (flipIdleTextboxPage(0)) return -1;
        if (!isXPressedAfterDialogue()) return -1;

        if (isKeyDown(InputButtons::BUTTON_CROSS)) {
            advanceTextbox(0);
            playSound(0, 3);
            return SELECTION_CURRENT;
        }

        if (isKeyDown(InputButtons::BUTTON_TRIANGLE)) {
            if (isTriggerSet(49)) {
                playSound(0, 11);
                return -1;
            }
            if (ACTIVE_INSTRUCTION == 0x64) {
                advanceTextbox(0);
                SELECTION_CURRENT -= 1;
                playSound(0, 4);
                return SELECTION_CURRENT;
            }

            advanceTextbox(0);
            SCRIPT_POINTER = SELECTION_END_POINTER;
            playSound(0, 4);
            return -1;
        }

        if (isKeyDown(InputButtons::BUTTON_UP)) {
            SELECTION_CURRENT = ring(SELECTION_CURRENT - 1, 0, SELECTION_COUNT);
            playSound(0, 2);
        }
        if (isKeyDown(InputButtons::BUTTON_DOWN)) {
            SELECTION_CURRENT = ring(SELECTION_CURRENT + 1, 0, SELECTION_COUNT);
            playSound(0, 2);
        }

        return -1;
    }

    int32_t tickConfirmDialogue()
    {
        if (flipIdleTextboxPage(0)) return 0;
        if (TEXTBOX_CLOSE_MODE == 0) TEXTBOX_SHOULD_RENDER = 1;
        if (!isXPressedAfterDialogue()) return 0;

        auto& textbox = TEXTBOX_DATA[0];

        switch (TEXTBOX_CLOSE_MODE) {
            case 0:
            {
                if (!isKeyDown(InputButtons::BUTTON_CROSS)) return 0;
                if (UI_BOX_DATA[0].state != 1) return 0;

                if (textbox.isDoubleBuffered) {
                    advanceTextbox(0);
                    playSound(0, 3);
                }
                else {
                    playSound(0, 3);
                    ACTIVE_INSTRUCTION = 0;
                }
                return 1;
            }
            case 2:
                if (AUTOCLOSE_FRAMES != 0) return 0;
            case 1:
            {
                if (textbox.isDoubleBuffered)
                    advanceTextbox(0);
                else
                    ACTIVE_INSTRUCTION = 0;
                return 1;
            }
        }

        return 0;
    }

    int32_t tickBackgroundDialogue()
    {
        if (flipIdleTextboxPage(0)) return 0;

        auto& box = TEXTBOX_DATA[0];

        if (box.renderCount != box.flipCount) return 0;

        box.idle      = 1;
        box.pageReady = 0;
        return 1;
    }
}
