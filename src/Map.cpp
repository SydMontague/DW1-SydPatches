#include "Map.hpp"

#include "Entity.hpp"
#include "Files.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Inventory.hpp"
#include "Math.hpp"
#include "NPCEntity.hpp"
#include "Partner.hpp"
#include "Sound.hpp"
#include "Tamer.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"
#include "extern/psx.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    constexpr auto NINJAMON_EFFECT_COUNT = 41;

    static int16_t mistOffsetX[4] = {-160, 160, 160, -160};
    static int16_t mistOffsetY[2] = {-120, 120};
    static uint8_t moveObjectDeltaX[12];
    static uint8_t moveObjectDeltaY[12];

    static int16_t ninjamonEffecX[NINJAMON_EFFECT_COUNT];
    static int16_t ninjamonEffecY[NINJAMON_EFFECT_COUNT];
    static int8_t ninjamonEffecXOffset[NINJAMON_EFFECT_COUNT];
    static int8_t ninjamonEffecYOffset[NINJAMON_EFFECT_COUNT];

    void loadMapDigimon(uint8_t* buffer, uint32_t mapId)
    {
        auto digimonCount = *reinterpret_cast<int16_t*>(buffer);
        auto* currentPtr  = buffer + 2;

        for (int32_t i = 0; i < digimonCount; i++)
        {
            auto* mapDigimon = reinterpret_cast<MapDigimon*>(currentPtr);

            MAP_DIGIMON_TABLE[i].typeId        = static_cast<DigimonType>(mapDigimon->typeId);
            MAP_DIGIMON_TABLE[i].followMode    = mapDigimon->followMode;
            MAP_DIGIMON_TABLE[i].posX          = mapDigimon->posX;
            MAP_DIGIMON_TABLE[i].posY          = mapDigimon->posY;
            MAP_DIGIMON_TABLE[i].posZ          = mapDigimon->posZ;
            MAP_DIGIMON_TABLE[i].rotX          = mapDigimon->rotX;
            MAP_DIGIMON_TABLE[i].rotY          = mapDigimon->rotY;
            MAP_DIGIMON_TABLE[i].rotZ          = mapDigimon->rotZ;
            MAP_DIGIMON_TABLE[i].trackingRange = mapDigimon->aiTrackRange;

            NPC_ENTITIES[i].unk2            = mapDigimon->unk2;
            NPC_ENTITIES[i].scriptId        = mapDigimon->scriptId;
            NPC_ENTITIES[i].stats.hp        = mapDigimon->maxHP;
            NPC_ENTITIES[i].stats.mp        = mapDigimon->maxMP;
            NPC_ENTITIES[i].stats.currentHP = mapDigimon->currentHP;
            NPC_ENTITIES[i].stats.currentMP = mapDigimon->currentMP;
            NPC_ENTITIES[i].stats.off       = mapDigimon->offense;
            NPC_ENTITIES[i].stats.def       = mapDigimon->defense;
            NPC_ENTITIES[i].stats.speed     = mapDigimon->speed;
            NPC_ENTITIES[i].stats.brain     = mapDigimon->brains;
            NPC_ENTITIES[i].bits            = mapDigimon->bits;
            NPC_ENTITIES[i].chargeMode      = mapDigimon->chargeMode;
            NPC_ENTITIES[i].unk1            = mapDigimon->unk5;

            NPC_ENTITIES[i].stats.moves[0]     = mapDigimon->moves[0];
            NPC_ENTITIES[i].stats.moves[1]     = mapDigimon->moves[1];
            NPC_ENTITIES[i].stats.moves[2]     = mapDigimon->moves[2];
            NPC_ENTITIES[i].stats.moves[3]     = mapDigimon->moves[3];
            NPC_ENTITIES[i].stats.movesPrio[0] = mapDigimon->movePrio[0];
            NPC_ENTITIES[i].stats.movesPrio[1] = mapDigimon->movePrio[1];
            NPC_ENTITIES[i].stats.movesPrio[2] = mapDigimon->movePrio[2];
            NPC_ENTITIES[i].stats.movesPrio[3] = mapDigimon->movePrio[3];
            NPC_ENTITIES[i].flee.x             = mapDigimon->fleeX;
            NPC_ENTITIES[i].flee.y             = mapDigimon->fleeY;
            NPC_ENTITIES[i].flee.z             = mapDigimon->fleeZ;
            MAP_DIGIMON_TABLE[i].animation     = 0;

            MAP_DIGIMON_TABLE[i].aiSections[0] = mapDigimon->aiSections[0];
            MAP_DIGIMON_TABLE[i].aiSections[1] = mapDigimon->aiSections[1];
            MAP_DIGIMON_TABLE[i].aiSections[2] = mapDigimon->aiSections[2];
            MAP_DIGIMON_TABLE[i].aiSections[3] = mapDigimon->aiSections[3];
            MAP_DIGIMON_TABLE[i].aiSections[4] = mapDigimon->aiSections[4];
            MAP_DIGIMON_TABLE[i].aiSections[5] = mapDigimon->aiSections[5];
            MAP_DIGIMON_TABLE[i].aiSections[6] = mapDigimon->aiSections[6];
            MAP_DIGIMON_TABLE[i].aiSections[7] = mapDigimon->aiSections[7];

            for (int32_t j = 0; j < mapDigimon->waypointCount; j++)
            {
                MAP_DIGIMON_TABLE[i].waypoints[j].x = mapDigimon->waypoints[j].x;
                MAP_DIGIMON_TABLE[i].waypoints[j].y = mapDigimon->waypoints[j].y;
                MAP_DIGIMON_TABLE[i].waypoints[j].z = mapDigimon->waypoints[j].z;
            }
            MAP_DIGIMON_TABLE[i].activeSecton = 0;

            currentPtr += sizeof(MapDigimon) + sizeof(SPosition3D) * mapDigimon->waypointCount;
        }
    }

    void loadMapEntities(uint8_t* buffer, uint32_t mapId, uint32_t exitId)
    {
        memcpy(&MAP_WARPS, buffer, sizeof(MapWarps));

        auto posX     = MAP_WARPS.spawnX[exitId];
        auto posY     = MAP_WARPS.spawnY[exitId];
        auto posZ     = MAP_WARPS.spawnZ[exitId];
        auto rotation = MAP_WARPS.rotation[exitId];

        setupTamerOnWarp(posX, posY, posZ, rotation);

        if (rotation <= 0x200 || rotation > 0xE00) posZ += 200;
        if (rotation > 0x200 && rotation <= 0x600) posX += 200;
        if (rotation > 0x600 && rotation <= 0xA00) posZ -= 200;
        if (rotation > 0xA00 && rotation <= 0xE00) posX -= 200;

        setupPartnerOnWarp(posX, posY, posZ, rotation);
        loadMapDigimon(buffer + sizeof(MapWarps), mapId);
        STORED_TAMER_POS = TAMER_ENTITY.posData->location;
    }

    int16_t getMapRotation()
    {
        auto diffZ = GS_VIEWPOINT.viewpointZ - GS_VIEWPOINT.refpointZ;
        auto diffX = GS_VIEWPOINT.viewpointX - GS_VIEWPOINT.refpointX;
        return atan(diffZ, diffX);
    }

    void clearMapObjects(LocalMapObjectInstance* instances)
    {
        for (uint32_t i = 0; i < 188; i++)
        {
            auto& data = instances[i];
            for (auto& val : data.animSprites)
                val = -1;
            for (auto& val : data.animTimes)
                val = 0;
            data.orderValue   = 0;
            data.timer        = 0;
            data.currentFrame = -1;
            data.flag         = 1;
        }
    }

    void loadMapObjects(LocalMapObjectInstance* mapObjects, uint8_t* data, uint32_t mapId)
    {
        int16_t count  = *reinterpret_cast<int16_t*>(data);
        MapObject* obj = reinterpret_cast<MapObject*>(data + 2);

        for (int32_t i = 0; i < count; i++)
            LOCAL_MAP_OBJECTS[i] = obj[i];

        auto* localInstanceCount  = reinterpret_cast<uint16_t*>(obj + count);
        MAP_OBJECT_INSTANCE_COUNT = *localInstanceCount;
        auto* instancePtr         = reinterpret_cast<MapObjectInstance*>(localInstanceCount + 1);

        for (int32_t i = 0; i < MAP_OBJECT_INSTANCE_COUNT; i++)
        {
            auto& instance = instancePtr[i];
            auto& object   = mapObjects[i];
            for (int32_t j = 0; j < 8; j++)
            {
                object.animSprites[j] = instance.animState[j];
                object.animTimes[j]   = instance.animDuration[j];
            }
            object.x    = instance.posX;
            object.y    = instance.posY;
            object.flag = instance.flag;

            // snow
            if (((mapId > 0x57 && mapId < 0x61) || (mapId > 0x83 && mapId < 0x88)) && i < 0x23)
            {
                if (i > 0x13) object.x = random(320);
                object.y = random(240);
                object.flag |= 0x80;
            }
        }
        for (auto& val : MAP_OBJECT_MOVE_TO_DATA)
            val = 0;
    }

    void loadMapImage1(uint8_t* buffer)
    {
        TIM_IMAGE image;
        libgpu_OpenTIM(buffer);
        libgpu_ReadTIM(&image);
        libgpu_LoadImage(image.pixelRect, image.pixelPtr);
        libgpu_DrawSync(0);
        if (image.clutRect->y != 480)
        {
            libgpu_LoadImage(image.clutRect, image.clutPtr);
            libgpu_DrawSync(0);
        }
    }

    void loadMapImage2(uint8_t* buffer, int32_t id)
    {
        TIM_IMAGE image;
        libgpu_OpenTIM(buffer);
        libgpu_ReadTIM(&image);
        libgpu_LoadImage(image.pixelRect, image.pixelPtr);
        libgpu_DrawSync(0);

        if (id != 0) return;

        for (int32_t i = 0; i < image.clutRect->height; i++)
        {
            RECT rect{.x = static_cast<int16_t>(16 * i), .y = 486, .width = 16, .height = 1};
            libgpu_LoadImage(&rect, image.clutPtr + (i * 8));
            libgpu_DrawSync(0); // vanilla doesn't do a sync here
        }
    }

    void calcMapObjectOrder(LocalMapObjectInstance* instances)
    {
        for (int32_t i = 0; i < MAP_OBJECT_INSTANCE_COUNT; i++)
        {
            auto& data = instances[i];

            data.currentFrame = 0;
            if ((data.flag & 0x80) != 0)
            {
                data.orderValue = 30;
                continue;
            }

            SVector worldPos;
            Position screenPos;
            for (auto val : data.animSprites)
            {
                if (val == -1 || val == -2) continue;

                worldPos.x = LOCAL_MAP_OBJECTS[val].someX;
                worldPos.y = LOCAL_MAP_OBJECTS[val].someY;
                worldPos.z = LOCAL_MAP_OBJECTS[val].someZ;
                break;
            }

            auto depth = worldPosToScreenPos(&worldPos, &screenPos) / 16;
            if (depth < 35) depth = 35;
            if (depth > 4095) depth = 4086;
            if (worldPos.y == 10000) depth = 4094;

            data.orderValue = depth;
        }
    }

    constexpr bool hasMist(int32_t mapId)
    {
        if (mapId > 114 && mapId < 120) return true;
        if (mapId == 121) return true;
        if (mapId == 163) return true;
        if (mapId == 220) return true;

        return false;
    }

    void buildMapOverlayPrim(POLY_FT4* prim,
                             LocalMapObjectInstance* instance,
                             LocalMapObject* object,
                             int16_t cameraX,
                             int16_t cameraY,
                             bool isFixedPos)
    {
        if (isFixedPos)
            setPosDataPolyFT4(prim, instance->x, instance->y, object->width, object->height);
        else
        {
            auto posX = (instance->x - 160) - (cameraX - (160 - DRAWING_OFFSET_X));
            auto posY = (instance->y - 120) - (cameraY - (120 - DRAWING_OFFSET_Y));
            setPosDataPolyFT4(prim, posX, posY, object->width, object->height);
        }

        prim->r0 = 128;
        prim->g0 = 128;
        prim->b0 = 128;
        setPolyFT4UV(prim, object->texX, object->texY, object->width, object->height);

        auto clut = object->clut;
        if (clut == 0xFF)
        {
            prim->tpage = libgpu_GetTPage(1, object->transparency, (object->texX / 256) * 128 + 384, 0);
            prim->clut  = getClut(0, 480);
        }
        else if (clut < 16)
        {
            prim->tpage = libgpu_GetTPage(0, object->transparency, (object->texX / 256) * 64 + 384, 0);
            prim->clut  = getClut(object->clut * 16, 486);
        }
        else
        {
            prim->tpage = libgpu_GetTPage(1, object->transparency, (object->texX / 256) * 128 + 384, 0);
            prim->clut  = getClut(0, 468 + object->clut);
        }
    }

    void buildSnowflakePrim(POLY_FT4* prim, LocalMapObjectInstance* instance, LocalMapObject* object)
    {
        auto firstSprite = instance->animSprites[0];
        int32_t xOffsetLimit;
        int32_t yOffset;
        switch (firstSprite)
        {
            case 0:
            case 3:
            case 4:
                xOffsetLimit = 6;
                yOffset      = 3;
                break;
            case 1:
            case 5:
            case 6:
                xOffsetLimit = 2;
                yOffset      = 1;
                break;
            case 2:
                xOffsetLimit = 4;
                yOffset      = 10;
                break;
        }

        int32_t xOffset = random(xOffsetLimit);
        if (xOffsetLimit / 2 < xOffset) xOffset = -(xOffset % (xOffsetLimit / 2));

        if ((CURRENT_FRAME % 3 == 0) && (firstSprite == 0 || firstSprite == 3 || firstSprite == 4))
            instance->x = ring(instance->x + xOffset, 0, 320);

        setPosDataPolyFT4(prim, instance->x - 160 + xOffset, instance->y - 120, object->width - 1, object->height - 1);
        setUVDataPolyFT4(prim, object->texX, object->texY, object->width - 1, object->height - 1);
        prim->r0    = 128;
        prim->b0    = 128;
        prim->g0    = 128;
        prim->tpage = libgpu_GetTPage(0, object->transparency, (object->texX / 256) * 64 + 384, 0);
        prim->clut  = getClut(object->clut * 16, 486);
        instance->y += yOffset;
        if (firstSprite != 2 && instance->y > 130)
        {
            auto roll = random(10);
            if (roll < 2 && instance->currentFrame == 0)
            {
                instance->currentFrame = 1;
                instance->timer        = 0;
            }
            if (instance->currentFrame > 1 && instance->animSprites[instance->currentFrame + 1] == -1)
            {
                instance->currentFrame = 0;
                instance->y            = 0;
            }
        }
        if (instance->y > 240) instance->y = 0;
    }

    inline void
    renderMapOverlay(LocalMapObjectInstance& instance, LocalMapObject& object, int32_t cameraX, int32_t cameraY)
    {
        bool onScreenX = (cameraX - 40 < instance.x + object.width) && (instance.x < cameraX + 360);
        bool onScreenY = (cameraY - 60 < instance.y + object.height) && (instance.y < cameraY + 300);

        // order value is impossible, as calcMapObjectOrder puts the minimum to 35?
        if (!(onScreenX && onScreenY) && (instance.flag & 0x80) == 0 && instance.orderValue >= 20) return;

        POLY_FT4* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyFT4(prim);
        libgpu_SetSemiTrans(prim, object.transparency == 4 ? 0 : 1);

        if ((instance.flag & 0x80) != 0)
            buildSnowflakePrim(prim, &instance, &object);
        else
            buildMapOverlayPrim(prim, &instance, &object, cameraX, cameraY, instance.orderValue < 20);

        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + instance.orderValue, prim);
        libgs_GsSetWorkBase(prim + 1);
    }

    inline bool isMistEnabled(int32_t mapId)
    {
        if (CURRENT_FRAME != 220 && CURRENT_FRAME != 163) return true;

        return !isTriggerSet(341);
    }

    void renderMist()
    {
        if (!isMistEnabled(CURRENT_SCREEN)) return;

        if (CURRENT_SCREEN != 220 && isTriggerSet(148))
        {
            MIST_CLUT_Y[0] = 192;
            MIST_CLUT_Y[1] = 128;
        }

        auto cameraXDiff = CAMERA_X_PREVIOUS - CAMERA_X;
        auto cameraYDiff = CAMERA_Y_PREVIOUS - CAMERA_Y;

        mistOffsetX[0] += cameraXDiff;
        if (PLAYTIME_FRAMES % 2 == 0) mistOffsetX[0]--;
        mistOffsetX[0] = ring(mistOffsetX[0], -480, 160);
        mistOffsetX[1] = ring(mistOffsetX[0] + 320, -480, 160);

        mistOffsetX[2] += (cameraXDiff * 12 / 10);
        if (PLAYTIME_FRAMES % 2 == 0) mistOffsetX[2]++;
        mistOffsetX[2] = ring(mistOffsetX[2], -160, 480);
        mistOffsetX[3] = ring(mistOffsetX[2] + 320, -160, 480);

        mistOffsetY[0] += cameraYDiff;
        mistOffsetY[0] = ring(mistOffsetY[0], -360, 120);
        mistOffsetY[1] = ring(mistOffsetY[0] + 240, -360, 120);

        for (int32_t i = -4; i < 4; i++)
        {
            POLY_FT4* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
            libgpu_SetPolyFT4(prim);
            libgpu_SetSemiTrans(prim, 1);
            if (i < 0)
            {
                prim->tpage = libgpu_GetTPage(0, 3, 704, 0);
                prim->clut  = getClut(0, 486);

                setPosDataPolyFT4(prim, mistOffsetX[abs(i + 4) % 2], mistOffsetY[abs(i + 4) / 2], 320, 240);
                prim->r0 = 150;
                prim->g0 = 150;
                prim->b0 = 150;
            }
            else
            {
                prim->tpage = libgpu_GetTPage(0, 1, 704, 0);
                if (CURRENT_SCREEN == 163 || CURRENT_SCREEN == 220)
                    prim->clut = getClut(0, 486);
                else if (CURRENT_SCREEN == 119)
                    prim->clut = getClut(MIST_CLUT_Y[0], 486);
                else
                    prim->clut = getClut(MIST_CLUT_Y[1], 486);

                prim->x0 = mistOffsetX[2 + (i % 2)];
                prim->x1 = mistOffsetX[2 + (i % 2)] - 320;
                prim->x2 = mistOffsetX[2 + (i % 2)];
                prim->x3 = mistOffsetX[2 + (i % 2)] - 320;

                prim->y0 = mistOffsetY[i / 2];
                prim->y1 = mistOffsetY[i / 2];
                prim->y2 = mistOffsetY[i / 2] + 240;
                prim->y3 = mistOffsetY[i / 2] + 240;

                prim->r0 = 80;
                prim->g0 = 80;
                prim->b0 = 80;
            }
            setUVDataPolyFT4(prim, 0, 0, 255, 200);
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 20, prim);
            libgs_GsSetWorkBase(prim + 1);
        }
    }

    void renderMapOverlays(LocalMapObjectInstance* objects, int32_t cameraX, int32_t cameraY)
    {
        if (MAP_LAYER_ENABLED == 0) return;
        if (hasMist(CURRENT_SCREEN)) renderMist();

        for (int32_t i = 0; i < 188; i++)
        {
            auto& data         = objects[i];
            auto currentFrame  = data.currentFrame;
            auto currentSprite = data.animSprites[currentFrame];

            if (currentFrame == -1 || currentSprite == -1) continue;

            if (currentSprite != -2 && data.flag != 1)
            {
                auto& mapObj = LOCAL_MAP_OBJECTS[currentSprite];
                renderMapOverlay(data, mapObj, cameraX, cameraY);
            }

            data.timer++;
            if (data.timer == data.animTimes[currentFrame])
            {
                data.currentFrame++;
                data.timer = 0;
                if (data.currentFrame >= 8 || data.animSprites[data.currentFrame] == -1) data.currentFrame = 0;
            }
        }
    }

    void storeMapObjectPosition(int16_t* xPtr, int16_t* yPtr, int32_t start, int32_t count)
    {
        for (int32_t i = 0; i < count; i++)
        {
            xPtr[i] = LOCAL_MAP_OBJECT_INSTANCE[start + i].x;
            yPtr[i] = LOCAL_MAP_OBJECT_INSTANCE[start + i].y;
        }
    }

    void moveMapObjects(int32_t start, int32_t count, int32_t xOffset, int32_t yOffset)
    {
        for (int32_t i = 0; i < count; i++)
        {
            LOCAL_MAP_OBJECT_INSTANCE[start + i].x += xOffset;
            LOCAL_MAP_OBJECT_INSTANCE[start + i].y += yOffset;
        }
    }

    void setMapObjectsFlag(int32_t start, int32_t count, int32_t flag)
    {
        for (int32_t i = 0; i < count; i++)
            LOCAL_MAP_OBJECT_INSTANCE[start + i].flag = flag;
    }

    void getDrawPosition(SVector* position, int16_t* outX, int16_t* outY)
    {
        libgs_GsSetLsMatrix(&libgs_REFERENCE_MATRIX);

        cop2_v0xy = position->x | (position->y << 16);
        cop2_v0z  = position->z | (position->pad << 16);

        asm volatile("cop2 0x180001");
        ScreenCoord val{.raw = static_cast<int32_t>(cop2_sxy2)};

        *outX = DRAWING_OFFSET_X + val.x + CAMERA_X;
        *outY = DRAWING_OFFSET_Y + val.y + CAMERA_Y;
    }

    bool
    moveMapObjectsWithLimit(int32_t start, int32_t count, int32_t xOffset, int32_t yOffset, int32_t maxX, int32_t maxY)
    {
        // this function only checks the limit on the first object?
        for (int32_t i = 0; i < count; i++)
        {
            LOCAL_MAP_OBJECT_INSTANCE[start + i].x += xOffset;
            LOCAL_MAP_OBJECT_INSTANCE[start + i].y += yOffset;
        }

        if (xOffset < 0 && LOCAL_MAP_OBJECT_INSTANCE[start].x <= maxX) return true;
        if (xOffset > 0 && LOCAL_MAP_OBJECT_INSTANCE[start].x >= maxX) return true;

        if (yOffset < 0 && LOCAL_MAP_OBJECT_INSTANCE[start].y <= maxY) return true;
        if (yOffset > 0 && LOCAL_MAP_OBJECT_INSTANCE[start].y >= maxY) return true;

        return false;
    }

    void loadMapObjectPosition(int16_t* xPtr, int16_t* yPtr, int32_t start, int32_t count)
    {
        for (int32_t i = 0; i < count; i++)
        {
            LOCAL_MAP_OBJECT_INSTANCE[start + i].x = xPtr[i];
            LOCAL_MAP_OBJECT_INSTANCE[start + i].y = yPtr[i];
            LOCAL_MAP_OBJECT_INSTANCE[start + i].flag &= 0xEF;
        }
    }

    void renderNinjamonEffect(int32_t instanceId)
    {
        for (int32_t i = 0; i < NINJAMON_EFFECT_COUNT; i++)
        {
            auto& data = LOCAL_MAP_OBJECT_INSTANCE[i];
            data.x += ninjamonEffecXOffset[i];
            data.y += ninjamonEffecYOffset[i];
            data.orderValue = 10;

            if (data.x > 160)
            {
                data.x                  = ninjamonEffecX[i];
                data.y                  = ninjamonEffecY[i];
                ninjamonEffecXOffset[i] = random(10) + 12;
                ninjamonEffecYOffset[i] = random(10) + 3;
            }
        }

        NPC_ACTIVE_ANIM[0]++;
        if (NPC_ACTIVE_ANIM[0] >= 60)
        {
            for (int32_t i = 0; i < NINJAMON_EFFECT_COUNT; i++)
            {
                LOCAL_MAP_OBJECT_INSTANCE[i].flag = 1;
            }
            removeObject(ObjectID::NINJAMON_EFFECT, 0);
        }
    }

    void createNinjamonEffect()
    {
        NPC_ACTIVE_ANIM[0] = 0;
        storeMapObjectPosition(ninjamonEffecX, ninjamonEffecY, 0, NINJAMON_EFFECT_COUNT);

        for (int32_t i = 0; i < NINJAMON_EFFECT_COUNT; i++)
        {
            ninjamonEffecXOffset[i] = random(10) + 12;
            ninjamonEffecYOffset[i] = random(10) + 3;
        }

        addObject(ObjectID::NINJAMON_EFFECT, 0, nullptr, renderNinjamonEffect);
    }

    bool tickMoveObjectTo(int32_t objectId, int32_t actorId, int32_t speed, int32_t posX, int32_t posY)
    {
        auto& data = LOCAL_MAP_OBJECT_INSTANCE[objectId];

        if (MAP_OBJECT_MOVE_TO_DATA[actorId] == 0)
        {
            moveObjectDeltaX[actorId]        = posX - data.x / speed;
            moveObjectDeltaY[actorId]        = posY - data.y / speed;
            MAP_OBJECT_MOVE_TO_DATA[actorId] = 1;
        }

        auto deltaX = moveObjectDeltaX[actorId];
        auto deltaY = moveObjectDeltaY[actorId];
        data.x += deltaX;
        data.y += deltaY;

        if (deltaX < 0 && data.x <= posX)
            data.x = posX;
        else if (deltaX > 0 && data.x >= posX)
            data.x = posX;
        else if (deltaX == 0)
            data.x = posX;

        if (deltaY < 0 && data.y <= posY)
            data.y = posY;
        else if (deltaY > 0 && data.y >= posY)
            data.y = posY;
        else if (deltaY == 0)
            data.y = posY;

        if (data.x == posX && data.y == posY)
        {
            MAP_OBJECT_MOVE_TO_DATA[actorId] = 0;
            return true;
        }

        return false;
    }

    constexpr int32_t getFirstSpriteIndex(LocalMapObjectInstance* instance)
    {
        for (auto val : instance->animSprites)
            if (val != -2) return val;

        return -1;
    }

    void spawnSpriteAtLocation(int16_t x, int16_t y, int16_t z, int32_t sprite, int16_t count)
    {
        // TODO this function breaks when trying to add more than 30 sprites
        SVector pos{.x = x, .y = y, .z = z};
        int16_t drawX;
        int16_t drawY;
        int16_t xPtr[30];
        int16_t yPtr[30];

        getDrawPosition(&pos, &drawX, &drawY);

        for (int32_t i = 0; i < count; i++)
        {
            int32_t spriteIndex = getFirstSpriteIndex(&LOCAL_MAP_OBJECT_INSTANCE[i]);

            // vanilla doesn't check if no sprite got found and thus potentially trashes its stack
            if (spriteIndex == -1) continue;

            auto& obj = LOCAL_MAP_OBJECTS[spriteIndex];
            xPtr[i]   = drawX - obj.width / 2;
            yPtr[i]   = drawY - obj.height / 2;
        }

        loadMapObjectPosition(xPtr, yPtr, sprite, count);
    }

    void spawnSpriteAtEntity(int32_t scriptId, int32_t nodeId, int32_t sprite)
    {
        auto* entity = getEntityFromScriptId(scriptId).first;
        auto* pos    = entity->posData[nodeId].posMatrix.work.t;

        spawnSpriteAtLocation(pos[0], pos[1], pos[2], sprite, 1);
    }

    void resetMapObjectAnimation(int32_t start, int32_t count)
    {
        for (int32_t i = 0; i < count; i++)
        {
            LOCAL_MAP_OBJECT_INSTANCE[start + i].timer        = 0;
            LOCAL_MAP_OBJECT_INSTANCE[start + i].currentFrame = 0;
        }
    }

    bool tickRemoveMist()
    {
        MIST_CLUT_Y[0] += 0x10;
        MIST_CLUT_Y[1] += 0x10;

        if (MIST_CLUT_Y[0] >= 192)
        {
            MIST_CLUT_Y[0] = 192;
            MIST_CLUT_Y[1] = 128;
            return true;
        }

        return false;
    }

    void loadMapCollisionData(uint8_t* data)
    {
        memcpy(MAP_COLLISION_DATA, data, 10000);
    }

    int32_t getTileTrigger(Vector* position)
    {
        auto tileX = getTileX(position->x);
        auto tileZ = getTileZ(position->z);
        auto data  = getTile(tileX, tileZ);

        if (data == 0x80) return -1;

        return data;
    }

    void setImpassableRect(int32_t tileX, int32_t tileY, int32_t width, int32_t height)
    {
        for (int32_t y = 0; y < height; y++)
        {
            auto curY = tileY + y;
            if (curY < 0 || curY >= 100) continue;

            for (int32_t x = 0; x < width; x++)
            {
                auto curX = tileX + x;
                if (curX < 0 || curX >= 100) continue;

                MAP_COLLISION_DATA[curY * 100 + curX] = 0x80;
            }
        }
    }

    void setImpassableSquare(int32_t tileX, int32_t tileY, int32_t radius)
    {
        setImpassableRect(tileX - radius, tileY - radius, radius * 2, radius * 2);
    }

    static void renderDroppedItemShadow(WorldItem* item)
    {
        auto posX = item->spriteLocation.x;
        auto posZ = item->spriteLocation.z;

        SVector point1 = {.x = static_cast<int16_t>(posX - 100), .y = 0, .z = static_cast<int16_t>(posZ - 100)};
        SVector point2 = {.x = static_cast<int16_t>(posX + 100), .y = 0, .z = static_cast<int16_t>(posZ - 100)};
        SVector point3 = {.x = static_cast<int16_t>(posX - 100), .y = 0, .z = static_cast<int16_t>(posZ + 100)};
        SVector point4 = {.x = static_cast<int16_t>(posX + 100), .y = 0, .z = static_cast<int16_t>(posZ + 100)};

        ScreenCoord pos1;
        ScreenCoord pos2;
        ScreenCoord pos3;
        Position screenPos;
        int32_t interpolVal;
        int32_t flag;

        libgte_RotTransPers3(&point1, &point2, &point3, &pos1.raw, &pos2.raw, &pos3.raw, &interpolVal, &flag);
        worldPosToScreenPos(&point4, &screenPos);

        POLY_FT4* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyFT4(prim);
        libgpu_SetSemiTrans(prim, 1);
        setPolyFT4UV(prim, 64, 128, 63, 63);
        prim->tpage = 0xDD;
        prim->clut  = getClut(0, 0x1e7);
        prim->r0    = 0x30;
        prim->g0    = 0x30;
        prim->b0    = 0x30;
        prim->x0    = pos1.x;
        prim->y0    = pos1.y;
        prim->x1    = pos2.x;
        prim->y1    = pos2.y;
        prim->x2    = pos3.x;
        prim->y2    = pos3.y;
        prim->x3    = screenPos.x;
        prim->y3    = screenPos.y;
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 0xFFD, prim);
        libgs_GsSetWorkBase(prim + 1);
    }

    void renderOverworldItem(WorldItem* item)
    {
        Position screenPos;

        auto depth = worldPosToScreenPos(&item->spriteLocation, &screenPos);
        auto width = (VIEWPORT_DISTANCE << 7) / depth;
        auto layer = depth >> 4;

        if (layer >= 0x1000 && layer <= 0) return;

        POLY_FT4* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyFT4(prim);
        prim->r0    = 0x80;
        prim->g0    = 0x80;
        prim->b0    = 0x80;
        prim->tpage = 5;
        setItemTexture(prim, item->type);
        // vanilla modifies the UV when width is larger than 32, but that seems to be redundant?

        setPosDataPolyFT4(prim, screenPos.x - width / 2, screenPos.y - width / 2, width, width);
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + layer, prim);
        libgs_GsSetWorkBase(prim + 1);
    }

    void initializeDroppedItems()
    {
        TAMER_ITEM.type = ItemType::NONE;
        for (auto& item : DROPPED_ITEMS)
            item.type = ItemType::NONE;
    }

    static void renderDroppedItem(int32_t instanceId)
    {
        if (!MAP_LAYER_ENABLED) return;

        renderOverworldItem(&DROPPED_ITEMS[instanceId]);
        renderDroppedItemShadow(&DROPPED_ITEMS[instanceId]);
    }

    void spawnDroppedItem(Entity* entity, ItemType item)
    {
        for (int32_t i = 0; i < 10; i++)
        {
            auto& data = DROPPED_ITEMS[i];
            if (data.type != ItemType::NONE) continue;

            data.type             = item;
            data.spriteLocation.x = entity->posData->location.x;
            data.spriteLocation.y = 0;
            data.spriteLocation.z = entity->posData->location.z;
            getModelTile(&entity->posData->location, &data.tileX, &data.tileY);
            addObject(ObjectID::DROPPED_ITEM, i, nullptr, renderDroppedItem);
            return;
        }
    }

    void spawnItem(ItemType itemId, int16_t tileX, int16_t tileY)
    {
        for (int32_t i = 0; i < 10; i++)
        {
            auto& data = DROPPED_ITEMS[i];
            if (data.type != ItemType::NONE) continue;

            data.type             = itemId;
            data.tileX            = tileX;
            data.tileY            = tileY;
            data.spriteLocation.x = (tileX - 50) * 100 + 50;
            data.spriteLocation.y = TAMER_ENTITY.posData->location.y; // why not 0?
            data.spriteLocation.z = (50 - tileY) * 100 - 50;
            addObject(ObjectID::DROPPED_ITEM, i, nullptr, renderDroppedItem);
            return;
        }
    }

    void deleteDroppedItem(int32_t instanceId)
    {
        removeObject(ObjectID::DROPPED_ITEM, instanceId);
        DROPPED_ITEMS[instanceId].type = ItemType::NONE;
    }

    void clearDroppedItems()
    {
        for (int32_t i = 0; i < 10; i++)
            if (DROPPED_ITEMS[i].type != ItemType::NONE) deleteDroppedItem(i);
    }

    bool pickupItem(int32_t dropId)
    {
        if (giveItem(DROPPED_ITEMS[dropId].type, 1))
        {
            deleteDroppedItem(dropId);
            return true;
        }
        return false;
    }

    bool isTileOffScreen(int16_t tileX, int16_t tileZ)
    {
        auto pos = getScreenPosition(tileX, tileZ);

        return !isInScreenRect(pos, 400, 320);
    }

    TileCoord getClosestTileOffScreen(int8_t startX, int8_t startY, int8_t targetX, int8_t targetY)
    {
        TileIterator itr(startX, startY, targetX, targetY);

        for (; itr.hasNext(); ++itr)
        {
            auto val = *itr;
            if (isTileOffScreen(val.tileX, val.tileY)) return val;
        }

        return {-1, -1};
    }

    bool isLinearPathBlocked(int32_t tileX1, int32_t tileY1, int32_t tileX2, int32_t tileY2)
    {
        tileX1 = clamp(tileX1, 0, 99);
        tileY1 = clamp(tileY1, 0, 99);
        tileX2 = clamp(tileX2, 0, 99);
        tileY2 = clamp(tileY2, 0, 99);

        if (tileX1 == tileX2 && tileY1 == tileY2) return false;

        TileIterator itr(tileX1, tileY1, tileX2, tileY2);
        for (; itr.hasNext(); ++itr)
        {
            auto val = *itr;

            if ((getTile(val.tileX, val.tileY) & 0x80) != 0) return true;
        }

        return false;
    }

    bool isFiveTileWidePathBlocked(int32_t tileX1, int32_t tileY1, int32_t tileX2, int32_t tileY2)
    {
        if (abs(tileX2 - tileX1) < abs(tileY2 - tileY1))
        {
            for (int32_t i = -2; i < 3; i++)
            {
                if (isLinearPathBlocked(tileX1 + i, tileY1, tileX2 + i, tileY2)) return true;
            }
        }
        else
        {
            for (int32_t i = -2; i < 3; i++)
            {
                if (isLinearPathBlocked(tileX1, tileY1 + i, tileX2, tileY2 + i)) return true;
            }
        }

        return false;
    }

    static void setMapTilePosData(POLY_FT4* prim, const MapTileData& data, int32_t offsetX, int32_t offsetY)
    {
        auto posX = (data.posX - 160) - (offsetX - (160 - DRAWING_OFFSET_X));
        auto posY = (data.posY - 120) - (offsetY - (120 - DRAWING_OFFSET_Y));

        setPosDataPolyFT4(prim, posX, posY, 128, 128);
    }

    static void renderMap(int32_t instanceId)
    {
        tickCameraFollowPlayer();

        auto& mapEntry = MAP_ENTRIES[CURRENT_SCREEN];

        if ((mapEntry.flags & 0x40) == 0 && (CURRENT_FRAME % 1200) == 0)
        {
            if (HOUR == 16) initializeDaytimeTransition(0);
            if (HOUR == 20) initializeDaytimeTransition(1);
            if (HOUR == 6) initializeDaytimeTransition(2);
        }

        for (int32_t i = 0; i < 3; i++)
        {
            for (int32_t j = 0; j < min(4, MAP_WIDTH); j++)
            {
                auto tileId    = (MAP_TILE_X + MAP_TILE_Y * MAP_WIDTH) + (MAP_WIDTH * i) + j;
                auto& tileData = MAP_TILE_DATA[tileId];

                POLY_FT4* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
                libgpu_SetPolyFT4(prim);

                prim->r0 = MAP_TILES[tileId] == 0xFF ? 0 : 128;
                prim->g0 = MAP_TILES[tileId] == 0xFF ? 0 : 128;
                prim->b0 = MAP_TILES[tileId] == 0xFF ? 0 : 128;
                setUVDataPolyFT4(prim, 0, tileData.texV, 128, (tileData.texV % 256) == 0 ? 128 : 127);
                prim->tpage = tileData.tpage;
                prim->clut  = tileData.clut;
                setMapTilePosData(prim, tileData, CAMERA_X, CAMERA_Y);

                libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 0xFFF, prim);
                libgs_GsSetWorkBase(prim + 1);
            }
        }

        renderMapOverlays(LOCAL_MAP_OBJECT_INSTANCE, CAMERA_X, CAMERA_Y);
        CAMERA_X_PREVIOUS = CAMERA_X;
        CAMERA_Y_PREVIOUS = CAMERA_Y;
    }

    void initializeMap()
    {
        for (auto& val : MAP_TILES)
            val = 0xFF;

        for (auto& val : MAP_TILE_DATA)
        {
            val.imagePtr = nullptr;
            val.posX     = 0;
            val.posY     = 0;
            val.texU     = 0;
            val.texV     = 0;
        }

        CURRENT_SCREEN            = 0xCC;
        PREVIOUS_SCREEN           = 0xCC;
        CURRENT_EXIT              = 9;
        PREVIOUS_EXIT             = 9;
        CAMERA_REACHED_TARGET     = 0xFF;
        CAMERA_HAS_TARGET         = false;
        DAYTIME_TRANSITION_FRAME  = 25;
        CURRENT_TIME_OF_DAY       = 0;
        CAMERA_UPDATE_TILES       = 0;
        SKIP_MAP_FILE_READ        = 0; // TODO remove, useless
        DAYTIME_TRANSITION_ACTIVE = false;
        SKIP_DAYTIME_TRANSITION   = 0;

        addObject(ObjectID::MAP, 0, nullptr, renderMap);
    }

    uint8_t getMapSoundId(uint32_t mapId)
    {
        return MAP_ENTRIES[mapId].flags & 0x1F;
    }

    static void loadMapSetup(MapSetup* map)
    {
        GS_VIEWPOINT.viewpointX = map->viewpoint.x * 2;
        GS_VIEWPOINT.viewpointY = map->viewpoint.y * 2;
        GS_VIEWPOINT.viewpointZ = map->viewpoint.z * 2;
        GS_VIEWPOINT.refpointX  = map->referencePoint.x * 2;
        GS_VIEWPOINT.refpointY  = map->referencePoint.y * 2;
        GS_VIEWPOINT.refpointZ  = map->referencePoint.z * 2;
        libgs_GsSetRefView2(&GS_VIEWPOINT);

        for (int32_t i = 0; i < map->lights.size(); i++)
        {
            LIGHT_DATA[i].r = map->lights[i].red;
            LIGHT_DATA[i].g = map->lights[i].green;
            LIGHT_DATA[i].b = map->lights[i].blue;
            LIGHT_DATA[i].x = map->lights[i].pos.x;
            LIGHT_DATA[i].y = map->lights[i].pos.y;
            LIGHT_DATA[i].z = map->lights[i].pos.z;
            libgs_GsSetFlatLight(i, &LIGHT_DATA[i]);
            MAP_LIGHT[i] = LIGHT_DATA[i];
        }

        // TODO read from unk1-3?
        libgs_GsSetAmbient(2048, 2048, 2048);

        VIEWPORT_DISTANCE = map->viewDistance;
        libgs_GsSetProjection(VIEWPORT_DISTANCE);

        // TODO this should be extracted into the partner code somehow
        auto region           = getRaiseData(PARTNER_ENTITY.type)->favoredRegion;
        PARTNER_AREA_RESPONSE = 0;
        for (int32_t i = 0; i < map->likedArea.size(); i++)
        {
            if (map->likedArea[i] == region) PARTNER_AREA_RESPONSE = 1;
            if (map->dislikedArea[i] == region) PARTNER_AREA_RESPONSE = 2;
        }

        MAP_WIDTH  = map->width;
        MAP_HEIGHT = map->height;
        for (int32_t i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
            MAP_TILES[i] = map->tiles[i];
    }

    void loadMap(uint32_t mapId)
    {
        auto& entry = MAP_ENTRIES[mapId];
        uint8_t path[64];

        sprintf(path, "\\MAP\\MAP%d\\%s.MAP", (mapId / 15) + 1, entry.filename);
        readFile(reinterpret_cast<char*>(path), &GENERAL_BUFFER);

        uint32_t* headerPtr = reinterpret_cast<uint32_t*>(&GENERAL_BUFFER);
        MapSetup* setup     = reinterpret_cast<MapSetup*>(&GENERAL_BUFFER + *headerPtr++);

        loadMapSetup(setup);
        clearMapObjects(LOCAL_MAP_OBJECT_INSTANCE);

        if (entry.num4bppImages != 0 || entry.num8bppImages != 0)
        {
            for (int32_t i = 0; i < entry.num8bppImages; i++)
                loadMapImage1(&GENERAL_BUFFER + *headerPtr++);
            for (int32_t i = 0; i < entry.num4bppImages; i++)
                loadMapImage2(&GENERAL_BUFFER + *headerPtr++, i);

            loadMapObjects(LOCAL_MAP_OBJECT_INSTANCE, &GENERAL_BUFFER + *headerPtr++, mapId);
        }

        clearMapDigimon();
        loadMapEntities(&GENERAL_BUFFER + *headerPtr++, mapId, CURRENT_EXIT);

        if (entry.doorsId != 0) { loadDoors(entry.doorsId - 1); }
        if (mapId > 100 && mapId < 104) initializeWarpCrystals(mapId);
        if (mapId == 165) initializeTrainingPoop();

        loadMapCollisionData(&GENERAL_BUFFER + *headerPtr++);

        loadMapSounds(getMapSoundId(mapId));
        initializePartnerWaypoint();
        initializeTamerWaypoints();
        checkFishingMap(mapId);
        checkCurlingMap(mapId);
        // checkShopMap(mapId); // unused as of vanilla 1.1
        checkArenaMap(mapId);

        CURRENT_SCREEN = mapId;
    }

    void readMapTFS(uint8_t mapId)
    {
        auto& entry = MAP_ENTRIES[mapId];
        uint8_t path[64];

        sprintf(path, "\\MAP\\MAP%d\\%s.TFS", (mapId / 15) + 1, entry.filename);
        readFile(reinterpret_cast<char*>(path), &GENERAL_BUFFER);
    }

    void setupMap()
    {
        ReadBuffer buff{&GENERAL_BUFFER};

        auto width        = buff.read<uint16_t>();
        auto height       = buff.read<uint16_t>();
        auto paletteCount = buff.read<uint32_t>();

        for (int32_t i = 0; i < paletteCount; i++)
        {
            MAP_CLUTS[i] = reinterpret_cast<uint16_t*>(buff.buffer);
            libgpu_LoadClut(buff.buffer, 0, 0x1E1 + i);
            buff.skip(sizeof(uint16_t) * 256);
        }

        updateTimeOfDay();

        for (int32_t y = 0; y < MAP_HEIGHT; y++)
        {
            for (int32_t x = 0; x < MAP_WIDTH; x++)
            {
                auto& tileData  = MAP_TILE_DATA[y * MAP_WIDTH + x];
                tileData.tileId = MAP_TILES[y * MAP_WIDTH + x];

                buff.skip(4); // tile X/Y offset? Unused?
                uint8_t* tileBuffer = nullptr;

                if (tileData.tileId != -1)
                {
                    tileBuffer = buff.buffer;
                    buff.skip(128 * 128);
                }
                fillTileData(&tileData, tileBuffer, x % 4 * 0x40 + 0x300, (y % 3) * 128, x * 128, y * 128);
            }
        }

        DRAW_OFFSET_LIMIT_X_MAX = MAP_WIDTH * 0x80 / 2;
        DRAW_OFFSET_LIMIT_Y_MAX = MAP_HEIGHT * 0x80 / 2;
        DRAW_OFFSET_LIMIT_X_MIN = -((MAP_WIDTH * 0x80 - 320) - DRAW_OFFSET_LIMIT_X_MAX);
        DRAW_OFFSET_LIMIT_Y_MIN = -((MAP_HEIGHT * 0x80 - 240) - DRAW_OFFSET_LIMIT_Y_MAX);
        CAMERA_X                = DRAW_OFFSET_LIMIT_X_MAX - 160;
        CAMERA_Y                = DRAW_OFFSET_LIMIT_Y_MAX - 120;

        DRAWING_OFFSET_X = 160;
        DRAWING_OFFSET_Y = 120;
        PLAYER_OFFSET_X  = 160;
        PLAYER_OFFSET_Y  = 120;
        initializeDrawingOffsets();

        MAP_TILE_X = CAMERA_X / 128;
        if (MAP_WIDTH < 5)
            MAP_TILE_X = 0;
        else if (MAP_TILE_X + 4 > MAP_WIDTH)
            MAP_TILE_X -= (MAP_TILE_X + 4 - MAP_WIDTH);

        MAP_TILE_Y = CAMERA_Y / 128;
        if (MAP_HEIGHT < 4)
            MAP_TILE_Y = 0;
        else if (MAP_TILE_Y + 3 > MAP_HEIGHT)
            MAP_TILE_Y -= (MAP_TILE_Y + 3 - MAP_HEIGHT);

        PREV_TILE_X = MAP_TILE_X;
        PREV_TILE_Y = MAP_TILE_Y;

        uploadMapTileImages(MAP_TILE_DATA.data(), MAP_TILE_X + MAP_TILE_Y * MAP_WIDTH);
        calcMapObjectOrder(LOCAL_MAP_OBJECT_INSTANCE);
        CAMERA_FOLLOW_PLAYER = 1;
    }

    void updateTimeOfDay()
    {
        if (DAYTIME_TRANSITION_ACTIVE) removeObject(ObjectID::DAYTIME_TRANSITION, DAYTIME_TRANSITION_TARGET);

        if ((MAP_ENTRIES[CURRENT_SCREEN].flags & 0x40) != 0) { libgpu_LoadClut(MAP_CLUTS[0], 0, 0x1E0); }
        else
        {
            auto factor = 10;

            if (HOUR >= 16 && HOUR <= 19)
            {
                libgpu_LoadClut(MAP_CLUTS[1], 0, 0x1E0);
                factor              = 7;
                CURRENT_TIME_OF_DAY = 0;
            }
            else if (HOUR < 6 || HOUR > 19)
            {
                factor = 5;
                libgpu_LoadClut(MAP_CLUTS[2], 0, 0x1E0);
                CURRENT_TIME_OF_DAY = 1;
            }
            else
            {
                factor = 10;

                libgpu_LoadClut(MAP_CLUTS[0], 0, 0x1E0);
                CURRENT_TIME_OF_DAY = 2;
            }
        }

        DAYTIME_TRANSITION_FRAME  = 0x19;
        DAYTIME_TRANSITION_ACTIVE = false;
        SKIP_DAYTIME_TRANSITION   = 0;
    }

}