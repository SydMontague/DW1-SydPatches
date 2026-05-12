#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "UIElements.hpp"
#include "extern/dtl/types.hpp"
#include "extern/dw1.hpp"

namespace
{
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

    MeramonShakeData meramonShakeData;

    void tickMeramonShake(int32_t instanceId)
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
} // namespace

extern "C"
{
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
}