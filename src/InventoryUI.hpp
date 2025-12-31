#pragma once
#include "extern/dw1.hpp"

extern "C"
{
    void addInventoryUI();
    void closeInventoryBoxes();
    void renderItemSprite(ItemType type, int16_t posX, int16_t posY, int32_t depth);
    void setItemTexture(POLY_FT4* prim, ItemType item);
}