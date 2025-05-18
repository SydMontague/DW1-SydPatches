#include "extern/dw1.hpp"

extern "C"
{
    void initializeDigimonObject(DigimonType type, int32_t instanceId, TickFunction tick);
    void setupEntityMatrix(int32_t entityId);
    void setEntityPosition(int32_t entityId, int32_t posX, int32_t posY, int32_t posZ);
    void setEntityRotation(int32_t entityId, int32_t rotX, int32_t rotY, int32_t rotZ);
    void loadMMD(DigimonType digimonType, EntityType entityType);
    void renderWireframed(GsDOBJ2* obj, int32_t wireFrameShare);
    ModelComponent* getEntityModelComponent(int32_t instance, EntityType entityType);
    EntityType getEntityTypeById(int32_t entityId);
    EntityType getEntityType(Entity* entity);
    void applyMMD(DigimonType digimonType, EntityType entityType, EvoModelData* modelData);
    void removeEntity(DigimonType type, int32_t instanceId);
    void unloadModel(int32_t id, EntityType type);
    void setupModelMatrix(PositionData* data);
    void startAnimation(Entity*, uint32_t animId);
    void tickAnimation(Entity* entity);
}