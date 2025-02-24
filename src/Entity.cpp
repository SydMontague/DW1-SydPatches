#include "Helper.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    // vanilla only allocates 8 vectors here, but there are 10 NPCs...
    static bool hasRotationData[10];
    static Vector rotationData[10];

    static int16_t moveToDeltaX;
    static int16_t moveToDeltaZ;
    static Vector previousCameraPos;
    static bool previousCameraPosInitialized;

    Entity* getEntityFromScriptId(uint8_t* entityId)
    {
        // TODO separate in and out parameters, once all users are implemented
        auto input = *entityId;
        if (input == 0xFD)
        {
            *entityId = 0;
            return &TAMER_ENTITY;
        }
        if (input == 0xFC)
        {
            *entityId = 1;
            return &PARTNER_ENTITY;
        }

        for (int32_t i = 0; i < 8; i++)
        {
            auto* entity = &NPC_ENTITIES[i];
            if (entity->scriptId == input)
            {
                *entityId = i + 2;
                return entity;
            }
        }

        return nullptr;
    }

    bool tickEntityMoveToAxis(uint32_t scriptId, int32_t target, int32_t axis, int32_t speed, bool withCamera)
    {
        uint8_t entityId = scriptId;
        auto entityPtr   = getEntityFromScriptId(&entityId);

        // vanilla doesn't do a nullptr check here, but you can't move an entity that doesn't exist
        if (entityPtr == nullptr) return true;

        int32_t* axisValue;
        if (axis == 0)
            axisValue = &entityPtr->posData->location.x;
        else if (axis == 1)
            axisValue = &entityPtr->posData->location.y;
        else
            axisValue = &entityPtr->posData->location.z;

        if (!previousCameraPosInitialized)
        {
            moveToDeltaX                 = (target - *axisValue) / speed;
            previousCameraPosInitialized = true;
            previousCameraPos            = entityPtr->posData->location;
        }
        else
        {
            *axisValue += moveToDeltaX;
            setEntityPosition(entityId,
                              entityPtr->posData->location.x,
                              entityPtr->posData->location.y,
                              entityPtr->posData->location.z);
            setupEntityMatrix(entityId);

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
            moveCameraByDiff(&previousCameraPos, &entityPtr->posData->location);
            previousCameraPos = entityPtr->posData->location;
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
        uint8_t entityId1 = scriptId1;
        auto entityPtr    = getEntityFromScriptId(&entityId1);

        // vanilla doesn't do a nullptr check here, but you can't move an entity that doesn't exist
        if (entityPtr == nullptr) return true;

        if (!previousCameraPosInitialized)
        {
            if (scriptId2 == 0xFF)
            {
                moveToDeltaX = (targetX - entityPtr->posData->location.x) / speed;
                moveToDeltaZ = (targetZ - entityPtr->posData->location.z) / speed;
            }
            else
            {
                uint8_t entityId2 = scriptId2;
                auto entityPtr2   = getEntityFromScriptId(&entityId2);
                // vanilla doesn't do a nullptr check here, but you can't move to an entity that doesn't exist
                if (entityPtr2 == nullptr) return true;
                moveToDeltaX = (entityPtr2->posData->location.x - entityPtr->posData->location.x) / speed;
                moveToDeltaZ = (entityPtr2->posData->location.z - entityPtr->posData->location.z) / speed;
            }

            previousCameraPosInitialized = true;
            previousCameraPos            = entityPtr->posData->location;
        }
        else
        {
            setEntityPosition(entityId1,
                              entityPtr->posData->location.x + moveToDeltaX,
                              entityPtr->posData->location.y,
                              entityPtr->posData->location.z + moveToDeltaZ);
            setupEntityMatrix(entityId1);

            // in vanilla, when the DELTA speed is 0 this function may never return true, as the entity never reaches
            // the target location. If the game waits for the movement to complete, this will cause a softlock.
            // We solve this by treating DELTA speeds of 0 as always completed
            bool finishedX = moveToDeltaX == 0;
            if (moveToDeltaX < 0) { finishedX = entityPtr->posData->location.x <= targetX; }
            else if (moveToDeltaX > 0) { finishedX = entityPtr->posData->location.x >= targetX; }

            bool finishedZ = moveToDeltaZ == 0;
            if (moveToDeltaZ < 0) { finishedZ = entityPtr->posData->location.z <= targetZ; }
            else if (moveToDeltaZ > 0) { finishedZ = entityPtr->posData->location.z >= targetZ; }

            if (finishedX) entityPtr->posData->location.x = targetX;
            if (finishedZ) entityPtr->posData->location.z = targetZ;
            if (finishedX && finishedZ)
            {
                previousCameraPosInitialized = false;
                return true;
            }
        }

        if (withCamera)
        {
            moveCameraByDiff(&previousCameraPos, &entityPtr->posData->location);
            previousCameraPos = entityPtr->posData->location;
        }
        return false;
    }

    bool tickEntitySetRotation(uint32_t scriptId, int16_t angle)
    {
        uint8_t id   = scriptId;
        auto* entity = getEntityFromScriptId(&id);
        if (entity != nullptr) entity->posData->rotation.y = angle;

        return true;
    }

    bool tickLookAtEntity(uint32_t scriptId1, uint32_t scriptId2)
    {
        uint8_t entityId1 = scriptId1;
        uint8_t entityId2 = scriptId2;
        auto entityPtr    = getEntityFromScriptId(&entityId1);
        auto entityPtr2   = getEntityFromScriptId(&entityId2);

        // vanilla doesn't do a nullptr check here, but unchecked access is iffy
        if (entityPtr == nullptr || entityPtr2 == nullptr) return true;

        if (hasRotationData[entityId1] == 0)
        {
            rotationData[entityId1]    = entityPtr2->posData->location;
            hasRotationData[entityId1] = 1;
        }
        else
        {
            int16_t targetAngle;
            int16_t cwDiff;
            int16_t ccDiff;
            getRotationDifference(entityPtr->posData, &rotationData[entityId1], &targetAngle, &ccDiff, &cwDiff);

            if (rotateEntity(&entityPtr->posData->rotation, targetAngle, ccDiff, cwDiff, 0x200))
            {
                hasRotationData[entityId1] = 0;
                return true;
            }
        }

        return false;
    }

    bool tickEntityWalkTo(uint32_t scriptId1, uint32_t scriptId2, int32_t targetX, int32_t targetZ, bool withCamera)
    {
        uint8_t entityId1 = scriptId1;
        uint8_t entityId2 = scriptId2;
        auto entityPtr    = getEntityFromScriptId(&entityId1);
        auto entityPtr2   = getEntityFromScriptId(&entityId2);

        // vanilla doesn't do a nullptr check here, but unchecked access is iffy
        if (entityPtr == nullptr) return true;

        Vector currentPos = entityPtr->posData->location;
        Vector targetPos  = {.x = targetX, .y = currentPos.y, .z = targetZ};

        if (entityPtr2 != nullptr) targetPos = entityPtr2->posData->location;

        if (entityId1 >= 2) NPC_IS_WALKING_TOWARDS[entityId1 - 2] = 1;

        if (!previousCameraPosInitialized && withCamera)
        {
            previousCameraPos            = currentPos;
            previousCameraPosInitialized = true;
        }

        entityLookAtLocation(entityPtr, &targetPos);

        if (withCamera)
        {
            moveCameraByDiff(&previousCameraPos, &currentPos);
            previousCameraPos = currentPos;
        }

        CollisionCode collision = CollisionCode::NONE;
        if (entityPtr2 != nullptr) { collision = entityCheckCollision(nullptr, entityPtr, 0, 0); }

        // In vanilla a movement is complete once the entity location is on the same tile as the target location.
        // However, in some constellations this might never happen and cause softlocks.
        // One common example is the left door in the third set of doors in Toy Town, where the target location is near
        // the edge of the tile, causing the player to constantly walk back and forth without ever finishing.
        // Additionally the collision check did ignore NPC8
        auto distance = getDistanceSquared(&currentPos, &targetPos);
        auto radius   = getDigimonData(entityPtr->type)->radius;

        if (distance < radius * radius || (collision != CollisionCode::NONE && collision < CollisionCode::MAP))
        {
            previousCameraPosInitialized = false;
            if (entityId1 >= 2) NPC_IS_WALKING_TOWARDS[entityId1 - 2] = 0;
            return true;
        }

        return false;
    }
}