
#include "Font.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "UIElements.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"

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
            Sprite s;
            s.clut         = getClut(208, 488);
            s.texture_page = 27;
            s.width        = 8;
            s.height       = 12;
            s.uvX          = buffer[numDigits - i - 1] * 8;
            s.uvV          = 240;
            s.render(posX + (empty + i) * 8, posY, layer, 0);
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
            Sprite s;
            s.clut         = getClut(256, 484);
            s.texture_page = 30;
            s.width        = 8;
            s.height       = 9;
            s.uvX          = buffer[numDigits - i - 1] * 8;
            s.uvV          = 183;
            s.render(posX + (empty + i) * 7, posY, layer, 0);
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
            Sprite s;
            s.clut         = getClut(256, 484);
            s.texture_page = 30;
            s.width        = 6;
            s.height       = 10;
            s.uvX          = 137 + buffer[numDigits - i - 1] * 6;
            s.uvV          = 156;
            s.render(posX + (empty + i) * 6, posY, layer, 0);
        }
    }
}