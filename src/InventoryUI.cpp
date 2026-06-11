#include "InventoryUI.hpp"

#include "AtlasFont.hpp"
#include "Font.hpp"
#include "GameMenu.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Inventory.hpp"
#include "InventoryUIHelpers.hpp"
#include "ItemFunctions.hpp"
#include "ItemInfo.hpp"
#include "LabeledUIBox.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "Sound.hpp"
#include "Tamer.hpp"
#include "ThrownItem.hpp"
#include "UIBox.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dtl/unique_ptr.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/stddef.hpp"

namespace
{
    constexpr int8_t FOCUS_NONE    = -1;
    constexpr int8_t FOCUS_LIST    = 0;
    constexpr int8_t FOCUS_INFO    = 1;
    constexpr int8_t FOCUS_OPTIONS = 2;

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

    constexpr RGB8 PANEL_FILL_COLOR = {.red = 12, .green = 22, .blue = 30};

    // List panel draws its own (tabbed) border, so drawBorder=false.
    constexpr UIBox::Style PANEL_STYLE{.color = PANEL_FILL_COLOR, .fill = UIBox::Fill::DOUBLE, .drawBorder = false};
    // Options panel and the labeled (info/capacity) panels use the auto-drawn border.
    constexpr UIBox::Style OPTIONS_STYLE{.color = PANEL_FILL_COLOR, .fill = UIBox::Fill::DOUBLE};
    constexpr UIBox::Style LABELED_PANEL_STYLE = OPTIONS_STYLE;

    // Render colors are doubled-and-clamped versions of VanillaText.hpp's TEXT_COLORS.
    // AtlasFont halves color.red/green/blue (`(c+1)/2`) before storing as prim color.
    // Combined with PSX modulation `output = texture*prim/128` over a pure-white
    // glyph atlas, color=0xFF yields pixel-perfect 0xFF output.
    constexpr RGB8 TEXT_LIGHT    = {.red = 0xFF, .green = 0xFF, .blue = 0xFF};
    constexpr RGB8 TEXT_CYAN     = {.red = 0xD2, .green = 0xFF, .blue = 0xFF};
    constexpr RGB8 TEXT_RED      = {.red = 0xFF, .green = 0x3C, .blue = 0xA0};
    constexpr RGB8 TEXT_ENABLED  = {.red = 0xDC, .green = 0xDC, .blue = 0xDC};
    constexpr RGB8 TEXT_DISABLED = {.red = 0x8C, .green = 0x8C, .blue = 0x8C};

    constexpr dtl::array<int8_t, 128> ITEM_CLUT_DATA{
        0,  1,  18, 3,  4,  8,  6,  0,  8,  6, 7,  4,  8,  9,  9,  10, 4,  11, 8,  12, 8,  11, 13, 10, 4,  14,
        6,  0,  4,  15, 14, 8,  8,  11, 14, 8, 16, 21, 17, 17, 17, 18, 19, 11, 9,  14, 21, 2,  3,  21, 16, 18,
        20, 11, 11, 1,  12, 4,  11, 7,  0,  2, 14, 20, 13, 13, 13, 13, 15, 8,  11, 5,  12, 1,  2,  4,  4,  5,
        4,  5,  16, 4,  5,  16, 22, 21, 12, 4, 12, 23, 23, 5,  8,  12, 5,  3,  14, 17, 4,  10, 19, 10, 4,  4,
        4,  10, 23, 5,  4,  16, 5,  10, 4,  7, 8,  14, 20, 9,  4,  16, 4,  23, 21, 4,  9,  4,  4,  4,
    };

    constexpr int16_t getSlotPosX(int32_t col)
    { return LIST_X + SLOT_OFFSET_X + col * (SLOT_WIDTH + SLOT_GAP_X); }

    inline int16_t getSlotPosY(int32_t visibleRowIdx)
    { return LIST_Y + SLOT_OFFSET_Y + SLOT_HEIGHT * visibleRowIdx; }

    uint8_t getUseColor(Item* item)
    {
        if (item->itemColor == 2) return 9;
        if (GAME_STATE == 0 && item->itemColor == 0) return 9;
        if ((GAME_STATE == 1 || GAME_STATE == 2 || GAME_STATE == 3) && item->itemColor == 1) return 9;
        return 10;
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

    struct ListView
    {
        UIBox box;   // opened with PANEL_STYLE
        dtl::array<AtlasString, 30> slotNameStrs;
        uint32_t scrollBopPhase = 0;
        int16_t  rowOffset      = 0;
        int16_t  visibleRows    = 0;
        int16_t  totalRows      = 0;
        int16_t  activeTabX     = 0;
        int16_t  activeTabW     = 0;
        int8_t   moveSourceSlot = -1;

        void applySize();
        RECT getCursorRect() const;
        void bakeSlot(int32_t i);
        void bakeAll();
        void open();
        void close();
        void tickInput();
        void render();
    };

    struct InfoView
    {
        LabeledUIBox box;   // fill + border + "ITEM DESCRIPTION" label
        AtlasString itemNameStr;
        AtlasString headlineStr;
        AtlasString detailStr;
        dtl::array<AtlasString, 4> descLines;
        uint8_t previousSelection = 0xFF;
        bool    needsUpdate       = true;

        void open();
        void updateDescription();
        void drawWrappedDescription(const char* src, int16_t maxWidth, int16_t baseX, int16_t baseY);
        void tickInput();
        void render();
    };

    struct OptionsView
    {
        UIBox box;   // opened with OPTIONS_STYLE
        AtlasString menuUseStr;
        AtlasString menuDropStr;
        AtlasString menuMoveStr;
        uint8_t menuSelected = 0;

        void open();
        void tickInput();
        void render();
    };

    struct CapacityBar
    {
        LabeledUIBox capBorder;   // fill + border + "CAPACITY" label
        AtlasString capacityUsedStr;
        AtlasString capacityMaxStr;
        AtlasString fullBadgeStr;

        void update();
        void render(int32_t labelDepth);
    };

    struct InventoryUI
    {
        ListView    listView;
        InfoView    infoView;
        OptionsView optionsView;
        CapacityBar capacityBar;

        uint32_t state         = 0;
        int8_t   focusedWindow = FOCUS_LIST;
        uint8_t  repeatTimer   = 0;

        void tick();
        void render();
    };

    dtl::unique_ptr<InventoryUI> data;

    bool isKey(InputButtons button)
    { return (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & button) != 0; }

    bool isKeyRepeat(InputButtons button)
    {
        if (!isKey(button) && (data->repeatTimer < 7 || (POLLED_INPUT & button) == 0)) return false;
        return true;
    }

    void ListView::applySize()
    {
        const auto rows = clamp((inv_filteredCount + 1) / 2, 1, 10);
        visibleRows     = static_cast<int16_t>(rows);
        box.resize({
            .x      = LIST_X,
            .y      = LIST_Y,
            .width  = LIST_WIDTH,
            .height = static_cast<int16_t>(rows * SLOT_HEIGHT + 14),
        });
    }

    RECT ListView::getCursorRect() const
    {
        const int32_t cursorPos = getVisibleRow(INVENTORY_POINTER);
        if (cursorPos < 0) return RECT{.x = 0, .y = 0, .width = 16, .height = 16};
        const int32_t cursorRow = cursorPos / 2 - rowOffset;
        const int32_t cursorCol = cursorPos & 1;
        return RECT{
            .x      = getSlotPosX(cursorCol),
            .y      = getSlotPosY(cursorRow),
            .width  = 16,
            .height = 16,
        };
    }

    void ListView::bakeSlot(int32_t i)
    {
        const auto type = INVENTORY_ITEM_TYPES[i];
        if (type == ItemType::NONE)
        {
            slotNameStrs[i] = {};
            return;
        }
        slotNameStrs[i] = getAtlasVanilla().render(getItem(type)->name, {.color = TEXT_LIGHT});
    }

    void ListView::bakeAll()
    {
        for (int32_t i = 0; i < INVENTORY_SIZE; i++) bakeSlot(i);
    }

    void InfoView::drawWrappedDescription(const char* src, int16_t maxWidth, int16_t baseX, int16_t baseY)
    {
        for (auto& l : descLines) l = {};

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
            descLines[lineIdx] = getAtlas7px().render(
                buf,
                {.x = baseX, .y = static_cast<int16_t>(baseY + lineIdx * 9), .color = TEXT_LIGHT});
            lineIdx++;

            srcPos += takeLen;
            while (src[srcPos] == ' ' || src[srcPos] == '\n') srcPos++;
        }
    }

    void InfoView::updateDescription()
    {
        const int16_t descX = INFO_X + 8;
        const int16_t descY = INFO_Y + 36;

        const auto type = INVENTORY_ITEM_TYPES[INVENTORY_POINTER];

        if (type == ItemType::NONE)
        {
            drawWrappedDescription("", 224, descX, descY);
            itemNameStr = {};
            headlineStr = {};
            detailStr   = {};
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

        drawWrappedDescription(desc, INFO_WIDTH - 16, descX, descY);

        itemNameStr = getAtlasVanilla().render(getItem(type)->name,
            {.x     = static_cast<int16_t>(box.finalPos().x + 32),
             .y     = static_cast<int16_t>(box.finalPos().y + 14),
             .color = TEXT_CYAN});
        headlineStr = getAtlasVanilla().render(
            info.headline, {.x = descX, .y = INFO_Y + 86, .color = TEXT_CYAN});
        detailStr = getAtlasVanilla().render(
            detail, {.x = descX, .y = INFO_Y + 98, .color = TEXT_LIGHT});
    }

    void CapacityBar::update()
    {
        const int16_t numX = CAP_X + 8;
        const int16_t numY = CAP_Y + 4;
        capacityUsedStr = getAtlas7px().render(
            format<8>("%d", countUsedSlots()).data(), {.x = numX, .y = numY, .color = TEXT_CYAN});
        const int16_t usedW  = static_cast<int16_t>(capacityUsedStr.getWidth());
        const int16_t slashX = numX + usedW + 2;
        capacityMaxStr = getAtlas7px().render(
            format<8>("%d", INVENTORY_SIZE).data(),
            {.x = static_cast<int16_t>(slashX + 6), .y = numY, .color = TEXT_CYAN});
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

    void ListView::render()
    {
        constexpr int32_t layer      = 6;
        constexpr int32_t depth      = 6;
        constexpr int32_t labelDepth = 5;

        if (!box.isOpen()) { box.render(layer); return; }
        // Content first, chrome last. PSX OT is LIFO at same depth — latest
        // submitted = head = drawn first = BEHIND. We want fills behind content.

        const int32_t cursorPos = getVisibleRow(INVENTORY_POINTER);
        const int32_t winLo     = rowOffset;
        const int32_t winHi     = rowOffset + visibleRows;
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

            renderItemSprite(type, posX + SLOT_SPRITE_OFF_X, posY + SLOT_SPRITE_OFF_Y, depth);

            auto& nameStr = slotNameStrs[i];
            nameStr.setPosition(posX + SLOT_NAME_OFF_X, posY + SLOT_NAME_OFF_Y);
            nameStr.render(depth);

            // Amount is short and changes color on cursor — let renderSlow's
            // RenderSettings do the right-alignment in one shot.
            getAtlasVanilla().renderSlow(
                format<8>("x%d", INVENTORY_ITEM_AMOUNTS[i]).data(),
                depth,
                {.x      = posX,
                 .y      = static_cast<int16_t>(posY + SLOT_AMOUNT_OFF_Y),
                 .color  = (p == cursorPos) ? TEXT_CYAN : TEXT_LIGHT,
                 .width  = SLOT_AMOUNT_OFF_X,
                 .alignX = AlignmentX::RIGHT});
        }

        if (cursorPos >= firstPos && cursorPos < lastPos)
        {
            const auto cursorX = getSlotPosX(cursorPos & 1);
            const auto cursorY = getSlotPosY(cursorPos / 2 - winLo);
            renderInventoryCursor(cursorX, cursorY, SLOT_WIDTH, SLOT_HEIGHT, depth);
        }

        if (moveSourceSlot >= 0)
        {
            const int32_t srcPos = getVisibleRow(moveSourceSlot);
            if (srcPos >= firstPos && srcPos < lastPos)
            {
                const auto srcX = getSlotPosX(srcPos & 1);
                const auto srcY = getSlotPosY(srcPos / 2 - winLo);
                renderInventoryCursor(srcX, srcY, SLOT_WIDTH, SLOT_HEIGHT, depth);
            }
        }

        const int16_t arrowCx     = LIST_X + LIST_WIDTH / 2;
        const int16_t listBottomY = box.finalPos().y + box.finalPos().height - 1;
        scrollBopPhase++;
        const int16_t bop = static_cast<int16_t>((scrollBopPhase >> 4) & 1);
        if (rowOffset > 0) renderScrollArrow(arrowCx, LIST_Y + 3 - bop, false, depth);
        if (rowOffset + visibleRows < totalRows)
            renderScrollArrow(arrowCx, listBottomY - 5 + bop, true, depth);

        if (totalRows > visibleRows)
        {
            const int16_t trackX = LIST_X + LIST_WIDTH - 7;
            const int16_t trackY = static_cast<int16_t>(LIST_Y + 8);
            const int16_t trackH = static_cast<int16_t>(listBottomY - 7 - trackY);
            const int16_t thumbH = static_cast<int16_t>((trackH * visibleRows) / totalRows);
            const int16_t thumbY = static_cast<int16_t>(
                trackY + ((trackH - thumbH) * rowOffset) / (totalRows - visibleRows));
            drawLine2P(0xffc269, trackX, thumbY, trackX, thumbY + thumbH, depth, 0);
        }

        renderCategoryTabs(TAB_Y, labelDepth - 1, &activeTabX, &activeTabW);
        RECT listRect{.x = LIST_X, .y = LIST_Y, .width = box.finalPos().width, .height = box.finalPos().height};
        renderUIBoxBorder(&listRect, labelDepth);

        data->capacityBar.render(labelDepth);

        box.render(layer);
    }

    void CapacityBar::render(int32_t labelDepth)
    {
        constexpr int32_t depth = 6;

        // fill (75/25 blend) + border + "CAPACITY" label, behind the readout below.
        capBorder.render(depth);

        const int32_t used = countUsedSlots();
        const int32_t cap  = INVENTORY_SIZE > 0 ? INVENTORY_SIZE : 1;

        const int16_t numX  = CAP_X + 8;
        const int16_t numY  = CAP_Y + 4;
        const int16_t usedW = static_cast<int16_t>(capacityUsedStr.getWidth());
        capacityUsedStr.render(labelDepth);
        const int16_t slashX = numX + usedW + 2;
        drawLine2P(SLASH_COLOR, slashX + 3, numY + 1, slashX, numY + 6, labelDepth, 0);
        capacityMaxStr.render(labelDepth);

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
            fullBadgeStr.render(labelDepth);
        }
    }

    void InfoView::render()
    {
        constexpr int32_t layer      = 4;
        constexpr int32_t depth      = 2;
        constexpr int32_t labelDepth = 1;

        if (!box.isOpen()) { box.render(layer); return; }

        // Panel chrome: fill + border at `layer`, label/notch at `layer - 1`.
        // Content sits at lower depths (in front) and never overlaps the label.
        box.render(layer);

        if (INVENTORY_ITEM_TYPES[INVENTORY_POINTER] == ItemType::NONE) return;

        if (needsUpdate || INVENTORY_POINTER != previousSelection)
        {
            updateDescription();
            previousSelection = INVENTORY_POINTER;
            needsUpdate       = false;
        }

        const int16_t bx = box.finalPos().x;
        const int16_t by = box.finalPos().y;
        const int16_t bw = box.finalPos().width;

        renderItemSprite(INVENTORY_ITEM_TYPES[INVENTORY_POINTER], bx + 10, by + 10, depth);
        itemNameStr.render(depth);

        const int16_t hdrSep = by + 30;
        drawLine2P(BAR_OUTLINE_COLOR, bx + 6, hdrSep, bx + bw - 7, hdrSep, labelDepth, 0);

        for (const auto& line : descLines) line.render(depth);

        const int16_t effSep = by + 80;
        drawLine2P(BAR_OUTLINE_COLOR, bx + 6, effSep, bx + bw - 7, effSep, labelDepth, 0);

        headlineStr.render(depth);
        detailStr.render(depth);
    }

    void OptionsView::render()
    {
        constexpr int32_t layer = 2;
        if (!box.isOpen()) { box.render(layer); return; }

        const auto baseX = box.finalPos().x + 9;
        const auto baseY = box.finalPos().y + 6;

        menuUseStr.render(2);
        menuDropStr.render(2);
        if (inv_currentCategory == 6) menuMoveStr.render(2);

        renderSelectionCursor(baseX, baseY + menuSelected * 18, 40, 16, 2);

        box.render(layer);
    }

    void OptionsView::tickInput()
    {
        const uint8_t maxOption = inv_currentCategory == 6 ? 2 : 1;

        if (isKeyRepeat(InputButtons::BUTTON_UP) && menuSelected > 0)
        {
            playSound(0, 2);
            menuSelected--;
        }
        if (isKeyRepeat(InputButtons::BUTTON_DOWN) && menuSelected < maxOption)
        {
            playSound(0, 2);
            menuSelected++;
        }

        if (isKey(InputButtons::BUTTON_TRIANGLE))
        {
            box.close(data->listView.getCursorRect());
        }
        else if (isKey(InputButtons::BUTTON_CROSS))
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
                    data->state = 201;
                    box.close(data->listView.getCursorRect());
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
                    data->listView.bakeAll();
                    data->listView.applySize();
                    data->listView.totalRows = static_cast<int16_t>(inv_filteredCount);
                    if (inv_filteredCount == 0)
                    {
                        data->listView.rowOffset = 0;
                        INVENTORY_POINTER        = 0;
                    }
                    else if (getVisibleRow(INVENTORY_POINTER) < 0)
                        INVENTORY_POINTER = inv_filteredIdx[0];
                    data->infoView.needsUpdate = true;
                    data->capacityBar.update();
                    box.close(data->listView.getCursorRect());
                    break;
                }
                case 2:
                {
                    data->listView.moveSourceSlot = static_cast<int8_t>(INVENTORY_POINTER);
                    box.close(data->listView.getCursorRect());
                    break;
                }
            }
        }
    }

    void OptionsView::open()
    {
        RECT start   = data->listView.getCursorRect();
        start.width  = SLOT_WIDTH;
        start.height = SLOT_HEIGHT;

        const bool    allTab = inv_currentCategory == 6;
        const int16_t menuH  = allTab ? static_cast<int16_t>(MENU_HEIGHT + 18) : MENU_HEIGHT;

        const int32_t cursorPos  = getVisibleRow(INVENTORY_POINTER);
        const int32_t visibleRow = cursorPos < 0 ? 0 : (cursorPos / 2 - data->listView.rowOffset);
        const int16_t finalX     = static_cast<int16_t>(start.x + 3);
        const int16_t finalY =
            visibleRow >= 7 ? static_cast<int16_t>(start.y - menuH) : static_cast<int16_t>(start.y + 17);

        RECT final = {.x = finalX, .y = finalY, .width = MENU_WIDTH, .height = menuH};

        const auto type      = INVENTORY_ITEM_TYPES[INVENTORY_POINTER];
        auto       useColor  = 10;
        auto       dropColor = 10;
        if (type != ItemType::NONE)
        {
            auto* item = getItem(type);
            useColor   = getUseColor(item);
            dropColor  = item->dropable ? 9 : 10;
        }
        const auto moveColor = (allTab && type != ItemType::NONE) ? 9 : 10;
        const auto colorFor  = [](int32_t c) { return c == 9 ? TEXT_ENABLED : TEXT_DISABLED; };

        const int16_t labelX = static_cast<int16_t>(finalX + 12);
        const int16_t labelY = static_cast<int16_t>(finalY + 8);
        menuUseStr  = getAtlasVanilla().render(
            "Use",  {.x = labelX, .y = labelY,                                   .color = colorFor(useColor)});
        menuDropStr = getAtlasVanilla().render(
            "Drop", {.x = labelX, .y = static_cast<int16_t>(labelY + 18),        .color = colorFor(dropColor)});
        if (allTab)
            menuMoveStr = getAtlasVanilla().render(
                "Move", {.x = labelX, .y = static_cast<int16_t>(labelY + 36),    .color = colorFor(moveColor)});
        else
            menuMoveStr = {};

        menuSelected = 0;
        box          = UIBox(final, OPTIONS_STYLE, start);
    }

    void ListView::tickInput()
    {
        // Don't touch previousSelection here - it tracks what the description
        // panel currently shows in VRAM and is updated by the renderer.
        const auto soundPrev = INVENTORY_POINTER;

        if (inv_categoriesPresent != 0)
        {
            const bool tabLeft  = isKey(InputButtons::BUTTON_L1);
            const bool tabRight = isKey(InputButtons::BUTTON_R1);
            if (tabLeft || tabRight)
            {
                int32_t newCat = shiftCategory(inv_currentCategory, inv_categoriesPresent, tabRight);
                if (newCat != static_cast<int32_t>(inv_currentCategory))
                {
                    moveSourceSlot      = -1;
                    inv_currentCategory = static_cast<uint8_t>(newCat);
                    rebuildFiltered();
                    applySize();
                    INVENTORY_POINTER = inv_filteredCount > 0 ? inv_filteredIdx[0] : 0;
                    rowOffset         = 0;
                    data->infoView.needsUpdate = true;
                    playSound(0, 2);
                }
            }
        }

        if (inv_filteredCount > 0)
        {
            int32_t pos = getVisibleRow(INVENTORY_POINTER);
            if (pos < 0) pos = 0;

            int32_t deltaRow = 0;
            int32_t deltaCol = 0;
            if (isKeyRepeat(InputButtons::BUTTON_UP))        deltaRow = -1;
            else if (isKeyRepeat(InputButtons::BUTTON_DOWN)) deltaRow = +1;
            if (isKeyRepeat(InputButtons::BUTTON_LEFT))         deltaCol = -1;
            else if (isKeyRepeat(InputButtons::BUTTON_RIGHT))   deltaCol = +1;

            if (deltaRow != 0 || deltaCol != 0)
            {
                // Validate the combined target so a sparse last row still permits a diagonal
                // into the populated cell (e.g. pos=3 + DOWN+LEFT → pos=4 even though pos=5 is empty).
                const int32_t newRow = pos / 2 + deltaRow;
                const int32_t newCol = (pos & 1) + deltaCol;
                const int32_t target = newRow * 2 + newCol;
                if (newRow >= 0 && newCol >= 0 && newCol <= 1 && target < inv_filteredCount)
                    INVENTORY_POINTER = inv_filteredIdx[target];
            }
        }
        if (soundPrev != INVENTORY_POINTER) playSound(0, 2);

        totalRows               = static_cast<int16_t>((inv_filteredCount + 1) / 2);
        const int32_t cursorRow = inv_filteredCount > 0 ? (getVisibleRow(INVENTORY_POINTER) / 2) : 0;
        if (rowOffset > cursorRow) rowOffset = static_cast<int16_t>(cursorRow);
        if (rowOffset + visibleRows - 1 < cursorRow) rowOffset = static_cast<int16_t>(cursorRow - visibleRows + 1);
        if (rowOffset < 0) rowOffset = 0;

        if (moveSourceSlot >= 0)
        {
            if (isKey(InputButtons::BUTTON_CROSS))
            {
                const int32_t src = moveSourceSlot;
                const int32_t dst = INVENTORY_POINTER;
                if (src != dst)
                {
                    dtl::swap(INVENTORY_ITEM_TYPES[src], INVENTORY_ITEM_TYPES[dst]);
                    dtl::swap(INVENTORY_ITEM_AMOUNTS[src], INVENTORY_ITEM_AMOUNTS[dst]);
                    dtl::swap(INVENTORY_ITEM_NAMES[src], INVENTORY_ITEM_NAMES[dst]);
                    rebuildFiltered();
                    bakeSlot(src);
                    bakeSlot(dst);
                    data->infoView.needsUpdate = true;
                }
                moveSourceSlot = -1;
                playSound(0, 4);
            }
            else if (isKey(InputButtons::BUTTON_TRIANGLE))
            {
                moveSourceSlot = -1;
                playSound(0, 4);
            }
            return;
        }

        if (isKey(InputButtons::BUTTON_CROSS))
        {
            if (inv_filteredCount == 0) { playSound(0, 11); }
            else if (GAME_STATE >= 1 && GAME_STATE <= 3) { data->state = 201; }
            else
            {
                data->optionsView.open();
                data->state         = 51;
                data->focusedWindow = FOCUS_OPTIONS;
            }
        }
        else if (isKey(InputButtons::BUTTON_SQUARE))
        {
            if (inv_filteredCount > 0)
            {
                data->state         = 70;
                data->focusedWindow = FOCUS_NONE;
                playSound(0, 2);
            }
        }
        else if (isKey(InputButtons::BUTTON_TRIANGLE)) { data->state = 3; }
    }

    void ListView::open()
    {
        if (!box.isClosed()) return;

        rowOffset = 0;
        totalRows = static_cast<int16_t>(inv_filteredCount);
        applySize();

        auto& work     = TAMER_ENTITY.posData[1].posMatrix.work.t;
        auto entityPos = getScreenPosition(work[0], work[1], work[2]);

        RECT finalPos = {
            .x      = LIST_X,
            .y      = LIST_Y,
            .width  = LIST_WIDTH,
            .height = box.finalPos().height,
        };
        RECT startPos = {
            .x      = static_cast<int16_t>(entityPos.screenX),
            .y      = static_cast<int16_t>(entityPos.screenY),
            .width  = 10,
            .height = 10,
        };

        box = UIBox(finalPos, PANEL_STYLE, startPos);
    }

    void ListView::close()
    {
        auto& work     = TAMER_ENTITY.posData[1].posMatrix.work.t;
        auto entityPos = getScreenPosition(work[0], work[1], work[2]);
        RECT target    = {
            .x      = static_cast<int16_t>(entityPos.screenX - 5),
            .y      = static_cast<int16_t>(entityPos.screenY - 5),
            .width  = 10,
            .height = 10,
        };
        box.close(target);
    }

    void InfoView::tickInput()
    {
        if (isKey(InputButtons::BUTTON_TRIANGLE) || isKey(InputButtons::BUTTON_SQUARE) ||
            isKey(InputButtons::BUTTON_CROSS))
        {
            box.close(data->listView.getCursorRect());
            data->focusedWindow = FOCUS_NONE;
            data->state         = 72;
            playSound(0, 4);
        }
    }

    void InfoView::open()
    {
        constexpr RECT finalPos = {.x = INFO_X, .y = INFO_Y, .width = INFO_WIDTH, .height = INFO_HEIGHT};
        box = LabeledUIBox(finalPos, "ITEM DESCRIPTION", LABELED_PANEL_STYLE, TEXT_CYAN, data->listView.getCursorRect());
    }

    void startThrowingItem()
    {
        startAnimation(&TAMER_ENTITY, 6);
        addThrownItem(INVENTORY_ITEM_TYPES[INVENTORY_POINTER]);
        removeItem(INVENTORY_ITEM_TYPES[INVENTORY_POINTER], 1);
    }

    void InventoryUI::tick()
    {
        if (POLLED_INPUT == POLLED_INPUT_PREVIOUS)
            repeatTimer++;
        else
            repeatTimer = 0;

        if (COMBAT_DATA_PTR->player.currentCommand[0] == BattleCommand::RUN) return;

        if (focusedWindow == FOCUS_LIST    && listView.box.isOpen())    listView.tickInput();
        if (focusedWindow == FOCUS_INFO    && infoView.box.isOpen())    infoView.tickInput();
        if (focusedWindow == FOCUS_OPTIONS && optionsView.box.isOpen()) optionsView.tickInput();

        switch (state)
        {
            case 0:
            {
                compactInventory();
                inv_currentCategory          = static_cast<uint8_t>(pickSmartDefault());
                listView.moveSourceSlot      = -1;
                rebuildFiltered();
                INVENTORY_POINTER            = inv_filteredCount > 0 ? inv_filteredIdx[0] : 0;
                infoView.previousSelection = 0xFF;
                infoView.needsUpdate       = true;
                // InfoView's labeled box is (re)constructed in InfoView::open().
                capacityBar.capBorder = LabeledUIBox(
                    {.x = CAP_X, .y = CAP_Y, .width = CAP_WIDTH, .height = CAP_HEIGHT},
                    "CAPACITY", LABELED_PANEL_STYLE, TEXT_CYAN);
                capacityBar.fullBadgeStr = getAtlas7px().render(
                    "FULL", {.x = CAP_X + CAP_WIDTH - 32 - 6 + 4, .y = CAP_Y + 4 + 1, .color = TEXT_RED});
                listView.bakeAll();
                listView.open();
                infoView.updateDescription();
                capacityBar.update();
                if (TAMER_ENTITY.animId != 4) startAnimation(&TAMER_ENTITY, 4);
                state = 1;
                break;
            }
            case 1:
            {
                if (listView.box.isOpen()) state = 2;
                break;
            }
            case 2: break;
            case 3:
            {
                focusedWindow = FOCUS_NONE;
                listView.close();
                state = 4;
                break;
            }
            case 4:
            {
                if (!listView.box.isClosed()) break;
                closeInventoryBoxes();   // data.reset() — `this` is dangling after this point

                if (GAME_STATE == 0)
                {
                    closeTriangleMenu();
                    addGameMenu();
                    startAnimation(&TAMER_ENTITY, 0);
                }
                return;
            }

            case 51:
            {
                if (!optionsView.box.isClosed()) break;
                focusedWindow = FOCUS_LIST;
                state         = 2;
                break;
            }

            case 70:
            {
                infoView.open();
                focusedWindow = FOCUS_INFO;
                state         = 71;
                break;
            }
            case 71: break;
            case 72:
            {
                if (!infoView.box.isClosed()) break;
                focusedWindow = FOCUS_LIST;
                state         = 2;
                break;
            }

            case 201:
            {
                if (!optionsView.box.isClosed()) break;
                listView.close();
                state = 202;
                break;
            }
            case 202:
            {
                if (!listView.box.isClosed()) break;
                state = 203;
                break;
            }
            case 203:
            {
                if (!listView.box.isClosed()) break;
                closeInventoryBoxes();   // data.reset() — `this` is dangling after this point
                if (GAME_STATE == 0)
                    startFeedingItem(INVENTORY_ITEM_TYPES[INVENTORY_POINTER]);
                else if (GAME_STATE == 1)
                    startThrowingItem();
                return;
            }
        }

        listView.box.tick();
        infoView.box.tick();
        optionsView.box.tick();
    }

    void InventoryUI::render()
    {
        listView.render();
        infoView.render();
        optionsView.render();
    }

    void tickInventoryUI(int32_t /*instanceId*/)   { data->tick(); }
    void renderInventoryUI(int32_t /*instanceId*/) { data->render(); }

} // namespace

extern "C"
{
    void closeInventoryBoxes()
    {
        if (!data) return;
        removeObject(ObjectID::INVENTORY, 0);
        data.reset();
    }

    void addInventoryUI()
    {
        if (data) return;
        if (TAMER_ITEM.type != ItemType::NONE) return;

        data = dtl::make_unique<InventoryUI>();
        addObject(ObjectID::INVENTORY, 0, tickInventoryUI, renderInventoryUI);
    }

    void initializeInventoryUI()
    {
        data.reset();
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
