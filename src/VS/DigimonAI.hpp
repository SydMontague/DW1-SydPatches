#pragma once
#include "../Entity.hpp"

void VS__faintDigimon(DigimonEntity* entity, FighterData* fighter, int32_t playerId);

extern "C"
{
    void VS__tickDigimonRotateKeepDistance(DigimonEntity* entity, DigimonEntity* other, FighterData* data);
    void VS__setWalking(DigimonEntity* entity, Stats* stats, BattleFlags flags);
    void VS__tickDigimonAttackLookAtTarget(DigimonEntity* entity, Vector* location, int dx, int dy);
}
