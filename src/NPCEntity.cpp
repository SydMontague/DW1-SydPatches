#include "Entity.hpp"
#include "Model.hpp"
#include "extern/STD.hpp"
#include "extern/dw1.hpp"

extern "C"
{
    void loadNPCModel(DigimonType digimonId)
    {
        loadMMD(digimonId, EntityType::NPC);
    }

    void NPCEntity_tick(int32_t instanceId)
    {
        if (!ENTITY_TABLE.getEntityById(instanceId)->isOnMap) return;

        switch (GAME_STATE)
        {
            case 0: NPCEntity_tickOverworld(instanceId, &MAP_DIGIMON_TABLE[instanceId - 2]); break;
            case 1:
            case 2:
            case 3: NPCEntity_tickBattle(instanceId); break;
            case 4:
            case 5: STD_NPCEntity_tickTournament(instanceId); break;
        }
    }

    void scriptUnloadEntity(int32_t entityId)
    {
        uint8_t _entityId = entityId;
        auto* entity      = getEntityFromScriptId(&_entityId);
        entity->isOnMap   = false;
        removeEntity(entity->type, _entityId);
    }

    void clearMapAITable(int32_t entityId)
    {
        if (entityId != -1)
        {
            MAP_DIGIMON_TABLE[entityId].hasWaypointTarget = false;
            MAP_DIGIMON_TABLE[entityId].lookAtTamerState  = 0;
            MAP_DIGIMON_TABLE[entityId].activeSecton      = 0;
        }
        else
        {
            for (int32_t i = 0; i < 8; i++)
            {
                MAP_DIGIMON_TABLE[i].hasWaypointTarget = false;
                MAP_DIGIMON_TABLE[i].lookAtTamerState  = 0;
                MAP_DIGIMON_TABLE[i].activeSecton      = 0;
            }
        }
    }

    void removeMapEntities()
    {
        // vanilla writes a stack local int32_t[8] array here to -1, but it seems unused?

        for (int32_t i = 2; i < 10; i++)
        {
            auto* entity = ENTITY_TABLE.getEntityById(i);
            if (entity == nullptr) continue;

            removeEntity(entity->type, i);
            ENTITY_TABLE.setEntity(i, nullptr);
        }

        for (int32_t i = 0; i < 8; i++)
        {
            if (LOADED_DIGIMON_MODELS[i] == -1) continue;
            unloadModel(LOADED_DIGIMON_MODELS[i], EntityType::NPC);
        }

        initializeLoadedNPCModels();
        clearMapAITable(-1);
    }

    void unloadDigimonModel(DigimonType digimonType)
    {
        unloadModel(static_cast<int32_t>(digimonType), EntityType::NPC);
    }

    void clearMapDigimon()
    {
        for (int32_t i = 0; i < 8; i++)
        {
            for (int32_t j = 0; j < 8; j++)
            {
                MAP_DIGIMON_TABLE[i].aiSections[j] = -1;
                MAP_DIGIMON_TABLE[i].waypoints[j]  = {};
            }

            MAP_DIGIMON_TABLE[i].activeSecton      = 0;
            MAP_DIGIMON_TABLE[i].typeId            = DigimonType::INVALID;
            MAP_DIGIMON_TABLE[i].waypointWaitTimer = 0;
            MAP_DIGIMON_TABLE[i].cwDiff            = 0;
            MAP_DIGIMON_TABLE[i].ccDiff            = 0;
            MAP_DIGIMON_TABLE[i].targetAngle       = 0;
            MAP_DIGIMON_TABLE[i].hasWaypointTarget = 0;
            MAP_DIGIMON_TABLE[i].lookAtTamerState  = 0;
            NPC_ACTIVE_ANIM[i + 2]                 = 0;
        }
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