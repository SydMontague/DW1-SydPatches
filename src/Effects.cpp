#include "GameObjects.hpp"
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
        addObject(ObjectID::KNOCKBACK_FX, id, tickParticleFX, renderParticleFX);
    }
}