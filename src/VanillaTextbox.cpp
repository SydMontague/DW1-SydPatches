#include "VanillaTextbox.hpp"
#include "Font.hpp"
#include "Helper.hpp"
#include "Input.hpp"
#include "Math.hpp"
#include "Script.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

namespace
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

    bool flipIdleTextboxPage(int32_t id)
    {
        auto& box = TEXTBOX_DATA[id];

        if (!box.isDoubleBuffered) return false;
        if (!box.registered) return true;
        if (box.writeCount != box.renderCount) return true;
        if (!box.idle) return false;
        if (box.renderCount == 0) return true;

        box.idle = 0;
        box.activeBufferId ^= 1;
        box.flipCount++;
        if (!box.pageReady) ACTIVE_INSTRUCTION = 0;
        TEXTBOX_OPEN_TIMER = 0;

        return true;
    }

    bool advanceTextbox(int32_t id)
    {
        auto& box = TEXTBOX_DATA[id];

        if (!box.registered) return false;

        if (box.pageReady) {
            if (box.renderCount == box.flipCount) {
                box.idle           = 1;
                box.pageReady      = 0;
                ACTIVE_INSTRUCTION = 0;
                return true;
            }
            box.activeBufferId ^= 1;
        }
        else {
            box.activeBufferId ^= 1;
            ACTIVE_INSTRUCTION = 0;
        }
        box.flipCount++;
        TEXTBOX_OPEN_TIMER = 0;
        return false;
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
} // namespace

extern "C"
{
    void tickScriptDialogueBox(int32_t id)
    {
        if (ACTIVE_INSTRUCTION == 0x10) {
            auto selection = tickSelectionDialogue();
            if (selection != 0xFFFF) SCRIPT_POINTER = ACTIVE_SCRIPT + SELECTION_POINTER[selection];
            return;
        }

        if (ACTIVE_INSTRUCTION == 0x1A) {
            tickConfirmDialogue();
            return;
        }

        if (ACTIVE_INSTRUCTION == 0x64) {
            switch (SCRIPT_TEXTBOX_MODE) {
                case ScriptTextboxMode::BACKGROUND:
                {
                    if (tickBackgroundDialogue()) {
                        ACTIVE_INSTRUCTION  = 0x64;
                        SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::NONE;
                    }
                    break;
                }
                case ScriptTextboxMode::SELECTION:
                {
                    auto selection = tickSelectionDialogue();
                    if (selection != 0xFFFF) {
                        SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::NONE;
                        ACTIVE_INSTRUCTION  = 0x64;
                        SCRIPT_STATE_2      = SCRIPT_NEXT_STATE_2 + selection;
                    }
                    break;
                }
                case ScriptTextboxMode::CONFIRM:
                {
                    if (tickConfirmDialogue()) {
                        SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::NONE;
                        SCRIPT_STATE_2      = SCRIPT_NEXT_STATE_2;
                        ACTIVE_INSTRUCTION  = 0x64;
                    }
                    break;
                }
                case ScriptTextboxMode::NONE: break;
            }
        }
    }

    void renderScriptDialogueBox(int32_t id)
    {
        const auto& box     = UI_BOX_DATA[0];
        const auto& textbox = TEXTBOX_DATA[0];

        auto offset = textbox.lineOffset * 12 + textbox.activeBufferId * 48;
        auto posX   = box.finalPos.x + 5;
        auto posY   = box.finalPos.y + 4;

        for (int32_t i = 0; i < 4; i++)
            renderStringNew(0, posX, posY + 13 * i, 252, 12, 704, offset + 12 * i + 256, 6, 1);

        if (ACTIVE_INSTRUCTION == 0x10 ||
            (ACTIVE_INSTRUCTION == 0x64 && SCRIPT_TEXTBOX_MODE == ScriptTextboxMode::SELECTION)) {
            renderDialogueSelectionCursor(posX, posY);
        }

        if (TEXTBOX_SHOULD_RENDER) renderUIBox(0);
    }

    bool setupBoxOrigin(int32_t speaker, RECT* result)
    {
        if (speaker == 0xFF) return false;

        if (TEXTBOX_ORIGIN_X == -9999) {
            auto id = scriptIdToEntityId(speaker);
            if (id == 0xFF) return false;

            auto pos  = getScreenPosition(*ENTITY_TABLE.getEntityById(id), 1);
            result->x = pos.screenX;
            result->y = pos.screenY;
        }
        else {
            auto pos         = getScreenPosition(TEXTBOX_ORIGIN_X, TEXTBOX_ORIGIN_Y, TEXTBOX_ORIGIN_Z);
            result->x        = pos.screenX - 5;
            result->y        = pos.screenY - 5;
            TEXTBOX_ORIGIN_X = -9999;
        }

        result->width  = 10;
        result->height = 10;
        return true;
    }

    void setupDialogueBox(int32_t speaker)
    {
        RECT origin;
        auto hasSpeaker = setupBoxOrigin(speaker, &origin);
        auto flags      = hasSpeaker ? 0xa1 : 0x21;
        constexpr RECT final{
            .x      = -130,
            .y      = 42,
            .width  = 262,
            .height = 59,
        };
        createTextbox(0, flags, &final, &origin, tickScriptDialogueBox, renderScriptDialogueBox);
        registerTextbox(0, 0, 4, 1, VRAMMode::FRONT_FULL);
    }

    void triggerBoxCloseFlag(int32_t id)
    {
        auto& box = TEXTBOX_DATA[id];
        if ((box.flags & 0xF) == 0) return;
        box.flags &= 0xF0;
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

    void initializeTextbox()
    {
        for (auto& box : TEXTBOX_DATA) {
            box.flags      = 0;
            box.lineCount  = 0;
            box.registered = 1;
        }

        TEXTBOX_LINES_USED = 0;
        DELAY_FRAMES       = 0;
        AUTOCLOSE_FRAMES   = 0;
        TEXTBOX_OPEN_TIMER = 0;
    }

    void tickTextboxHandling(int32_t mode)
    {
        if (ACTIVE_INSTRUCTION == 0xFF) {
            for (const auto& box : UI_BOX_DATA) {
                if (box.state != 0) break;
                setTextColor(1);
                IS_SCRIPT_PAUSED = 1;
            }
            return;
        }

        bool alreadyDrawn = false;
        for (auto i = 0; i < TEXTBOX_DATA.size(); i++) {
            auto& box = TEXTBOX_DATA[i];

            if (box.lineCount == 0) continue;
            if (box.registered == 1 && box.writeCount != box.renderCount) {
                if (box.isDoubleBuffered) {
                    box.writeRow = (box.activeBufferId ^ 1) * box.lineCount;
                    auto coords  = getVRAMModeCordsNew(box.vramMode);
                    clearTextSubArea2(coords.posX,
                                      (box.lineOffset + box.writeRow) * 12,
                                      coords.width,
                                      box.lineCount * 12);
                    box.registered = 0;
                }
                else {
                    box.writeRow   = 0;
                    box.registered = 0;
                }
            }

            if (!alreadyDrawn && box.registered == 0) alreadyDrawn = drawTextboxStrings(i, mode);

            auto stateFlag = box.flags & 0xF;
            auto state     = UI_BOX_DATA[i].state;
            if (stateFlag == state) continue;

            if (stateFlag != 1) {
                if (state == 1)
                    closeTextbox(i, 0);
                else
                    continue;
            }

            if (box.registered == 1 && state == 0) {
                auto typeFlag = (box.flags >> 4) & 3;
                if ((box.flags & 0x80) == 0) {
                    createStaticUIBox(i,
                                      typeFlag == 0,
                                      typeFlag,
                                      &box.textboxPos,
                                      box.tickFunction,
                                      box.renderFunction);
                }
                else {
                    createAnimatedUIBox(i,
                                        typeFlag == 0,
                                        typeFlag,
                                        &box.textboxPos,
                                        &box.textboxOrigin,
                                        box.tickFunction,
                                        box.renderFunction);
                }
            }
        }

        if (DELAY_FRAMES != 0) DELAY_FRAMES--;
        if (AUTOCLOSE_FRAMES != 0) AUTOCLOSE_FRAMES--;
        TEXTBOX_SHOULD_RENDER = 0;
        TEXTBOX_OPEN_TIMER++;
    }

    void getVRAMModeCoords(VRAMMode mode, uint32_t* posX, uint32_t* width)
    {
        auto coords = getVRAMModeCordsNew(mode);
        *posX       = coords.posX;
        *width      = coords.width;
    }

    void closeBox(int32_t id)
    {
        triggerBoxCloseFlag(id);
        closeTextbox(id, nullptr);
    }

    void closeAllTextboxes()
    {
        for (auto i = 0; i < TEXTBOX_DATA.size(); i++) {
            const auto& box = TEXTBOX_DATA[i];
            if ((box.flags & 0xF) == 0) continue;
            if ((box.flags & 0x40) != 0) {
                ACTIVE_INSTRUCTION = 0xFF;
                IS_SCRIPT_PAUSED   = 0;
            }
            closeBox(i);
        }
    }

    void registerTextbox(int id, uint32_t lineOffset, uint32_t lineCount, bool isDoubleBuffered, VRAMMode vramMode)
    {
        auto& box            = TEXTBOX_DATA[id];
        box.lineOffset       = lineOffset;
        box.lineCount        = lineCount;
        box.writeRow         = 0;
        box.vramMode         = vramMode;
        box.activeBufferId   = 0;
        box.isDoubleBuffered = isDoubleBuffered;
        box.writeCount       = 0;
        box.renderCount      = 0;
        box.flipCount        = 0;
        box.registered       = 1;
        box.pageReady        = 0;
        box.idle             = 1;

        auto totalLines = lineCount;
        if (isDoubleBuffered) totalLines *= 2;
        TEXTBOX_LINES_USED += totalLines;
        setTextColor(1);
        auto coords = getVRAMModeCordsNew(vramMode);
        clearTextSubArea2(coords.posX, lineOffset * 12, coords.width, totalLines * 12);
    }

    void createTextbox(int32_t id,
                       uint8_t flags,
                       const RECT* textboxPos,
                       const RECT* textboxOrigin,
                       TickFunction tickFunction,
                       RenderFunction renderFunction)
    {
        auto& box = TEXTBOX_DATA[id];
        if ((box.flags & 0xF) == 1) closeTextbox(id, nullptr);

        box.flags          = flags;
        box.textboxPos     = *textboxPos;
        box.textboxOrigin  = *textboxOrigin;
        box.tickFunction   = tickFunction;
        box.renderFunction = renderFunction;
        setFreshDialogue();
    }
}

void closeTextbox(int32_t id, RECT* final)
{
    auto& box     = UI_BOX_DATA[id];
    auto& textbox = TEXTBOX_DATA[id];

    if (box.state == 0 || box.state == 3) return;
    if ((textbox.flags & 0x40) == 0)
        removeStaticUIBox(id);
    else
        removeAnimatedUIBox(id, final);
    clearTextboxLineCount(id);
}
