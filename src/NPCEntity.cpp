#include "Model.hpp"
#include "extern/dw1.hpp"

extern "C"
{
    void loadNPCModel(DigimonType digimonId)
    {
        loadMMD(digimonId, EntityType::NPC);
    }

    bool scriptSetDigimon(DigimonType digimonId, int32_t actorId, uint8_t autoTalk)
    {
        auto& mapEntity = MAP_DIGIMON_TABLE[actorId];

        if (mapEntity.typeId != digimonId) return false;

        auto entityId = actorId + 2;
        if (ENTITY_TABLE.getEntityById(entityId) != nullptr)
            removeEntity(ENTITY_TABLE.getEntityById(entityId)->type, entityId);

        ENTITY_TABLE.setEntity(entityId, &NPC_ENTITIES[actorId]);

        auto entity = ENTITY_TABLE.getEntityById(entityId);

        initializeDigimonObject(digimonId, entityId, NPCEntity_tick);
        setEntityPosition(entityId, mapEntity.posX, mapEntity.posY, mapEntity.posZ);
        setEntityRotation(entityId, mapEntity.rotX, mapEntity.rotY, mapEntity.rotZ);
        setupEntityMatrix(entityId);
        startAnimation(ENTITY_TABLE.getEntityById(entityId), mapEntity.animation);

        NPC_ENTITIES[actorId].autotalk  = autoTalk;
        NPC_IS_WALKING_TOWARDS[actorId] = 0;
        entity->isOnMap                 = true;
        entity->isOnScreen              = !entityIsOffScreen(entity, 320, 240);
        return true;
    }
}