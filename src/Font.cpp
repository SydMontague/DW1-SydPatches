#include "Font.hpp"

#include "UIElements.hpp"
#include "Utils.hpp"
#include "VanillaText.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"

extern "C"
{
    constexpr uint32_t RENDER_COLS      = 128;
    constexpr uint32_t RENDER_ROWS      = 32;
    constexpr uint32_t RENDER_AREA_SIZE = RENDER_COLS * RENDER_ROWS;

    uint32_t RENDER_AREA_POINTER          = 0;
    uint8_t RENDER_AREA[RENDER_AREA_SIZE] = {0};

    Font vanillaFont = {
        .height       = 11,
        .getWidth     = getWidthVanilla,
        .getRow       = getRowVanilla,
        .getCodePoint = getCodePointVanilla,
    };

    GlyphData* getGlyphVanilla(uint16_t codepoint)
    {
        CHAR_TO_GLYPH_TABLE[78] = codepoint;
        uint32_t i              = 0;
        for (; CHAR_TO_GLYPH_TABLE[i] != codepoint; i++)
            ;

        return GLYPH_DATA + i;
    }

    uint8_t getWidthVanilla(uint16_t codepoint)
    {
        return getGlyphVanilla(codepoint)->width;
    }

    uint16_t getRowVanilla(uint16_t codepoint, uint8_t row)
    {
        return getGlyphVanilla(codepoint)->pixelData[row];
    }

    uint16_t getCodePointVanilla(const uint8_t* string, uint32_t index)
    {
        const uint8_t* ptr = jis_at_index(string, index);
        uint8_t firstByte  = *ptr;
        if (firstByte >= 0x80)
        {
            uint16_t bla = firstByte << 8 | ptr[1];
            return bla;
        }
        if (firstByte == 0x2e) return 0x8142;
        if (firstByte == 0x27) return 0x8175;

        return convertAsciiToJis(firstByte);
    }

    uint8_t* getRenderArea(uint32_t height, uint32_t width)
    {
        uint32_t size = height * ((width + 3) / 4) * 4;

        if ((RENDER_AREA_POINTER + size) > RENDER_AREA_SIZE) RENDER_AREA_POINTER = 0;

        uint8_t* ptr = RENDER_AREA + RENDER_AREA_POINTER;

        RENDER_AREA_POINTER += size;
        return ptr;
    }

    uint32_t getStringWidth(Font* font, const uint8_t* string)
    {
        GetCodePointFn getCodePoint = font->getCodePoint;
        GetWidthFn getWidth         = font->getWidth;
        uint32_t index              = 0;
        uint32_t width              = 0;
        while (true)
        {
            uint16_t codepoint = getCodePoint(string, index++);
            if (codepoint == 0) break;

            width += getWidth(codepoint);
        }

        return width;
    }

    uint16_t drawGlyphNew(Font* font, uint16_t codepoint, uint8_t* render_area, uint8_t offset_x, uint32_t string_width)
    {
        uint8_t* render_start = render_area + offset_x / 2;
        uint8_t font_height   = font->height;
        uint8_t glyph_width   = font->getWidth(codepoint);

        for (int i = 0; i < font->height; i++)
        {
            uint16_t row_data = font->getRow(codepoint, i);
            uint8_t* draw_row = render_start + (((string_width + 3) / 4) * 4 * i) / 2;
            uint8_t width     = glyph_width;

            if (offset_x % 2 == 1)
            {
                uint8_t tmp = *draw_row;
                if ((row_data & 0x8000) == 0) tmp = tmp | COLORCODE_HIGHBITS;
                *draw_row = tmp;
                draw_row++;
                width--;
                row_data = row_data << 1;
            }

            for (int j = 0; j < width; j += 2)
            {
                uint8_t tmp = 0;
                if ((row_data & 0x8000) == 0) tmp = COLORCODE_LOWBITS;
                if ((row_data & 0x4000) == 0) tmp = tmp | COLORCODE_HIGHBITS;
                *draw_row = tmp;
                draw_row++;
                row_data = row_data << 2;
            }
        }

        return glyph_width;
    }

    uint16_t drawStringNew(Font* font, const uint8_t* string, int16_t start_x, int16_t start_y)
    {
        uint32_t string_width = getStringWidth(font, string);
        uint16_t font_height  = font->height;
        uint8_t* render_area  = getRenderArea(font->height, string_width);
        memset(render_area, 0, string_width * font_height);

        RECT rect = {
            .x      = start_x,
            .y      = start_y,
            .width  = static_cast<int16_t>((string_width + 3) / 4),
            .height = static_cast<int16_t>(font->height),
        };

        uint32_t index = 0;
        uint32_t width = 0;
        while (true)
        {
            uint16_t codepoint = font->getCodePoint(string, index++);
            if (codepoint == 0) break;

            width += drawGlyphNew(font, codepoint, render_area, width, string_width);
        }

        libgpu_LoadImage(&rect, reinterpret_cast<uint32_t*>(render_area));
        return width;
    }

    void renderStringNew(int32_t colorId,
                         int16_t posX,
                         int16_t posY,
                         int16_t uvWidth,
                         int16_t uvHeight,
                         int16_t uvX,
                         int16_t uvY,
                         int32_t offset,
                         int32_t hasShadow)
    {
        POLY_FT4* base = (POLY_FT4*)libgs_GsGetWorkBase();
        libgpu_SetPolyFT4(base);
        base->tpage = (uvX / 64) + (uvY >= 256 ? 0x10 : 0);
        base->clut  = libgpu_GetClut(0xd0, 0x1e8);
        if (colorId != -1)
        {
            base->r0 = TEXT_COLORS[colorId].red;
            base->g0 = TEXT_COLORS[colorId].green;
            base->b0 = TEXT_COLORS[colorId].blue;
        }
        else
        {
            base->r0 = 0;
            base->g0 = 0;
            base->b0 = 0;
        }
        setUVDataPolyFT4(base, 4 * (uvX % 64), uvY % 256, uvWidth, uvHeight);
        setPosDataPolyFT4(base, posX, posY, uvWidth, uvHeight);
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + offset, base);
        libgs_GsSetWorkBase(base + 1);

        if (hasShadow) renderStringNew(-1, posX + 1, posY + 1, uvWidth, uvHeight, uvX, uvY, offset, 0);
    }
}