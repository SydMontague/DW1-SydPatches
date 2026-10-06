#include "Curling.hpp"
#include "CurlingRenderTypes.hpp"
#include "CurlingTypes.hpp"

#include "../Input.hpp"
#include "../Sound.hpp"
#include "../UIElements.hpp"
#include "../VanillaText.hpp"
#include "../extern/dw1.hpp"

extern const dtl::array<const char*, 2> KAR_PROMPT_STRINGS;

namespace
{
    constexpr RECT FINAL_RECT        = {-130, 42, 262, 59};
    constexpr RECT PROMPT_START_RECT = {75, -5, 10, 10};
    constexpr RECT HINT_START_RECT   = {0, 0, 10, 10};
    uint8_t hintLine                 = 0;
} // namespace

extern "C"
{
    [[gnu::optimize("Os")]]
    int32_t KAR_tickYesNoPrompt()
    {
        if (KAR_HINT_STATE < 2) {
            const int32_t line = KAR_HINT_STATE;
            // TODO: Migrate to AtlasStrings when their allocation lifetime is managed.
            drawString(KAR_PROMPT_STRINGS[line], 0, line * 13 + 221);
            KAR_HINT_STATE++;
        }

        if (UI_BOX_DATA[0].state == 0) {
            if (KAR_HINT_STATE == 2) {
                createAnimatedUIBox(0, 0, 2, &FINAL_RECT, &PROMPT_START_RECT, nullptr, nullptr);
                KAR_HINT_STATE = 3;
            }
            else if (KAR_HINT_STATE == 3 || KAR_HINT_STATE == 4)
                return static_cast<int8_t>(KAR_HINT_STATE - 2);
        }

        if (UI_BOX_DATA[0].state == 1) {
            if (isKeyDownPolled(InputButtons::BUTTON_CROSS)) {
                removeAnimatedUIBox(0, nullptr);
                return 0;
            }
            if (isKeyDownPolled(InputButtons::BUTTON_UP)) {
                KAR_HINT_STATE = KAR_HINT_STATE == 3 ? 4 : 3;
                playSound(0, 2);
            }
            else if (isKeyDownPolled(InputButtons::BUTTON_DOWN)) {
                KAR_HINT_STATE = KAR_HINT_STATE == 3 ? 4 : 3;
                playSound(0, 2);
            }
            renderString(0, -124, 48, 82, 13, 0, 220, 6, 1);
            renderString(0, -124, 61, 92, 13, 83, 220, 6, 1);
            renderSelectionCursor(FINAL_RECT.x + 6, FINAL_RECT.y + 7 + (KAR_HINT_STATE - 3) * 13, 94, 13, 6);
        }
        return 0;
    }

    [[gnu::optimize("Os")]]
    int32_t KAR_tickHintBox(int32_t page)
    {
        if (KAR_HINT_STATE == 0) return 0;
        const int8_t mode = KAR_HINT_STATE;
        if (UI_BOX_DATA[0].state == 0) {
            if (mode != 1) return 0;
            const RECT* startPosition = page < 4 ? &PROMPT_START_RECT : &HINT_START_RECT;
            createAnimatedUIBox(0, 0, 2, &FINAL_RECT, startPosition, nullptr, nullptr);
            clearTextArea();
            hintLine = 0;
            return 1;
        }

        const int8_t pageCount =
            static_cast<int8_t>(KAR_drawHintPage(page, static_cast<int8_t>(hintLine), KAR_OPPONENT_ENTITY == 2));
        if (hintLine < pageCount * 4) hintLine++;

        if (UI_BOX_DATA[0].state == 1) {
            if (isKeyDownPolled(InputButtons::BUTTON_CROSS)) {
                KAR_HINT_STATE++;
                playSound(0, 3);
                if (pageCount < KAR_HINT_STATE) {
                    removeAnimatedUIBox(0, nullptr);
                    KAR_HINT_STATE = -1;
                }
            }
            if (KAR_HINT_STATE != -1) {
                renderString(0,
                             UI_BOX_DATA[0].finalPos.x + 4,
                             UI_BOX_DATA[0].finalPos.y + 2,
                             252,
                             52,
                             0,
                             (KAR_HINT_STATE - 1) * 52,
                             6,
                             1);
                renderUIBox(0);
            }
        }
        return 1;
    }
}
