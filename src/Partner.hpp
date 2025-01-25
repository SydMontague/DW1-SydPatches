#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    extern void createPoopPile(int16_t tileX, int16_t tileY);
    extern void handleConditionBubble();
    extern void handlePoopWeightLoss(DigimonType type);
    extern void handleWildPoop();
    extern void setFoodTimer(DigimonType type);
    extern void renderPoop(int32_t instanceId);
}