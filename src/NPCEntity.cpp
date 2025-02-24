#include "Entity.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "Tamer.hpp"
#include "extern/STD.hpp"
#include "extern/dw1.hpp"

extern "C"
{
    void loadNPCModel(DigimonType digimonId)
    {
        loadMMD(digimonId, EntityType::NPC);
    }

    bool isInTrackingRect(MapDigimonEntity* mapDigimon, Vector* location)
    {
        return isWithinRect(mapDigimon->posX, mapDigimon->posZ, mapDigimon->trackingRange, location);
    }

    bool isInTrackingRadius(Entity* entity, Entity* otherEntity, MapDigimonEntity* mapDigimon)
    {
        auto& ownPos   = entity->posData->location;
        auto& otherPos = otherEntity->posData->location;
        auto dist      = mapDigimon->trackingRange * 6 / 10;

        return ownPos.x - otherPos.x + ownPos.z - otherPos.z < (dist * dist);
    }

    void tickTrackingTamer2(MapDigimonEntity* mapDigimon,
                            Entity* entity,
                            Entity* otherEntity,
                            int32_t instanceId,
                            int32_t animId)
    {
        if (mapDigimon->lookAtTamerState == 0)
        {
            if (mapDigimon->animation != animId)
            {
                mapDigimon->animation = animId;
                startAnimation(entity, animId);
            }
            mapDigimon->lookAtTamerState = 1;
        }
        else
            tickTrackingTamer4(mapDigimon, entity, otherEntity, instanceId);
    }

    void tickTrackingTamer(MapDigimonEntity* mapDigimon, Entity* entity, Entity* otherEntity, int32_t instanceId)
    {
        if (mapDigimon->lookAtTamerState == 0)
        {
            mapDigimon->targetLocation = otherEntity->posData->location;
            mapDigimon->animation      = 2;
            startAnimation(entity, mapDigimon->animation);
            mapDigimon->lookAtTamerState = 1;
        }
        else
            tickTrackingTamer3(mapDigimon, entity, instanceId);
    }

    void tickLookingAtTamer(MapDigimonEntity* mapDigimon, Entity* entity, Entity* otherEntity)
    {
        constexpr auto ROTATION_SPEED = 113;

        if (mapDigimon->lookAtTamerState == 0)
        {
            mapDigimon->animation = 0;
            startAnimation(entity, mapDigimon->animation);
            mapDigimon->lookAtTamerState = 1;
        }
        else
        {
            // vanilla uses the respective fields of the MapDigimonEntity, but since those are always overwritten before
            // used we can get rid of this state in favor of local variables
            int16_t targetAngle;
            int16_t ccDiff;
            int16_t cwDiff;

            getRotationDifference(entity->posData, &otherEntity->posData->location, &targetAngle, &ccDiff, &cwDiff);
            rotateEntity(&entity->posData->rotation, &targetAngle, &ccDiff, &cwDiff, ROTATION_SPEED);

            if (!isInTrackingRect(mapDigimon, &otherEntity->posData->location))
            {
                mapDigimon->lookAtTamerState  = 0;
                mapDigimon->hasWaypointTarget = false;
            }
        }
    }

    void tickWaypointAI(MapDigimonEntity* mapDigimon, Entity* entity, uint32_t entityId)
    {
        auto speed = mapDigimon->aiSections[mapDigimon->activeSecton];

        if (speed == 0)
            tickWaypointWait(mapDigimon, entity);
        else if (speed == 1)
            tickWaypointWalk(mapDigimon, entity, 2, entityId);
        else if (speed == 2)
            tickWaypointWalk(mapDigimon, entity, 4, entityId);

        if (mapDigimon->activeSecton >= 8 || mapDigimon->aiSections[mapDigimon->activeSecton] == -1)
            mapDigimon->activeSecton = 0;
    }

    void NPCEntity_tickOverworld(int32_t instanceId, MapDigimonEntity* mapDigimon)
    {
        NPCEntity* entity = reinterpret_cast<NPCEntity*>(ENTITY_TABLE.getEntityById(instanceId));
        if (entity == nullptr) return;
        if (IS_IN_MENU == 1) return;

        if (NPC_ACTIVE_ANIM[instanceId] != 0) {}
        else if (mapDigimon->stopAnim)
        {
            if (mapDigimon->animation != 0)
            {
                mapDigimon->animation = 0;
                startAnimation(entity, mapDigimon->animation);
            }
        }
        else
        {
            if (mapDigimon->lookAtTamerState == 0) tickWaypointAI(mapDigimon, entity, instanceId);

            auto inTrackingRange = isInTrackingRect(mapDigimon, &TAMER_ENTITY.posData->location);
            auto inSomeRange     = isInTrackingRadius(entity, &TAMER_ENTITY, mapDigimon);

            switch (mapDigimon->followMode)
            {
                case 2:
                case 11:
                    if (inTrackingRange || mapDigimon->lookAtTamerState != 0)
                        tickLookingAtTamer(mapDigimon, entity, &TAMER_ENTITY);
                    break;
                case 3:
                case 4:
                case 5:
                case 12:
                case 13:
                case 14:
                    if (inTrackingRange || mapDigimon->lookAtTamerState != 0)
                        tickTrackingTamer(mapDigimon, entity, &TAMER_ENTITY, instanceId);
                    break;
                case 6:
                case 15:
                    if (inSomeRange || mapDigimon->lookAtTamerState != 0)
                        tickTrackingTamer2(mapDigimon, entity, &TAMER_ENTITY, instanceId, 2);
                    break;
                case 7:
                case 16:
                    if (inSomeRange || mapDigimon->lookAtTamerState != 0)
                        tickTrackingTamer2(mapDigimon, entity, &TAMER_ENTITY, instanceId, 4);
                    break;
                case 8:
                case 9:
                case 17:
                case 18:
                    if (inTrackingRange || mapDigimon->lookAtTamerState != 0)
                        tickTrackingTamer2(mapDigimon, entity, &TAMER_ENTITY, instanceId, 2);
                    break;
            }

            auto collisionResult                = entityCheckCollision(nullptr, entity, 0, 0);
            NPC_COLLISION_STATE[instanceId - 2] = collisionResult;
            if (collisionResult == CollisionCode::TAMER && Tamer_getState() == 0 &&
                NPC_ENTITIES[instanceId - 2].autotalk == 1)
            {
                entity->animFlag |= 2;
                if (IS_SCRIPT_PAUSED == 1)
                {
                    removeTriangleMenu();
                    closeInventoryBoxes();
                    removeUIBox1();
                    TALKED_TO_ENTITY = instanceId;
                    callScriptSection(CURRENT_SCRIPT_ID, entity->scriptId, 1);
                }
            }
            if (collisionResult != CollisionCode::NONE && NPC_IS_WALKING_TOWARDS[instanceId - 2] == 0 &&
                entity->animId > 1 && entity->animId < 5)
                collisionGrace(0, entity, 0, 0);
        }

        entity->isOnScreen = !entityIsOffScreen(entity, 320, 240);
        tickAnimation(entity);
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