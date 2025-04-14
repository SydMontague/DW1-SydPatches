#pragma once
#include "extern/dw1.hpp"
#include "extern/libgte.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    int16_t getMapRotation();

    /*
     * Gets the trigger set for a tile.
     * If the tile is impassable -1 is returned.
     * If the tile is passable, but has not trigger, 0 is returned.
     */
    int32_t getTileTrigger(Vector* position);

    void renderOverworldItem(WorldItem* item);
}