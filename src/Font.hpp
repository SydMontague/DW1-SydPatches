#pragma once
#include "extern/dw1.hpp"

extern "C"
{
    using GetWidthFn     = uint8_t (*)(uint16_t);
    using GetRowFn       = uint16_t (*)(uint16_t, uint8_t);
    using GetCodePointFn = uint16_t (*)(const uint8_t*, uint32_t);

    struct CustomFont
    {
        uint8_t height;
        bool isWide;
        uint16_t mappingCount;
    };

    struct GlyphMapping
    {
        uint16_t codepoint;
        uint16_t index;
    };

    struct Glyph5PX
    {
        uint8_t width;
        uint8_t rows[6]; // height
    };

    struct Glyph7PX
    {
        uint8_t width;
        uint8_t rows[8]; // height
    };
    
    struct Font
    {
        uint8_t height;
        GetWidthFn getWidth;
        GetRowFn getRow;
        GetCodePointFn getCodePoint;
    };

    extern Font vanillaFont;
    extern Font myFont5px;
    extern Font myFont7px;

    GlyphData* getGlyphVanilla(uint16_t codepoint);
    uint8_t getWidthVanilla(uint16_t codepoint);
    uint16_t getRowVanilla(uint16_t codepoint, uint8_t row);
    uint16_t getCodePointVanilla(const uint8_t* string, uint32_t index);

    uint16_t drawStringNew(Font* font, const uint8_t* string, int16_t start_x, int16_t start_y);

    void renderStringNew(int32_t colorId,
                         int16_t posX,
                         int16_t posY,
                         int16_t uvWidth,
                         int16_t uvHeight,
                         int16_t uvX,
                         int16_t uvY,
                         int32_t offset,
                         int32_t hasShadow);
}