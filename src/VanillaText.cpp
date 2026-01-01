#include "VanillaText.hpp"

#include "Font.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "UIElements.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"

namespace
{
    void renderPredrawnNumber(int32_t posX,
                              int32_t posY,
                              int32_t colorId,
                              int16_t width,
                              int16_t height,
                              int32_t uvX,
                              int32_t uvY,
                              int32_t texturePage,
                              int32_t clut,
                              int32_t layer)
    {
        auto* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyFT4(prim);
        prim->r0    = TEXT_COLORS[colorId].red;
        prim->g0    = TEXT_COLORS[colorId].green;
        prim->b0    = TEXT_COLORS[colorId].blue;
        prim->clut  = clut;
        prim->tpage = texturePage;
        setUVDataPolyFT4(prim, uvX, uvY, width, height);
        setPosDataPolyFT4(prim, posX, posY, width, height);
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + layer, prim);
        libgs_GsSetWorkBase(prim + 1);
    }
} // namespace

extern "C"
{
    void setEntityTextDigit(POLY_FT4* prim, int32_t dx, int32_t dy)
    {
        libgpu_SetPolyFT4(prim);
        prim->tpage = 0x1e;
        prim->clut  = getClut(dx, dy);
    }

    void initStringFT4(POLY_FT4* prim)
    {
        libgpu_SetPolyFT4(prim);
        prim->tpage = 0x1b;
        prim->clut  = getClut(208, 488);
    }

    void convertValueToDigits(int32_t maxDigits, int32_t value, int32_t* outDigits, int32_t* outBuffer)
    {
        // the vanilla function uses sprintf and Gabumon knows what, I think this version is cleaner
        // TODO vanilla doesn't handle negative numbers at all?
        *outDigits = getDigitCount(value);

        auto current = value;
        for (int32_t i = 0; i < maxDigits; i++)
        {
            outBuffer[i] = current % 10;
            current /= 10;
        }
    }

    void renderNumber(int32_t colorId, int32_t posX, int32_t posY, int32_t digits, int32_t value, int32_t layer)
    {
        dtl::array<int32_t, 16> buffer{};
        int32_t numDigits;
        convertValueToDigits(digits, value, &numDigits, buffer.data());
        const auto empty = digits - numDigits;

        for (int32_t i = 0; i < numDigits; i++)
        {
            const auto finalX = posX + (empty + i) * 8;
            const auto finalU = buffer[numDigits - i - 1] * 8;
            renderPredrawnNumber(finalX, posY, colorId, 8, 12, finalU, 240, 27, getClut(208, 488), layer);
        }
    }

    void drawEntityText(int32_t colorId, int32_t digits, int32_t posX, int32_t posY, int32_t value, int32_t layer)
    {
        dtl::array<int32_t, 16> buffer{};
        int32_t numDigits;
        convertValueToDigits(digits, value, &numDigits, buffer.data());
        const auto empty = digits - numDigits;

        for (int32_t i = 0; i < numDigits; i++)
        {
            const auto finalX = posX + (empty + i) * 7;
            const auto finalU = buffer[numDigits - i - 1] * 8;
            renderPredrawnNumber(finalX, posY, colorId, 8, 9, finalU, 183, 30, getClut(256, 484), layer);
        }
    }

    void renderItemAmount(int32_t colorId, int32_t digits, int32_t posX, int32_t posY, int32_t value, int32_t layer)
    {
        dtl::array<int32_t, 16> buffer{};
        int32_t numDigits;
        convertValueToDigits(digits, value, &numDigits, buffer.data());
        const auto empty = digits - numDigits;

        for (int32_t i = 0; i < numDigits; i++)
        {
            const auto finalX = posX + (empty + i) * 6;
            const auto finalU = 137 + buffer[numDigits - i - 1] * 6;
            renderPredrawnNumber(finalX, posY, colorId, 6, 10, finalU, 156, 30, getClut(256, 484), layer);
        }
    }

    void renderString(int32_t colorId,
                      int16_t posX,
                      int16_t posY,
                      int16_t uvWidth,
                      int16_t uvHeight,
                      int16_t uvX,
                      int16_t uvY,
                      int32_t offset,
                      int32_t hasShadow)
    {
        renderStringNew(colorId, posX, posY, uvWidth, uvHeight, (uvX / 4) + 704, uvY + 256, offset, hasShadow);
    }
}