#pragma once
#include "Helper.hpp"
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
    bool pickupItem(int32_t dropId);
    bool isFiveTileWidePathBlocked(int32_t tileX1, int32_t tileY1, int32_t tileX2, int32_t tileY2);
    bool isTileOffScreen(int16_t tileX, int16_t tileZ);
    TileCoord getClosestTileOffScreen(int8_t startX, int8_t startY, int8_t targetX, int8_t targetY);
    void readMapTFS(uint8_t map);
    void updateTimeOfDay();
    bool isInDaytimeTransition();
    void initializeDaytimeTransition(int32_t mode);
    void uploadMapTileImages(MapTileData* tileData, int32_t tileOffset);
    void handleTileUpdate(uint32_t input, bool updateAll);
    void addMapNameObject(int32_t mapId);
    void changeMap(uint32_t map, uint32_t exit);
    void renderMap(int32_t instanceId);
}