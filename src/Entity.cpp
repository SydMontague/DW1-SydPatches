#include "Entity.hpp"

#include "Helper.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "Utils.hpp"
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

    bool checkMapCollisionX(Entity* entity, bool isMaxZ)
    {
        auto& pos   = entity->posData->location;
        auto radius = getDigimonData(entity->type)->radius;

        auto tileXMin = getTileX(pos.x - radius / 2);
        auto tileXMax = getTileX(pos.x + radius / 2);
        auto tileZ    = isMaxZ ? getTileZ(pos.z - radius) : getTileZ(pos.z + radius);

        for (auto i = tileXMin; i <= tileXMax; i++)
            if ((getTile(i, tileZ) & 0x80) != 0) return true;

        return false;
    }

    bool checkMapCollisionZ(Entity* entity, bool isMaxX)
    {
        auto& pos   = entity->posData->location;
        auto radius = getDigimonData(entity->type)->radius;

        auto tileZMin = getTileZ(pos.z + radius / 2);
        auto tileZMax = getTileZ(pos.z - radius / 2);
        auto tileX    = isMaxX ? getTileX(pos.x + radius) : getTileX(pos.x - radius);

        for (auto i = tileZMin; i <= tileZMax; i++)
            if ((getTile(tileX, i) & 0x80) != 0) return true;

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

    bool isRectInRect(RECT* rect, int32_t minX, int32_t minY, int32_t maxX, int32_t maxY)
    {
        if (rect->x + rect->width < minX) return false;
        if (rect->x > maxX) return false;
        if (rect->y - rect->height > minY) return false;
        if (rect->y < maxY) return false;

        return true;
    }

    constexpr bool hasMovedOutsideOfArea(ScreenPos oldPos, ScreenPos newPos, int16_t width, int16_t height)
    {
        if (newPos.screenX < -width / 2 && newPos.screenX < oldPos.screenX) return true;
        if (newPos.screenX > width / 2 && oldPos.screenX < newPos.screenX) return true;
        if (newPos.screenY < -height / 2 && newPos.screenY < oldPos.screenY) return true;
        if (newPos.screenY > height / 2 && oldPos.screenY < newPos.screenY) return true;

        return false;
    }

    bool entityCheckCombatArea(Entity* entity, Vector* oldPos, int16_t width, int16_t height)
    {
        if (GAME_STATE == 4) return false;

        auto& newPos = entity->posData->location;
        auto radius  = getDigimonData(entity->type)->radius;

        struct AreaPoint
        {
            int16_t height;
            int8_t modX;
            int8_t modY;
        };

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
            auto screenOld = getScreenPosition(oldPos->x + radius * point.modX,
                                               oldPos->y + point.height,
                                               oldPos->z + radius * point.modY);

            if (hasMovedOutsideOfArea(screenOld, screenNew, width, height)) return true;
        }

        return false;
    }

    void entityLookAtLocation(Entity* entity, Vector* location)
    {
        if (entity == nullptr || location == nullptr) return;

        auto& entityLoc = entity->posData->location;

        entity->posData->rotation.y = atan(location->z - entityLoc.z, location->x - entityLoc.x);
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

        libgs_GsGetLws(posData.obj.coord2, &lw, &ls);
        libgs_GsSetLightMatrix(&lw);
        libgs_GsSetLsMatrix(&ls);
        libgs_GsSortObject4(&posData.obj, FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER], 5, &SCRATCHPAD);
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