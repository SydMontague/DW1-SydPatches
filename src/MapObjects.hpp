#pragma once
#include "extern/stddef.hpp"

extern "C"
{
    void loadWarpCrystals(int32_t mapId);
    void loadTrainingPoop();
    void loadDoors(int32_t doorEntryId);
    bool tickOpenChestTray(uint32_t chestId);
    bool tickCloseChestTray(uint32_t chestId);
}