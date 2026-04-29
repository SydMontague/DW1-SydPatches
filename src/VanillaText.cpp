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
    struct Ascii2ShiftJIS
    {
        uint16_t shiftJis;
        uint16_t ascii;
    };

    constexpr dtl::array<Ascii2ShiftJIS, 3> CHAR_MAPPING_NORMAL{{
        {.shiftJis = 0x824F, .ascii = 0x30},
        {.shiftJis = 0x8260, .ascii = 0x41},
        {.shiftJis = 0x8281, .ascii = 0x61},
    }};

    constexpr dtl::array<Ascii2ShiftJIS, 33> CHAR_MAPPING_SPECIAL{{
        {.shiftJis = 0x8140, .ascii = 0x20}, {.shiftJis = 0x8149, .ascii = 0x21}, {.shiftJis = 0x8168, .ascii = 0x22},
        {.shiftJis = 0x8194, .ascii = 0x23}, {.shiftJis = 0x8190, .ascii = 0x24}, {.shiftJis = 0x8193, .ascii = 0x25},
        {.shiftJis = 0x8195, .ascii = 0x26}, {.shiftJis = 0x8166, .ascii = 0x27}, {.shiftJis = 0x8169, .ascii = 0x28},
        {.shiftJis = 0x816a, .ascii = 0x29}, {.shiftJis = 0x8196, .ascii = 0x2a}, {.shiftJis = 0x817b, .ascii = 0x2b},
        {.shiftJis = 0x8143, .ascii = 0x2c}, {.shiftJis = 0x817c, .ascii = 0x2d}, {.shiftJis = 0x8142, .ascii = 0x2e},
        {.shiftJis = 0x815e, .ascii = 0x2f}, {.shiftJis = 0x8146, .ascii = 0x3a}, {.shiftJis = 0x8147, .ascii = 0x3b},
        {.shiftJis = 0x8171, .ascii = 0x3c}, {.shiftJis = 0x8181, .ascii = 0x3d}, {.shiftJis = 0x8172, .ascii = 0x3e},
        {.shiftJis = 0x8148, .ascii = 0x3f}, {.shiftJis = 0x8197, .ascii = 0x40}, {.shiftJis = 0x816d, .ascii = 0x5b},
        {.shiftJis = 0x815f, .ascii = 0x5c}, {.shiftJis = 0x816e, .ascii = 0x5d}, {.shiftJis = 0x814f, .ascii = 0x5e},
        {.shiftJis = 0x8151, .ascii = 0x5f}, {.shiftJis = 0x8165, .ascii = 0x60}, {.shiftJis = 0x816f, .ascii = 0x7b},
        {.shiftJis = 0x8162, .ascii = 0x7c}, {.shiftJis = 0x8170, .ascii = 0x7d}, {.shiftJis = 0x8150, .ascii = 0x7e},
    }};

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

    bool isAsciiEncoded(const char* value)
    {
        return *value > -1;
    }

    uint16_t convertAsciiToJis(uint8_t input)
    {
        auto specialOffset = 0;
        auto charType      = 0;

        if (input >= 0x20 && input <= 0x2F)
            specialOffset = 1;
        else if (input >= 0x30 && input <= 0x39)
            charType = 0;
        else if (input >= 0x3A && input <= 0x40)
            specialOffset = 11;
        else if (input >= 0x41 && input <= 0x5A)
            charType = 1;
        else if (input >= 0x5B && input <= 0x60)
            specialOffset = 0x25;
        else if (input >= 0x61 && input <= 0x7A)
            charType = 2;
        else if (input >= 0x7B && input <= 0x7E)
            specialOffset = 0x3F;
        else
            return 0;

        if (specialOffset == 0)
        {
            auto& mapping = CHAR_MAPPING_NORMAL[charType];
            return input + mapping.shiftJis - mapping.ascii;
        }
        else
            return CHAR_MAPPING_SPECIAL[(input - 0x20) - (specialOffset - 1)].shiftJis;
    }

    int32_t swapShortBytes(int32_t input)
    {
        return (input >> 8 | input << 8) & 0xFFFF;
    }

    void asciiToShiftJIS(uint8_t* input, uint16_t* output)
    {
        const auto length = strlen(input);

        for (int32_t i = 0; i < length; i++)
            output[i] = swapShortBytes(convertAsciiToJis(input[i]));

        output[length] = 0;
    }
}