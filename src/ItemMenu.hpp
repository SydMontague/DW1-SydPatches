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
    void renderItemMenuSprite(int16_t depth, int32_t id, int16_t posX, int16_t posY);
    void renderItemMenuScrollBar(ItemMenuBox* menu);
    void renderItemMenuItemList(ItemMenuBox* menu,
                                int16_t stringX,
                                int16_t stringY,
                                int16_t spriteX,
                                int16_t spriteY,
                                int32_t spriteType);
    void playShopSoundOnlyInSavannah();
}

void renderCardSprite(uint8_t cardId, int16_t posX, int16_t posY, int32_t depth);
