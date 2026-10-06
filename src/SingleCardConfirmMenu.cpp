

#include "Font.hpp"
#include "Input.hpp"
#include "ItemMenu.hpp"
#include "Script.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "constants.hpp"
#include "extern/dw1.hpp"

namespace
{
    struct CardShopLineData
    {
        int16_t uvX;
        int16_t uvY;
        int16_t posX;
        int16_t posY;
        int16_t width;
    };

    constexpr RECT final{.x = -56, .y = -21, .width = 112, .height = 42};
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

    void tickSingleCardShop(int32_t)
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

    void renderSingleCardShop(int32_t)
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
} // namespace

extern "C"
{
    int32_t createSingleCardConfirmBox(RECT* rect)
    {
        auto menu      = getItemMenuFromType();
        auto entry     = (menu->scrollOffset + menu->cursorOffset) * 2;
        SHOP_ITEM_TYPE = menu->itemList[entry];
        auto amount    = menu->itemList[entry + 1];

        if (ITEM_MENU_TYPE == 5) amount = amount & 0x80;

        if (SHOP_ITEM_TYPE == 0xFF || amount == 0) {
            playSound(0, 11);
            return 0;
        }

        ITEM_MENU_SUB_TEXTBOX_LINE = (TEXTBOX_DATA[0].activeBufferId ^ 1) * TEXTBOX_DATA[0].lineCount;

        rect->x += UI_BOX_DATA[1].finalPos.x;
        rect->y += UI_BOX_DATA[1].finalPos.y + menu->cursorOffset * 18;
        createTextbox(3, 0xC1, &final, rect, tickSingleCardShop, renderSingleCardShop);
        registerTextbox(3, ITEM_MENU_SUB_TEXTBOX_LINE, 2, 0, VRAMMode::FRONT_FULL);
        showMapheadTextbox(0, 0xFF, 3, 1240);
        SHOP_AMOUNT = 0;
        playSound(0, 3);
        return 1;
    }
}
