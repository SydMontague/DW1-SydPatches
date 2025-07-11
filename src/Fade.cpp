#include "GameObjects.hpp"
#include "GameTime.hpp"
#include "Helper.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"

extern "C"
{
    void initializeFadeData()
    {
        FADE_DATA.fadeOutTarget  = 0;
        FADE_DATA.fadeOutCurrent = 0;
        FADE_DATA.fadeInTarget   = 0;
        FADE_DATA.fadeInCurrent  = 0;
        FADE_DATA.fadeProgress   = 0;
        FADE_DATA.fadeMode       = FadeMode::BLACK_FADE;
        FADE_DATA.fadeProtection = 0;
    }

    void renderFade(uint8_t progress)
    {
        POLY_FT4* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyFT4(prim);
        libgpu_SetSemiTrans(prim, 1);
        prim->tpage = getTPage(0, static_cast<int32_t>(FADE_DATA.fadeMode), 896, 448);
        prim->clut  = getClut(256, 502);
        setPosDataPolyFT4(prim, -160, -120, 320, 240);
        setUVDataPolyFT4(prim, 250, 253, 2, 2);
        prim->r0 = progress;
        prim->g0 = progress;
        prim->b0 = progress;
        if (FADE_DATA.fadeMode == FadeMode::BLACK_FADE)
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 1, prim);
        else
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 10, prim);

        libgs_GsSetWorkBase(prim + 1);
    }

    void renderFadeOut(int32_t instanceId)
    {
        FADE_DATA.fadeProgress += (160 / FADE_DATA.fadeOutTarget);

        if (FADE_DATA.fadeProgress > 160) FADE_DATA.fadeProgress = 160;

        renderFade(FADE_DATA.fadeProgress);
        FADE_DATA.fadeOutCurrent++;
    }

    void renderFadeIn(int32_t instanceId)
    {
        if (FADE_DATA.fadeInCurrent >= FADE_DATA.fadeInTarget)
        {
            FADE_DATA.fadeInCurrent  = 0;
            FADE_DATA.fadeProtection = 0;
            removeObject(ObjectID::FADE, instanceId);
        }
        else
        {
            auto progress = FADE_DATA.fadeInCurrent * (160 / FADE_DATA.fadeInTarget);
            if (progress < 1) progress = FADE_DATA.fadeInCurrent;

            FADE_DATA.fadeProgress = 160 - progress;
            renderFade(FADE_DATA.fadeProgress);
            FADE_DATA.fadeInCurrent++;
        }
    }

    void fadeFromWhite(int32_t frames)
    {
        removeObject(ObjectID::FADE, 0);
        FADE_DATA.fadeInTarget   = frames + 1;
        FADE_DATA.fadeInCurrent  = 1;
        FADE_DATA.fadeOutCurrent = 0;
        FADE_DATA.fadeMode       = FadeMode::WHITE_FADE;
        // vanilla writes to 0x134cbc, but it seems unused?
        addObject(ObjectID::FADE, 0, nullptr, renderFadeIn);
    }

    void fadeToWhite(int32_t frames)
    {
        if (FADE_DATA.fadeOutCurrent != 0 || FADE_DATA.fadeInCurrent != 0) removeObject(ObjectID::FADE, 0);
        FADE_DATA.fadeOutTarget  = frames + 1;
        FADE_DATA.fadeOutCurrent = 1;
        FADE_DATA.fadeMode       = FadeMode::WHITE_FADE;
        addObject(ObjectID::FADE, 0, nullptr, renderFadeOut);
    }

    void fadeFromBlack(int32_t frames)
    {
        removeObject(ObjectID::FADE, 0);
        FADE_DATA.fadeInTarget   = frames + 1;
        FADE_DATA.fadeInCurrent  = 1;
        FADE_DATA.fadeOutCurrent = 0;
        FADE_DATA.fadeMode       = FadeMode::BLACK_FADE;
        addObject(ObjectID::FADE, 0, nullptr, renderFadeIn);
    }

    void fadeToBlack(int32_t frames)
    {
        if (FADE_DATA.fadeOutCurrent != 0 || FADE_DATA.fadeInCurrent != 0) removeObject(ObjectID::FADE, 0);

        FADE_DATA.fadeOutTarget  = frames + 1;
        FADE_DATA.fadeOutCurrent = 1;
        FADE_DATA.fadeMode       = FadeMode::BLACK_FADE;
        addObject(ObjectID::FADE, 0, nullptr, renderFadeOut);
        stopGameTime(); // TODO this should be responsibility of the caller
        FADE_DATA.fadeProtection = 1;
    }
}