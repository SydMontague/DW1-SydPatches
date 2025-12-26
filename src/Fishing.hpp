#pragma once
#include "extern/stddef.hpp"

extern "C"
{
    void setFishingDisabled();
    void setFishingEnabled();
    bool hasFishingRod();
    void initializeFishing();
    bool isFishing();
    void deinitializeFishing();
    void checkFishingMap(int32_t mapId);
}