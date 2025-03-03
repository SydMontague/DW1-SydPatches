
#include "Helper.hpp"
#include "Math.hpp"
#include "UIElements.hpp"
#include "constants.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"

extern "C"
{
    constexpr int32_t STATUS_UI_X_MIN = 75;
    constexpr int32_t STATUS_UI_X_MAX = 240;

    void renderStatusBars(bool isTimeRunning)
    {
        if (isTimeRunning)
            STATUS_UI_OFFSET_X -= 50;
        else
            STATUS_UI_OFFSET_X += 50;

        STATUS_UI_OFFSET_X = clamp(STATUS_UI_OFFSET_X, STATUS_UI_X_MIN, STATUS_UI_X_MAX);

        renderRectPolyFT4(STATUS_UI_OFFSET_X, 85, 76, 8, 0, 236, 5, 0x7D06, 15, 0);
        renderRectPolyFT4(STATUS_UI_OFFSET_X, 97, 76, 8, 0, 236, 5, 0x7D06, 15, 0);

        auto happyX = PARTNER_PARA.happiness < HAPPINESS_MID ? 97 : 64;
        auto discX  = PARTNER_PARA.discipline < DISCIPLINE_MID ? 33 : 22;
        if (PLAYTIME_FRAMES % 10 < 5)
        {
            if (PARTNER_AREA_RESPONSE == 2) happyX += 11;
            if (PARTNER_AREA_RESPONSE == 1) happyX -= 11;
        }
        auto happyClut = getClut(0x60, 0x1f7);
        auto discClut  = getClut(0x60, 0x1f2);
        renderRectPolyFT4(STATUS_UI_OFFSET_X + 3, 83, 11, 11, happyX, 244, 5, happyClut, 14, 0);
        renderRectPolyFT4(STATUS_UI_OFFSET_X + 3, 95, 11, 11, discX, 244, 5, discClut, 14, 0);

        // clang-format off
        int32_t barValues[4];
        barValues[0] = clamp((PARTNER_PARA.happiness - HAPPINESS_MIN) * 100 / (HAPPINESS_MAX - HAPPINESS_MID), 0, 100);
        barValues[1] = clamp((PARTNER_PARA.happiness - HAPPINESS_MID) * 100 / (HAPPINESS_MAX - HAPPINESS_MID), 0, 100);
        barValues[2] = clamp((PARTNER_PARA.discipline - DISCIPLINE_MIN) * 100 / (DISCIPLINE_MAX - DISCIPLINE_MID), 0, 100);
        barValues[3] = clamp((PARTNER_PARA.discipline - DISCIPLINE_MID) * 100 / (DISCIPLINE_MAX - DISCIPLINE_MID), 0, 100);
        // clang-format on

        for (int32_t i = 0; i < 4; i++)
        {
            auto clut      = getClut(0x60, 0x1f7);
            int32_t width  = (barValues[i] * 50) / 100;
            int32_t x      = STATUS_UI_OFFSET_X + 18;
            int32_t y      = 88 + 12 * ((i & 2) / 2);
            int32_t v      = 151 + 2 * (i & 1);
            int32_t offset = 0;
            while (width > 0)
            {
                renderRectPolyFT4(x + offset, y, min(width, 20), 2, 232, v, 5, clut, 14 - (i & 1), 0);
                width -= 20;
                offset += 20;
            }
        }
    }
}
