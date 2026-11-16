#pragma once
#include "extern/stddef.hpp"

extern "C"
{
    void loadWarpCrystals(int32_t mapId);
    void loadTrainingPoop();
    void loadDoors(int32_t doorEntryId);
    bool tickOpenChestTray(uint32_t chestId);
    bool tickCloseChestTray(uint32_t chestId);
    void initializeChest();
    bool moveAngemonPedestal();
    void decreaseDirtPileSize();
    void moveBoulder(int32_t diffX, int32_t diffZ);
    void openToyTownBox(int8_t boxId);
    void somethingToyTown(int8_t boxId);
    void setDirtCartModel(int32_t model);
    void loadDirtPileModel();
    void loadDirtCartModel();
    void spawnGearbox();
    void spawnToyTownBoxes();
    void spawnAngemonPedestal();
}
