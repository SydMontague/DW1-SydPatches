#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    void initializeStatusObjects();
    void createPoopPile(int16_t tileX, int16_t tileY);
    void handleConditionBubble();
    void handlePoopWeightLoss(DigimonType type);
    void handleWildPoop();
    void setFoodTimer(DigimonType type);
    void renderPoop(int32_t instanceId);
    void setupPartnerOnWarp(int32_t posX, int32_t posY, int32_t posZ, int32_t rotation);
    void tickConditionBoundaries();
    void handlePostBattleTiredness();
    void setPartnerIdling();
    void initializePartnerWaypoint();
}