
#include "Font.hpp"
#include "Helper.hpp"
#include "Input.hpp"
#include "Inventory.hpp"
#include "InventoryUI.hpp"
#include "ItemMenu.hpp"
#include "RecycleShop.hpp"
#include "Script.hpp"
#include "Sound.hpp"
#include "constants.hpp"
#include "extern/dw1.hpp"

// TODO: use AtlasStrings
namespace
{
    struct ShopLineData
    {
        int16_t posX;
        int16_t posY;
        uint16_t width;
    };

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

    constexpr RECT final{.x = -65, .y = -42, .width = 130, .height = 83};

    void updateItemMenuAmountBoxString()
    {
        auto* dst = TEXTBOX_LINES_PTR + ITEM_MENU_SUB_TEXTBOX_LINE * 0x40;
        uint8_t* name;

        if (ITEM_MENU_TYPE < 3) {
            name = getItem(static_cast<ItemType>(SHOP_ITEM_TYPE))->name;
        }
        else {
            auto type = static_cast<DigimonType>(CARD_DATA[SHOP_ITEM_TYPE].type);
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

        auto value = dtl::min(SHOP_ITEM_PRICE * SHOP_AMOUNT, MAX_MONEY);
        end        = intToStringSJIS(end + 2, value, 6, 0);
        end[0]     = 0;
        end[1]     = 0;

        TEXTBOX_DATA[3].pageReady = 1;
        TEXTBOX_DATA[3].writeCount++;
    }

    void tickItemMenuAmountBox(int32_t)
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
                MONEY         = dtl::min(MONEY + SHOP_AMOUNT * SHOP_ITEM_PRICE, MAX_MONEY);
                setCardAmount(cardType, getCardAmount(cardType) - SHOP_AMOUNT);
                speaker        = readPStat(254);
                SCRIPT_STATE_2 = 4;
            }
            else if (ITEM_MENU_TYPE == 1) {
                MONEY = dtl::min(MONEY + SHOP_AMOUNT * SHOP_ITEM_PRICE, MAX_MONEY);
                removeItem(static_cast<ItemType>(SHOP_ITEM_TYPE), SHOP_AMOUNT);
                speaker        = readPStat(254);
                SCRIPT_STATE_2 = 7;
            }
            else {
                auto price = SHOP_ITEM_PRICE;
                if (isPartnerBaby()) {
                    price         = (SHOP_ITEM_PRICE * 90) / 100;
                    SHOP_VARIABLE = SHOP_AMOUNT * (SHOP_ITEM_PRICE - price);
                }
                MONEY = dtl::max(MONEY - SHOP_AMOUNT * price, 0);
                giveItem(static_cast<ItemType>(SHOP_ITEM_TYPE), SHOP_AMOUNT);
                if (ITEM_MENU_TYPE == 2) {
                    auto id = getRecycleId(static_cast<ItemType>(SHOP_ITEM_TYPE));
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

    void renderItemMenuAmountBox(int32_t)
    {
        const auto posX       = UI_BOX_DATA[3].finalPos.x;
        const auto posY       = UI_BOX_DATA[3].finalPos.y;
        const auto lineOffset = TEXTBOX_DATA[3].lineOffset * 12;

        renderHorizontalLine(3, 4, 23, 122);
        renderHorizontalLine(3, 12, 60, 106);
        renderInsetWithoutBox(3, 85, 42, 26, 14);

        if (ITEM_MENU_TYPE < 3)
            renderItemSprite(static_cast<ItemType>(SHOP_ITEM_TYPE), posX + 8, posY + 5, 3);
        else
            renderCardSprite(CARD_DATA[SHOP_ITEM_TYPE].rarity, posX + 10, posY + 7, 3);

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

} // namespace

extern "C"
{
    void createItemMenuAmountBox(RECT* rect)
    {
        auto menu      = getItemMenuFromType();
        auto entry     = (menu->scrollOffset + menu->cursorOffset) * 2;
        SHOP_ITEM_TYPE = menu->itemList[entry];
        auto amount    = menu->itemList[entry + 1];

        if (SHOP_ITEM_TYPE == 0xFF) {
            playSound(0, 11);
            return;
        }

        if (ITEM_MENU_TYPE >= 3) {
            SHOP_ITEM_PRICE = CARD_PRICES[CARD_DATA[SHOP_ITEM_TYPE].rarity] / 2;
            MAX_SHOP_AMOUNT = amount;
        }
        else if (ITEM_MENU_TYPE == 1) {
            if ((amount & 0x80) == 0) {
                playSound(0, 11);
                return;
            }
            SHOP_ITEM_PRICE = getItem(static_cast<ItemType>(SHOP_ITEM_TYPE))->value / 2;
            MAX_SHOP_AMOUNT = amount & 0x7F;
        }
        else {
            if (ITEM_MENU_TYPE == 0) {
                if (amount == 0) {
                    playSound(0, 11);
                    return;
                }
                MAX_SHOP_AMOUNT = 99;
            }
            else {
                if ((amount & 0x80) == 0) {
                    playSound(0, 11);
                    return;
                }
                MAX_SHOP_AMOUNT = amount & 0x7F;
            }

            SHOP_ITEM_PRICE = getItem(static_cast<ItemType>(SHOP_ITEM_TYPE))->value;
            auto count      = getItemCount(static_cast<ItemType>(SHOP_ITEM_TYPE));
            MAX_SHOP_AMOUNT = dtl::min(99U - count, static_cast<uint32_t>(MAX_SHOP_AMOUNT));
            MAX_SHOP_AMOUNT = dtl::clamp(MONEY / SHOP_ITEM_PRICE, 0, static_cast<int32_t>(MAX_SHOP_AMOUNT));
        }

        SHOP_AMOUNT = 1;
        // vanilla sets NAMING_CURRENT_LETTER, but never uses it?
        ITEM_MENU_SUB_TEXTBOX_LINE = (TEXTBOX_DATA[0].activeBufferId ^ 1) * TEXTBOX_DATA[0].lineCount;

        rect->x += UI_BOX_DATA[1].finalPos.x;
        rect->y += UI_BOX_DATA[1].finalPos.y + menu->cursorOffset * 18;
        createTextbox(3, 0xC1, &final, rect, tickItemMenuAmountBox, renderItemMenuAmountBox);
        registerTextbox(3, ITEM_MENU_SUB_TEXTBOX_LINE, 1, 0, VRAMMode::FRONT_FULL);
        updateItemMenuAmountBoxString();
        playSound(0, 3);
    }
}
