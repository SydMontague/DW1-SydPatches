#pragma once
#include "../Entity.hpp"

void VS__faintDigimon(DigimonEntity* entity, FighterData* fighter, int32_t playerId);

extern "C"
{
    void VS__setWalking(DigimonEntity* entity, Stats* stats, BattleFlags flags);
}
