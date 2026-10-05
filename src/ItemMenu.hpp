#pragma once

#include "extern/dw1.hpp"

extern "C"
{
    bool isItemMenuBoxBusy(ItemMenuBox* box);
    ItemMenuBox* getItemMenuFromType(void);
    void itemMenuCursorTop(ItemMenuBox* menu, int32_t count, int32_t mode);
    void itemMenuCursorBottom(ItemMenuBox* menu, int32_t count, int32_t mode);
    void itemMenuCursorUp(ItemMenuBox* menu, int32_t mode);
    void itemMenuCursorDown(ItemMenuBox* menu, int32_t mode);
}
