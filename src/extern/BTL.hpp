#include "dw1.hpp"

extern "C"
{
    extern uint8_t BTL_EMBEDDED_TEXTURE1;
    extern uint8_t BTL_EMBEDDED_TEXTURE2;
    extern uint8_t BTL_CONFUSION_MODEL;
    extern uint8_t BTL_STUN_MODEL;
    extern uint8_t BTL_UNKNOWN_MODEL;
    extern dtl::array<uint8_t, 1024> BTL_END_BOX_TEXTBUFFER;

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
    void BTL_appendMoveLearnedText(int move);
    void BTL_tickBattleEndText();
    void BTL_renderBattleEndText(int32_t layer);
    void BTL_initializeBattleEndText(int32_t vOffset, int16_t unk2, RECT* pos);
    void BTL_appendItemDroppedText(Entity* entity);
    void BTL_appendInjuredText(uint8_t* entity);
    void BTL_appendCommandLearnedText();
    void BTL_appendMPBonusText();
    void BTL_battleTickFrame();
    void BTL_drawBattleEndText(bool isDrawing);
    bool BTL_isEndBoxTextFinished();
}