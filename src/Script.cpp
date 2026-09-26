
#include "Inventory.hpp"
#include "Math.hpp"
#include "Tournament.hpp"
#include "extern/dtl/vector.hpp"
#include "extern/dw1.hpp"

extern "C"
{
    int32_t getScriptSyncBit()
    {
        return SOME_SCRIPT_SYNC_BIT;
    }

    void scriptStartTournament()
    {
        if (SCRIPT_STATE_2 == 3) ACTIVE_INSTRUCTION = 0;
        if (SCRIPT_STATE_2 == 2) {
            closeBox(0);
            startTournament();
            ACTIVE_INSTRUCTION = 0;
        }
        if (SCRIPT_STATE_2 == 1) return;
        if (SCRIPT_STATE_2 == 0) SCRIPT_STATE_2 = 2;
    }

    void checkTournamentMedalConditions()
    {
        // in vanilla this is leftover code that is duplicated elsewhere
        // removing this fixes the ability to get the 100 tournament won medal by winning just 100 battles
        return;
    }

    void scriptCheckTournamentMedal()
    {
        ACTIVE_INSTRUCTION = 0;
        // in vanilla this is leftover code that is duplicated elsewhere
        // removing this fixes the ability to get the 100 tournament won medal by winning just 100 battles
        return;
    }

    void handleItemLoss()
    {
        dtl::vector<int32_t> validSlots;
        validSlots.reserve(INVENTORY_SIZE);
        for (auto i = 0; i < INVENTORY_SIZE; i++) {
            if (INVENTORY_ITEM_TYPES[i] != ItemType::NONE) {
                validSlots.push_back(i);
            }
        }

        auto itemCount = validSlots.size() * 30 / 100;
        writePStat(200, itemCount); // TODO: is this ever actually used?

        for (auto i = 0; i < itemCount; i++) {
            // vanilla counts the number of taken item slots here to determine the upper limit, which means that after
            // the first iteration the last items in the bag can't be lost anymore
            // instead we're always using the full range of viable items
            // TODO: if a slot is rolled twice, one item fewer will be lost, do we want to keep it that way?
            auto roll      = random(validSlots.size());
            auto type      = INVENTORY_ITEM_TYPES[roll];
            auto recycleId = getRecycleId(type);
            if (recycleId != 0xFF) {
                auto& amount = GAME_STATE_PTR->recycleItems[recycleId];
                amount += INVENTORY_ITEM_AMOUNTS[roll];
                if (amount > 99) amount = 99;
            }
            removeItem(type, 99);
        }
    }
}
