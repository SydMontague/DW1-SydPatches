#include "Math.hpp"
#include "Partner.hpp"
#include "Tamer.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"
#include "extern/stddef.hpp"

extern "C"
{
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
                return;
            }

            SVector worldPos;
            Position screenPos;
            for (auto val : data.animSprites)
            {
                if (val == 0xFFFF || val == 0xFFFE) continue;

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
}