#pragma once
#include "dw1.hpp"
#include "libgte.hpp"

extern "C"
{
    extern Vector INTRO_TARGET_POS;
    extern dtl::array<dtl::array<uint8_t, 14>, 116> VS__INTRO_DIGIMON_NAMES;

    void VS__setupIntroCamera(Entity* entity, int32_t distance, int32_t spot);
    void VS__tickFrame();
    void VS__addTimer(int32_t seconds);
    void VS__addHPMPBar(int32_t player);
    void VS__addCommandBar(int32_t player);
    void VS__initializeEFEEngine(void*);
    void VS__loadMoveEFE(int16_t* moves, int16_t* effectIds, int8_t* isLoaded);
    void VS__addBattleTextIn();
    void VS__addBattleTextOut();
    void VS__removeBattleTextIn();
    void VS__removeBattleTextOut();
    int32_t VS__isBattleTextFinished();
    void VS__tickBattleResultScreen(int32_t hasLostP1, int32_t hasLostP2);
    void VS__trySetAttackerCamera(DigimonEntity* entity, int32_t random, int32_t range);
    void VS__setMoveAnim(DigimonEntity* entity, FighterData* fighter, int32_t playerId, uint8_t moveSlot);
    void VS__setChargeupFlag(DigimonEntity* entity, FighterData* fighter, uint8_t move);
    bool VS__canAffordMove(DigimonEntity* entity, FighterData* fighter, int32_t moveSlot);
    int32_t VS__getMoveCommandModerate(int32_t playerId, uint16_t* data);
    int32_t VS__getMoveCommandAttack(int32_t playerId, uint16_t* data);
    int32_t VS__getMoveCommandYourCall(DigimonEntity* entity, FighterData* fighter, int32_t playerId);
    void VS__handleConfusion(DigimonEntity* entity, FighterData* fighter, int32_t playerId);
    void VS__removeAllStatusFX(DigimonEntity* entity, FighterData* fighter);
}
