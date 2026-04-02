#pragma once
#include "extern/dw1.hpp"

extern "C"
{
    void handleBattleIdle(DigimonEntity* entity, Stats* stats, BattleFlags flags);
    void startBattleIdleAnimation(Entity* entity, Stats* stats, BattleFlags flags);
    void battleStatsGainsAndDrops(ItemType* droppedItems);
    void handleBattleInjury();
    void battleMoveLearning();
}