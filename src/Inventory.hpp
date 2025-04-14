#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    uint8_t getItemCount(ItemType item);
    bool giveItem(ItemType type, uint8_t amount);
    void removeItem(ItemType type, int32_t amount);
}