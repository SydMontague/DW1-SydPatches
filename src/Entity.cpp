#include "Entity.hpp"

#include "Helper.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

Pair<Entity*, uint8_t> getEntityFromScriptId(uint8_t entityId)
{
    // vanilla uses an output parameter instead of returing a pair
    if (entityId == 0xFD) return {&TAMER_ENTITY, 0};
    if (entityId == 0xFC) return {&PARTNER_ENTITY, 1};

    for (int32_t i = 0; i < 8; i++)
    {
        auto* entity = &NPC_ENTITIES[i];
        if (entity->scriptId == entityId) return {entity, static_cast<uint8_t>(i + 2)};
    }

    return {nullptr, entityId};
}

extern "C"
{
    // vanilla only allocates 8 vectors here, but there are 10 NPCs...
    static bool hasRotationData[10];
    static Vector rotationData[10];

    static int16_t moveToDeltaX;
    static int16_t moveToDeltaZ;
    static Vector previousCameraPos;
    static bool previousCameraPosInitialized;

    bool tickEntityMoveToAxis(uint32_t scriptId, int32_t target, int32_t axis, int32_t speed, bool withCamera)
    {
        auto entity = getEntityFromScriptId(scriptId);

        // vanilla doesn't do a nullptr check here, but you can't move an entity that doesn't exist
        if (entity.first == nullptr) return true;

        int32_t* axisValue;
        if (axis == 0)
            axisValue = &entity.first->posData->location.x;
        else if (axis == 1)
            axisValue = &entity.first->posData->location.y;
        else
            axisValue = &entity.first->posData->location.z;

        if (!previousCameraPosInitialized)
        {
            moveToDeltaX                 = (target - *axisValue) / speed;
            previousCameraPosInitialized = true;
            previousCameraPos            = entity.first->posData->location;
        }
        else
        {
            *axisValue += moveToDeltaX;
            setEntityPosition(entity.second,
                              entity.first->posData->location.x,
                              entity.first->posData->location.y,
                              entity.first->posData->location.z);
            setupEntityMatrix(entity.second);

            // in vanilla, when the DELTA speed is 0 this function may never return true, as the entity never reaches
            // the target location. If the game waits for the movement to complete, this will cause a softlock.
            // We solve this by treating DELTA speeds of 0 as always completed
            bool reachedTarget = moveToDeltaX == 0;
            if (moveToDeltaX < 0)
                reachedTarget = *axisValue <= target;
            else if (moveToDeltaX > 0)
                reachedTarget = *axisValue >= target;

            if (reachedTarget)
            {
                *axisValue                   = target;
                previousCameraPosInitialized = false;
                return true;
            }
        }

        if (withCamera)
        {
            moveCameraByDiff(&previousCameraPos, &entity.first->posData->location);
            previousCameraPos = entity.first->posData->location;
        }

        return false;
    }

    bool tickEntityMoveTo(uint32_t scriptId1,
                          uint32_t scriptId2,
                          int32_t targetX,
                          int32_t targetZ,
                          int8_t speed,
                          bool withCamera)
    {
        auto entity1 = getEntityFromScriptId(scriptId1);

        // vanilla doesn't do a nullptr check here, but you can't move an entity that doesn't exist
        if (entity1.first == nullptr) return true;

        if (!previousCameraPosInitialized)
        {
            if (scriptId2 == 0xFF)
            {
                moveToDeltaX = (targetX - entity1.first->posData->location.x) / speed;
                moveToDeltaZ = (targetZ - entity1.first->posData->location.z) / speed;
            }
            else
            {
                auto entity2 = getEntityFromScriptId(scriptId2);
                // vanilla doesn't do a nullptr check here, but you can't move to an entity that doesn't exist
                if (entity2.first == nullptr) return true;
                moveToDeltaX = (entity2.first->posData->location.x - entity1.first->posData->location.x) / speed;
                moveToDeltaZ = (entity2.first->posData->location.z - entity1.first->posData->location.z) / speed;
            }

            previousCameraPosInitialized = true;
            previousCameraPos            = entity1.first->posData->location;
        }
        else
        {
            setEntityPosition(entity1.second,
                              entity1.first->posData->location.x + moveToDeltaX,
                              entity1.first->posData->location.y,
                              entity1.first->posData->location.z + moveToDeltaZ);
            setupEntityMatrix(entity1.second);

            // in vanilla, when the DELTA speed is 0 this function may never return true, as the entity never reaches
            // the target location. If the game waits for the movement to complete, this will cause a softlock.
            // We solve this by treating DELTA speeds of 0 as always completed
            bool finishedX = moveToDeltaX == 0;
            if (moveToDeltaX < 0) { finishedX = entity1.first->posData->location.x <= targetX; }
            else if (moveToDeltaX > 0) { finishedX = entity1.first->posData->location.x >= targetX; }

            bool finishedZ = moveToDeltaZ == 0;
            if (moveToDeltaZ < 0) { finishedZ = entity1.first->posData->location.z <= targetZ; }
            else if (moveToDeltaZ > 0) { finishedZ = entity1.first->posData->location.z >= targetZ; }

            if (finishedX) entity1.first->posData->location.x = targetX;
            if (finishedZ) entity1.first->posData->location.z = targetZ;
            if (finishedX && finishedZ)
            {
                previousCameraPosInitialized = false;
                return true;
            }
        }

        if (withCamera)
        {
            moveCameraByDiff(&previousCameraPos, &entity1.first->posData->location);
            previousCameraPos = entity1.first->posData->location;
        }
        return false;
    }

    bool tickEntitySetRotation(uint32_t scriptId, int16_t angle)
    {
        auto* entity = getEntityFromScriptId(scriptId).first;
        if (entity != nullptr) entity->posData->rotation.y = angle;

        return true;
    }

    bool tickLookAtEntity(uint32_t scriptId1, uint32_t scriptId2)
    {
        auto entity1 = getEntityFromScriptId(scriptId1);
        auto entity2 = getEntityFromScriptId(scriptId2);

        // vanilla doesn't do a nullptr check here, but unchecked access is iffy
        if (entity1.first == nullptr || entity2.first == nullptr) return true;

        if (hasRotationData[entity1.second] == 0)
        {
            rotationData[entity1.second]    = entity2.first->posData->location;
            hasRotationData[entity1.second] = 1;
        }
        else
        {
            int16_t targetAngle;
            int16_t cwDiff;
            int16_t ccDiff;
            getRotationDifference(entity1.first->posData,
                                  &rotationData[entity1.second],
                                  &targetAngle,
                                  &ccDiff,
                                  &cwDiff);

            if (rotateEntity(&entity1.first->posData->rotation, targetAngle, ccDiff, cwDiff, 0x200))
            {
                hasRotationData[entity1.second] = 0;
                return true;
            }
        }

        return false;
    }

    bool tickEntityWalkTo(uint32_t scriptId1, uint32_t scriptId2, int32_t targetX, int32_t targetZ, bool withCamera)
    {
        auto entity1 = getEntityFromScriptId(scriptId1);
        auto entity2 = getEntityFromScriptId(scriptId2);

        // vanilla doesn't do a nullptr check here, but unchecked access is iffy
        if (entity1.first == nullptr) return true;

        Vector currentPos = entity1.first->posData->location;
        Vector targetPos  = {.x = targetX, .y = currentPos.y, .z = targetZ};

        if (entity2.first != nullptr) targetPos = entity2.first->posData->location;

        if (entity1.second >= 2) NPC_IS_WALKING_TOWARDS[entity1.second - 2] = 1;

        if (!previousCameraPosInitialized && withCamera)
        {
            previousCameraPos            = currentPos;
            previousCameraPosInitialized = true;
        }

        entityLookAtLocation(entity1.first, &targetPos);

        if (withCamera)
        {
            moveCameraByDiff(&previousCameraPos, &currentPos);
            previousCameraPos = currentPos;
        }

        CollisionCode collision = CollisionCode::NONE;
        if (entity2.first != nullptr) { collision = entityCheckCollision(nullptr, entity1.first, 0, 0); }

        // In vanilla a movement is complete once the entity location is on the same tile as the target location.
        // However, in some constellations this might never happen and cause softlocks.
        // One common example is the left door in the third set of doors in Toy Town, where the target location is near
        // the edge of the tile, causing the player to constantly walk back and forth without ever finishing.
        // Additionally the collision check did ignore NPC8
        auto distance = getDistanceSquared(&currentPos, &targetPos);
        auto radius   = getDigimonData(entity1.first->type)->radius;

        if (distance < radius * radius || (collision != CollisionCode::NONE && collision < CollisionCode::MAP))
        {
            previousCameraPosInitialized = false;
            if (entity1.second >= 2) NPC_IS_WALKING_TOWARDS[entity1.second - 2] = 0;
            return true;
        }

        return false;
    }

    bool hasAttackEquipped(DigimonEntity* entity)
    {
        for (int32_t i = 0; i < 3; i++)
        {
            auto move = entity->stats.moves[i];
            auto tech = entityGetTechFromAnim(entity, move);
            if (move != 0xFF && tech != 0xFF && MOVE_DATA[tech].power != 0) return true;
        }

        return false;
    }
}