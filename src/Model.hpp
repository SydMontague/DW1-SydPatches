#include "extern/dw1.hpp"

extern "C"
{
    extern void initializeDigimonObject(DigimonType type, int32_t instanceId, TickFunction tick);
    extern void setupEntityMatrix(int32_t entityId);
    extern void setEntityPosition(int32_t entityId, int32_t posX, int32_t posY, int32_t posZ);
    extern void setEntityRotation(int32_t entityId, int32_t rotX, int32_t rotY, int32_t rotZ);
    extern void loadMMD(DigimonType digimonType, EntityType entityType);
    extern void renderWireframed(GsDOBJ2* obj, int32_t wireFrameShare);
}