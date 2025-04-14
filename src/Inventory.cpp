#include "Entity.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
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
}