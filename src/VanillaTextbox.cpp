#include "Font.hpp"
#include "Helper.hpp"
#include "Input.hpp"
#include "Math.hpp"
#include "Script.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
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
                SELECTION_CURRENT = SELECTION_COUNT - 1;
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

    void renderDialogueSelectionCursor(int32_t x, int32_t y)
    {
        if (TEXTBOX_DATA[0].idle == 1) return;

        renderSelectionCursor(x - 1,
                              y + SELECTION_CURSOR_OFFSET_Y + SELECTION_CURRENT * 13 - 2,
                              SELECTION_CURSOR_WIDTH,
                              13,
                              6);
    }

    void tickCustomSizedTextbox()
    {
        if (flipIdleTextboxPage(0)) return;
        if (!isXPressedAfterDialogue()) return;
        if (UI_BOX_DATA[0].state != 1) return;

        if (isKeyDown(InputButtons::BUTTON_CROSS)) {
            auto result = advanceTextbox(0);
            if (result) triggerBoxCloseFlag(0);
            playSound(0, 3);
        }
        else if (isKeyDown(InputButtons::BUTTON_TRIANGLE)) {
            while (!TEXTBOX_DATA[0].pageReady)
                showTextboxReady(0, 0xFF);
            ACTIVE_INSTRUCTION   = 0;
            TEXTBOX_DATA[0].idle = 1;
            triggerBoxCloseFlag(0);
            playSound(0, 4);
        }
    }

    void renderCustomSizedTextbox()
    {
        const auto& box = TEXTBOX_DATA[0];
        const auto uvY  = (box.lineOffset + box.activeBufferId) * box.lineCount * 12 + 256;
        const auto posX = UI_BOX_DATA[0].finalPos.x + 5;
        const auto posY = UI_BOX_DATA[0].finalPos.y + 4;

        for (auto i = 0; i < box.lineCount; i++)
            renderStringNew(0, posX, posY + i * 13, 252, 12, 704, uvY + i * 12, 6, 1);
    }
}
