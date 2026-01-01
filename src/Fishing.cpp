
#include "Files.hpp"
#include "GameTime.hpp"
#include "Helper.hpp"
#include "extern/FISH.hpp"
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

enum class FishingRodType
{
    NONE,
    OLD_ROD,
    GOOD_ROD,
};

extern "C"
{
    FishingRodType getBestFishingRod()
    {
        if (isTriggerSet(46)) return FishingRodType::GOOD_ROD;
        if (isTriggerSet(45)) return FishingRodType::OLD_ROD;

        return FishingRodType::NONE;
    }

    bool hasFoodItem()
    {
        for (auto& type : INVENTORY_ITEM_TYPES)
        {
            if (type == ItemType::NONE) continue;
            if (getItem(type)->sortingValue == 2) return true;
        }
        return false;
    }

    void setFishingDisabled()
    {
        if (FISHING_DATA_PTR == nullptr) return;
        FISHING_DATA_PTR->fishingEnabled = 1;
    }

    void setFishingEnabled()
    {
        if (FISHING_DATA_PTR == nullptr) return;
        FISHING_DATA_PTR->fishingEnabled = 0;
    }

    uint32_t hasFishingRod()
    {
        if (FISHING_DATA_PTR == nullptr) return 0;
        if (FISHING_DATA_PTR->fishingEnabled != 0) return 0;
        if (getBestFishingRod() == FishingRodType::NONE || !hasFoodItem()) return 1;

        return 2;
    }

    void initializeFishing()
    {
        FISH_loadFishing(&FISHING_DATA_PTR->rod);
    }

    bool isFishing()
    {
        if (FISHING_DATA_PTR == nullptr) return false;
        if (FISHING_DATA_PTR->rod.fishingState == 0) return false;

        return true;
    }

    void deinitializeFishing()
    {
        if (FISHING_DATA_PTR == nullptr) return;

        FISH_deinitialize();
        FISHING_DATA_PTR = nullptr;
    }

    void checkFishingMap(int32_t mapId)
    {
        deinitializeFishing();
        if (mapId == 6 || mapId == 8)
        {
            loadDynamicLibrary(Overlay::FISH_REL, nullptr, false, nullptr, 0);

            FISHING_DATA_PTR = &FISHING_DATA;
            bzero(&FISHING_DATA, sizeof(FISHING_DATA));
            FISHING_DATA_PTR->header  = 0x55dddd55;
            FISHING_DATA_PTR->trailer = 0x55dddd55;
            FISHING_DATA_PTR->mapId   = mapId;
            for (auto& mapData : FISHING_MAP_DATA)
            {
                if (mapId != mapData.mapId) continue;
                FISHING_DATA_PTR->mapDataPtr = &mapData;
                break;
            }

            FISH_init();
            // vanilla calls hasFishingRod here, but the return value is unused and the function has no side effects
        }
        startGameTime();
    }
}