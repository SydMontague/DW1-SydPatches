#include "Effects.hpp"

#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    struct EntityParticleFXData
    {
        int16_t counter = -1;
        uint16_t bone;
        Entity* entity;
        uint16_t mode;
        uint16_t pad;
    };

    struct EntityParticleFXMode
    {
        uint8_t scaleX;
        uint8_t scaleY;
        uint8_t x;
        uint8_t y;
        uint8_t width;
        uint8_t height;
    };

    struct CloudFXData
    {
        int16_t frame;
        int16_t posX;
        int16_t posZ;
    };

    constexpr RGB8 particleColors[7] = {
        {.red = 0xFF, .green = 0x6E, .blue = 0x46},
        {.red = 0xFF, .green = 0x6E, .blue = 0x46},
        {.red = 0xC8, .green = 0xC8, .blue = 0x64},
        {.red = 0xCC, .green = 0x00, .blue = 0x88},
        {.red = 0x32, .green = 0x32, .blue = 0xFF},
        {.red = 0x96, .green = 0x96, .blue = 0x96},
        {.red = 0xFF, .green = 0x6E, .blue = 0x6E},
    };
    constexpr EntityParticleFXMode entityParticleFXModes[9] = {
        {.scaleX = 0x78, .scaleY = 0x50, .x = 0x80, .y = 0x80, .width = 0x0F, .height = 0x0F},
        {.scaleX = 0x78, .scaleY = 0x50, .x = 0xC0, .y = 0x80, .width = 0x0F, .height = 0x0F},
        {.scaleX = 0xC8, .scaleY = 0x50, .x = 0x80, .y = 0x90, .width = 0x1F, .height = 0x07},
        {.scaleX = 0xC8, .scaleY = 0x96, .x = 0x80, .y = 0x80, .width = 0x0F, .height = 0x0F},
        {.scaleX = 0xC8, .scaleY = 0x96, .x = 0xC0, .y = 0x80, .width = 0x0F, .height = 0x0F},
        {.scaleX = 0xFA, .scaleY = 0x64, .x = 0x80, .y = 0x90, .width = 0x1F, .height = 0x07},
        {.scaleX = 0x64, .scaleY = 0x64, .x = 0x80, .y = 0x80, .width = 0x0F, .height = 0x0F},
        {.scaleX = 0x64, .scaleY = 0x64, .x = 0xC0, .y = 0x80, .width = 0x0F, .height = 0x0F},
        {.scaleX = 0xC8, .scaleY = 0x64, .x = 0x80, .y = 0x90, .width = 0x1F, .height = 0x07},
    };
    constexpr uint8_t flashCountMapping[4]    = {2, 3, 2, 0};
    constexpr uint8_t particleCountMapping[4] = {25, 48, 20, 0};
    constexpr int16_t cloudYData[14]          = {-50, -54, -58, -62, -66, -70, -74, -78, -82, -86, -90, -93, -96, -99};
    constexpr uint8_t cloudUData[14]          = {0, 0, 32, 32, 32, 64, 64, 64, 64, 64, 96, 96, 96, 96};
    constexpr uint8_t cloudColorData[14] =
        {0x80, 0x80, 0x80, 0x80, 0x79, 0x73, 0x6C, 0x66, 0x60, 0x59, 0x53, 0x4C, 0x46, 0x40};
    constexpr int16_t cloudScaleData[14] = {0x2000,
                                            0x2EE0,
                                            0x3840,
                                            0x4000,
                                            0x4000,
                                            0x5000,
                                            0x5000,
                                            0x5000,
                                            0x5000,
                                            0x5400,
                                            0x5800,
                                            0x5C00,
                                            0x6000,
                                            0x6400};

    static EntityParticleFXData entityParticleFXData[20];
    static ParticleFXData particleFXData[4];
    static SPosition3D fxParticleData[50];
    static CloudFXData cloudFXData[60];

    static GsSPRITE cloudFXSprite = {
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

    void removeEntityParticleFX(int32_t instanceId)
    {
        entityParticleFXData[instanceId].counter = -1;
        removeObject(ObjectID::ENTITY_PARTICLE_FX, instanceId);
    }

    void renderEntityParticleFX(int32_t instanceId)
    {
        auto& data     = entityParticleFXData[instanceId];
        auto& modeData = entityParticleFXModes[data.mode];
        SVector bonePos;
        Position screenPos;
        bonePos.x  = data.entity->posData->posMatrix.work.t[0];
        bonePos.y  = data.entity->posData->posMatrix.work.t[1];
        bonePos.z  = data.entity->posData->posMatrix.work.t[2];
        auto depth = worldPosToScreenPos(&bonePos, &screenPos);

        POLY_FT4* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyFT4(prim);
        libgpu_SetSemiTrans(prim, 1);
        prim->r0    = 0x80;
        prim->g0    = 0x80;
        prim->b0    = 0x80;
        prim->tpage = 0x3C;
        prim->clut  = 0x7A8C;

        auto counter = data.counter % 4;

        prim->u0 = modeData.x + (modeData.width + 1) * counter;
        prim->v0 = modeData.y;

        prim->u1 = modeData.x + (modeData.width + 1) * (counter + 1);
        prim->v1 = modeData.y;

        prim->u2 = modeData.x + (modeData.width + 1) * counter;
        prim->v2 = modeData.y + modeData.height;

        prim->u3 = modeData.x + (modeData.width + 1) * (counter + 1);
        prim->v3 = modeData.y + modeData.height;

        addFXPrim(prim, screenPos.x, screenPos.y, modeData.scaleX, modeData.scaleY, depth);
    }

    void tickEntityParticleFX(int32_t instanceId)
    {
        auto& data = entityParticleFXData[instanceId];

        if (data.counter <= 0)
        {
            removeEntityParticleFX(instanceId);
            return;
        }

        if (data.counter % 4 == 0)
        {
            data.bone = random2(getDigimonData(data.entity->type)->boneCount - 1) + 1;
            data.mode = random2(9);
        }
        data.counter--;
    }

    void initializeEntityParticleFX()
    {
        for (auto& data : entityParticleFXData)
            data.counter = -1;
    }

    void addEntityParticleFX(Entity* entity, int32_t counter)
    {
        int32_t i = 0;
        for (auto& data : entityParticleFXData)
        {
            if (data.counter < 0)
            {
                data.counter = counter;
                data.bone    = random2(getDigimonData(entity->type)->boneCount - 1) + 1;
                data.mode    = 0;
                data.entity  = entity;
                addObject(ObjectID::ENTITY_PARTICLE_FX, i, tickEntityParticleFX, renderEntityParticleFX);
            }

            i++;
        }
    }

    void initializeParticleFX()
    {
        for (ParticleFXData& data : particleFXData)
            data.tickCount = -1;

        for (SPosition3D& data : fxParticleData)
        {
            Matrix matrix;
            SVector rotation;
            rotation.x = ((random2(128) - 64) << 6);
            rotation.y = ((random2(128) - 64) << 6);
            rotation.z = ((random2(128) - 64) << 6);
            SVector vec2;
            vec2.x = 0;
            vec2.y = 0;
            vec2.z = (random2(150) + 250) * 16;

            libgte_RotMatrixZYX(&rotation, &matrix);
            libgte_ApplyMatrixSV(&matrix, &vec2, &vec2);

            data.x = vec2.x;
            data.y = vec2.y;
            data.z = vec2.z;
        }
    }

    inline ParticleFXData* getFreeParticleFXData(int32_t* id)
    {
        int32_t i = 0;
        for (auto& data : particleFXData)
        {
            if (data.tickCount == -1)
            {
                if (id != nullptr) *id = i;
                return &data;
            }
            i++;
        }

        return nullptr;
    }

    POLY_FT4* renderParticleFlashLoop(POLY_FT4* prim,
                                      ParticleFlashData* data,
                                      int32_t finalScaledX,
                                      int32_t finalScaledY,
                                      int32_t loopCount,
                                      int32_t innerLoopCount,
                                      int32_t heightStep,
                                      int32_t widthStep,
                                      int32_t uStep,
                                      int32_t vStep,
                                      uint32_t red,
                                      uint32_t green,
                                      uint32_t blue)
    {
        for (int32_t i = 0; i <= loopCount; i++)
        {
            auto yPos = (data->screenPos.y + finalScaledY) + (heightStep * i);

            if (yPos > (-DRAWING_OFFSET_Y + 240) || (yPos + heightStep) < -DRAWING_OFFSET_Y) continue;

            for (int32_t j = 0; j <= innerLoopCount; j++)
            {
                auto xPos = (data->screenPos.x + finalScaledX) + (widthStep * j);
                if (xPos > (-DRAWING_OFFSET_X + 320) || (xPos + widthStep) < -DRAWING_OFFSET_X) continue;

                libgpu_SetPolyFT4(prim);
                libgpu_SetSemiTrans(prim, 1);
                prim->tpage = data->tpage;
                prim->clut  = data->clut;
                prim->x0    = xPos;
                prim->y0    = yPos;
                prim->x1    = xPos + widthStep;
                prim->y1    = yPos;
                prim->x2    = xPos;
                prim->y2    = yPos + heightStep;
                prim->x3    = xPos + widthStep;
                prim->y3    = yPos + heightStep;
                if (j == innerLoopCount)
                {
                    prim->x1 = data->screenPos.x;
                    prim->x3 = data->screenPos.x;
                }
                if (i == loopCount)
                {
                    prim->y2 = data->screenPos.y;
                    prim->y3 = data->screenPos.y;
                }
                prim->r0 = red;
                prim->g0 = green;
                prim->b0 = blue;

                prim->u0 = data->uBase + uStep * j;
                prim->u1 = data->uBase + uStep * (j + 1) - 1;
                prim->u2 = data->uBase + uStep * j;
                prim->u3 = data->uBase + uStep * (j + 1) - 1;

                prim->v0 = data->vBase + vStep * i;
                prim->v1 = data->vBase + vStep * i;
                prim->v2 = data->vBase + vStep * (i + 1) - 1;
                prim->v3 = data->vBase + vStep * (i + 1) - 1;

                libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + data->depth, prim);
                prim += 1;
            }
        }
        return prim;
    }

    void renderParticleFlash(ParticleFlashData* data)
    {
        // TODO is this ever going to loop? This function is ugly af
        if (data->depth <= 32 || data->depth >= 4096) return;
        if (abs(data->screenPos.x) >= 1024 || abs(data->screenPos.y) >= 512) return;

        uint8_t sizeX = data->sizeX - 1;
        uint8_t sizeY = data->sizeY - 1;
        auto scaledX  = (data->scale * sizeX) / 1024;
        auto scaledY  = (data->scale * sizeY) / 1024;

        if (scaledX <= 0) return;
        if (scaledY <= 0) return;

        auto finalScaledX = scaledX + 1;
        auto finalScaledY = scaledY + 1;

        uint32_t innerLoopCount;
        if (finalScaledX < 1024 && finalScaledY < 512)
            innerLoopCount = 0;
        else
            innerLoopCount = (((scaledX + 2) / 1024 + 1) | 3) + 1;

        uint32_t loopCount = ((scaledY + 2) / 512) + 1;
        auto widthStep     = (scaledX + 2) / (innerLoopCount + 1);
        auto heightStep    = (scaledY + 2) / (loopCount + 1);
        auto uStep         = (sizeX + 1) / (innerLoopCount + 1);
        auto vStep         = (sizeY + 1) / (loopCount + 1);
        auto red           = data->red * data->colorScale / 128;
        auto green         = data->green * data->colorScale / 128;
        auto blue          = data->blue * data->colorScale / 128;

        POLY_FT4* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());

        prim = renderParticleFlashLoop(prim,
                                       data,
                                       -finalScaledX,
                                       -finalScaledY,
                                       loopCount,
                                       innerLoopCount,
                                       heightStep,
                                       widthStep,
                                       uStep,
                                       vStep,
                                       red,
                                       green,
                                       blue);
        prim = renderParticleFlashLoop(prim,
                                       data,
                                       finalScaledX,
                                       -finalScaledY,
                                       loopCount,
                                       innerLoopCount,
                                       heightStep,
                                       -widthStep,
                                       uStep,
                                       vStep,
                                       red,
                                       green,
                                       blue);
        prim = renderParticleFlashLoop(prim,
                                       data,
                                       -finalScaledX,
                                       finalScaledY,
                                       loopCount,
                                       innerLoopCount,
                                       -heightStep,
                                       widthStep,
                                       uStep,
                                       vStep,
                                       red,
                                       green,
                                       blue);
        prim = renderParticleFlashLoop(prim,
                                       data,
                                       finalScaledX,
                                       finalScaledY,
                                       loopCount,
                                       innerLoopCount,
                                       -heightStep,
                                       -widthStep,
                                       uStep,
                                       vStep,
                                       red,
                                       green,
                                       blue);

        libgs_GsSetWorkBase(prim);
    }

    void renderParticleFX(int32_t instanceId)
    {
        auto& data         = particleFXData[instanceId];
        auto flashCount    = flashCountMapping[data.mode];
        auto particleCount = particleCountMapping[data.mode];
        auto tickTarget    = data.tickTarget;
        auto tickCount     = data.tickCount;

        if (data.tickCount == 2)
        {
            for (int32_t i = 0; i < flashCount; i++)
            {
                SVector worldPos;
                Position screenPos;
                ParticleFlashData flashData;
                worldPos.x = data.pos.x + data.flashX[i];
                worldPos.y = data.pos.y + data.flashY[i];
                worldPos.z = data.pos.z + data.flashZ[i];
                auto depth = worldPosToScreenPos(&worldPos, &flashData.screenPos);

                flashData.sizeX      = 64;
                flashData.sizeY      = 64;
                flashData.tpage      = 0xbd;
                flashData.clut       = 0x79C0;
                flashData.uBase      = 64;
                flashData.vBase      = 192;
                flashData.red        = data.r;
                flashData.green      = data.g;
                flashData.blue       = data.b;
                flashData.colorScale = 0x80;
                flashData.scale      = (data.flashScale[i] * VIEWPORT_DISTANCE * 8) / depth;
                flashData.depth      = depth / 16;
                if (flashData.depth > 32 && flashData.depth < 4096) renderParticleFlash(&flashData);
            }
        }

        if (tickCount < tickTarget)
        {
            RGB8 color;
            color.red   = lerp(data.r, 0, 0, tickTarget, tickCount);
            color.green = lerp(data.g, 0, 0, tickTarget, tickCount);
            color.blue  = lerp(data.b, 0, 0, tickTarget, tickCount);
            auto scale  = sin(lerp(0, 110, 0, tickTarget, tickCount)) / 128;

            for (int32_t i = 0; i < particleCount; i++)
            {
                SVector pos;
                pos.x = data.pos.x + (fxParticleData[i].x * scale) / 512;
                pos.y = data.pos.y + (fxParticleData[i].y * scale) / 512;
                pos.z = data.pos.z + (fxParticleData[i].z * scale) / 512;
                renderFXParticle(&pos, 40, &color);
            }
        }
    }

    void tickParticleFX(int32_t instanceId)
    {
        auto& data   = particleFXData[instanceId];
        auto* entity = data.entity;
        data.tickCount++;
        data.cloudDelay--;

        if (data.tickCount == 10)
        {
            if (data.mode == 1)
            {
                addEntityParticleFX(entity, 22);
                addEntityParticleFX(entity, 22);
                addEntityParticleFX(entity, 22);
            }
            if (data.mode == 0 && entity != nullptr)
            {
                addEntityParticleFX(entity, 14);
                addEntityParticleFX(entity, 14);
                addEntityParticleFX(entity, 14);
            }
        }
        else if (data.tickCount == 13)
        {
            if (data.mode == 1)
            {
                addEntityParticleFX(entity, 22);
                addEntityParticleFX(entity, 14);
            }
            if (data.mode == 0 && entity != nullptr)
            {
                addEntityParticleFX(entity, 14);
                addEntityParticleFX(entity, 14);
            }
        }
        else
        {
            auto delay = data.cloudDelay;
            if (delay < 4 && delay >= 0 && data.mode == 1)
            {
                auto pos    = entity->posData;
                auto baseX  = pos->posMatrix.work.t[0];
                auto baseZ  = pos->posMatrix.work.t[2];
                auto radius = getDigimonData(entity->type)->radius;

                for (int32_t i = 0; i < data.cloudDelay + 2; i++)
                {
                    auto roll   = rand();
                    auto offset = radius + random2(radius / 2);

                    SVector vec;
                    vec.x = baseX + (offset * sin(roll) >> 12);
                    vec.y = 0;
                    vec.z = baseZ - (offset * cos(roll) >> 12);
                    createCloudFX(&vec);
                }
            }
            else if (data.tickTarget <= data.tickCount && delay < 0)
            {
                data.tickCount = -1;
                removeObject(ObjectID::PARTICLE_FX, instanceId);
            }
        }
    }

    void createParticleFX(uint32_t color, int32_t mode, SVector* position, Entity* entity, int16_t cloudDelay)
    {
        int32_t id;
        ParticleFXData* data = getFreeParticleFXData(&id);
        if (data == nullptr) return;

        data->flashX[0]     = random2(200) - 100;
        data->flashY[0]     = random2(200) - 100;
        data->flashZ[0]     = random2(200) - 100;
        data->flashScale[0] = random2(170) + 250;
        data->flashX[1]     = random2(200) - 100;
        data->flashY[1]     = random2(200) - 100;
        data->flashZ[1]     = random2(200) - 100;
        data->flashScale[1] = random2(170) + 250;
        if (mode == 1)
        {
            data->flashX[2]     = random2(200) - 100;
            data->flashY[2]     = random2(200) - 100;
            data->flashZ[2]     = random2(200) - 100;
            data->flashScale[2] = random2(200) + 300;
            data->tickTarget    = 22;
        }
        else if (mode == 0)
            data->tickTarget = 19;
        else if (mode == 2)
            data->tickTarget = 14;
        else
            return;

        if (color > 6) return;
        auto& colorData = particleColors[color];
        data->r         = colorData.red;
        data->g         = colorData.green;
        data->b         = colorData.blue;

        data->tickCount  = 0;
        data->color      = color;
        data->mode       = mode;
        data->pos        = *position;
        data->entity     = entity;
        data->cloudDelay = cloudDelay + 4;
        addObject(ObjectID::PARTICLE_FX, id, tickParticleFX, renderParticleFX);
    }

    void initializeCloudFXData()
    {
        for (auto& data : cloudFXData)
            data.frame = -1;
    }

    void removeAllCloudFX()
    {
        int32_t instanceId = 0;
        for (auto& data : cloudFXData)
        {
            if (data.frame != -1) removeObject(ObjectID::CLOUD_FX, instanceId);
            instanceId++;
        }
    }

    void renderCloudFX(int32_t instanceId)
    {
        auto& data = cloudFXData[instanceId];
        SVector worldPos;
        Position screenPos;
        worldPos.x           = data.posX;
        worldPos.y           = cloudYData[data.frame];
        worldPos.z           = data.posZ;
        auto depth           = worldPosToScreenPos(&worldPos, &screenPos);
        cloudFXSprite.u      = cloudUData[data.frame];
        cloudFXSprite.r      = cloudColorData[data.frame];
        cloudFXSprite.g      = cloudColorData[data.frame];
        cloudFXSprite.b      = cloudColorData[data.frame];
        cloudFXSprite.x      = screenPos.x;
        cloudFXSprite.y      = screenPos.y;
        cloudFXSprite.scaleX = (cloudScaleData[data.frame] * VIEWPORT_DISTANCE) / depth;
        cloudFXSprite.scaleY = (cloudScaleData[data.frame] * VIEWPORT_DISTANCE) / depth;

        // vanilla calls renderSprite (0x800da36c) here, but we inlined it.
        // It modified the passed sprite, which is kinda dirty.
        auto origin = depth / 16;
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