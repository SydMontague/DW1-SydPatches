#include "AtlasFont.hpp"
#include "Font.hpp"
#include "Input.hpp"
#include "Sound.hpp"
#include "extern/dw1.hpp"

namespace
{
    constexpr RECT final{.x = -132, .y = -11, .width = 264, .height = 22};
    constexpr RenderSettings settings{
        .x        = final.x + 6,
        .y        = final.y + 5,
        .baseClut = 1,
        .width    = final.width - 12,
        .height   = final.height - 10,
    };

    AtlasString description;

    void tickItemMenuDescriptionBox(int32_t)
    {
        constexpr auto mask = InputButtons::BUTTON_START | InputButtons::BUTTON_CROSS | InputButtons::BUTTON_TRIANGLE;

        if (UI_BOX_DATA[3].state != 1) return;
        if (!isXPressedAfterDialogue()) return;
        if (!isKeyDown(mask)) return;

        triggerBoxCloseFlag(3);
        playSound(0, 3);
        description = {};
    }

    void renderItemMenuDescriptionBox(int32_t)
    {
        description.render(1);
    }
} // namespace

extern "C"
{
    bool createItemMenuDescriptionBox(ItemMenuBox* menu, RECT* rect, int32_t boxId)
    {
        auto offset = (menu->scrollOffset + menu->cursorOffset) * 2;
        auto item   = menu->itemList[offset];

        if (item == 0xFF) return false;

        description = getAtlasVanilla().render(ITEM_DESC_PTR[item], settings);
        rect->x += UI_BOX_DATA[boxId].finalPos.x;
        rect->y += UI_BOX_DATA[boxId].finalPos.y + menu->cursorOffset * 18;

        createTextbox(3, 0xC1, &final, rect, tickItemMenuDescriptionBox, renderItemMenuDescriptionBox);
        registerTextbox(3, ITEM_MENU_SUB_TEXTBOX_LINE, 1, 0, VRAMMode::FRONT_FULL);

        return true;
    }
}
