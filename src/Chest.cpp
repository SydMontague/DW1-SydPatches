#include "extern/dw1.hpp"

extern "C"
{
    void clearChests()
    {
        for (auto& chest : CHEST_ARRAY)
        {
            chest.item    = ItemType::NONE;
            chest.isTaken = false;
        }
    }
}