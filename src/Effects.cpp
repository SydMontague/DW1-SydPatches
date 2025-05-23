#include "Effects.hpp"

#include "DigimonData.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"
#include "extern/psx.hpp"
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

    struct MeramonShakeData
    {
        int32_t backupOffsetX;
        int32_t backupOffsetY;
        int16_t height;
        int16_t width;
        int16_t posY;
        int16_t posX;
        uint8_t frameCount;
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
    static MeramonShakeData meramonShakeData;

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

    void addFXPrim(POLY_FT4* prim, int16_t x, int16_t y, uint32_t scaleX, int16_t scaleY, uint32_t distance)
    {
        const auto scaledX = (scaleX * VIEWPORT_DISTANCE) / distance;
        const auto scaledY = (scaleY * VIEWPORT_DISTANCE) / distance;
        const auto minX    = x - scaledX / 2;
        const auto maxX    = x + scaledX / 2;
        const auto minY    = y - scaledY / 2;
        const auto maxY    = y + scaledY / 2;
        const auto depth   = distance / 16;

        prim->x0 = minX;
        prim->x1 = maxX;
        prim->x2 = minX;
        prim->x3 = maxX;
        prim->y0 = minY;
        prim->y1 = minY;
        prim->y2 = maxY;
        prim->y3 = maxY;
        if (depth <= 32 || depth >= 4096) return;

        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + depth, prim);
        libgs_GsSetWorkBase(prim + 1);
    }

    void renderEntityParticleFX(int32_t instanceId)
    {
        auto& data          = entityParticleFXData[instanceId];
        auto& modeData      = entityParticleFXModes[data.mode];
        const auto& bonePos = data.entity->posData->posMatrix.work.t;
        auto mapPos         = getMapPosition(bonePos[0], bonePos[1], bonePos[2]);

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

        addFXPrim(prim, mapPos.screenX, mapPos.screenY, modeData.scaleX, modeData.scaleY, mapPos.depth);
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
        // vanilla checks if the sprite would be fully off screen but we do that because
        // HOLY FUCK THIS FUNCTION PISSES ME OFF AND I WANT TO MOVE ON
        // TODO make nicer
        for (int32_t i = 0; i <= loopCount; i++)
        {
            auto yPos = (data->screenPos.y + finalScaledY) + (heightStep * i);

            // if (yPos > (-DRAWING_OFFSET_Y + 240) || (yPos + heightStep) < -DRAWING_OFFSET_Y) continue;

            for (int32_t j = 0; j <= innerLoopCount; j++)
            {
                auto xPos = (data->screenPos.x + finalScaledX) + (widthStep * j);
                // if (xPos > (-DRAWING_OFFSET_X + 320) || (xPos + widthStep) < -DRAWING_OFFSET_X) continue;

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

    void renderFXParticle(SVector* pos, int16_t scale, const RGB8* color)
    {
        auto* prim  = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        auto mapPos = getMapPosition(*pos);
        libgpu_SetPolyFT4(prim);
        libgpu_SetSemiTrans(prim, 1);

        prim->code |= 2;
        prim->r0    = color->red;
        prim->g0    = color->green;
        prim->b0    = color->blue;
        prim->tpage = 0x3C;
        prim->clut  = 0x7A4C;
        prim->u0    = 0;
        prim->u1    = 15;
        prim->u2    = 0;
        prim->u3    = 15;
        prim->v0    = 160;
        prim->v1    = 160;
        prim->v2    = 175;
        prim->v3    = 175;
        addFXPrim(prim, mapPos.screenX, mapPos.screenY, scale, scale, mapPos.depth);
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
                ParticleFlashData flashData;
                auto mapPos = getMapPosition(data.pos.x + data.flashX[i],
                                             data.pos.y + data.flashY[i],
                                             data.pos.z + data.flashZ[i]);

                flashData.screenPos.x = mapPos.screenX;
                flashData.screenPos.y = mapPos.screenY;
                flashData.sizeX       = 64;
                flashData.sizeY       = 64;
                flashData.tpage       = 0xbd;
                flashData.clut        = 0x79C0;
                flashData.uBase       = 64;
                flashData.vBase       = 192;
                flashData.red         = data.r;
                flashData.green       = data.g;
                flashData.blue        = data.b;
                flashData.colorScale  = 0x80;
                flashData.scale       = (data.flashScale[i] * VIEWPORT_DISTANCE * 8) / mapPos.depth;
                flashData.depth       = mapPos.depth / 16;
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
        auto& data  = cloudFXData[instanceId];
        auto mapPos = getMapPosition(data.posX, cloudYData[data.frame], data.posZ);

        cloudFXSprite.u      = cloudUData[data.frame];
        cloudFXSprite.r      = cloudColorData[data.frame];
        cloudFXSprite.g      = cloudColorData[data.frame];
        cloudFXSprite.b      = cloudColorData[data.frame];
        cloudFXSprite.x      = mapPos.screenX;
        cloudFXSprite.y      = mapPos.screenY;
        cloudFXSprite.scaleX = (cloudScaleData[data.frame] * VIEWPORT_DISTANCE) / mapPos.depth;
        cloudFXSprite.scaleY = (cloudScaleData[data.frame] * VIEWPORT_DISTANCE) / mapPos.depth;

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

    /*
     * Healing Particle Effect
     */

    struct HealingParticleData
    {
        int32_t counter;
        SVector pos;
    };

    struct HealingParticle
    {
        int16_t frameId;
        int16_t offsetX;
        int16_t offsetY;
        int16_t offsetZ;
        int16_t unk1;
        int16_t radius;
        uint16_t hasParticle2;
        uint16_t unknown;
        HealingParticleData particles1[20];
        HealingParticleData particles2[20];
    };

    static HealingParticle HEALING_PARTICLES;
    constexpr RGB8 PARTICLE_COLOR1[18] = {
        {0x02, 0x04, 0x01},
        {0x04, 0x06, 0x02},
        {0x07, 0x09, 0x03},
        {0x0a, 0x0b, 0x03},
        {0x0c, 0x0d, 0x04},
        {0x0f, 0x10, 0x04},
        {0x12, 0x12, 0x05},
        {0x15, 0x15, 0x06},
        {0x17, 0x17, 0x06},
        {0x1a, 0x19, 0x07},
        {0x1d, 0x1c, 0x07},
        {0x1f, 0x1e, 0x08},
        {0x22, 0x21, 0x09},
        {0x25, 0x23, 0x09},
        {0x27, 0x25, 0x0a},
        {0x2a, 0x28, 0x0a},
        {0x2d, 0x2a, 0x0b},
        {0x30, 0x2d, 0x0c},

    };
    constexpr RGB8 PARTICLE_COLOR2[18] = {
        {0x04, 0x09, 0x03},
        {0x0a, 0x0e, 0x05},
        {0x10, 0x13, 0x06},
        {0x16, 0x18, 0x07},
        {0x1b, 0x1d, 0x09},
        {0x21, 0x23, 0x0a},
        {0x27, 0x28, 0x0b},
        {0x2d, 0x2d, 0x0d},
        {0x33, 0x32, 0x0e},
        {0x39, 0x37, 0x0f},
        {0x3f, 0x3d, 0x10},
        {0x44, 0x42, 0x12},
        {0x4a, 0x47, 0x13},
        {0x50, 0x4c, 0x14},
        {0x56, 0x51, 0x16},
        {0x5c, 0x57, 0x17},
        {0x62, 0x5c, 0x18},
        {0x68, 0x61, 0x1a},
    };
    constexpr RGB8 PARTICLE_COLOR3[5] = {
        {0x47, 0x51, 0x0},
        {0x60, 0x6b, 0x19},
        {0x7d, 0x86, 0x3d},
        {0xb3, 0xbb, 0x7a},
        {0xd7, 0xdc, 0xb7},
    };

    void tickHealingParticles(int32_t instanceId)
    {
        auto frameId = HEALING_PARTICLES.frameId++;

        if (frameId >= 44)
        {
            HEALING_PARTICLES.frameId = -1;
            removeObject(ObjectID::HEALING_PARTICLES, instanceId);
            return;
        }

        if (frameId < 20)
        {
            Matrix mat;
            auto& particle = HEALING_PARTICLES.particles1[frameId];

            particle.counter = 17;
            SVector rot      = {.x = 0, .y = static_cast<int16_t>(frameId * 0x280 & 0xFFF), .z = 0};
            SVector pos      = {.x = HEALING_PARTICLES.radius, .y = 0, .z = 0};
            libgte_RotMatrixZYX(&rot, &mat);
            libgte_ApplyMatrixSV(&mat, &pos, &particle.pos);

            particle.pos.x += HEALING_PARTICLES.offsetX;
            particle.pos.y += HEALING_PARTICLES.offsetY - lerp(0, HEALING_PARTICLES.offsetY, 0, 20, frameId);
            particle.pos.z += HEALING_PARTICLES.offsetZ;
        }

        for (auto& particle : HEALING_PARTICLES.particles1)
        {
            particle.counter--;
            particle.pos.pad = (rand() % 70) + 15;
        }

        if (frameId < 39)
        {
            auto id = frameId % 5;

            for (int32_t i = 0; i < 4; i++)
            {
                auto& particle = HEALING_PARTICLES.particles2[id + i * 5];

                auto val1          = lerp(0, 512, 0, 13, frameId);
                auto val2          = i * 13;
                auto progressValue = val1 + 512 / (val2 / 4);
                auto sinVal        = sin(progressValue);
                auto cosVal        = cos(progressValue);

                particle.counter = 5;
                particle.pos.x =
                    HEALING_PARTICLES.offsetX + rand() % 60 - 30 - (HEALING_PARTICLES.radius + 150) * sinVal;
                particle.pos.z =
                    HEALING_PARTICLES.offsetZ + rand() % 60 - 30 + (HEALING_PARTICLES.radius + 150) * cosVal;
                particle.pos.y = lerp(HEALING_PARTICLES.offsetY + rand() % 60 - 30, 0, 0, 0x27, frameId);
            }
        }

        for (auto& particle : HEALING_PARTICLES.particles2)
            particle.counter--;
    }

    void renderHealingParticles(int32_t instanceId)
    {
        for (auto& particle : HEALING_PARTICLES.particles1)
        {
            if (particle.counter < 0) continue;

            renderFXParticle(&particle.pos, particle.pos.pad, &PARTICLE_COLOR1[particle.counter]);
            renderFXParticle(&particle.pos, particle.pos.pad / 2, &PARTICLE_COLOR2[particle.counter]);
        }

        if (HEALING_PARTICLES.hasParticle2 == 0) return;

        for (auto& particle : HEALING_PARTICLES.particles2)
        {
            if (particle.counter < 0) continue;

            renderFXParticle(&particle.pos, 24, &PARTICLE_COLOR3[particle.counter]);
        }
    }

    void addHealingParticleEffect(Entity* entity, int32_t param)
    {
        // vanilla treats these as if HEALING_PARTICLES is an array, but there is only one instance
        auto data = getDigimonData(entity->type);

        HEALING_PARTICLES.frameId      = 0;
        HEALING_PARTICLES.offsetX      = entity->posData->posMatrix.work.t[0];
        HEALING_PARTICLES.offsetY      = -70 - data->height;
        HEALING_PARTICLES.offsetZ      = entity->posData->posMatrix.work.t[2];
        HEALING_PARTICLES.radius       = data->radius * 12 / 10;
        HEALING_PARTICLES.hasParticle2 = param;

        for (int32_t i = 0; i < 20; i++)
        {
            HEALING_PARTICLES.particles1[i].counter = -1;
            HEALING_PARTICLES.particles2[i].counter = -1;
        }

        addObject(ObjectID::HEALING_PARTICLES, 0, tickHealingParticles, renderHealingParticles);
    }

    void initializeHealingParticles()
    {
        HEALING_PARTICLES.frameId = -1;
    }

    static void tickMeramonShake(int32_t instanceId)
    {
        auto frameCount = meramonShakeData.frameCount;

        auto offsetX = 1;
        if (frameCount < 31)
            offsetX = 1;
        else if (frameCount < 61)
            offsetX = 2;
        else if (frameCount < 121)
            offsetX = 3;
        else if (frameCount < 181)
            offsetX = 2;

        if (frameCount % 2 == 0)
        {
            meramonShakeData.posX   = max(meramonShakeData.posX - 32, -200);
            meramonShakeData.posY   = max(meramonShakeData.posY - 32, -200);
            meramonShakeData.width  = meramonShakeData.posX * -2;
            meramonShakeData.height = meramonShakeData.posY * -2;
            offsetX                 = -offsetX;
        }

        DRAWING_OFFSET_X += offsetX;

        auto prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyFT4(prim);
        libgpu_SetSemiTrans(prim, 1);
        prim->tpage = 0xdd;
        prim->clut  = getClut(0, 0x1e7);
        setPosDataPolyFT4(prim,
                          meramonShakeData.posX,
                          meramonShakeData.posY,
                          meramonShakeData.width,
                          meramonShakeData.height);
        setUVDataPolyFT4(prim, 64, 128, 63, 63);
        prim->r0 = 0;
        prim->g0 = 255;
        prim->b0 = 255;
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 9, prim);
        libgs_GsSetWorkBase(prim + 1);

        meramonShakeData.frameCount++;
        if (meramonShakeData.frameCount > 209)
        {
            DRAWING_OFFSET_X = meramonShakeData.backupOffsetX;
            DRAWING_OFFSET_Y = meramonShakeData.backupOffsetY;
            removeObject(ObjectID::MERAMON_SHAKE, instanceId);
        }
    }

    void createMeramonShake()
    {
        meramonShakeData.backupOffsetX = DRAWING_OFFSET_X;
        meramonShakeData.backupOffsetY = DRAWING_OFFSET_Y;
        meramonShakeData.frameCount    = 0;
        meramonShakeData.width         = 0;
        meramonShakeData.height        = 0;
        meramonShakeData.posX          = 0;
        meramonShakeData.posY          = 0;
        addObject(ObjectID::MERAMON_SHAKE, 0, tickMeramonShake, nullptr);
    }

    void getRViewCopy(GsRVIEW2* val)
    {
        *val = GS_VIEWPOINT_COPY;
    }

    void getViewportDistanceCopy(uint32_t* val)
    {
        *val = VIEWPORT_DISTANCE_COPY;
    }

    void getDrawingOffsetCopy(int32_t* offsetX, int32_t* offsetY)
    {
        *offsetX = DRAWING_OFFSET_X_COPY;
        *offsetY = DRAWING_OFFSET_Y_COPY;
    }

    void translateConditionFXToEntity(Entity* entity, SVector* out)
    {
        auto type    = getOriginalType(entity->type);
        auto& offset = CONDITION_FX_OFFSETS[static_cast<int32_t>(type)];
        auto& node   = entity->posData[offset.pad].posMatrix.work;

        libgte_ApplyMatrixSV(&node, &offset, out);
        out->x += node.t[0];
        out->y += node.t[1];
        out->z += node.t[2];
    }

    void addScreenPolyFT3(POLY_FT3* prim, SVector* pos1, SVector* pos2, SVector* pos3)
    {
        setRotTransMatrix(&libgs_REFERENCE_MATRIX);

        int32_t dummy1;
        int32_t dummy2;
        ScreenCoord xy0;
        ScreenCoord xy1;
        ScreenCoord xy2;
        auto screenZ = libgte_RotTransPers3(pos1, pos2, pos3, &xy0.raw, &xy1.raw, &xy2.raw, &dummy1, &dummy2);

        if (screenZ <= 0x20 || screenZ >= 0x1000) return;

        prim->x0 = xy0.x;
        prim->x1 = xy1.x;
        prim->x2 = xy2.x;
        prim->y0 = xy0.y;
        prim->y1 = xy1.y;
        prim->y2 = xy2.y;

        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + screenZ, prim);
        libgs_GsSetWorkBase(prim + 1);
    }

    void addScreenPolyFT4(POLY_FT4* prim, SVector* pos1, SVector* pos2, SVector* pos3, SVector* pos4)
    {
        setRotTransMatrix(&libgs_REFERENCE_MATRIX);
        int32_t emp;
        int32_t ty;
        ScreenCoord xy0;
        ScreenCoord xy1;
        ScreenCoord xy2;
        ScreenCoord xy3;
        auto screenZ = libgte_RotTransPers4(pos1, pos2, pos3, pos4, &xy0.raw, &xy1.raw, &xy2.raw, &xy3.raw, &emp, &ty);

        prim->x0 = xy0.x;
        prim->x1 = xy1.x;
        prim->x2 = xy2.x;
        prim->x3 = xy3.x;
        prim->y0 = xy0.y;
        prim->y1 = xy1.y;
        prim->y2 = xy2.y;
        prim->y3 = xy3.y;

        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + screenZ, prim);
        libgs_GsSetWorkBase(prim + 1);
    }

    void renderSprite(GsSPRITE* sprite, int16_t x, int16_t y, uint32_t distance, int32_t scaleX, int32_t scaleY)
    {
        auto depth     = distance / 16;
        sprite->x      = x;
        sprite->y      = y;
        sprite->scaleX = (scaleX * VIEWPORT_DISTANCE) / distance;
        sprite->scaleY = (scaleY * VIEWPORT_DISTANCE) / distance;
        if (depth <= 0 || depth >= 4096) return;

        libgs_GsSortSprite(sprite, ACTIVE_ORDERING_TABLE, depth);
    }

    void renderTMDModel(TMDModel* model,
                        int32_t objId,
                        GsCOORDINATE2* baseCoord,
                        GsCOORDINATE2* superCoord,
                        Vector* translation,
                        SVector* rotation,
                        Vector* scale)
    {
        Matrix lw;
        Matrix ls;
        GsDOBJ2 obj;
        libgs_GsLinkObject4(reinterpret_cast<uint32_t*>(model->objects), &obj, objId);
        libgs_GsInitCoordinate2(superCoord, baseCoord);

        baseCoord->flag = 0;
        obj.attribute   = 0;
        obj.coord2      = baseCoord;

        libgte_RotMatrix(rotation, &baseCoord->coord);
        libgte_ScaleMatrix(&baseCoord->coord, scale);
        libgte_TransMatrix(&baseCoord->coord, translation);

        libgs_GsGetLws(obj.coord2, &lw, &ls);
        libgs_GsSetLightMatrix(&lw);
        libgs_GsSetLsMatrix(&ls);

        libgs_GsSortObject4(&obj, ACTIVE_ORDERING_TABLE, 2, &SCRATCHPAD);
    }
}