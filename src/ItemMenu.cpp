#include "Helper.hpp"
#include "Input.hpp"
#include "Inventory.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "extern/dw1.hpp"

namespace
{
    constexpr auto BUYABLE_TRIGGER_START = 0x180;

    bool readSelectedItemMerit()
    {
        auto menu      = getItemMenuFromType();
        auto item      = menu->scrollOffset + menu->cursorOffset;
        auto entry     = reinterpret_cast<ItemMenuSellEntry*>(menu->itemList[item]);
        SHOP_ITEM_TYPE = static_cast<ItemType>(entry->itemType);

        if (SHOP_ITEM_TYPE == ItemType::NONE || entry->amount == 0) {
            playSound(0, 11);
            return false;
        }

        SHOP_VARIABLE       = getItem(SHOP_ITEM_TYPE)->meritValue;
        SCRIPT_STATE_2      = 11;
        SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::NONE;
        playSound(0, 3);
        return true;
    }
} // namespace

extern "C"
{
    void shopFillBuyItemList()
    {
        auto size                 = INVENTORY_SIZE;
        bool hasSpace             = false;
        auto itemList             = reinterpret_cast<ItemMenuBuyEntry*>(ITEM_MENU_LEFT->itemList);
        ITEM_MENU_LEFT->itemCount = 0;

        for (auto i = 0; i < INVENTORY_SIZE; i++) {
            if (INVENTORY_ITEM_TYPES[i] == ItemType::NONE) {
                hasSpace = true;
                break;
            }
        }

        // TODO use reflections once possible
        for (auto i = 0; i < 128; i++) {
            auto itemType = static_cast<ItemType>(i);

            if (!isTriggerSet(BUYABLE_TRIGGER_START + i)) continue;

            ITEM_MENU_LEFT->itemCount++;
            itemList->itemType = i;
            itemList->buyable  = true;

            if (MONEY < getItem(itemType)->value)
                itemList->buyable = false;
            else {
                auto itemCount = getItemCount(itemType);
                if (itemCount == 99 || (!hasSpace && itemCount == 0)) itemList->buyable = false;
            }

            itemList++;
        }
    }

    bool shopFillSellItemList()
    {
        auto canSellAnything       = false;
        auto itemList              = reinterpret_cast<ItemMenuSellEntry*>(ITEM_MENU_RIGHT->itemList);
        ITEM_MENU_RIGHT->itemCount = 0;

        for (auto i = 0; i < INVENTORY_SIZE; i++) {
            auto type   = INVENTORY_ITEM_TYPES[i];
            auto amount = INVENTORY_ITEM_AMOUNTS[i];

            if (type != ItemType::NONE && getItem(type)->dropable) {
                amount |= 0x80;
                canSellAnything = true;
            }

            ITEM_MENU_RIGHT->itemCount++;
            itemList->itemType = static_cast<uint8_t>(type);
            itemList->amount   = amount;
            itemList++;
        }

        return canSellAnything;
    }

    void tickItemMenu()
    {
        auto menu = getItemMenuFromType();

        if (isItemMenuBoxBusy(menu)) return;
        if (UI_BOX_DATA[3].state != 0) return;
        if (UI_BOX_DATA[1].state != 1) return;
        if (!isXPressedAfterDialogue()) return;
        if (SHOP_VARIABLE != 0) return;
        if (SCRIPT_STATE_2 != 1) return;

        if (isKeyDown(InputButtons::BUTTON_CROSS)) {
            RECT rect = ITEM_MENU_DESCRIPTION_RECTS[ITEM_MENU_TYPE];
            if (ITEM_MENU_TYPE == 5)
                createSingleCardShopMenu(&rect);
            else if (ITEM_MENU_TYPE == 7)
                readSelectedItemMerit();
            else
                createItemMenuAmountBox(&rect);
        }
        else if (isKeyDown(InputButtons::BUTTON_TRIANGLE)) {
            if (ITEM_MENU_TYPE == 7)
                SCRIPT_STATE_2 = 10;
            else if (ITEM_MENU_TYPE == 5) {
                if (isTriggerSet(3)) {
                    writePStat(254, 255);
                    unsetTrigger(3);
                    SCRIPT_STATE_2 = 4;
                }
            }
            else
                SCRIPT_STATE_2 = 8;
            playSound(0, 4);
        }
        else if (isKeyDown(InputButtons::BUTTON_UP)) {
            if (isKeyPressed(InputButtons::BUTTON_R1))
                itemMenuCursorTop(menu, 9, 0);
            else
                itemMenuCursorUp(menu, 0);
        }
        else if (isKeyDown(InputButtons::BUTTON_DOWN)) {
            if (isKeyPressed(InputButtons::BUTTON_R1))
                itemMenuCursorBottom(menu, 9, 0);
            else
                itemMenuCursorDown(menu, 0);
        }
        else if (isKeyDown(InputButtons::BUTTON_START)) {
            RECT rect = ITEM_MENU_DESCRIPTION_RECTS[ITEM_MENU_TYPE];
            createItemMenuDescriptionBox(menu, &rect, 1);
            playSound(0, 3);
        }
    }

    void renderItemMenu()
    {
        constexpr dtl::array<int16_t, 8> selectionCursorWidths{170, 202, 202, 170, 202, 146, 146, 170};
        const auto x        = UI_BOX_DATA[1].finalPos.x;
        const auto y        = UI_BOX_DATA[1].finalPos.y;
        const auto itemPosY = y + 5;

        renderItemMenuSprite(1, 0, x + 8, itemPosY);
        if (ITEM_MENU_TYPE == 5) {
            renderItemMenuSprite(1, 3, x + 128, itemPosY);
        }
        else if (ITEM_MENU_TYPE == 7)
            renderItemMenuSprite(1, 2, x + 128, itemPosY);
        else {
            renderItemMenuSprite(1, 1, x + 128, itemPosY);
            if (ITEM_MENU_TYPE != 0) renderItemMenuSprite(1, 3, x + 182, itemPosY);
        }

        auto menu = getItemMenuFromType();
        renderItemMenuScrollBar(menu);
        renderSelectionCursor(x + 5, y + menu->cursorOffset * 18 + 17, selectionCursorWidths[ITEM_MENU_TYPE], 18, 5);
        renderItemMenuItemList(menu, x + 26, y + 19, x + 8, y + 18, 0);
    }
}
