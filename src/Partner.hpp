#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    void initializeStatusObjects();
    void tickConditionBubble();
    void handlePoopWeightLoss(DigimonType type);
    void setFoodTimer(DigimonType type);
    void renderPoop(int32_t instanceId);
    void setupPartnerOnWarp(int32_t posX, int32_t posY, int32_t posZ, int32_t rotation);
    void tickConditionBoundaries();
    void handlePostBattleTiredness();
    void setPartnerIdling();
    void initializePartnerWaypoint();
    void Partner_tickCollision();
    void tickPartnerWaypoints();
    void Partner_setFullState(uint8_t state, uint8_t substate);
    void Partner_setState(uint8_t state);
    void Partner_setSubState(uint8_t state);
    uint8_t Partner_getState();
    uint8_t Partner_getSubState();
}