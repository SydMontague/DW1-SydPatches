#pragma once
#include "UIElements.hpp"
#include "extern/dw1.hpp"

extern "C"
{
    extern uint8_t inv_currentCategory;
    extern uint8_t inv_categoriesPresent;
    extern int32_t inv_filteredCount;
    extern int8_t  inv_filteredIdx[30];

    int32_t firstPresentCategory(uint8_t mask);
    int32_t shiftCategory(int32_t from, uint8_t mask, bool forward);
    int32_t pickSmartDefault();
    int32_t getVisibleRow(int32_t absoluteIdx);
    void rebuildFiltered();
    void compactInventory();
    void renderTabBox(int16_t x, int16_t y, int16_t w, int16_t h, bool active, int32_t depth);
    void renderCategoryTabs(SimpleTextSprite* labels, int16_t tabY, int32_t depth,
                            int16_t* outActiveX, int16_t* outActiveW);
}
