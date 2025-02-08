#include "Helper.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    // vanilla only allocates 8 vectors here, but there are 10 NPCs...
    static bool hasRotationData[10];
    static Vector rotationData[10];

    Entity* getEntityFromScriptId(uint8_t* param)
    {
        // TODO separate in and out parameters, once all users are implemented
        auto input = *param;
        if (input == 0xFD)
        {
            *param = 0;
            return &TAMER_ENTITY;
        }
        if (input == 0xFC)
        {
            *param = 1;
            return &PARTNER_ENTITY;
        }

        for (int32_t i = 0; i < 8; i++)
        {
            auto* entity = &NPC_ENTITIES[i];
            if (entity->scriptId == input)
            {
                *param = i + 2;
                return entity;
            }
        }

        return nullptr;
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

            if (rotateEntity(&entityPtr->posData->rotation, &targetAngle, &ccDiff, &cwDiff, 0x200))
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

        if (!PREVIOUS_CAMERA_POS_INITIALIZED && withCamera)
        {
            PREVIOUS_CAMERA_POS             = currentPos;
            PREVIOUS_CAMERA_POS_INITIALIZED = true;
        }

        entityLookAtLocation(entityPtr, &targetPos);

        if (withCamera)
        {
            moveCameraByDiff(&PREVIOUS_CAMERA_POS, &currentPos);
            PREVIOUS_CAMERA_POS = currentPos;
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
            PREVIOUS_CAMERA_POS_INITIALIZED = 0;
            if (entityId1 >= 2) NPC_IS_WALKING_TOWARDS[entityId1 - 2] = 0;
            return true;
        }

        return false;
    }
}