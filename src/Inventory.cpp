#include "Entity.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    void setInventorySize(uint8_t newSize)
    {
        INVENTORY_SIZE = newSize;
    }

    uint8_t getItemCount(ItemType item)
    {
        for (int32_t i = 0; i < INVENTORY_SIZE; i++)
        {
            if (INVENTORY_ITEM_TYPES[i] == item) return INVENTORY_ITEM_AMOUNTS[i];
        }

        return 0;
    }

    bool giveItem(ItemType type, uint8_t amount)
    {
        for (int32_t i = 0; i < INVENTORY_SIZE; i++)
        {
            if (INVENTORY_ITEM_TYPES[i] == type)
            {
                auto current = INVENTORY_ITEM_AMOUNTS[i];
                if (current == 99) return false;

                INVENTORY_ITEM_AMOUNTS[i] = min(current + amount, 99);
                return true;
            }
        }

        for (int32_t i = 0; i < INVENTORY_SIZE; i++)
        {
            if (INVENTORY_ITEM_TYPES[i] != ItemType::NONE) continue;

            INVENTORY_ITEM_TYPES[i]   = type;
            INVENTORY_ITEM_AMOUNTS[i] = amount;
            // vanilla sets the INVENTORY_ITEM_NAMES field, but this variable is related to drawing the inventory text,
            // which did not happen yet. So it should be useless and thus can be omitted.

            return true;
        }

        return false;
    }

    void removeItem(ItemType type, int32_t amount)
    {
        if (type == ItemType::NONE) return;

        for (int32_t i = 0; i < INVENTORY_SIZE; i++)
        {
            if (INVENTORY_ITEM_TYPES[i] != type) continue;

            INVENTORY_ITEM_AMOUNTS[i] = max(INVENTORY_ITEM_AMOUNTS[i] - amount, 0);
            if (INVENTORY_ITEM_AMOUNTS[i] == 0)
            {
                INVENTORY_ITEM_TYPES[i] = ItemType::NONE;
                INVENTORY_ITEM_NAMES[i] = 0xFF;
            }
        }
    }
}