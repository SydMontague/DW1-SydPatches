#include "Effects.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "extern/dtl/types.hpp"
#include "extern/dw1.hpp"

namespace
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

    dtl::array<EntityParticleFXData, 20> entityParticleFXData;

    void removeEntityParticleFX(int32_t instanceId)
    {
        entityParticleFXData[instanceId].counter = -1;
        removeObject(ObjectID::ENTITY_PARTICLE_FX, instanceId);
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
} // namespace

void initializeEntityParticleFX()
{
    for (auto& data : entityParticleFXData)
        data.counter = -1;
}
extern "C"
{
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
                return;
            }

            i++;
        }
    }
}