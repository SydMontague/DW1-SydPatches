#include "InventoryUI.hpp"

#include "Font.hpp"
#include "GameMenu.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Inventory.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "Sound.hpp"
#include "Tamer.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

namespace
{
    constexpr int16_t TOP_X = -152;
    constexpr int16_t TOP_Y = -104;

    constexpr int16_t BOTTOM_X = -152;
    constexpr int16_t BOTTOM_Y = 77;

    constexpr auto MENU_WIDTH  = 58;
    constexpr auto MENU_HEIGHT = 84;

    constexpr auto SLOT_WIDTH              = 138;
    constexpr auto SLOT_HEIGHT             = 18;
    constexpr auto SLOT_GAP                = 6;
    constexpr auto SLOT_OFFSET_X           = 7;
    constexpr auto SLOT_OFFSET_Y           = 7;
    constexpr auto SLOT_SPRITE_OFFSET_X    = 3;
    constexpr auto SLOT_SPRITE_OFFSET_Y    = 1;
    constexpr auto SLOT_NAME_OFFSET_X      = 22;
    constexpr auto SLOT_NAME_OFFSET_Y      = 3;
    constexpr auto SLOT_AMOUNT_OFFSET_X    = 124;
    constexpr auto SLOT_AMOUNT_OFFSET_Y    = 3;
    constexpr uint32_t MOVE_SELECTOR_COLOR = 0x5d4af1;

    constexpr dtl::array<dtl::array<uint8_t, 6>, 3> sortOrder = {{
        {0, 1, 3, 2, 5, 4},
        {2, 3, 0, 5, 4, 1},
        {1, 2, 0, 3, 5, 4},
    }};

    constexpr int16_t getSlotPosX(int32_t slotId)
    {
        return TOP_X + SLOT_OFFSET_X + (SLOT_WIDTH + SLOT_GAP) * (slotId % 2);
    }

    constexpr int16_t getSlotPosY(int32_t slotId)
    {
        return TOP_Y + SLOT_OFFSET_Y + SLOT_HEIGHT * (slotId / 2);
    }

    consteval dtl::array<SimpleTextSprite, 30> initializeNameArray()
    {
        dtl::array<SimpleTextSprite, 30> data;
        for (int32_t i = 0; i < data.size(); i++)
            data[i] = SimpleTextSprite(704 + (i % 2) * 32, 256 + (i / 2) * 12);

        return data;
    }

    consteval dtl::array<SimpleTextSprite, 30> initializeAmountArray()
    {
        dtl::array<SimpleTextSprite, 30> data;
        for (int32_t i = 0; i < data.size(); i++)
            data[i] = SimpleTextSprite(704 + (i % 2) * 32 + 24, 256 + (i / 2) * 12);

        return data;
    }

    constexpr uint8_t getUseColor(uint32_t gameState, Item* item)
    {
        if (item->itemColor == 2) return 9;
        if (GAME_STATE == 0 && item->itemColor == 0) return 9;
        if ((GAME_STATE == 1 || GAME_STATE == 2 || GAME_STATE == 3) && item->itemColor == 1) return 9;

        return 10;
    }

    bool isInventoryOpen = false;
    uint32_t state;
    uint32_t subState;
    int32_t moveSource;
    uint8_t previousSelection;
    uint8_t menuSelected;
    uint8_t sortSelected;
    uint8_t dropSelected;
    uint8_t focusedWindow;
    uint8_t repeatTimer;

    dtl::array<SimpleTextSprite, 30> itemNames   = initializeNameArray();
    dtl::array<SimpleTextSprite, 30> itemAmounts = initializeAmountArray();
    SimpleTextSprite useString(704 + 0, 256 + 180);
    SimpleTextSprite moveString(704 + 16, 256 + 180);
    SimpleTextSprite sortString(704 + 32, 256 + 180);
    SimpleTextSprite dropString(704 + 48, 256 + 180);
    SimpleTextSprite youSureString(704 + 0, 256 + 192);
    SimpleTextSprite yesString(704 + 48, 256 + 192);
    SimpleTextSprite noString(704 + 56, 256 + 192);
    SimpleTextSprite descString(704 + 0, 256 + 204);
    SimpleTextSprite sortBattleString(704 + 0, 256 + 240);
    SimpleTextSprite sortRaiseString(704 + 16, 256 + 240);
    SimpleTextSprite sortBasicString(704 + 32, 256 + 240);

    bool isInventoryKeyDown(InputButtons button)
    {
        return (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & button) != 0;
    }

    bool isInventoryKeyDownRepeat(InputButtons button)
    {
        if (!isInventoryKeyDown(button) && (repeatTimer < 7 || (POLLED_INPUT & button) == 0)) return false;
        return true;
    }

    void swapItem()
    {
        dtl::swap(INVENTORY_ITEM_TYPES[INVENTORY_POINTER], INVENTORY_ITEM_TYPES[moveSource]);
        dtl::swap(INVENTORY_ITEM_AMOUNTS[INVENTORY_POINTER], INVENTORY_ITEM_AMOUNTS[moveSource]);
        dtl::swap(INVENTORY_ITEM_NAMES[INVENTORY_POINTER], INVENTORY_ITEM_NAMES[moveSource]);
        previousSelection = moveSource;
    }

    bool isMenuEnabled(int32_t menuId, ItemType type)
    {
        if (menuId == 1 || menuId == 2) return true;

        if (type == ItemType::NONE) return false;

        auto* item = getItem(type);
        if (menuId == 0 && getUseColor(GAME_STATE, item) == 9) return true;
        if (menuId == 3 && item->dropable) return true;

        return false;
    }

    void sortItems(int32_t sortMode)
    {
        const auto& orderData = sortOrder[sortMode];

        struct Entry
        {
            ItemType type;
            uint8_t amount;
            uint8_t nameId;
            int32_t sortValue{0x7FFFFFFF};

            bool operator<(const Entry& other) const { return sortValue < other.sortValue; }
        };

        dtl::array<Entry, 30> arr;

        for (int32_t i = 0; i < arr.size(); i++)
        {
            auto type      = INVENTORY_ITEM_TYPES[i];
            auto sortValue = type == ItemType::NONE ? 255 : orderData[getItem(type)->sortingValue];

            arr[i] = {
                .type      = type,
                .amount    = INVENTORY_ITEM_AMOUNTS[i],
                .nameId    = INVENTORY_ITEM_NAMES[i],
                .sortValue = static_cast<int32_t>(type) + sortValue * 256,
            };
        }

        dtl::sort(arr.begin(), arr.end());

        for (int32_t i = 0; i < arr.size(); i++)
        {
            INVENTORY_ITEM_TYPES[i]   = arr[i].type;
            INVENTORY_ITEM_AMOUNTS[i] = arr[i].amount;
            INVENTORY_ITEM_NAMES[i]   = arr[i].nameId;
        }
    }

    void drawInventoryText()
    {
        clearTextArea();

        for (int32_t i = 0; i < INVENTORY_SIZE; i++)
        {
            auto type = INVENTORY_ITEM_TYPES[i];
            if (type == ItemType::NONE) continue;

            uint8_t amount[8];
            sprintf(amount, "%2d", INVENTORY_ITEM_AMOUNTS[i]);
            itemNames[i].draw(&vanillaFont, getItem(type)->name);
            itemAmounts[i].draw(&fixedNumbersFont, amount);
            INVENTORY_ITEM_NAMES[i] = i;
        }

        useString.draw(&vanillaFont, "Use");
        moveString.draw(&vanillaFont, "Move");
        sortString.draw(&vanillaFont, "Sort");
        dropString.draw(&vanillaFont, "Drop");
        youSureString.draw(&vanillaFont, "Are you sure?");
        yesString.draw(&vanillaFont, "Yes");
        noString.draw(&vanillaFont, "No");

        sortBattleString.draw(&vanillaFont, "Battle");
        sortRaiseString.draw(&vanillaFont, "Raise");
        sortBasicString.draw(&vanillaFont, "Basic");
    }

    void renderInventoryTop()
    {
        auto depth = 6;
        auto& box  = UI_BOX_DATA[0];

        auto itemOffset = box.rowOffset * 2;

        for (int32_t i = 0; i < box.visibleRows * 2; i++)
        {
            auto itemType = INVENTORY_ITEM_TYPES[itemOffset + i];
            auto itemName = INVENTORY_ITEM_NAMES[itemOffset + i];
            if (itemType == ItemType::NONE) continue;

            const auto posX       = getSlotPosX(i);
            const auto posY       = getSlotPosY(i);
            const auto spritePosX = posX + SLOT_SPRITE_OFFSET_X;
            const auto spritePosY = posY + SLOT_SPRITE_OFFSET_Y;
            const auto namePosX   = posX + SLOT_NAME_OFFSET_X;
            const auto namePosY   = posY + SLOT_NAME_OFFSET_Y;
            const auto amountPosX = posX + SLOT_AMOUNT_OFFSET_X;
            const auto amountPosY = posY + SLOT_AMOUNT_OFFSET_Y;

            auto textColor = getItem(itemType)->itemColor;
            if (textColor == 0xFF)
                textColor = 8;
            else
                textColor += 5;

            renderItemSprite(itemType, spritePosX, spritePosY, depth);

            itemNames[itemName].render(namePosX, namePosY, textColor, depth, true);
            itemAmounts[itemName].render(amountPosX, amountPosY, 0, depth, true);
        }

        if (moveSource != -1 && moveSource >= itemOffset && moveSource < itemOffset + (box.visibleRows) * 2)
        {
            const auto minX = getSlotPosX(moveSource);
            const auto minY = getSlotPosY(moveSource - box.rowOffset * 2);
            const auto maxX = minX + SLOT_WIDTH;
            const auto maxY = minY + SLOT_HEIGHT;

            drawLine3P(MOVE_SELECTOR_COLOR, minX, minY, minX, maxY, maxX, maxY, depth, 0);
            drawLine3P(MOVE_SELECTOR_COLOR, maxX, maxY, maxX, minY, minX, minY, depth, 0);
        }

        const auto cursorX = getSlotPosX(INVENTORY_POINTER);
        const auto cursorY = getSlotPosY(INVENTORY_POINTER - box.rowOffset * 2);
        renderSelectionCursor(cursorX, cursorY, SLOT_WIDTH, SLOT_HEIGHT, depth);
    }

    void updateItemDescription()
    {
        descString.clear();
        descString.draw(&vanillaFont, ITEM_DESC_PTR[static_cast<int32_t>(INVENTORY_ITEM_TYPES[INVENTORY_POINTER])]);
    }

    void renderInventoryBottom()
    {
        if (INVENTORY_ITEM_TYPES[INVENTORY_POINTER] == ItemType::NONE) return;

        if (INVENTORY_POINTER != previousSelection) updateItemDescription();

        auto& box = UI_BOX_DATA[1];

        descString.render(box.finalPos.x + 26, box.finalPos.y + 8, 9, 5, true);
    }

    void renderItemOptions()
    {
        auto& box = UI_BOX_DATA[2];
        auto type = INVENTORY_ITEM_TYPES[INVENTORY_POINTER];

        auto useColor  = 10;
        auto dropColor = 10;

        if (type != ItemType::NONE)
        {
            auto* item = getItem(INVENTORY_ITEM_TYPES[INVENTORY_POINTER]);
            useColor   = getUseColor(GAME_STATE, item);
            dropColor  = item->dropable ? 9 : 10;
        }
        auto baseX = box.finalPos.x + 9;
        auto baseY = box.finalPos.y + 6;

        useString.render(baseX + 3, baseY + 2, useColor, 4, true);
        moveString.render(baseX + 3, baseY + 20, 9, 4, true);
        sortString.render(baseX + 3, baseY + 38, 9, 4, true);
        dropString.render(baseX + 3, baseY + 56, dropColor, 4, true);

        renderSelectionCursor(baseX, baseY + menuSelected * 18, 40, 16, 4);
    }

    void tickSortOption()
    {
        if (focusedWindow != 3) return;

        if (isInventoryKeyDownRepeat(InputButtons::BUTTON_UP) && sortSelected > 0)
        {
            playSound(0, 2);
            sortSelected--;
        }
        if (isInventoryKeyDownRepeat(InputButtons::BUTTON_DOWN) && sortSelected < 2)
        {
            playSound(0, 2);
            sortSelected++;
        }

        if (isInventoryKeyDown(InputButtons::BUTTON_TRIANGLE)) { removeAnimatedUIBox(3, nullptr); }
        else if (isInventoryKeyDown(InputButtons::BUTTON_CROSS))
        {
            removeAnimatedUIBox(3, nullptr);
            state         = 61;
            focusedWindow = -1;

            sortItems(sortSelected);
        }
    }

    void renderSortOption()
    {
        auto baseX = UI_BOX_DATA[3].finalPos.x + 9;
        auto baseY = UI_BOX_DATA[3].finalPos.y + 6;

        sortBattleString.render(baseX + 3, baseY + 2, 9, 3, 1);
        sortRaiseString.render(baseX + 3, baseY + 20, 9, 3, 1);
        sortBasicString.render(baseX + 3, baseY + 38, 9, 3, 1);

        renderSelectionCursor(baseX, baseY + sortSelected * 18, 54, 16, 3);
    }

    void createSortOption()
    {
        sortSelected  = 0;
        focusedWindow = 3;
        state         = 52;

        RECT start = {
            .x      = static_cast<int16_t>(UI_BOX_DATA[2].finalPos.x + 9),
            .y      = static_cast<int16_t>(UI_BOX_DATA[2].finalPos.y + 42),
            .width  = 40,
            .height = 16,
        };
        RECT final = {
            .x      = static_cast<int16_t>(UI_BOX_DATA[2].finalPos.x + UI_BOX_DATA[2].finalPos.width),
            .y      = UI_BOX_DATA[2].finalPos.y,
            .width  = 72,
            .height = 66,
        };

        createAnimatedUIBox(3, 1, 0, &final, &start, tickSortOption, renderSortOption);
    }

    static void tickDropOption()
    {
        if (focusedWindow != 3) return;

        if (isInventoryKeyDownRepeat(InputButtons::BUTTON_LEFT) && dropSelected > 0)
        {
            playSound(0, 2);
            dropSelected--;
        }
        if (isInventoryKeyDownRepeat(InputButtons::BUTTON_RIGHT) && dropSelected < 1)
        {
            playSound(0, 2);
            dropSelected++;
        }

        if (isInventoryKeyDown(InputButtons::BUTTON_TRIANGLE)) { removeAnimatedUIBox(3, nullptr); }
        else if (isInventoryKeyDown(InputButtons::BUTTON_CROSS))
        {
            removeAnimatedUIBox(3, nullptr);
            state         = 61;
            focusedWindow = -1;

            if (dropSelected == 0)
                removeItem(INVENTORY_ITEM_TYPES[INVENTORY_POINTER], INVENTORY_ITEM_AMOUNTS[INVENTORY_POINTER]);
        }
    }

    static void renderDropOption()
    {
        auto baseX = UI_BOX_DATA[3].finalPos.x;
        auto baseY = UI_BOX_DATA[3].finalPos.y;

        youSureString.render(baseX + 12, baseY + 8, 9, 3, 1);
        yesString.render(baseX + 23, baseY + 32, 9, 3, 1);
        noString.render(baseX + 75, baseY + 32, 9, 3, 1);

        renderSelectionCursor(baseX + 18 + dropSelected * 52, baseY + 30, 42, 16, 3);
    }

    void createDropOptions()
    {
        dropSelected  = 1;
        focusedWindow = 3;
        state         = 52;

        constexpr RECT final = {.x = -64, .y = -38, .width = 128, .height = 53};
        RECT start           = {
                      .x      = static_cast<int16_t>(UI_BOX_DATA[2].finalPos.x + 9),
                      .y      = static_cast<int16_t>(UI_BOX_DATA[2].finalPos.y + 60),
                      .width  = 40,
                      .height = 16,
        };

        createAnimatedUIBox(3, 1, 0, &final, &start, tickDropOption, renderDropOption);
    }

    void tickInventoryOptions()
    {
        if (focusedWindow != 2) return;

        if (isInventoryKeyDownRepeat(InputButtons::BUTTON_UP) && menuSelected > 0)
        {
            playSound(0, 2);
            menuSelected--;
        }
        if (isInventoryKeyDownRepeat(InputButtons::BUTTON_DOWN) && menuSelected < 3)
        {
            playSound(0, 2);
            menuSelected++;
        }

        if (isInventoryKeyDown(InputButtons::BUTTON_TRIANGLE))
            removeAnimatedUIBox(2, nullptr);
        else if (isInventoryKeyDown(InputButtons::BUTTON_CROSS))
        {
            if (!isMenuEnabled(menuSelected, INVENTORY_ITEM_TYPES[INVENTORY_POINTER]))
            {
                playSound(0, 11);
                return;
            }

            switch (menuSelected)
            {
                case 0:
                {
                    state = 201;
                    removeAnimatedUIBox(2, nullptr);
                    break;
                }
                case 1:
                {
                    moveSource = INVENTORY_POINTER;
                    removeAnimatedUIBox(2, nullptr);
                    break;
                }
                case 2:
                {
                    createSortOption();
                    break;
                }
                case 3:
                {
                    createDropOptions();
                    break;
                }
            }
        }
    }

    void createInventoryOptions()
    {
        auto slotId        = INVENTORY_POINTER - UI_BOX_DATA[0].rowOffset * 2;
        auto slotX         = getSlotPosX(INVENTORY_POINTER);
        auto slotY         = getSlotPosY(slotId);
        int16_t finalSlotX = slotX + 3;
        int16_t finalSlotY = slotId >= 10 ? slotY - MENU_HEIGHT : slotY + 17;

        RECT start = {.x = slotX, .y = slotY, .width = SLOT_WIDTH, .height = SLOT_HEIGHT};
        RECT final = {.x = finalSlotX, .y = finalSlotY, .width = MENU_WIDTH, .height = MENU_HEIGHT};

        menuSelected = 0;
        createAnimatedUIBox(2, 1, 0, &final, &start, tickInventoryOptions, renderItemOptions);
    }

    void tickInventoryTop()
    {
        if (focusedWindow != 0) return;

        auto& box = UI_BOX_DATA[0];

        previousSelection = INVENTORY_POINTER;
        if (isInventoryKeyDownRepeat(InputButtons::BUTTON_UP) && INVENTORY_POINTER > 1) INVENTORY_POINTER -= 2;
        if (isInventoryKeyDownRepeat(InputButtons::BUTTON_DOWN) && INVENTORY_POINTER < INVENTORY_SIZE - 2)
            INVENTORY_POINTER += 2;
        if (isInventoryKeyDown(InputButtons::BUTTON_LEFT) && (INVENTORY_POINTER % 2) != 0) INVENTORY_POINTER -= 1;
        if (isInventoryKeyDown(InputButtons::BUTTON_RIGHT) && (INVENTORY_POINTER % 2) == 0) INVENTORY_POINTER += 1;
        if ((POLLED_INPUT & InputButtons::BUTTON_R1) != 0)
        {
            if (isInventoryKeyDown(InputButtons::BUTTON_DOWN))
                INVENTORY_POINTER = (box.totalRows - 1) * 2 + (INVENTORY_POINTER & 1);
            if (isInventoryKeyDown(InputButtons::BUTTON_UP)) INVENTORY_POINTER = INVENTORY_POINTER & 1;
        }
        if (previousSelection != INVENTORY_POINTER) playSound(0, 2);
        if (box.rowOffset * 2 > INVENTORY_POINTER) box.rowOffset = INVENTORY_POINTER / 2;
        if ((box.rowOffset + box.visibleRows - 1) * 2 + 1 < INVENTORY_POINTER)
            box.rowOffset = INVENTORY_POINTER / 2 - box.visibleRows + 1;

        if (isInventoryKeyDown(InputButtons::BUTTON_SELECT))
        {
            moveSource = INVENTORY_POINTER == moveSource ? -1 : INVENTORY_POINTER;
            if (moveSource == -1)
                playSound(0, 4);
            else
                playSound(0, 3);
        }

        if (isInventoryKeyDown(InputButtons::BUTTON_CROSS))
        {
            if (moveSource != -1)
            {
                swapItem();
                moveSource = -1;
                playSound(0, 3);
            }
            else
            {
                createInventoryOptions();
                state         = 51;
                focusedWindow = 2;
            }
        }
        else if (isInventoryKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            if (moveSource != -1)
            {
                playSound(0, 4);
                moveSource = -1;
            }
            else
                state = 3;
        }
    }

    void createInventoryTop()
    {
        auto& box = UI_BOX_DATA[0];

        if (box.frame != 0) return;

        box.rowOffset   = 0;
        box.totalRows   = INVENTORY_SIZE / 2;
        box.visibleRows = min(box.totalRows, 9);
        auto features   = box.totalRows > box.visibleRows ? 6 : 2;

        auto& work     = TAMER_ENTITY.posData[1].posMatrix.work.t;
        auto entityPos = getScreenPosition(work[0], work[1], work[2]);
        RECT finalPos  = {
             .x      = TOP_X,
             .y      = TOP_Y,
             .width  = 304,
             .height = static_cast<int16_t>((box.visibleRows * 18) + 14),
        };
        RECT startPos = {
            .x      = static_cast<int16_t>(entityPos.screenX),
            .y      = static_cast<int16_t>(entityPos.screenY),
            .width  = 10,
            .height = 10,
        };

        createAnimatedUIBox(0, 0, features, &finalPos, &startPos, tickInventoryTop, renderInventoryTop);
    }

    void createInventoryBottom()
    {
        constexpr RECT finalPos = {.x = BOTTOM_X, .y = BOTTOM_Y, .width = 304, .height = 28};
        constexpr RECT startPos = {.x = TOP_X + 8, .y = TOP_Y + 14, .width = 16, .height = 16};

        createAnimatedUIBox(1, 0, 2, &finalPos, &startPos, nullptr, renderInventoryBottom);
    }

    void closeInventoryBottom()
    {
        auto x      = getSlotPosX(INVENTORY_POINTER);
        auto y      = getSlotPosY(INVENTORY_POINTER - UI_BOX_DATA[0].rowOffset * 2);
        RECT target = {.x = x, .y = y, .width = 16, .height = 16};

        removeAnimatedUIBox(1, &target);
    }

    void closeInventoryTop()
    {
        auto& work     = TAMER_ENTITY.posData[1].posMatrix.work.t;
        auto entityPos = getScreenPosition(work[0], work[1], work[2]);
        RECT target    = {
               .x      = static_cast<int16_t>(entityPos.screenX - 5),
               .y      = static_cast<int16_t>(entityPos.screenY - 5),
               .width  = 10,
               .height = 10,
        };

        removeAnimatedUIBox(0, &target);
    }

    void tickInventoryUI(int32_t instanceId)
    {
        if (POLLED_INPUT == POLLED_INPUT_PREVIOUS)
            repeatTimer++;
        else
            repeatTimer = 0;

        if (COMBAT_DATA_PTR->player.currentCommand[0] == BattleCommand::RUN) return;

        switch (state)
        {
            case 0:
            {
                moveSource        = -1;
                INVENTORY_POINTER = 0;
                previousSelection = -1;
                createInventoryTop();
                drawInventoryText();
                updateItemDescription();
                if (TAMER_ENTITY.animId != 4) startAnimation(&TAMER_ENTITY, 4);
                state = 1;
                break;
            }
            case 1:
            {
                if (UI_BOX_DATA[0].state == 1)
                {
                    createInventoryBottom();
                    state = 2;
                }
                break;
            }
            case 2:
            {
                break;
            }
            case 3:
            {
                moveSource    = -1;
                focusedWindow = -1;
                closeInventoryBottom();
                state = 4;
                break;
            }
            case 4:
            {
                if (UI_BOX_DATA[1].state != 0) return;
                closeInventoryTop();
                state = 5;
                break;
            }
            case 5:
            {
                if (UI_BOX_DATA[0].state != 0) return;

                closeInventoryBoxes();

                if (GAME_STATE == 0)
                {
                    closeTriangleMenu();
                    addGameMenu();
                    startAnimation(&TAMER_ENTITY, 0);
                }
                break;
            }

            // wait for options menu to close
            case 51:
            {
                if (UI_BOX_DATA[2].state != 0) return;

                focusedWindow = 0;
                state         = 2;
                break;
            }
            case 52:
            {
                if (UI_BOX_DATA[3].state != 0) return;

                state         = 51;
                focusedWindow = 2;
                break;
            }
            case 61:
            {
                if (UI_BOX_DATA[3].state != 0) return;

                removeAnimatedUIBox(2, nullptr);
                state = 51;
                break;
            }

            // use
            case 201:
            {
                if (UI_BOX_DATA[2].state != 0) return;
                closeInventoryBottom();
                state = 202;
                break;
            }
            case 202:
            {
                if (UI_BOX_DATA[1].state != 0) return;
                closeInventoryTop();
                state = 203;
                break;
            }
            case 203:
            {
                if (UI_BOX_DATA[0].state != 0) return;
                closeInventoryBoxes();

                if (GAME_STATE == 0)
                    startFeedingItem(INVENTORY_ITEM_TYPES[INVENTORY_POINTER]);
                else if (GAME_STATE == 1)
                    startThrowingItem();
                break;
            }
        }
    }

} // namespace

extern "C"
{
    void closeInventoryBoxes()
    {
        if (!isInventoryOpen) return;

        isInventoryOpen = false;
        state           = -1;
        removeStaticUIBox(0);
        removeStaticUIBox(1);
        removeStaticUIBox(2);
        removeStaticUIBox(3);

        removeObject(ObjectID::INVENTORY, 0);
    }

    void addInventoryUI()
    {
        if (isInventoryOpen) return;
        if (UI_BOX_DATA[0].state != 0) return;
        if (TAMER_ITEM.type != ItemType::NONE) return;

        focusedWindow   = 0;
        moveSource      = -1;
        state           = 0;
        isInventoryOpen = true;
        addObject(ObjectID::INVENTORY, 0, tickInventoryUI, nullptr);
    }

    void initializeInventoryUI()
    {
        state           = -1;
        isInventoryOpen = false;
    }
}