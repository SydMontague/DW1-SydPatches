#include "GameObjects.hpp"
#include "Math.hpp"
#include "extern/dtl/types.hpp"

namespace
{
    struct FrameData
    {
        int16_t y;
        uint8_t u;
        uint8_t color;
        int16_t scale;
    };

    constexpr dtl::array<FrameData, 14> frameData{{
        {.y = -50, .u = 0, .color = 0x80, .scale = 0x2000},
        {.y = -54, .u = 0, .color = 0x80, .scale = 0x2EE0},
        {.y = -58, .u = 32, .color = 0x80, .scale = 0x3840},
        {.y = -62, .u = 32, .color = 0x80, .scale = 0x4000},
        {.y = -66, .u = 32, .color = 0x79, .scale = 0x4000},
        {.y = -70, .u = 64, .color = 0x73, .scale = 0x5000},
        {.y = -74, .u = 64, .color = 0x6C, .scale = 0x5000},
        {.y = -78, .u = 64, .color = 0x66, .scale = 0x5000},
        {.y = -82, .u = 64, .color = 0x60, .scale = 0x5000},
        {.y = -86, .u = 64, .color = 0x59, .scale = 0x5400},
        {.y = -90, .u = 96, .color = 0x53, .scale = 0x5800},
        {.y = -93, .u = 96, .color = 0x4C, .scale = 0x5C00},
        {.y = -96, .u = 96, .color = 0x46, .scale = 0x6000},
        {.y = -99, .u = 96, .color = 0x40, .scale = 0x6400},
    }};

    struct CloudFXData
    {
        int16_t frame;
        int16_t posX;
        int16_t posZ;
    };

    dtl::array<CloudFXData, 60> cloudFXData;
    GsSPRITE cloudFXSprite = {
        .attribute = 0x50000000,
        .x         = 0,
        .y         = 0,
        .width     = 32,
        .height    = 32,
        .tpage     = 0x3C,
        .u         = 0,
        .v         = 128,
        .clutX     = 192,
        .clutY     = 488,
        .r         = 128,
        .g         = 128,
        .b         = 128,
        .mx        = 16,
        .my        = 16,
        .scaleX    = 0,
        .scaleY    = 0,
        .rotate    = 0,
    };

    void renderCloudFX(int32_t instanceId)
    {
        auto& data        = cloudFXData[instanceId];
        const auto& fData = frameData[data.frame];
        auto mapPos       = getMapPosition(data.posX, fData.y, data.posZ);

        cloudFXSprite.u      = fData.u;
        cloudFXSprite.r      = fData.color;
        cloudFXSprite.g      = fData.color;
        cloudFXSprite.b      = fData.color;
        cloudFXSprite.x      = mapPos.screenX;
        cloudFXSprite.y      = mapPos.screenY;
        cloudFXSprite.scaleX = (fData.scale * VIEWPORT_DISTANCE) / mapPos.depth;
        cloudFXSprite.scaleY = (fData.scale * VIEWPORT_DISTANCE) / mapPos.depth;

        // vanilla calls renderSprite (0x800da36c) here, but we inlined it.
        // It modified the passed sprite, which is kinda dirty.
        auto origin = mapPos.depth / 16;
        if (origin >= 0 && origin < 0x1000) libgs_GsSortSprite(&cloudFXSprite, ACTIVE_ORDERING_TABLE, origin);
    }

    void tickCloudFX(int32_t instanceId)
    {
        auto& data = cloudFXData[instanceId];
        data.frame++;

        if (data.frame > 13)
        {
            data.frame = -1;
            removeObject(ObjectID::CLOUD_FX, instanceId);
        }
    }
} // namespace

void initializeCloudFXData()
{
    for (auto& data : cloudFXData)
        data.frame = -1;
}

extern "C"
{
    void removeAllCloudFX()
    {
        int32_t instanceId = 0;
        for (auto& data : cloudFXData)
        {
            if (data.frame != -1) removeObject(ObjectID::CLOUD_FX, instanceId);
            instanceId++;
        }
    }

    void createCloudFX(SVector* position)
    {
        int32_t instanceId = 0;
        for (auto& data : cloudFXData)
        {
            if (data.frame == -1)
            {
                data.frame = 0;
                data.posX  = position->x;
                data.posZ  = position->z;
                addObject(ObjectID::CLOUD_FX, instanceId, tickCloudFX, renderCloudFX);
                return;
            }
            instanceId++;
        }
    }
}