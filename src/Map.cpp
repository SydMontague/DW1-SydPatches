#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "Partner.hpp"
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

    void loadMapObjects(LocalMapObjectInstance* mapObjects, int16_t* data, uint32_t mapId)
    {
        int16_t count  = *data;
        MapObject* obj = reinterpret_cast<MapObject*>(data + 1);

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
}