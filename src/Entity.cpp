#include "Entity.hpp"

#include "Camera.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "Utils.hpp"
#include "VanillaText.hpp"
#include "extern/dw1.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"
#include "extern/psx.hpp"
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
    static dtl::array<EntityTextData, 4> entityTextData;

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

    void getModelTile(Vector* location, int16_t* tileX, int16_t* tileY)
    {
        *tileX = getTileX(location->x);
        *tileY = getTileZ(location->z);
    }

    void getEntityTile(Entity* entity, int8_t* tileX, int8_t* tileY)
    {
        *tileX = getTileX(entity->posData->location.x);
        *tileY = getTileZ(entity->posData->location.z);
    }

    bool entityIsInEntity(Entity* self, Entity* other)
    {
        auto selfRadius  = getDigimonData(self->type)->radius;
        auto selfX       = self->posData->location.x;
        auto selfZ       = self->posData->location.z;
        auto otherRadius = getDigimonData(other->type)->radius;
        auto otherX      = other->posData->location.x;
        auto otherZ      = other->posData->location.z;

        if (otherX + otherRadius < selfX - selfRadius) return false;
        if (otherX - otherRadius > selfX + selfRadius) return false;
        if (otherZ + otherRadius < selfZ - selfRadius) return false;
        if (otherZ - otherRadius > selfZ + selfRadius) return false;

        return true;
    }

    void entityLookAtLocation(Entity* entity, Vector* location)
    {
        if (entity == nullptr || location == nullptr) return;

        auto& entityLoc = entity->posData->location;

        entity->posData->rotation.y = atan(location->z - entityLoc.z, location->x - entityLoc.x);
    }

    constexpr bool hasMovedOutsideOfArea(ScreenPos oldPos, ScreenPos newPos, int16_t width, int16_t height)
    {
        if (newPos.screenX < -width / 2 && newPos.screenX < oldPos.screenX) return true;
        if (newPos.screenX > width / 2 && oldPos.screenX < newPos.screenX) return true;
        if (newPos.screenY < -height / 2 && newPos.screenY < oldPos.screenY) return true;
        if (newPos.screenY > height / 2 && oldPos.screenY < newPos.screenY) return true;

        return false;
    }

    static bool
    entityCheckCombatArea(Entity* entity, const Vector& newPos, const Vector& oldPos, int16_t width, int16_t height)
    {
        if (width == 0 || height == 0) return false;
        if (GAME_STATE == 4) return false;

        auto radius = getDigimonData(entity->type)->radius;

        struct AreaPoint
        {
            int16_t height;
            int8_t modX;
            int8_t modY;
        };

        // TODO shouldn't this be the Digimon type height instead of a fixed value?
        constexpr AreaPoint points[] = {
            {.height = 0, .modX = -1, .modY = 1},
            {.height = 0, .modX = 1, .modY = 1},
            {.height = 0, .modX = -1, .modY = -1},
            {.height = 0, .modX = 1, .modY = -1},
            {.height = -200, .modX = -1, .modY = 1},
            {.height = -200, .modX = 1, .modY = 1},
            {.height = -200, .modX = -1, .modY = -1},
            {.height = -200, .modX = 1, .modY = -1},
        };

        for (auto point : points)
        {
            auto screenNew = getScreenPosition(newPos.x + radius * point.modX,
                                               newPos.y + point.height,
                                               newPos.z + radius * point.modY);
            auto screenOld = getScreenPosition(oldPos.x + radius * point.modX,
                                               oldPos.y + point.height,
                                               oldPos.z + radius * point.modY);

            if (hasMovedOutsideOfArea(screenOld, screenNew, width, height)) return true;
        }

        return false;
    }

    bool isOffScreen(Position* pos, int32_t width, int32_t height)
    {
        // TODO: remove once all users have been migrated to isInScreenRect directly
        ScreenPos newPos = {
            .screenX = static_cast<int16_t>(pos->x - (160 - DRAWING_OFFSET_X)),
            .screenY = static_cast<int16_t>(pos->y - (120 - DRAWING_OFFSET_Y)),
            .depth   = 0,
        };

        return !isInScreenRect(newPos, width, height);
    }

    bool entityIsOffScreen(Entity* entity, int16_t width, int16_t height)
    {
        if (entity->type == DigimonType::NPC_KUNEMON && entity->animId == 0x1C) return false;

        auto digiRadius = getDigimonData(entity->type)->radius;
        auto digiHeight = getDigimonData(entity->type)->height;
        const auto& pos = entity->posData[0].location;

        return !isBoxOnScreen(&pos, digiRadius, digiHeight);
    }

    static Vector entityMoveForward(const Entity* entity)
    {
        Vector vec{};
        Vector movement;
        Matrix rotMatrix;

        const auto& momentum = entity->momentum[0];

        if (momentum.subDelta[8] != 0 && (momentum.subScale[8] - momentum.subDelta[8] < 1))
            vec.z = (momentum.delta[8] + momentum.subValue[8]) << 15;
        else
            vec.z = momentum.delta[8] << 15;

        libgte_RotMatrix(&entity->posData[0].rotation, &rotMatrix);
        libgte_ApplyMatrixLV(&rotMatrix, &vec, &movement);

        return {
            .x = (entity->locX + movement.x) >> 15,
            .y = 0,
            .z = (entity->locZ + movement.z) >> 15,
        };
    }

    static bool checkMapCollisionX(Entity* entity, const Vector& pos, bool isMaxZ)
    {
        auto radius = getDigimonData(entity->type)->radius;

        auto tileXMin = getTileX(pos.x - radius / 2);
        auto tileXMax = getTileX(pos.x + radius / 2);
        auto tileZ    = isMaxZ ? getTileZ(pos.z - radius) : getTileZ(pos.z + radius);

        for (auto i = tileXMin; i <= tileXMax; i++)
            if ((getTile(i, tileZ) & 0x80) != 0) return true;

        return false;
    }

    static bool checkMapCollisionZ(Entity* entity, const Vector& pos, bool isMaxX)
    {
        auto radius = getDigimonData(entity->type)->radius;

        auto tileZMin = getTileZ(pos.z + radius / 2);
        auto tileZMax = getTileZ(pos.z - radius / 2);
        auto tileX    = isMaxX ? getTileX(pos.x + radius) : getTileX(pos.x - radius);

        for (auto i = tileZMin; i <= tileZMax; i++)
            if ((getTile(tileX, i) & 0x80) != 0) return true;

        return false;
    }

    static bool checkMapCollision(Entity* entity, const Vector& pos, int32_t diffX, int32_t diffZ)
    {
        if (diffX < 0 && checkMapCollisionZ(entity, pos, false)) return true;
        if (diffX > 0 && checkMapCollisionZ(entity, pos, true)) return true;
        if (diffZ < 0 && checkMapCollisionX(entity, pos, true)) return true;
        if (diffZ > 0 && checkMapCollisionX(entity, pos, false)) return true;

        return false;
    }

    static bool
    entityCheckEntityCollision(Entity* self, const Vector& selfLoc, Entity* other, int32_t diffX, int32_t diffZ)
    {
        // TODO refactor, this code is kinda ugly and entity collision sucks in vanilla
        auto selfRadius  = getDigimonData(self->type)->radius;
        auto otherRadius = getDigimonData(other->type)->radius;

        const auto& otherLoc = other->posData[0].location;

        BoundingBox2D selfBB(selfLoc.x, selfLoc.z, selfRadius);
        BoundingBox2D otherBB(otherLoc.x, otherLoc.z, otherRadius);

        if (GAME_STATE == 0 && self == &TAMER_ENTITY)
        {
            if ((POLLED_INPUT & BUTTON_CROSS) != 0 && self->animId == 0)
            {
                auto rot = self->posData[0].rotation.y;
                if (rot > 0 && rot < 0x800) selfBB.minX -= 50;
                if (rot > 0x800 && rot < 0x1000) selfBB.maxX += 50;
                if (rot < 0x400 && rot > 0xC00) selfBB.minY -= 50;
                if (rot > 0x400 && rot < 0xC00) selfBB.maxY += 50;
            }

            if (otherBB.overlaps(selfBB.minX, selfBB.maxY, selfBB.minX, selfBB.minY)) return true;
            if (otherBB.overlaps(selfBB.maxX, selfBB.maxY, selfBB.maxX, selfBB.minY)) return true;
            if (otherBB.overlaps(selfBB.minX, selfBB.minY, selfBB.maxX, selfBB.minY)) return true;
            if (otherBB.overlaps(selfBB.minX, selfBB.maxY, selfBB.maxX, selfBB.maxY)) return true;

            return selfBB.overlaps(otherBB);
        }
        else
        {
            if (diffX < 0 && otherBB.overlaps(selfBB.minX, selfBB.maxY, selfBB.minX, selfBB.minY)) return true;
            if (diffX > 0 && otherBB.overlaps(selfBB.maxX, selfBB.maxY, selfBB.maxX, selfBB.minY)) return true;
            if (diffZ < 0 && otherBB.overlaps(selfBB.minX, selfBB.minY, selfBB.maxX, selfBB.minY)) return true;
            if (diffZ > 0 && otherBB.overlaps(selfBB.minX, selfBB.maxY, selfBB.maxX, selfBB.maxY)) return true;
        }

        return false;
    }

    CollisionCode entityCheckCollision(Entity* ignore, Entity* self, int32_t width, int32_t height)
    {
        self->animFlag &= 5;
        const auto& oldLoc = self->posData[0].location;
        auto newLoc        = entityMoveForward(self);

        auto diffX    = newLoc.x - oldLoc.x;
        auto diffZ    = newLoc.z - oldLoc.z;
        auto rotation = self->posData->rotation.y;
        if (rotation == 0x400 || rotation == 0xC00) diffZ = 0;
        if (rotation == 0 || rotation == 0x800) diffX = 0;

        if (entityCheckCombatArea(self, newLoc, oldLoc, width, height)) return CollisionCode::SCREEN;
        if (checkMapCollision(self, newLoc, diffX, diffZ)) return CollisionCode::MAP;

        for (int32_t i = 0; i < 10; i++)
        {
            auto* other = ENTITY_TABLE.getEntityById(i);

            if (isInvisible(other)) continue;
            if (other == self) continue;
            if (other == ignore) continue;
            if (i == 0 && GAME_STATE == 1 && (self->animId == 0x24 || self->animId == 0x23)) continue;
            if (i != 0 && static_cast<DigimonEntity*>(other)->stats.currentHP == 0) continue;

            if (entityCheckEntityCollision(self, newLoc, other, diffX, diffZ)) return static_cast<CollisionCode>(i);
        }

        return CollisionCode::NONE;
    }

    constexpr uint16_t COLLISION_GRACE_ROTATION[8][4] = {
        {0x000, 0x400, 0xC00, 0x800},
        {0x400, 0x000, 0x800, 0xC00},
        {0x400, 0x800, 0x000, 0xC00},
        {0x800, 0x400, 0xC00, 0x000},
        {0x800, 0xC00, 0x400, 0x000},
        {0xC00, 0x800, 0x000, 0x400},
        {0xC00, 0x000, 0x800, 0x400},
        {0x000, 0xC00, 0x400, 0x800},
    };

    void collisionGrace(Entity* ignore, Entity* self, int32_t dx, int32_t dy)
    {
        self->animFlag &= 5;

        auto collision = entityCheckCollision(ignore, self, dx, dy);
        if (collision == CollisionCode::NONE) return;

        auto baseRotation = posmod(self->posData->rotation.y, 4096);
        uint8_t flag      = 0;

        for (int32_t i = 0; i < 4; i++)
        {
            auto graceRotation        = COLLISION_GRACE_ROTATION[baseRotation >> 9][i];
            self->posData->rotation.y = graceRotation;

            if (entityCheckCollision(ignore, self, dx, dy) == CollisionCode::NONE)
            {
                auto diffLeft  = posmod((baseRotation - graceRotation), 4096);
                auto diffRight = posmod((graceRotation - baseRotation), 4096);

                auto finalRot = baseRotation;
                if ((diffLeft - diffRight) < 0)
                    finalRot -= (min(0x50, diffLeft));
                else
                    finalRot += (min(0x50, diffRight));

                self->posData->rotation.y = posmod(finalRot, 4096);
                self->animFlag |= flag;
                return;
            }

            auto val = posmod(self->posData->rotation.y, 4096);
            flag |= (8 << ((val >> 10) & 0x1F));
        }

        self->posData->rotation.y = posmod(baseRotation + 0x20, 4096);
        self->animFlag |= 2;
    }

    void initializeEntityText()
    {
        for (auto& val : entityTextData)
        {
            for (int32_t i = 0; i < 8; i++)
            {
                val.activeList[i]      = 0xFF;
                val.entries[i].frameId = -1;
            }
            val.activeElements = 0;
        }
    }

    void removeEntityText(int32_t entityId)
    {
        auto& entry = entityTextData[entityId];

        if (entry.activeElements == 0) return;

        removeObject(ObjectID::ENTITY_TEXT, entityId);
        entry.activeElements = 0;
        for (int32_t i = 0; i < 8; i++)
        {
            entry.activeList[i]      = 0xFF;
            entry.entries[i].frameId = -1;
        }
    }

    static void renderEntityText(int32_t id)
    {
        // vanilla sorts the activeList

        constexpr dtl::array<int8_t, 24> yOffsets{0,  -7, -5, -5, -2, -1, 1, 2, 5, 5, 7, -4,
                                                  -3, -2, 0,  -1, 1,  0,  2, 3, 0, 0, 0, 0};
        constexpr dtl::array<uint8_t, 9> iconOffsets{0xE0, 0xB0, 0xC8, 0xB8, 0xC0, 0xD0, 0, 0, 0};

        auto& textEntry = entityTextData[id];

        for (int32_t i = 0; i < textEntry.entries.size(); i++)
        {
            if (textEntry.activeList[i] == 0xFF) continue;

            auto& entry = textEntry.entries[i];
            if (entry.frameId == -1) continue;
            if (i != 0)
            {
                auto val = textEntry.entries[textEntry.activeList[i] - 1].frameId;
                if (val != -1 && val < 11) continue; // delay
            }
            if (entry.frameId < 21) entry.y += yOffsets[entry.frameId];

            auto posX = entry.x;
            auto posY = entry.y;

            if (GAME_STATE == 4)
            {
                auto entity = ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[id]);
                auto pos    = getScreenPosition(*entity, 0); // vanilla uses getEntityScreenPos

                posX += pos.screenX;
                posY += pos.screenY - 8;
            }

            drawEntityText(entry.color, entry.numDigits, posX, posY, entry.value, 14 - i);
            if (entry.icon != 0 && entry.icon < iconOffsets.size())
                drawEntityTextIcon(posX - 8, posY, iconOffsets[entry.icon], 14 - i);

            entry.frameId += 1;
            if (entry.frameId > 30)
            {
                entry.frameId           = -1;
                textEntry.activeList[i] = 0xFF;
            }
        }

        for (const auto& val : textEntry.activeList)
            if (val != 0xFF) return;

        removeEntityText(id);
    }

    void addEntityText(DigimonEntity* entity, int32_t entityId, int8_t color, int32_t value, int8_t icon)
    {
        // vanilla resorts the activeList here, but I don't think that's necessary?
        auto& textData = entityTextData[entityId];

        if (textData.activeElements >= 8) return;

        for (int32_t i = 0; i < textData.entries.size(); i++)
        {
            auto& entry = textData.entries[i];
            if (entry.frameId != -1) continue;

            entry.numDigits = getDigitCount(value);
            entry.value     = value;
            entry.color     = color;
            entry.icon      = icon;
            entry.frameId   = 0;

            entry.x = textData.activeElements * 4;
            entry.y = textData.activeElements * 4;
            if (GAME_STATE != 4)
            {
                auto pos = getScreenPosition(*entity, 0); // vanilla uses getEntityScreenPos
                entry.x += pos.screenX;
                entry.y += pos.screenY - 8;
            }

            for (auto& activeEntry : textData.activeList)
                if (activeEntry == 0xFF)
                {
                    activeEntry = i;
                    break;
                }

            if (textData.activeElements == 0) addObject(ObjectID::ENTITY_TEXT, entityId, nullptr, renderEntityText);

            textData.activeElements++;
            break;
        }
    }
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

static GsRVIEW2 digiviceView = {
    .viewpointX      = 1300,
    .viewpointY      = 0,
    .viewpointZ      = -3280,
    .refpointX       = 0,
    .refpointY       = 0,
    .refpointZ       = 0,
    .viewpoint_twist = 0,
    .super           = nullptr,
};

void renderDigiviceEntity(Entity* entity, int32_t entityId, int32_t refX)
{
    FRAMEBUFFER_OT[0]->length = 9;
    FRAMEBUFFER_OT[0]->origin = FRAMEBUFFER0_ORIGIN;
    FRAMEBUFFER_OT[1]->length = 9;
    FRAMEBUFFER_OT[1]->origin = FRAMEBUFFER1_ORIGIN;

    digiviceView.refpointX = refX;

    libgs_GsSetProjection(0x200);
    libgs_GsSetRefView2(&digiviceView);
    libgs_GsClearOt(0, 5, FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER]);

    constexpr GsF_LIGHT light1{.x = 100, .y = 100, .z = 100, .r = 128, .g = 128, .b = 128};
    constexpr GsF_LIGHT light2{.x = 0, .y = 0, .z = 0, .r = 128, .g = 128, .b = 128};
    libgs_GsSetFlatLight(0, &light1);
    libgs_GsSetFlatLight(1, &light2);
    libgs_GsSetFlatLight(2, &light2);

    auto data     = getRaiseData(entity->type);
    auto digiData = getDigimonData(entity->type);
    auto oldLoc   = entity->posData->location;
    auto oldRot   = entity->posData->rotation;
    auto animId   = entity->animId;

    setEntityPosition(entityId, data->viewX, data->viewY, data->viewZ);
    setEntityRotation(entityId, 0, 0, 0);
    setupEntityMatrix(entityId);
    startAnimation(entity, 0);
    tickAnimation(entity);

    Matrix lw = libgs_REFERENCE_MATRIX;
    Matrix ls = libgs_REFERENCE_MATRIX;

    for (int32_t i = 0; i < digiData->boneCount; i++)
    {
        auto posData = entity->posData[i];
        if (posData.obj.tmd == nullptr) continue;

        drawObject(&posData.obj, FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER], 5);
    }

    libgs_GsSortOt(FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
    libgs_GsSetProjection(VIEWPORT_DISTANCE);
    libgs_GsSetRefView2(&GS_VIEWPOINT);

    libgs_GsSetFlatLight(0, &LIGHT_DATA[0]);
    libgs_GsSetFlatLight(1, &LIGHT_DATA[1]);
    libgs_GsSetFlatLight(2, &LIGHT_DATA[2]);

    setEntityPosition(entityId, oldLoc.x, oldLoc.y, oldLoc.z);
    setEntityRotation(entityId, oldRot.x, oldRot.y, oldRot.z);
    setupEntityMatrix(entityId);
    startAnimation(entity, animId);
    tickAnimation(entity);
}