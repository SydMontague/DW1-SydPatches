#include "extern/dw1.hpp"

extern "C"
{
    extern void initializeDigimonObject(DigimonType type, int32_t instanceId, TickFunction tick);
    extern void setupModelMatrix(PositionData* data);
    extern void setEntityPosition(int32_t entityId, int32_t posX, int32_t posY, int32_t posZ);
}