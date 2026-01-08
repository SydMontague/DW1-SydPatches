#include "dw1.hpp"

extern "C"
{
    extern uint8_t BTL_EMBEDDED_TEXTURE1;
    extern uint8_t BTL_EMBEDDED_TEXTURE2;
    extern uint8_t BTL_CONFUSION_MODEL;
    extern uint8_t BTL_STUN_MODEL;
    extern uint8_t BTL_UNKNOWN_MODEL;

    BattleResult BTL_battleMain();
    void BTL_healStatusEffect(bool isVarious);
    void BTL_removeDeathCountdown();

    void BTL_initializeBattleItemParticles();
    void BTL_initializeUnk3();
    void BTL_initializeUnk2();
    void BTL_initializePoisonBubble();
    void BTL_loadEmbeddedTextures(void*, void*);
    void BTL_initializeConfusionEffect(void*);
    void BTL_initializeStunEffect(void*);
    void BTL_initializeEFEEngine(void*);
    void BTL_loadMoveEFE(int16_t* moves, int16_t* effectIds, int8_t* isLoaded);
}