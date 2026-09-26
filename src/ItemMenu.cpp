#include "Font.hpp"
#include "Helper.hpp"
#include "Input.hpp"
#include "Inventory.hpp"
#include "InventoryUI.hpp"
#include "RecycleShop.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "extern/dtl/algorithm.hpp"
#include "extern/dw1.hpp"

namespace
{
    struct ShopLineData
    {
        int16_t posX;
        int16_t posY;
        uint16_t width;
    };

    struct CardShopLineData
    {
        int16_t uvX;
        int16_t uvY;
        int16_t posX;
        int16_t posY;
        int16_t width;
    };

    constexpr auto BUYABLE_TRIGGER_START = 0x180;

    constexpr dtl::array<ShopLineData, 5> AMOUNT_BOX_LINE_DATA{{
        {
            .posX  = 26,
            .posY  = 7,
            .width = 8,
        },
        {
            .posX  = 74,
            .posY  = 27,
            .width = 5,
        },
        {
            .posX  = 89,
            .posY  = 43,
            .width = 2,
        },
        {
            .posX  = 64,
            .posY  = 65,
            .width = 6,
        },
        {
            .posX  = 22,
            .posY  = 43,
            .width = 1,
        },
    }};

    constexpr dtl::array<CardShopLineData, 3> CARD_SHOP_LINE_DATA{{
        {
            .uvX   = 0,
            .uvY   = 0,
            .posX  = 4,
            .posY  = 2,
            .width = 96,
        },
        {
            .uvX   = 0,
            .uvY   = 12,
            .posX  = 14,
            .posY  = 18,
            .width = 36,
        },
        {
            .uvX   = 36,
            .uvY   = 12,
            .posX  = 68,
            .posY  = 18,
            .width = 36,
        },
    }};

    constexpr dtl::array<int32_t, 5> CARD_PRICES{5000, 1500, 500, 100, 50};

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

    int32_t getShopkeeperScriptID()
    {
        constexpr dtl::array<DigimonType, 12> SHOPKEEPER_TYPES{
            DigimonType::NPC_PATAMON,
            DigimonType::NPC_UNIMON,
            DigimonType::NPC_BIYOMON,
            DigimonType::NPC_MONOCHROMON,
            DigimonType::NPC_DEVIMON,
            DigimonType::NPC_MAMEMON,
            DigimonType::NPC_NUMEMON,
            DigimonType::NPC_MOJYAMON,
            DigimonType::MARKET_MANAGER,
            DigimonType::BETAMON,
            DigimonType::INVALID,
            DigimonType::TAMER,
        };

        const auto speaker = readPStat(0xFE);
        if (speaker == 0xFF) return 0x4CE;

        const auto entityId = scriptIdToEntityId(speaker);
        if (entityId == 0xFF) return 0x4C4;

        const auto type = ENTITY_TABLE.getEntityById(entityId)->type;
        for (auto i = 0; i < SHOPKEEPER_TYPES.size(); i++)
            if (type == SHOPKEEPER_TYPES[i]) return 0x4C4 + i;

        return 0x4C4;
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

    void tickItemMenuAmountBox()
    {
        if (UI_BOX_DATA[3].state != 1) return;
        if (!isXPressedAfterDialogue()) return;

        if (SHOP_AMOUNT != MAX_SHOP_AMOUNT && isKeyDown(InputButtons::BUTTON_UP)) {
            const auto amount = isKeyPressed(InputButtons::BUTTON_CIRCLE) ? 10 : 1;
            SHOP_AMOUNT       = dtl::min(static_cast<uint8_t>(SHOP_AMOUNT + amount), MAX_SHOP_AMOUNT);
            playSound(0, 2);
        }
        else if (SHOP_AMOUNT > 1 && isKeyDown(InputButtons::BUTTON_DOWN)) {
            const auto amount = isKeyPressed(InputButtons::BUTTON_CIRCLE) ? 10 : 1;
            SHOP_AMOUNT       = dtl::max(SHOP_AMOUNT - amount, 1);
            playSound(0, 2);
        }
        else if (isKeyDown(InputButtons::BUTTON_TRIANGLE)) {
            triggerBoxCloseFlag(3);
            playSound(0, 4);
        }
        else if (isKeyDown(InputButtons::BUTTON_SQUARE)) {
            SHOP_AMOUNT = MAX_SHOP_AMOUNT;
            playSound(0, 4);
        }
        else if (isKeyDown(InputButtons::BUTTON_CROSS)) {
            auto speaker = 0;

            if (ITEM_MENU_TYPE >= 3) {
                auto cardType = static_cast<int32_t>(SHOP_ITEM_TYPE);
                MONEY         = dtl::min(MONEY + SHOP_AMOUNT * SHOP_ITEM_PRICE, 999999U);
                setCardAmount(cardType, getCardAmount(cardType) - SHOP_AMOUNT);
                speaker        = readPStat(254);
                SCRIPT_STATE_2 = 4;
            }
            else if (ITEM_MENU_TYPE == 1) {
                MONEY = dtl::min(MONEY + SHOP_AMOUNT * SHOP_ITEM_PRICE, 999999U);
                removeItem(SHOP_ITEM_TYPE, SHOP_AMOUNT);
                speaker        = readPStat(254);
                SCRIPT_STATE_2 = 7;
            }
            else {
                auto price = SHOP_ITEM_PRICE;
                if (isPartnerBaby()) {
                    price         = (SHOP_ITEM_PRICE * 90) / 100;
                    SHOP_VARIABLE = SHOP_AMOUNT * (SHOP_ITEM_PRICE - price);
                }
                MONEY = dtl::max(MONEY - SHOP_AMOUNT * price, 0U);
                giveItem(SHOP_ITEM_TYPE, SHOP_AMOUNT);
                if (ITEM_MENU_TYPE == 2) {
                    auto id = getRecycleId(SHOP_ITEM_TYPE);
                    GAME_STATE_PTR->recycleItems[id] -= SHOP_AMOUNT;
                }
                speaker        = 0xfd;
                SCRIPT_STATE_2 = 7;
            }

            RECT box;
            setupBoxOrigin(speaker, &box);
            triggerBoxCloseFlag(3);
            closeTextbox(3, &box);
            UPDATE_SHOP_BIT_BOX = true;
            playShopSoundOnlyInSavannah();
        }

        updateItemMenuAmountBoxString();
    }

    void renderItemMenuAmountBox()
    {
        const auto posX       = UI_BOX_DATA[3].finalPos.x;
        const auto posY       = UI_BOX_DATA[3].finalPos.y;
        const auto lineOffset = TEXTBOX_DATA[3].lineOffset * 12;

        renderHorizontalLine(3, 4, 23, 122);
        renderHorizontalLine(3, 12, 60, 106);
        renderInsetWithoutBox(3, 85, 42, 26, 14);

        if (ITEM_MENU_TYPE < 3)
            renderItemSprite(SHOP_ITEM_TYPE, posX + 8, posY + 5, 3);
        else
            renderCardSprite(CARD_DATA[static_cast<int32_t>(SHOP_ITEM_TYPE)].rarity, posX + 10, posY + 7, 3);

        int32_t uvOffsetX = 0;
        for (int32_t i = 0; i < 4; i++) {
            const auto& data = AMOUNT_BOX_LINE_DATA[i];
            renderStringNew(0,
                            posX + data.posX,
                            posY + data.posY,
                            data.width * 12,
                            12,
                            uvOffsetX * 3 + 704,
                            lineOffset + 256,
                            3,
                            1);
            uvOffsetX += data.width;
        }
    }

    void updateItemMenuAmountBoxString()
    {
        auto* dst = TEXTBOX_LINES_PTR + ITEM_MENU_SUB_TEXTBOX_LINE * 0x40;
        uint8_t* name;

        if (ITEM_MENU_TYPE < 3) {
            name = getItem(SHOP_ITEM_TYPE)->name;
        }
        else {
            auto type = static_cast<DigimonType>(CARD_DATA[static_cast<uint8_t>(SHOP_ITEM_TYPE)].type);
            name      = getDigimonData(type)->name;
        }

        strcpy(dst, name);
        auto length = strlen(dst);

        auto end = dst + length;
        end[0]   = 0x18;
        end[1]   = 0;

        end    = intToStringSJIS(end + 2, SHOP_ITEM_PRICE, 5, 0);
        end[0] = 0x19;
        end[1] = 0;

        end    = intToStringSJIS(end + 2, SHOP_AMOUNT, 2, 0);
        end[0] = 0x1a;
        end[1] = 0;

        auto value = dtl::min(SHOP_ITEM_PRICE * SHOP_AMOUNT, 999999U);
        end        = intToStringSJIS(end + 2, value, 6, 0);
        end[0]     = 0;
        end[1]     = 0;

        TEXTBOX_DATA[3].pageReady = 1;
        TEXTBOX_DATA[3].writeCount++;
    }

    void tickSingleCardShop()
    {
        if (UI_BOX_DATA[3].state != 1) return;
        if (!isXPressedAfterDialogue()) return;

        if (isKeyDown(InputButtons::BUTTON_TRIANGLE)) {
            triggerBoxCloseFlag(3);
            playSound(0, 4);
        }
        else if (isKeyDown(InputButtons::BUTTON_LEFT)) {
            SHOP_AMOUNT = 0;
            playSound(0, 2);
        }
        else if (isKeyDown(InputButtons::BUTTON_RIGHT)) {
            SHOP_AMOUNT = 1;
            playSound(0, 2);
        }
        else if (isKeyDown(InputButtons::BUTTON_CROSS)) {
            triggerBoxCloseFlag(3);
            if (SHOP_AMOUNT != 0)
                playSound(0, 4);
            else {
                const auto cardType = static_cast<uint8_t>(SHOP_ITEM_TYPE);
                if (ITEM_MENU_TYPE == 5) {
                    writePStat(0xFE, cardType);
                    unsetTrigger(3);
                    SCRIPT_STATE_2 = 4;
                    playSound(0, 3);
                }
                else {
                    auto menu   = getItemMenuFromType();
                    auto amount = getCardAmount(cardType);
                    setCardAmount(cardType, amount + 1);
                    MONEY -= CARD_PRICES[CARD_DATA[cardType].rarity];
                    GAME_STATE_PTR->dailySingleCards[menu->cursorOffset] = 0xFF;
                    UPDATE_SHOP_BIT_BOX                                  = 1;
                    SCRIPT_STATE_2                                       = 4;
                    playShopSoundOnlyInSavannah();
                }
            }
        }
    }

    void renderSingleCardShop()
    {
        const auto posX       = UI_BOX_DATA[3].finalPos.x + 4;
        const auto posY       = UI_BOX_DATA[3].finalPos.y + 3;
        const auto lineOffset = TEXTBOX_DATA[3].lineOffset * 12;

        for (const auto& entry : CARD_SHOP_LINE_DATA) {
            renderStringNew(0,
                            posX + entry.posX,
                            posY + entry.posY,
                            entry.width,
                            12,
                            (entry.uvX / 4) + 704,
                            lineOffset + entry.uvY + 256,
                            3,
                            1);
        }
        renderSelectionCursor(posX + 8 + SHOP_AMOUNT * 47, posY + 18, 40, 14, 3);
    }

    void tickShopBitsBox()
    {
        if (!UPDATE_SHOP_BIT_BOX) return;

        UPDATE_SHOP_BIT_BOX       = 0;
        const auto tmpInstruction = ACTIVE_INSTRUCTION;
        const auto line           = BIT_BOX_SHOW_BITS ? 6 : 7;

        showShopkeeperTextbox(line, 255, 2);

        ACTIVE_INSTRUCTION = tmpInstruction;
    }

    void renderShopBitsBox()
    {
        auto posX = UI_BOX_DATA[2].finalPos.x + 22;
        auto posY = UI_BOX_DATA[2].finalPos.y;
        auto uvY  = TEXTBOX_DATA[2].lineOffset * 12;

        renderStringNew(0, posX, posY + 4, 48, 12, 704, uvY + 256, 4, 1);
        renderStringNew(0, posX, posY + 17, 60, 12, 704 + 48 / 4, uvY + 256, 4, 1);
    }

    uint8_t* resolveMapHeadEntry(uint32_t sectionId, int32_t line)
    {
        auto script = getScript(0);
        auto offset = getScriptSection(script, sectionId);
        return script + *reinterpret_cast<uint16_t*>(offset + line * 4 + 2) + 2;
    }

    uint8_t* getShopkeeperLine(int32_t line)
    {
        auto scriptId = getShopkeeperScriptID();
        return resolveMapHeadEntry(scriptId, line);
    }
}
