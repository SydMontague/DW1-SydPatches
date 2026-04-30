#include "InventoryUIHelpers.hpp"

#include "Helper.hpp"
#include "UIElements.hpp"
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    uint8_t inv_currentCategory   = 0;
    uint8_t inv_categoriesPresent = 0;
    int32_t inv_filteredCount     = 0;
    int8_t  inv_filteredIdx[30]   = {};

    // ALL (bit 6) renders first; the per-bucket tabs follow in original 0..5 order.
    static constexpr int32_t TAB_ORDER[7]  = {6, 0, 1, 2, 3, 4, 5};
    static constexpr int32_t ALL_CATEGORY  = 6;

    int32_t firstPresentCategory(uint8_t mask)
    {
        for (int32_t k = 0; k < 7; k++)
            if (mask & (1 << TAB_ORDER[k])) return TAB_ORDER[k];
        return 0;
    }

    int32_t shiftCategory(int32_t from, uint8_t mask, bool forward)
    {
        if (mask == 0) return from;
        int32_t fromPos = 0;
        for (int32_t k = 0; k < 7; k++)
            if (TAB_ORDER[k] == from) { fromPos = k; break; }
        for (int32_t step = 1; step <= 7; step++)
        {
            const int32_t pos = forward ? (fromPos + step) % 7 : (fromPos - step + 7) % 7;
            const int32_t b   = TAB_ORDER[pos];
            if (mask & (1 << b)) return b;
        }
        return from;
    }

    int32_t pickSmartDefault()
    {
        const auto& stats = PARTNER_ENTITY.stats;
        const auto& cond  = PARTNER_PARA.condition;

        const bool lowHP = stats.currentHP * 100 < stats.hp * 30;
        const bool lowMP = stats.currentMP * 100 < stats.mp * 30;

        if (lowHP || lowMP || cond.isSick || cond.isInjured) return 0;
        if (cond.isHungry || cond.isSleepy)                  return 2;
        return 0;
    }

    int32_t getVisibleRow(int32_t absoluteIdx)
    {
        for (int32_t i = 0; i < inv_filteredCount; i++)
            if (inv_filteredIdx[i] == absoluteIdx) return i;
        return -1;
    }

    void rebuildFiltered()
    {
        const bool inBattle = GAME_STATE >= 1 && GAME_STATE <= 3;

        // Sort buckets in one pass. The vanilla sort separates HP/MP heals (0)
        // from cure-status items (1) but we present them under one Heal tab.
        int8_t buckets[30];
        inv_categoriesPresent = 0;
        for (int32_t i = 0; i < INVENTORY_SIZE; i++)
        {
            auto t = INVENTORY_ITEM_TYPES[i];
            if (t == ItemType::NONE) { buckets[i] = -1; continue; }
            auto* item = getItem(t);
            // In battle, only items usable in battle (itemColor 1 or 2) are visible.
            if (inBattle && item->itemColor != 1 && item->itemColor != 2)
            {
                buckets[i] = -1;
                continue;
            }
            int32_t b = item->sortingValue;
            if (b == 1) b = 0;
            buckets[i] = static_cast<int8_t>(b);
            if (b >= 0 && b < 6) inv_categoriesPresent |= static_cast<uint8_t>(1 << b);
        }
        // ALL is overworld-only: in battle the unfiltered view would just
        // duplicate the per-category tabs.
        if (!inBattle && inv_categoriesPresent != 0)
            inv_categoriesPresent |= static_cast<uint8_t>(1 << ALL_CATEGORY);

        int32_t target  = inv_currentCategory;
        bool    isAll   = target == ALL_CATEGORY;
        if (inv_categoriesPresent != 0 && !isAll && (inv_categoriesPresent & (1 << target)) == 0)
        {
            target = firstPresentCategory(inv_categoriesPresent);
            isAll  = target == ALL_CATEGORY;
            inv_currentCategory = static_cast<uint8_t>(target);
        }

        inv_filteredCount = 0;
        for (int32_t i = 0; i < INVENTORY_SIZE; i++)
        {
            if (buckets[i] < 0) continue;
            if (isAll || buckets[i] == target)
                inv_filteredIdx[inv_filteredCount++] = static_cast<int8_t>(i);
        }
    }

    void compactInventory()
    {
        int32_t writeIdx = 0;
        for (int32_t readIdx = 0; readIdx < INVENTORY_SIZE; readIdx++)
        {
            if (INVENTORY_ITEM_TYPES[readIdx] == ItemType::NONE) continue;
            if (writeIdx != readIdx)
            {
                INVENTORY_ITEM_TYPES[writeIdx]   = INVENTORY_ITEM_TYPES[readIdx];
                INVENTORY_ITEM_AMOUNTS[writeIdx] = INVENTORY_ITEM_AMOUNTS[readIdx];
                INVENTORY_ITEM_NAMES[writeIdx]   = INVENTORY_ITEM_NAMES[readIdx];
                INVENTORY_ITEM_TYPES[readIdx]    = ItemType::NONE;
                INVENTORY_ITEM_AMOUNTS[readIdx]  = 0;
            }
            writeIdx++;
        }
    }

    // Vanilla-DW1-style tab: rounded-left + repeating-middle + angled-right corners
    // from menu tab textures (page 5, v=0x8C). Width must be 14 + multiple of 4.
    void renderTabBox(int16_t x, int16_t y, int16_t w, int16_t /*h*/, bool active, int32_t depth)
    {
        const int32_t  height = active ? 11 : 12;
        const uint16_t clut   = getClut(0x60, 0x1EC);
        // Active = full bright (flag 0). Inactive = extra-dim (flag 0x20) so the
        // selected tab's blue clearly stands out against the others.
        const char     flag   = active ? 0 : 0x20;
        renderRectPolyFT4(x, y, 7, height, 0xD4, 0x8C, 5, clut, depth, flag);
        const int32_t middleSlots = (w - 14) / 4;
        for (int32_t i = 0; i < middleSlots; i++)
            renderRectPolyFT4(x + 7 + i * 4, y, 4, height, 0xE2, 0x8C, 5, clut, depth, flag);
        renderRectPolyFT4(x + w - 9, y, 7, height, 0xDB, 0x8C, 5, clut, depth, flag);
    }

    void renderCategoryTabs(SimpleTextSprite* labels, int16_t tabY, int32_t depth,
                            int16_t* outActiveX, int16_t* outActiveW)
    {
        *outActiveX = 0;
        *outActiveW = 0;
        if (inv_categoriesPresent == 0) return;

        int16_t cursorX = -152;
        for (int32_t k = 0; k < 7; k++)
        {
            const int32_t i = TAB_ORDER[k];
            if (!(inv_categoriesPresent & (1 << i))) continue;

            const int16_t labelW  = static_cast<int16_t>(labels[i].getWidth());
            const int16_t middleW = static_cast<int16_t>(((labelW + 4) + 3) & ~3);
            const int16_t tabW    = static_cast<int16_t>(14 + middleW);
            const bool    active  = (i == static_cast<int32_t>(inv_currentCategory));

            renderTabBox(cursorX, tabY, tabW, 12, active, depth);

            const int16_t labelX = static_cast<int16_t>(cursorX + (tabW - labelW) / 2);
            // Active = mustard (#c8b432) on bright blue tab; inactive = mid-grey on dark tab.
            labels[i].render(labelX, static_cast<int16_t>(tabY + 2), active ? 4 : 0, depth - 1, true);

            if (active)
            {
                *outActiveX = cursorX;
                *outActiveW = tabW;
            }

            cursorX = static_cast<int16_t>(cursorX + tabW);
        }
    }
}
