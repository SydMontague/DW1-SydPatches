#include "InventoryUI.hpp"

#include "Font.hpp"
#include "GameMenu.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Inventory.hpp"
#include "InventoryUIHelpers.hpp"
#include "ItemFunctions.hpp"
#include "ItemInfo.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "Sound.hpp"
#include "Tamer.hpp"
#include "ThrownItem.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/stddef.hpp"

namespace
{
    constexpr int32_t BOX_LIST    = 0;
    constexpr int32_t BOX_INFO    = 2;
    constexpr int32_t BOX_OPTIONS = 4;

    constexpr int16_t TAB_X     = -152;
    constexpr int16_t TAB_WIDTH = 148;
    // Active tab is 1 px shorter than inactive (11 vs 12) so the panel
    // border's cyan row shows through below it as the selected-tab underline.
    constexpr int16_t TAB_HEIGHT = 11;

    constexpr int16_t LIST_X     = -152;
    constexpr int16_t LIST_WIDTH = 304;

    constexpr int16_t CAP_X      = -152;
    constexpr int16_t CAP_Y      = 94;
    constexpr int16_t CAP_WIDTH  = 304;
    constexpr int16_t CAP_HEIGHT = 24;

    constexpr int16_t TAB_Y  = -114;
    constexpr int16_t LIST_Y = -104;

    constexpr int16_t INFO_X      = -120;
    constexpr int16_t INFO_Y      = -60;
    constexpr int16_t INFO_WIDTH  = 240;
    constexpr int16_t INFO_HEIGHT = 120;

    constexpr int16_t SLOT_WIDTH        = 142;
    constexpr int16_t SLOT_HEIGHT       = 18;
    constexpr int16_t SLOT_OFFSET_X     = 5;
    constexpr int16_t SLOT_OFFSET_Y     = 7;
    constexpr int16_t SLOT_GAP_X        = 4; // horizontal gap between the two columns
    constexpr int16_t SLOT_SPRITE_OFF_X = 1;
    constexpr int16_t SLOT_SPRITE_OFF_Y = 1;
    constexpr int16_t SLOT_NAME_OFF_X   = 22;
    constexpr int16_t SLOT_NAME_OFF_Y   = 3;
    // Right-edge X of the amount text within the slot; render position adjusted
    // by the actual sprite width so amounts right-align ("x4" and "x99" share an edge).
    constexpr int16_t SLOT_AMOUNT_OFF_X = 138;
    constexpr int16_t SLOT_AMOUNT_OFF_Y = 3;

    constexpr int16_t MENU_WIDTH  = 58;
    constexpr int16_t MENU_HEIGHT = 48;

    // drawLine2P/3P encode color as 0xBBGGRR.
    constexpr uint32_t CURSOR_BRACKET_COLOR = 0xffef84;
    constexpr uint32_t SCROLL_ARROW_COLOR   = 0x60d0ff;
    constexpr uint32_t BAR_OUTLINE_COLOR    = 0xffef84;
    constexpr uint32_t SLASH_COLOR          = 0xffef84;
    constexpr uint32_t BADGE_RED_COLOR      = 0x2020ff;

    constexpr int32_t COLOR_CYAN  = 2;
    constexpr int32_t COLOR_LIGHT = 0;

    constexpr dtl::array<int8_t, 128> ITEM_CLUT_DATA{
        0,  1,  18, 3,  4,  8,  6,  0,  8,  6, 7,  4,  8,  9,  9,  10, 4,  11, 8,  12, 8,  11, 13, 10, 4,  14,
        6,  0,  4,  15, 14, 8,  8,  11, 14, 8, 16, 21, 17, 17, 17, 18, 19, 11, 9,  14, 21, 2,  3,  21, 16, 18,
        20, 11, 11, 1,  12, 4,  11, 7,  0,  2, 14, 20, 13, 13, 13, 13, 15, 8,  11, 5,  12, 1,  2,  4,  4,  5,
        4,  5,  16, 4,  5,  16, 22, 21, 12, 4, 12, 23, 23, 5,  8,  12, 5,  3,  14, 17, 4,  10, 19, 10, 4,  4,
        4,  10, 23, 5,  4,  16, 5,  10, 4,  7, 8,  14, 20, 9,  4,  16, 4,  23, 21, 4,  9,  4,  4,  4,
    };

    constexpr const char* CATEGORY_LABELS[] = {
        "Heal",
        "Status",
        "Food",
        "Boost",
        "Digivolve",
        "Misc",
        "All",
    };

    constexpr int16_t getSlotPosX(int32_t col)
    { return LIST_X + SLOT_OFFSET_X + col * (SLOT_WIDTH + SLOT_GAP_X); }

    inline int16_t getSlotPosY(int32_t visibleRowIdx)
    { return LIST_Y + SLOT_OFFSET_Y + SLOT_HEIGHT * visibleRowIdx; }

    void applyListSize()
    {
        auto& box       = UI_BOX_DATA[BOX_LIST];
        const auto rows = clamp((inv_filteredCount + 1) / 2, 1, 10);
        box.visibleRows = static_cast<uint16_t>(rows);

        box.finalPos.y      = LIST_Y;
        box.finalPos.height = static_cast<int16_t>(rows * SLOT_HEIGHT + 14);
    }

    RECT getCursorSlotRect();

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

    consteval dtl::array<SimpleTextSprite, 7> initializeTabArray()
    {
        dtl::array<SimpleTextSprite, 7> data;
        for (int32_t i = 0; i < data.size(); i++)
            data[i] = SimpleTextSprite(640, 256 + 156 + i * 12);
        return data;
    }

    uint8_t getUseColor(Item* item)
    {
        if (item->itemColor == 2) return 9;
        if (GAME_STATE == 0 && item->itemColor == 0) return 9;
        if ((GAME_STATE == 1 || GAME_STATE == 2 || GAME_STATE == 3) && item->itemColor == 1) return 9;
        return 10;
    }

    bool isInventoryOpen = false;
    uint32_t state;
    uint8_t previousSelection;
    uint8_t menuSelected;
    uint8_t focusedWindow;
    uint8_t repeatTimer;
    bool descNeedsUpdate;

    int16_t activeTabX;
    int16_t activeTabW;

    dtl::array<SimpleTextSprite, 30> itemNames   = initializeNameArray();
    dtl::array<SimpleTextSprite, 30> itemAmounts = initializeAmountArray();
    dtl::array<SimpleTextSprite, 7> tabLabels    = initializeTabArray();

    SimpleTextSprite useString(704 + 0, 256 + 180);
    SimpleTextSprite dropString(704 + 48, 256 + 180);
    SimpleTextSprite moveString(704 + 96, 256 + 180);

    int8_t inv_moveSourceSlot = -1;

    SimpleTextSprite labelItem(640, 256 + 0);
    SimpleTextSprite labelDesc(640, 256 + 8);
    SimpleTextSprite labelCapacity(640, 256 + 32);
    SimpleTextSprite labelFull(640, 256 + 40);

    SimpleTextSprite effectString0(640, 256 + 68);
    SimpleTextSprite effectString1(640, 256 + 80);

    SimpleTextSprite capacityUsedStr(640, 256 + 92);
    SimpleTextSprite capacityMaxStr(640 + 32, 256 + 92);

    SimpleTextSprite descLine0(640, 256 + 108);
    SimpleTextSprite descLine1(640, 256 + 120);
    SimpleTextSprite descLine2(640, 256 + 132);
    SimpleTextSprite descLine3(640, 256 + 144);

    bool isInventoryKeyDown(InputButtons button)
    { return (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & button) != 0; }

    bool isInventoryKeyDownRepeat(InputButtons button)
    {
        if (!isInventoryKeyDown(button) && (repeatTimer < 7 || (POLLED_INPUT & button) == 0)) return false;
        return true;
    }

    bool isMenuEnabled(int32_t menuId, ItemType type)
    {
        if (type == ItemType::NONE) return false;
        auto* item = getItem(type);
        if (menuId == 0 && getUseColor(item) == 9) return true;
        if (menuId == 1 && item->dropable) return true;
        if (menuId == 2 && inv_currentCategory == 6) return true;
        return false;
    }

    int32_t countUsedSlots()
    {
        int32_t used = 0;
        for (int32_t i = 0; i < INVENTORY_SIZE; i++)
            if (INVENTORY_ITEM_TYPES[i] != ItemType::NONE) used++;
        return used;
    }

    void drawWrappedDescription(const char* src, int16_t maxWidth)
    {
        SimpleTextSprite* lines[4] = {&descLine0, &descLine1, &descLine2, &descLine3};
        for (auto* l : lines) l->clear();

        if (src == nullptr) return;

        int32_t srcPos  = 0;
        int32_t lineIdx = 0;
        char buf[64];

        while (src[srcPos] != '\0' && lineIdx < 4)
        {
            int32_t curLen      = 0;
            int16_t curWidth    = 0;
            int32_t lastSpaceAt = -1;

            while (true)
            {
                char c = src[srcPos + curLen];
                if (c == '\0' || c == '\n') break;
                uint8_t cw = myFont7px.getWidth(static_cast<uint16_t>(c & 0xFF));
                if (curWidth + cw > maxWidth) break;
                if (c == ' ') lastSpaceAt = curLen;
                curWidth += cw;
                curLen++;
                if (curLen >= 60) break;
            }

            char endChar = src[srcPos + curLen];
            int32_t takeLen = (endChar == '\0' || endChar == '\n' || lastSpaceAt <= 0) ? curLen : lastSpaceAt;
            if (takeLen <= 0) break;

            for (int32_t j = 0; j < takeLen; j++)
                buf[j] = src[srcPos + j];
            buf[takeLen] = '\0';
            lines[lineIdx]->draw(&myFont7px, reinterpret_cast<const uint8_t*>(buf));
            lineIdx++;

            srcPos += takeLen;
            while (src[srcPos] == ' ' || src[srcPos] == '\n') srcPos++;
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
            sprintf(amount, "x%d", INVENTORY_ITEM_AMOUNTS[i]);
            itemNames[i].draw(&vanillaFont, getItem(type)->name);
            itemAmounts[i].draw(&vanillaFont, amount);
            INVENTORY_ITEM_NAMES[i] = i;
        }

        useString.draw(&vanillaFont, "Use");
        dropString.draw(&vanillaFont, "Drop");
        moveString.draw(&vanillaFont, "Move");

        labelItem.draw(&myFont7px, "ITEM");
        labelDesc.draw(&myFont7px, "ITEM DESCRIPTION");
        labelCapacity.draw(&myFont7px, "CAPACITY");
        labelFull.draw(&myFont7px, "FULL");

        for (int32_t i = 0; i < 7; i++)
            tabLabels[i].draw(&myFont7px, CATEGORY_LABELS[i]);
    }

    const char* getLevelName(Level level)
    {
        switch (level)
        {
            case Level::FRESH: return "Fresh";
            case Level::IN_TRAINING: return "In-training";
            case Level::ROOKIE: return "Rookie";
            case Level::CHAMPION: return "Champion";
            case Level::ULTIMATE: return "Ultimate";
            default: return "?";
        }
    }

    void updateItemDescription()
    {
        const auto type = INVENTORY_ITEM_TYPES[INVENTORY_POINTER];

        if (type == ItemType::NONE)
        {
            drawWrappedDescription("", 224);
            effectString0.clear();
            effectString1.clear();
            return;
        }

        const auto& info = getItemInfo(type);

        // Digivolution items compose detail + desc at runtime from DIGIMON_DATA
        // so we don't bake 45+ unique strings into rodata.
        const char* detail = info.detail;
        const char* desc   = info.desc;
        uint8_t evoBuf[40];
        uint8_t descBuf[96];
        if (info.detail[0] == '\0')
        {
            const auto target = getEvoItemTarget(type);
            if (target != DigimonType::INVALID)
            {
                const auto& tdata    = DIGIMON_DATA[static_cast<int32_t>(target)];
                const auto* tname    = reinterpret_cast<const char*>(tdata.name);
                const auto* toLevel  = getLevelName(tdata.level);
                const auto fromIdx   = static_cast<int32_t>(tdata.level) - 1;
                const auto fromLevel = getLevelName(static_cast<Level>(fromIdx));

                sprintf(evoBuf, "Use to digivolve to %s", tname);
                detail = reinterpret_cast<const char*>(evoBuf);

                sprintf(descBuf, "Evolves a %s partner into %s (%s).", fromLevel, tname, toLevel);
                desc = reinterpret_cast<const char*>(descBuf);
            }
        }

        drawWrappedDescription(desc, INFO_WIDTH - 16);

        effectString0.clear();
        effectString0.draw(&vanillaFont, info.headline);
        effectString1.clear();
        effectString1.draw(&vanillaFont, detail);
    }

    void updateCapacity()
    {
        capacityUsedStr.clear();
        capacityMaxStr.clear();
        uint8_t bufA[8];
        uint8_t bufB[8];
        sprintf(bufA, "%d", countUsedSlots());
        sprintf(bufB, "%d", INVENTORY_SIZE);
        capacityUsedStr.draw(&myFont7px, bufA);
        capacityMaxStr.draw(&myFont7px, bufB);
    }

    void renderSectionBorder(int16_t panelX,
                             int16_t panelY,
                             int16_t panelW,
                             int16_t panelH,
                             SimpleTextSprite& label,
                             int32_t depth)
    {
        const int16_t labelW = static_cast<int16_t>(label.getWidth());
        const int16_t notchX = panelX + 6;
        const int16_t notchW = labelW + 4;

        RECT rect{.x = panelX, .y = panelY, .width = panelW, .height = panelH};
        renderUIBoxBorder(&rect, depth, notchX, notchW);

        label.render(notchX + 2, panelY - 4, COLOR_CYAN, depth, true);
    }

    void renderInventoryCursor(int16_t x, int16_t y, int16_t w, int16_t h, int32_t depth)
    {
        constexpr int16_t leg = 5;
        const int16_t x1      = x;
        const int16_t y1      = y;
        const int16_t x2      = x + w - 1;
        const int16_t y2      = y + h - 1;

        drawLine2P(CURSOR_BRACKET_COLOR, x1, y1, x1 + leg, y1, depth, 0);
        drawLine2P(CURSOR_BRACKET_COLOR, x1, y1, x1, y1 + leg, depth, 0);
        drawLine2P(CURSOR_BRACKET_COLOR, x2 - leg, y1, x2, y1, depth, 0);
        drawLine2P(CURSOR_BRACKET_COLOR, x2, y1, x2, y1 + leg, depth, 0);
        drawLine2P(CURSOR_BRACKET_COLOR, x1, y2 - leg, x1, y2, depth, 0);
        drawLine2P(CURSOR_BRACKET_COLOR, x1, y2, x1 + leg, y2, depth, 0);
        drawLine2P(CURSOR_BRACKET_COLOR, x2, y2 - leg, x2, y2, depth, 0);
        drawLine2P(CURSOR_BRACKET_COLOR, x2 - leg, y2, x2, y2, depth, 0);

        GsBOXF box{
            .attribute = 0x40000000,
            .x         = static_cast<int16_t>(x + 1),
            .y         = static_cast<int16_t>(y + 1),
            .width     = static_cast<uint16_t>(w - 2),
            .height    = static_cast<uint16_t>(h - 2),
            .r         = 0x32,
            .g         = 0x6e,
            .b         = 0xa0,
        };
        libgs_GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, depth);
    }

    void renderScrollArrow(int16_t cx, int16_t y, bool down, int32_t depth)
    {
        if (down)
        {
            drawLine2P(SCROLL_ARROW_COLOR, cx - 3, y + 0, cx + 3, y + 0, depth, 0);
            drawLine2P(SCROLL_ARROW_COLOR, cx - 2, y + 1, cx + 2, y + 1, depth, 0);
            drawLine2P(SCROLL_ARROW_COLOR, cx - 1, y + 2, cx + 1, y + 2, depth, 0);
            drawLine2P(SCROLL_ARROW_COLOR, cx + 0, y + 3, cx + 0, y + 3, depth, 0);
        }
        else
        {
            drawLine2P(SCROLL_ARROW_COLOR, cx + 0, y + 0, cx + 0, y + 0, depth, 0);
            drawLine2P(SCROLL_ARROW_COLOR, cx - 1, y + 1, cx + 1, y + 1, depth, 0);
            drawLine2P(SCROLL_ARROW_COLOR, cx - 2, y + 2, cx + 2, y + 2, depth, 0);
            drawLine2P(SCROLL_ARROW_COLOR, cx - 3, y + 3, cx + 3, y + 3, depth, 0);
        }
    }

    void renderInventoryTop(int32_t /*instanceId*/)
    {
        constexpr int32_t depth      = 6;
        constexpr int32_t labelDepth = 5;
        auto& box                    = UI_BOX_DATA[BOX_LIST];

        const int32_t cursorPos = getVisibleRow(INVENTORY_POINTER);
        const int32_t winLo     = box.rowOffset;
        const int32_t winHi     = box.rowOffset + box.visibleRows;
        const int32_t firstPos  = winLo * 2;
        const int32_t lastPos   = winHi * 2;
        for (int32_t p = firstPos; p < lastPos && p < inv_filteredCount; p++)
        {
            const int32_t i   = inv_filteredIdx[p];
            const auto type   = INVENTORY_ITEM_TYPES[i];
            const int32_t row = p / 2 - winLo;
            const int32_t col = p & 1;
            const auto posX   = getSlotPosX(col);
            const auto posY   = getSlotPosY(row);
            const auto nameId = INVENTORY_ITEM_NAMES[i];

            renderItemSprite(type, posX + SLOT_SPRITE_OFF_X, posY + SLOT_SPRITE_OFF_Y, depth);
            itemNames[nameId].render(posX + SLOT_NAME_OFF_X, posY + SLOT_NAME_OFF_Y, COLOR_LIGHT, depth, true);
            const int16_t amountW = static_cast<int16_t>(itemAmounts[nameId].getWidth());
            itemAmounts[nameId].render(posX + SLOT_AMOUNT_OFF_X - amountW,
                                       posY + SLOT_AMOUNT_OFF_Y,
                                       p == cursorPos ? COLOR_CYAN : COLOR_LIGHT,
                                       depth,
                                       true);
        }

        if (cursorPos >= firstPos && cursorPos < lastPos)
        {
            const auto cursorX = getSlotPosX(cursorPos & 1);
            const auto cursorY = getSlotPosY(cursorPos / 2 - winLo);
            renderInventoryCursor(cursorX, cursorY, SLOT_WIDTH, SLOT_HEIGHT, depth);
        }

        if (inv_moveSourceSlot >= 0)
        {
            const int32_t srcPos = getVisibleRow(inv_moveSourceSlot);
            if (srcPos >= firstPos && srcPos < lastPos)
            {
                const auto srcX = getSlotPosX(srcPos & 1);
                const auto srcY = getSlotPosY(srcPos / 2 - winLo);
                renderInventoryCursor(srcX, srcY, SLOT_WIDTH, SLOT_HEIGHT, depth);
            }
        }

        const int16_t arrowCx     = LIST_X + LIST_WIDTH / 2;
        const int16_t listBottomY = box.finalPos.y + box.finalPos.height - 1;
        static uint32_t scrollBopPhase = 0;
        scrollBopPhase++;
        const int16_t bop = static_cast<int16_t>((scrollBopPhase >> 4) & 1);
        if (box.rowOffset > 0) renderScrollArrow(arrowCx, LIST_Y + 3 - bop, false, depth);
        if (box.rowOffset + box.visibleRows < box.totalRows)
            renderScrollArrow(arrowCx, listBottomY - 5 + bop, true, depth);

        if (box.totalRows > box.visibleRows)
        {
            const int16_t trackX = LIST_X + LIST_WIDTH - 7;
            const int16_t trackY = static_cast<int16_t>(LIST_Y + 8);
            const int16_t trackH = static_cast<int16_t>(listBottomY - 7 - trackY);
            const int16_t thumbH = static_cast<int16_t>((trackH * box.visibleRows) / box.totalRows);
            const int16_t thumbY = static_cast<int16_t>(
                trackY + ((trackH - thumbH) * box.rowOffset) / (box.totalRows - box.visibleRows));
            drawLine2P(0xffc269, trackX, thumbY, trackX, thumbY + thumbH, depth, 0);
        }

        renderCategoryTabs(&tabLabels[0], TAB_Y, labelDepth - 1, &activeTabX, &activeTabW);
        RECT listRect{.x = LIST_X, .y = LIST_Y, .width = box.finalPos.width, .height = box.finalPos.height};
        renderUIBoxBorder(&listRect, labelDepth, 0, 0);

        // Two semi-trans fills = 75/25 blend matching the description-window preset.
        renderBox(CAP_X + 4, CAP_Y + 3, CAP_WIDTH - 8, CAP_HEIGHT - 3, 12, 22, 30, 1, depth);
        renderBox(CAP_X + 4, CAP_Y + 3, CAP_WIDTH - 8, CAP_HEIGHT - 3, 12, 22, 30, 1, depth);
        renderSectionBorder(CAP_X, CAP_Y, CAP_WIDTH, CAP_HEIGHT, labelCapacity, labelDepth);

        const int32_t used = countUsedSlots();
        const int32_t cap  = INVENTORY_SIZE > 0 ? INVENTORY_SIZE : 1;

        const int16_t numX  = CAP_X + 8;
        const int16_t numY  = CAP_Y + 4;
        const int16_t usedW = static_cast<int16_t>(capacityUsedStr.getWidth());
        capacityUsedStr.render(numX, numY, COLOR_CYAN, labelDepth, true);
        const int16_t slashX = numX + usedW + 2;
        drawLine2P(SLASH_COLOR, slashX + 3, numY + 1, slashX, numY + 6, labelDepth, 0);
        capacityMaxStr.render(slashX + 6, numY, COLOR_CYAN, labelDepth, true);

        const int16_t barX = CAP_X + 8;
        const int16_t barY = CAP_Y + 13;
        const int16_t barW = CAP_WIDTH - 16;
        const int16_t barH = 6;

        const int16_t fillW = static_cast<int16_t>((static_cast<int32_t>(barW) * used) / cap);
        if (fillW > 0) renderBox(barX, barY, fillW, barH, 0x69, 0xc2, 0xff, 0, labelDepth);
        drawLine2P(BAR_OUTLINE_COLOR, barX, barY, barX + barW, barY, labelDepth, 0);
        drawLine2P(BAR_OUTLINE_COLOR, barX, barY + barH, barX + barW, barY + barH, labelDepth, 0);
        drawLine2P(BAR_OUTLINE_COLOR, barX, barY, barX, barY + barH, labelDepth, 0);
        drawLine2P(BAR_OUTLINE_COLOR, barX + barW, barY, barX + barW, barY + barH, labelDepth, 0);

        if (used >= INVENTORY_SIZE)
        {
            const int16_t badgeW = 32;
            const int16_t badgeH = 10;
            const int16_t badgeX = CAP_X + CAP_WIDTH - badgeW - 6;
            const int16_t badgeY = CAP_Y + 4;
            renderBorderBox(badgeX,
                            badgeY,
                            badgeW - 1,
                            badgeH - 1,
                            BADGE_RED_COLOR,
                            BADGE_RED_COLOR,
                            0x40,
                            0x00,
                            0x00,
                            labelDepth + 1);
            labelFull.render(badgeX + 4, badgeY + 1, 6, labelDepth, true);
        }
    }

    void renderInventoryInfo(int32_t /*instanceId*/)
    {
        constexpr int32_t depth      = 2;
        constexpr int32_t labelDepth = 1;
        auto& box                    = UI_BOX_DATA[BOX_INFO];

        renderSectionBorder(box.finalPos.x,
                            box.finalPos.y,
                            box.finalPos.width,
                            box.finalPos.height,
                            labelDesc,
                            labelDepth);

        if (INVENTORY_ITEM_TYPES[INVENTORY_POINTER] == ItemType::NONE) return;

        if (descNeedsUpdate || INVENTORY_POINTER != previousSelection)
        {
            updateItemDescription();
            previousSelection = INVENTORY_POINTER;
            descNeedsUpdate   = false;
        }

        const int16_t bx = box.finalPos.x;
        const int16_t by = box.finalPos.y;
        const int16_t bw = box.finalPos.width;

        renderItemSprite(INVENTORY_ITEM_TYPES[INVENTORY_POINTER], bx + 10, by + 10, depth);
        const auto nameId = INVENTORY_ITEM_NAMES[INVENTORY_POINTER];
        itemNames[nameId].render(bx + 32, by + 14, COLOR_CYAN, depth, true);

        const int16_t hdrSep = by + 30;
        drawLine2P(BAR_OUTLINE_COLOR, bx + 6, hdrSep, bx + bw - 7, hdrSep, labelDepth, 0);

        const int16_t descX = bx + 8;
        const int16_t descY = by + 36;
        descLine0.render(descX, descY + 0, COLOR_LIGHT, depth, true);
        descLine1.render(descX, descY + 9, COLOR_LIGHT, depth, true);
        descLine2.render(descX, descY + 18, COLOR_LIGHT, depth, true);
        descLine3.render(descX, descY + 27, COLOR_LIGHT, depth, true);

        const int16_t effSep = by + 80;
        drawLine2P(BAR_OUTLINE_COLOR, bx + 6, effSep, bx + bw - 7, effSep, labelDepth, 0);

        effectString0.render(descX, by + 86, COLOR_CYAN, depth, true);
        effectString1.render(descX, by + 98, COLOR_LIGHT, depth, true);
    }

    void renderItemOptions(int32_t /*instanceId*/)
    {
        auto& box = UI_BOX_DATA[BOX_OPTIONS];
        auto type = INVENTORY_ITEM_TYPES[INVENTORY_POINTER];

        auto useColor  = 10;
        auto dropColor = 10;
        if (type != ItemType::NONE)
        {
            auto* item = getItem(INVENTORY_ITEM_TYPES[INVENTORY_POINTER]);
            useColor   = getUseColor(item);
            dropColor  = item->dropable ? 9 : 10;
        }
        const bool allTab    = inv_currentCategory == 6;
        const auto moveColor = (allTab && type != ItemType::NONE) ? 9 : 10;
        auto       baseX     = box.finalPos.x + 9;
        auto       baseY     = box.finalPos.y + 6;

        useString.render(baseX + 3, baseY + 2, useColor, 2, true);
        dropString.render(baseX + 3, baseY + 20, dropColor, 2, true);
        if (allTab) moveString.render(baseX + 3, baseY + 38, moveColor, 2, true);

        renderSelectionCursor(baseX, baseY + menuSelected * 18, 40, 16, 2);
    }

    void tickInventoryOptions(int32_t /*instanceId*/)
    {
        if (focusedWindow != BOX_OPTIONS) return;

        const uint8_t maxOption = inv_currentCategory == 6 ? 2 : 1;

        if (isInventoryKeyDownRepeat(InputButtons::BUTTON_UP) && menuSelected > 0)
        {
            playSound(0, 2);
            menuSelected--;
        }
        if (isInventoryKeyDownRepeat(InputButtons::BUTTON_DOWN) && menuSelected < maxOption)
        {
            playSound(0, 2);
            menuSelected++;
        }

        if (isInventoryKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            RECT target = getCursorSlotRect();
            removeAnimatedUIBox(BOX_OPTIONS, &target);
        }
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
                    state       = 201;
                    RECT target = getCursorSlotRect();
                    removeAnimatedUIBox(BOX_OPTIONS, &target);
                    break;
                }
                case 1:
                {
                    const auto type   = INVENTORY_ITEM_TYPES[INVENTORY_POINTER];
                    const auto amount = INVENTORY_ITEM_AMOUNTS[INVENTORY_POINTER];
                    spawnDroppedItem(&TAMER_ENTITY, type, amount);
                    setIsStandingOnDrop(true);
                    removeItem(type, amount);
                    compactInventory();
                    rebuildFiltered();
                    applyListSize();
                    UI_BOX_DATA[BOX_LIST].totalRows = inv_filteredCount;
                    if (inv_filteredCount == 0)
                    {
                        UI_BOX_DATA[BOX_LIST].rowOffset = 0;
                        INVENTORY_POINTER               = 0;
                    }
                    else if (getVisibleRow(INVENTORY_POINTER) < 0)
                        INVENTORY_POINTER = inv_filteredIdx[0];
                    descNeedsUpdate = true;
                    updateCapacity();
                    RECT target = getCursorSlotRect();
                    removeAnimatedUIBox(BOX_OPTIONS, &target);
                    break;
                }
                case 2:
                {
                    inv_moveSourceSlot = static_cast<int8_t>(INVENTORY_POINTER);
                    RECT target        = getCursorSlotRect();
                    removeAnimatedUIBox(BOX_OPTIONS, &target);
                    break;
                }
            }
        }
    }

    void createInventoryOptions()
    {
        RECT start   = getCursorSlotRect();
        start.width  = SLOT_WIDTH;
        start.height = SLOT_HEIGHT;

        const bool    allTab = inv_currentCategory == 6;
        const int16_t menuH  = allTab ? static_cast<int16_t>(MENU_HEIGHT + 18) : MENU_HEIGHT;

        const int32_t cursorPos  = getVisibleRow(INVENTORY_POINTER);
        const int32_t visibleRow = cursorPos < 0 ? 0 : (cursorPos / 2 - UI_BOX_DATA[BOX_LIST].rowOffset);
        const int16_t finalX     = static_cast<int16_t>(start.x + 3);
        const int16_t finalY =
            visibleRow >= 7 ? static_cast<int16_t>(start.y - menuH) : static_cast<int16_t>(start.y + 17);

        RECT final = {.x = finalX, .y = finalY, .width = MENU_WIDTH, .height = menuH};

        menuSelected = 0;
        createAnimatedUIBox(BOX_OPTIONS, 2, 0x12, &final, &start, tickInventoryOptions, renderItemOptions);
    }

    void tickInventoryTop(int32_t /*instanceId*/)
    {
        if (focusedWindow != BOX_LIST) return;

        auto& box = UI_BOX_DATA[BOX_LIST];

        // Don't touch previousSelection here - it tracks what the description
        // panel currently shows in VRAM and is updated by the renderer.
        const auto soundPrev = INVENTORY_POINTER;

        if (inv_categoriesPresent != 0)
        {
            const bool tabLeft  = isInventoryKeyDown(InputButtons::BUTTON_L1);
            const bool tabRight = isInventoryKeyDown(InputButtons::BUTTON_R1);
            if (tabLeft || tabRight)
            {
                int32_t newCat = shiftCategory(inv_currentCategory, inv_categoriesPresent, tabRight);
                if (newCat != static_cast<int32_t>(inv_currentCategory))
                {
                    inv_moveSourceSlot  = -1;
                    inv_currentCategory = static_cast<uint8_t>(newCat);
                    rebuildFiltered();
                    applyListSize();
                    INVENTORY_POINTER = inv_filteredCount > 0 ? inv_filteredIdx[0] : 0;
                    box.rowOffset     = 0;
                    descNeedsUpdate   = true;
                    playSound(0, 2);
                }
            }
        }

        if (inv_filteredCount > 0)
        {
            int32_t pos = getVisibleRow(INVENTORY_POINTER);
            if (pos < 0) pos = 0;
            int32_t newPos = pos;
            if (isInventoryKeyDownRepeat(InputButtons::BUTTON_UP) && pos >= 2) newPos = pos - 2;
            if (isInventoryKeyDownRepeat(InputButtons::BUTTON_DOWN) && pos + 2 < inv_filteredCount) newPos = pos + 2;
            if (isInventoryKeyDownRepeat(InputButtons::BUTTON_LEFT) && (pos & 1) == 1) newPos = pos - 1;
            if (isInventoryKeyDownRepeat(InputButtons::BUTTON_RIGHT) && (pos & 1) == 0 && pos + 1 < inv_filteredCount)
                newPos = pos + 1;
            if (newPos != pos) INVENTORY_POINTER = inv_filteredIdx[newPos];
        }
        if (soundPrev != INVENTORY_POINTER) playSound(0, 2);

        box.totalRows           = (inv_filteredCount + 1) / 2;
        const int32_t cursorRow = inv_filteredCount > 0 ? (getVisibleRow(INVENTORY_POINTER) / 2) : 0;
        if (box.rowOffset > cursorRow) box.rowOffset = cursorRow;
        if (box.rowOffset + box.visibleRows - 1 < cursorRow) box.rowOffset = cursorRow - box.visibleRows + 1;
        if (box.rowOffset < 0) box.rowOffset = 0;

        if (inv_moveSourceSlot >= 0)
        {
            if (isInventoryKeyDown(InputButtons::BUTTON_CROSS))
            {
                const int32_t src = inv_moveSourceSlot;
                const int32_t dst = INVENTORY_POINTER;
                if (src != dst)
                {
                    dtl::swap(INVENTORY_ITEM_TYPES[src], INVENTORY_ITEM_TYPES[dst]);
                    dtl::swap(INVENTORY_ITEM_AMOUNTS[src], INVENTORY_ITEM_AMOUNTS[dst]);
                    dtl::swap(INVENTORY_ITEM_NAMES[src], INVENTORY_ITEM_NAMES[dst]);
                    rebuildFiltered();
                    descNeedsUpdate = true;
                }
                inv_moveSourceSlot = -1;
                playSound(0, 4);
            }
            else if (isInventoryKeyDown(InputButtons::BUTTON_TRIANGLE))
            {
                inv_moveSourceSlot = -1;
                playSound(0, 4);
            }
            return;
        }

        if (isInventoryKeyDown(InputButtons::BUTTON_CROSS))
        {
            if (inv_filteredCount == 0) { playSound(0, 11); }
            else if (GAME_STATE >= 1 && GAME_STATE <= 3) { state = 201; }
            else
            {
                createInventoryOptions();
                state         = 51;
                focusedWindow = BOX_OPTIONS;
            }
        }
        else if (isInventoryKeyDown(InputButtons::BUTTON_SQUARE))
        {
            if (inv_filteredCount > 0)
            {
                state         = 70;
                focusedWindow = -1;
                playSound(0, 2);
            }
        }
        else if (isInventoryKeyDown(InputButtons::BUTTON_TRIANGLE)) { state = 3; }
    }

    void createInventoryTop()
    {
        auto& box = UI_BOX_DATA[BOX_LIST];
        if (box.frame != 0) return;

        box.rowOffset = 0;
        box.totalRows = inv_filteredCount;
        applyListSize();

        auto& work     = TAMER_ENTITY.posData[1].posMatrix.work.t;
        auto entityPos = getScreenPosition(work[0], work[1], work[2]);

        RECT finalPos = {
            .x      = LIST_X,
            .y      = LIST_Y,
            .width  = LIST_WIDTH,
            .height = box.finalPos.height,
        };
        RECT startPos = {
            .x      = static_cast<int16_t>(entityPos.screenX),
            .y      = static_cast<int16_t>(entityPos.screenY),
            .width  = 10,
            .height = 10,
        };

        // features bits: 2 = translucent fill, 8 = skip engine auto-border (we draw a notched one), 16 = 75/25 blend.
        createAnimatedUIBox(BOX_LIST, 2, 26, &finalPos, &startPos, tickInventoryTop, renderInventoryTop);
    }

    RECT getCursorSlotRect()
    {
        const int32_t cursorPos = getVisibleRow(INVENTORY_POINTER);
        if (cursorPos < 0) return RECT{.x = 0, .y = 0, .width = 16, .height = 16};
        const int32_t cursorRow = cursorPos / 2 - UI_BOX_DATA[BOX_LIST].rowOffset;
        const int32_t cursorCol = cursorPos & 1;
        return RECT{
            .x      = getSlotPosX(cursorCol),
            .y      = getSlotPosY(cursorRow),
            .width  = 16,
            .height = 16,
        };
    }

    void tickInventoryInfo(int32_t /*instanceId*/)
    {
        if (focusedWindow != BOX_INFO) return;
        if (isInventoryKeyDown(InputButtons::BUTTON_TRIANGLE) || isInventoryKeyDown(InputButtons::BUTTON_SQUARE) ||
            isInventoryKeyDown(InputButtons::BUTTON_CROSS))
        {
            RECT target = getCursorSlotRect();
            removeAnimatedUIBox(BOX_INFO, &target);
            focusedWindow = -1;
            state         = 72;
            playSound(0, 4);
        }
    }

    void createInventoryInfo()
    {
        constexpr RECT finalPos = {.x = INFO_X, .y = INFO_Y, .width = INFO_WIDTH, .height = INFO_HEIGHT};
        RECT startPos           = getCursorSlotRect();
        createAnimatedUIBox(BOX_INFO, 2, 26, &finalPos, &startPos, tickInventoryInfo, renderInventoryInfo);
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
        removeAnimatedUIBox(BOX_LIST, &target);
    }

    void startThrowingItem()
    {
        startAnimation(&TAMER_ENTITY, 6);
        addThrownItem(INVENTORY_ITEM_TYPES[INVENTORY_POINTER]);
        removeItem(INVENTORY_ITEM_TYPES[INVENTORY_POINTER], 1);
    }

    void tickInventoryUI(int32_t /*instanceId*/)
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
                compactInventory();
                inv_currentCategory = static_cast<uint8_t>(pickSmartDefault());
                inv_moveSourceSlot  = -1;
                rebuildFiltered();
                INVENTORY_POINTER = inv_filteredCount > 0 ? inv_filteredIdx[0] : 0;
                previousSelection = -1;
                descNeedsUpdate   = true;
                drawInventoryText();
                createInventoryTop();
                updateItemDescription();
                updateCapacity();
                if (TAMER_ENTITY.animId != 4) startAnimation(&TAMER_ENTITY, 4);
                state = 1;
                break;
            }
            case 1:
            {
                if (UI_BOX_DATA[BOX_LIST].state == 1) state = 2;
                break;
            }
            case 2: break;
            case 3:
            {
                focusedWindow = -1;
                closeInventoryTop();
                state = 4;
                break;
            }
            case 4:
            {
                if (UI_BOX_DATA[BOX_LIST].state != 0) return;
                closeInventoryBoxes();

                if (GAME_STATE == 0)
                {
                    closeTriangleMenu();
                    addGameMenu();
                    startAnimation(&TAMER_ENTITY, 0);
                }
                break;
            }

            case 51:
            {
                if (UI_BOX_DATA[BOX_OPTIONS].state != 0) return;
                focusedWindow = BOX_LIST;
                state         = 2;
                break;
            }

            case 70:
            {
                createInventoryInfo();
                focusedWindow = BOX_INFO;
                state         = 71;
                break;
            }
            case 71: break;
            case 72:
            {
                if (UI_BOX_DATA[BOX_INFO].state != 0) return;
                focusedWindow = BOX_LIST;
                state         = 2;
                break;
            }

            case 201:
            {
                if (UI_BOX_DATA[BOX_OPTIONS].state != 0) return;
                closeInventoryTop();
                state = 202;
                break;
            }
            case 202:
            {
                if (UI_BOX_DATA[BOX_LIST].state != 0) return;
                state = 203;
                break;
            }
            case 203:
            {
                if (UI_BOX_DATA[BOX_LIST].state != 0) return;
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

        removeStaticUIBox(BOX_LIST);
        removeStaticUIBox(BOX_INFO);
        removeStaticUIBox(BOX_OPTIONS);

        removeObject(ObjectID::INVENTORY, 0);
    }

    void addInventoryUI()
    {
        if (isInventoryOpen) return;
        if (UI_BOX_DATA[BOX_LIST].state != 0) return;
        if (TAMER_ITEM.type != ItemType::NONE) return;

        focusedWindow   = BOX_LIST;
        state           = 0;
        isInventoryOpen = true;
        addObject(ObjectID::INVENTORY, 0, tickInventoryUI, nullptr);
    }

    void initializeInventoryUI()
    {
        state           = -1;
        isInventoryOpen = false;
    }

    void setItemTexture(POLY_FT4* prim, ItemType item)
    {
        auto col = static_cast<int32_t>(item) % 16;
        auto row = static_cast<int32_t>(item) / 16;

        auto width  = col != 15 ? 16 : 15;
        auto height = row != 15 ? 16 : 15;

        setUVDataPolyFT4(prim, col * 16, row * 16, width, height);
        prim->clut = getClut(0xe0, ITEM_CLUT_DATA[static_cast<int32_t>(item)] + 0x1e8);
    }

    void renderItemSprite(ItemType type, int16_t posX, int16_t posY, int32_t depth)
    {
        auto prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyFT4(prim);
        prim->tpage = 5;
        setItemTexture(prim, type);
        prim->r0 = 128;
        prim->g0 = 128;
        prim->b0 = 128;

        setPosDataPolyFT4(prim, posX, posY, prim->u1 - prim->u0, prim->v2 - prim->v0);
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + depth, prim);
        libgs_GsSetWorkBase(prim + 1);
    }
}
