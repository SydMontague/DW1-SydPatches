#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "extern/dw1.hpp"
#include "extern/libgte.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    constexpr RGB8 particleColors[7] = {
        {.red = 0xFF, .green = 0x6E, .blue = 0x46},
        {.red = 0xFF, .green = 0x6E, .blue = 0x46},
        {.red = 0xC8, .green = 0xC8, .blue = 0x64},
        {.red = 0xCC, .green = 0x00, .blue = 0x88},
        {.red = 0x32, .green = 0x32, .blue = 0xFF},
        {.red = 0x96, .green = 0x96, .blue = 0x96},
        {.red = 0xFF, .green = 0x6E, .blue = 0x6E},
    };
    constexpr uint8_t flashCountMapping[4]    = {2, 3, 2, 0};
    constexpr uint8_t particleCountMapping[4] = {25, 48, 20, 0};

    void initializeParticleFX()
    {
        for (ParticleFXData& data : PARTICLE_FX_DATA)
            data.tickCount = -1;

        for (SPosition3D& data : FX_PARTICLE_DATA)
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
        for (auto& data : PARTICLE_FX_DATA)
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

    void renderParticleFX(int32_t instanceId)
    {
        auto& data         = PARTICLE_FX_DATA[instanceId];
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
                pos.x = data.pos.x + (FX_PARTICLE_DATA[i].x * scale) / 512;
                pos.y = data.pos.y + (FX_PARTICLE_DATA[i].y * scale) / 512;
                pos.z = data.pos.z + (FX_PARTICLE_DATA[i].z * scale) / 512;
                renderFXParticle(&pos, 40, &color);
            }
        }
    }

    void tickParticleFX(int32_t instanceId)
    {
        auto& data   = PARTICLE_FX_DATA[instanceId];
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
}