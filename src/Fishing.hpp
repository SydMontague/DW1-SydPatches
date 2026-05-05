#pragma once
#include "extern/stddef.hpp"

extern "C"
{
    void setFishingDisabled();
    void setFishingEnabled();
    uint32_t hasFishingRod();
    void initializeFishing();
    bool isFishing();
    void deinitializeFishing();
    void checkFishingMap(int32_t mapId);
}