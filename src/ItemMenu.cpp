#include "Helper.hpp"
#include "Inventory.hpp"
#include "extern/dw1.hpp"

namespace
{
    constexpr auto BUYABLE_TRIGGER_START = 0x180;
}

extern "C"
{
    void shopFillBuyItemList()
    {
        auto size                 = INVENTORY_SIZE;
        bool hasSpace             = false;
        auto itemList             = reinterpret_cast<ItemMenuBuyEntry*>(ITEM_MENU_LEFT->itemList);
        ITEM_MENU_LEFT->itemCount = 0;

        for (auto i = 0; i < INVENTORY_SIZE; i++) {
            if (INVENTORY_ITEM_TYPES[i] == ItemType::NONE) {
                hasSpace = true;
                break;
            }
        }

        // TODO use reflections once possible
        for (auto i = 0; i < 128; i++) {
            auto itemType = static_cast<ItemType>(i);

            if (!isTriggerSet(BUYABLE_TRIGGER_START + i)) continue;

            ITEM_MENU_LEFT->itemCount++;
            itemList->itemType = i;
            itemList->buyable  = true;

            if (MONEY < getItem(itemType)->value)
                itemList->buyable = false;
            else {
                auto itemCount = getItemCount(itemType);
                if (itemCount == 99 || (!hasSpace && itemCount == 0)) itemList->buyable = false;
            }

            itemList++;
        }
    }

    bool shopFillSellItemList()
    {
        auto canSellAnything       = false;
        auto itemList              = reinterpret_cast<ItemMenuSellEntry*>(ITEM_MENU_RIGHT->itemList);
        ITEM_MENU_RIGHT->itemCount = 0;

        for (auto i = 0; i < INVENTORY_SIZE; i++) {
            auto type   = INVENTORY_ITEM_TYPES[i];
            auto amount = INVENTORY_ITEM_AMOUNTS[i];

            if (type != ItemType::NONE && getItem(type)->dropable) {
                amount |= 0x80;
                canSellAnything = true;
            }

            ITEM_MENU_RIGHT->itemCount++;
            itemList->itemType = static_cast<uint8_t>(type);
            itemList->amount   = amount;
            itemList++;
        }

        return canSellAnything;
    }
}
