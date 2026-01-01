
#include "Font.hpp"
#include "GameObjects.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "extern/dw1.hpp"
#include "extern/libetc.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"

namespace
{
    SimpleTextSprite pauseString(704 + 30, 256 + 240);
    bool hasPauseBpx       = false;
    uint32_t inputCurrent  = 0;
    uint32_t inputPrevious = 0;

    void renderPauseBox(int32_t instance)
    {
        GsBOXF box{
            .attribute = 0x40000000,
            .x         = -160,
            .y         = -120,
            .width     = 320,
            .height    = 240,
            .r         = 0,
            .g         = 0,
            .b         = 0,
        };

        libgs_GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, 7 - instance);
        pauseString.render(UI_BOX_DATA[5].finalPos.x + 6, UI_BOX_DATA[5].finalPos.y + 6, 0, 0, 1);
    };

    void pauseFrame()
    {
        ACTIVE_FRAMEBUFFER    = libgs_GsGetActiveBuffer();
        ACTIVE_ORDERING_TABLE = GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER;
        libgs_GsSetWorkBase(GS_WORK_BASES + ACTIVE_FRAMEBUFFER);
        libgs_GsClearOt(0, 0, ACTIVE_ORDERING_TABLE);
        tickObjects();
        renderObjects();
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 32, &DRAW_OFFSETS[ACTIVE_FRAMEBUFFER]);
        libgpu_DrawSync(0);
        libetc_vsync(3);
        libgs_GsSetOrign(DRAWING_OFFSET_X, DRAWING_OFFSET_Y);
        libgs_GsSwapDispBuff();
        libgs_GsSortClear(0, 0, 0, ACTIVE_ORDERING_TABLE);
        libgs_GsDrawOt(ACTIVE_ORDERING_TABLE);
    }

    constexpr bool isPausePressed()
    {
        inputPrevious = inputCurrent;
        inputCurrent  = libetc_PadRead(1);
        return (inputCurrent & 0x800) != 0 && (inputPrevious & 0x800) == 0;
    }

} // namespace

extern "C"
{
    void removePauseBox()
    {
        if (!hasPauseBpx) return;

        removeStaticUIBox(5);
        hasPauseBpx = false;
        playSound(0, 3);
    }

    void createPauseBox()
    {
        if (hasPauseBpx) return;

        pauseString.draw(&vanillaFont, "Pause");
        RECT rect{
            .x      = -26,
            .y      = -14,
            .width  = 56,
            .height = 24,
        };

        createStaticUIBox(5, 1, 0, &rect, nullptr, renderPauseBox);
        hasPauseBpx = true;
        playSound(0, 3);
    }

    void handlePause()
    {
        removePauseBox();

        if (FADE_DATA.fadeProtection == 1) return;
        if (GAME_STATE == 0 && (readPStat(0) == 3 || IS_GAMETIME_RUNNING == 0)) return;

        if (!isPausePressed()) return;

        createPauseBox();
        pauseFrame();
        pauseFrame();

        while (!isPausePressed())
            ;
    }
}