#include "ItemMenu.hpp"
#include "BitsBox.hpp"
#include "Font.hpp"
#include "Helper.hpp"
#include "Input.hpp"
#include "Inventory.hpp"
#include "InventoryUI.hpp"
#include "ItemMenuAmountBox.hpp"
#include "RecycleShop.hpp"
#include "Script.hpp"
#include "SingleCardConfirmMenu.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "extern/dtl/algorithm.hpp"
#include "extern/dw1.hpp"

namespace
{
    constexpr auto BUYABLE_TRIGGER_START = 0x180;

    bool readSelectedItemMerit()
    {
        auto menu      = getItemMenuFromType();
        auto item      = menu->scrollOffset + menu->cursorOffset;
        auto entry     = reinterpret_cast<ItemMenuSellEntry*>(menu->itemList[item]);
        SHOP_ITEM_TYPE = entry->itemType;

        if (static_cast<ItemType>(SHOP_ITEM_TYPE) == ItemType::NONE || entry->amount == 0) {
            playSound(0, 11);
            return false;
        }

        SHOP_VARIABLE       = getItem(static_cast<ItemType>(SHOP_ITEM_TYPE))->meritValue;
        SCRIPT_STATE_2      = 11;
        SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::NONE;
        playSound(0, 3);
        return true;
    }

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

    bool hasAnythingToSell()
    {
        for (auto i = 0; i < 128; i++)
            if (isTriggerSet(0x180 + i)) return true;

        return false;
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
    void tickItemMenu(int32_t)
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
                createSingleCardConfirmBox(&rect);
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

    void renderItemMenu(int32_t)
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
} // namespace

extern "C"
{
    void tickItemMenuDescriptionBox()
    {
        constexpr auto mask = InputButtons::BUTTON_START | InputButtons::BUTTON_CROSS | InputButtons::BUTTON_TRIANGLE;

        if (UI_BOX_DATA[3].state != 1) return;
        if (!isXPressedAfterDialogue()) return;
        if (!isKeyDown(mask)) return;

        triggerBoxCloseFlag(3);
        playSound(0, 3);
    }

    void renderItemMenuDescriptionBox()
    {
        const auto& pos   = UI_BOX_DATA[3].finalPos;
        const auto offset = TEXTBOX_DATA[3].lineOffset * 12;
        renderStringNew(0, pos.x + 6, pos.y + 5, 252, 12, 704, offset + 256, 3, 1);
    }

    void allocateItemMenuBox(ItemMenuBox** data,
                             uint32_t bufferSize,
                             int32_t numSlots,
                             int16_t xOffset,
                             int16_t yOffset,
                             int32_t width,
                             int32_t height)
    {
        // TODO turn ItemMenuBox into a proper RAII object
        *data                  = new ItemMenuBox();
        (*data)->itemList      = new uint8_t[bufferSize];
        (*data)->isInitialized = 0;
        (*data)->numSlots      = numSlots;
        (*data)->xOffset       = xOffset;
        (*data)->yOffset       = yOffset;
        (*data)->scrollWidth   = width;
        (*data)->scrollHeight  = height;
    }

    void destroyItemMenuBox(ItemMenuBox** data)
    {
        delete[] (*data)->itemList;
        delete *data;
        *data = nullptr;
    }

    void showShopkeeperTextbox(int32_t line, int32_t speaker, int32_t textboxId)
    {
        showMapheadTextbox(line, speaker, textboxId, 0xFF);
    }

    void showShopkeepSelection(int32_t boxId, int32_t speaker, int32_t selectionCount, uint32_t* result)
    {
        showMapheadSelection(boxId, speaker, selectionCount, result, 0xFF);
    }

    void initItemMenuBox(ItemMenuBox* menu, int8_t textboxId, uint8_t stringOffset)
    {
        if (menu->isInitialized) return;

        menu->isInitialized = true;
        menu->textboxId     = textboxId;
        menu->scrollOffset  = 0;
        menu->cursorOffset  = 0;
        menu->unk2          = 0;
        menu->unk3          = 0;
        for (auto i = 0; i < menu->numSlots; i++)
            menu->stringOffset[i] = stringOffset + i;
    }

    [[gnu::optimize("Os")]]
    void updateItemMenuStrings(ItemMenuBox* menu, int32_t lineStart, int32_t mode)
    {
        auto& textbox    = TEXTBOX_DATA[menu->textboxId];
        auto stringCount = dtl::min(menu->itemCount - menu->scrollOffset, static_cast<int32_t>(menu->numSlots));

        if (stringCount == 0) {
            auto ptr = TEXTBOX_LINES_PTR + lineStart * 0x40;
            if (textbox.vramMode == VRAMMode::END_HALF) {
                ptr += 0x20;
            }
            if (textbox.isDoubleBuffered) {
                ptr += ((textbox.activeBufferId ^ 1) * textbox.lineCount * 0x40);
            }

            ptr[0] = 0;
            ptr[1] = 0;
        }
        else {
            for (auto i = 0; i < stringCount; i++) {
                auto withNewLine = menu->stringOffset[i] == (lineStart + stringCount - 1);
                switch (mode) {
                    case 0: calculateItemMenuStrings(menu, i, withNewLine); break;
                    case 1: calculateCardMenuStrings(menu, i, withNewLine); break;
                    case 2: calculateMusicMenuStrings(menu, i, withNewLine); break;
                    case 3: calculateBirdramonMenuStrings(menu, i, withNewLine); break;
                    default: calculateItemListStrings(menu, i, withNewLine); break;
                }
            }
        }

        textbox.pageReady = 1;
        textbox.writeCount++;
    }

    bool isItemMenuBoxBusy(ItemMenuBox* box)
    {
        if (box == nullptr) return true;

        return isTextboxBusy(box->textboxId);
    }

    void createItemMenu()
    {
        auto speaker = (ITEM_MENU_TYPE == 1 || ITEM_MENU_TYPE == 5) ? 0xFD : readPStat(0xFE);

        RECT box;
        setupBoxOrigin(speaker, &box);
        auto* menu = getItemMenuFromType();
        createTextbox(1, 0xF1, &ITEM_MENU_POS[ITEM_MENU_TYPE], &box, tickItemMenu, renderItemMenu);
        registerTextbox(1, 9, 6, true, VRAMMode::FRONT_FULL);
        initItemMenuBox(menu, 1, 9);
        updateItemMenuStrings(menu, 9, 0);
        SHOP_VARIABLE = 0;
    }

    void openDiscardItem()
    {
        switch (SCRIPT_STATE_2) {
            case 0:
            {
                allocateItemMenuBox(&ITEM_MENU_RIGHT, INVENTORY_SIZE << 1, 6, 0x9a, 0x18, 6, 0x5a);
                if (shopFillSellItemList() == 0) {
                    setTrigger(3);
                    writePStat(0xfe, 0xff);
                    SCRIPT_STATE_2      = 2;
                    SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::NONE;
                }
                else {
                    SCRIPT_STATE_2      = 3;
                    SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::NONE;
                }
                break;
            }
            case 2:
            {
                destroyItemMenuBox(&ITEM_MENU_RIGHT);
                ACTIVE_INSTRUCTION = 0;
                break;
            }
            case 3:
            {
                setInputRepeatMask(InputButtons::BUTTON_UP | InputButtons::BUTTON_DOWN);
                ITEM_MENU_TYPE = 5;
                createItemMenu();
                SCRIPT_STATE_2 = 1;
                break;
            }
            case 4:
            {
                setInputRepeatMask(0);
                triggerBoxCloseFlag(1);
                SCRIPT_STATE_2 = 2;
                break;
            }
        }
    }

    ItemMenuBox* getItemMenuFromType(void)
    {
        switch (ITEM_MENU_TYPE) {
            case 0:
            case 2:
            case 3:
            case 4:
            case 6: return ITEM_MENU_LEFT;
            case 1:
            case 5:
            case 7: return ITEM_MENU_RIGHT;
        }
        return nullptr;
    }

    void openShop()
    {
        const auto speaker = readPStat(0xFE);

        switch (SCRIPT_STATE_2) {
            case 0:
            {
                allocateItemMenuBox(&ITEM_MENU_LEFT, 256, 6, 178, 24, 6, 90);
                allocateItemMenuBox(&ITEM_MENU_RIGHT, INVENTORY_SIZE * 2, 6, 210, 24, 6, 90);
                SHOP_ACTION_SELECTED = 0;
                HAS_BOUGHT_ANYTHING  = 0;

                if (hasAnythingToSell()) {
                    showShopkeeperTextbox(0, speaker, 0);
                    SCRIPT_STATE_2      = 1;
                    SCRIPT_NEXT_STATE_2 = 3;
                    SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::CONFIRM;
                }
                else {
                    showShopkeeperTextbox(1, speaker, 0);
                    SCRIPT_STATE_2      = 1;
                    SCRIPT_NEXT_STATE_2 = 2;
                    SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::CONFIRM;
                }
                break;
            }
            case 2:
            {
                destroyItemMenuBox(&ITEM_MENU_RIGHT);
                destroyItemMenuBox(&ITEM_MENU_LEFT);
                ACTIVE_INSTRUCTION = 0;
                break;
            }
            case 3:
            {
                createShopBitsBox(1);
                showShopkeepSelection(2, 0xFD, 3, &SHOP_ACTION_SELECTED);
                SCRIPT_STATE_2      = 1;
                SCRIPT_NEXT_STATE_2 = 4;
                SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::SELECTION;
                break;
            }
            case 4:
            {
                setInputRepeatMask(InputButtons::BUTTON_UP | InputButtons::BUTTON_DOWN);
                ITEM_MENU_TYPE = 0;
                shopFillBuyItemList();
                createItemMenu();
                showShopkeeperTextbox(8, speaker, 0);
                SCRIPT_STATE_2      = 1;
                SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::BACKGROUND;
                break;
            }
            case 5:
            {
                setInputRepeatMask(InputButtons::BUTTON_UP | InputButtons::BUTTON_DOWN);
                ITEM_MENU_TYPE = 1;
                shopFillSellItemList();
                createItemMenu();
                showShopkeeperTextbox(9, speaker, 0);
                SCRIPT_STATE_2      = 1;
                SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::BACKGROUND;
                break;
            }
            case 6:
            {
                triggerBoxCloseFlag(2);
                if (HAS_BOUGHT_ANYTHING == 0)
                    showShopkeeperTextbox(5, speaker, 0);
                else
                    showShopkeeperTextbox(4, speaker, 0);
                SCRIPT_STATE_2      = 1;
                SCRIPT_NEXT_STATE_2 = 2;
                SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::CONFIRM;
                break;
            }
            case 7:
            {
                setInputRepeatMask(0);
                triggerBoxCloseFlag(1);
                if (ITEM_MENU_TYPE == 0 && isPartnerBaby())
                    showShopkeeperTextbox(13, speaker, 0);
                else
                    showShopkeeperTextbox(10, speaker, 0);

                SCRIPT_STATE_2      = 1;
                SCRIPT_NEXT_STATE_2 = 3;
                SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::CONFIRM;
                HAS_BOUGHT_ANYTHING = 1;
                break;
            }
            case 8:
            {
                setInputRepeatMask(0);
                triggerBoxCloseFlag(1);
                showShopkeeperTextbox(11, speaker, 0);
                SCRIPT_STATE_2 = 9;
                break;
            }
            case 9:
            {
                SCRIPT_STATE_2      = 1;
                SCRIPT_NEXT_STATE_2 = 3;
                SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::CONFIRM;
                break;
            }
        }
    }

    void itemMenuCursorTop(ItemMenuBox* menu, int32_t count, int32_t mode)
    {
        itemMenuCursorMoveToTop(menu);
        updateItemMenuStrings(menu, count, mode);
    }

    void itemMenuCursorUp(ItemMenuBox* menu, int32_t mode)
    {
        if (menu->scrollOffset + menu->cursorOffset == 0) {
            playSound(0, 11);
            return;
        }

        if (menu->cursorOffset != 0) {
            menu->cursorOffset--;
        }
        else {
            menu->scrollOffset--;

            auto backup = menu->stringOffset[menu->numSlots - 1];
            for (auto i = menu->numSlots - 1; i > 0; i--)
                menu->stringOffset[i] = menu->stringOffset[i - 1];
            menu->stringOffset[0] = backup;

            updateItemMenuLine(menu, mode);
        }

        playSound(0, 2);
    }

    void itemMenuCursorBottom(ItemMenuBox* menu, int32_t count, int32_t mode)
    {
        itemMenuCursorMoveToBottom(menu);
        updateItemMenuStrings(menu, count, mode);
    }

    void itemMenuCursorDown(ItemMenuBox* menu, int32_t mode)
    {
        if (menu->scrollOffset + menu->cursorOffset + 1 >= menu->itemCount) {
            playSound(0, 11);
            return;
        }

        if (menu->cursorOffset + 1 == menu->numSlots) {
            menu->scrollOffset++;

            auto backup = menu->stringOffset[0];
            for (auto i = 1; i < menu->numSlots; i++)
                menu->stringOffset[i - 1] = menu->stringOffset[i];
            menu->stringOffset[menu->numSlots - 1] = backup;

            updateItemMenuLine(menu, mode);
        }
        else
            menu->cursorOffset++;

        playSound(0, 2);
    }
}
