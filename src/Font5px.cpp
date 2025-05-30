#include "Font.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"

extern "C"
{
    static CustomFont myFont = {
        .height       = 6,
        .isWide       = false,
        .mappingCount = 141,
    };

    static GlyphMapping myMapping[141] = {
        {.codepoint = 0x20, .index = 0},    {.codepoint = 0x21, .index = 1},    {.codepoint = 0x22, .index = 2},
        {.codepoint = 0x23, .index = 3},    {.codepoint = 0x25, .index = 4},    {.codepoint = 0x27, .index = 5},
        {.codepoint = 0x28, .index = 6},    {.codepoint = 0x29, .index = 7},    {.codepoint = 0x2a, .index = 8},
        {.codepoint = 0x2b, .index = 9},    {.codepoint = 0x2c, .index = 10},   {.codepoint = 0x2d, .index = 11},
        {.codepoint = 0x2e, .index = 12},   {.codepoint = 0x2f, .index = 13},   {.codepoint = 0x30, .index = 14},
        {.codepoint = 0x31, .index = 15},   {.codepoint = 0x32, .index = 16},   {.codepoint = 0x33, .index = 17},
        {.codepoint = 0x34, .index = 18},   {.codepoint = 0x35, .index = 19},   {.codepoint = 0x36, .index = 20},
        {.codepoint = 0x37, .index = 21},   {.codepoint = 0x38, .index = 22},   {.codepoint = 0x39, .index = 23},
        {.codepoint = 0x3a, .index = 24},   {.codepoint = 0x3b, .index = 25},   {.codepoint = 0x3c, .index = 26},
        {.codepoint = 0x3d, .index = 27},   {.codepoint = 0x3e, .index = 28},   {.codepoint = 0x3f, .index = 29},
        {.codepoint = 0x41, .index = 30},   {.codepoint = 0x42, .index = 31},   {.codepoint = 0x43, .index = 32},
        {.codepoint = 0x44, .index = 33},   {.codepoint = 0x45, .index = 34},   {.codepoint = 0x46, .index = 35},
        {.codepoint = 0x47, .index = 36},   {.codepoint = 0x48, .index = 37},   {.codepoint = 0x49, .index = 38},
        {.codepoint = 0x4a, .index = 39},   {.codepoint = 0x4b, .index = 40},   {.codepoint = 0x4c, .index = 41},
        {.codepoint = 0x4d, .index = 42},   {.codepoint = 0x4e, .index = 43},   {.codepoint = 0x4f, .index = 44},
        {.codepoint = 0x50, .index = 45},   {.codepoint = 0x51, .index = 46},   {.codepoint = 0x52, .index = 47},
        {.codepoint = 0x53, .index = 48},   {.codepoint = 0x54, .index = 49},   {.codepoint = 0x55, .index = 50},
        {.codepoint = 0x56, .index = 51},   {.codepoint = 0x57, .index = 52},   {.codepoint = 0x58, .index = 53},
        {.codepoint = 0x59, .index = 54},   {.codepoint = 0x5a, .index = 55},   {.codepoint = 0x5f, .index = 56},
        {.codepoint = 0xffff, .index = 57}, {.codepoint = 0x817b, .index = 9},  {.codepoint = 0x8279, .index = 55},
        {.codepoint = 0x8278, .index = 54}, {.codepoint = 0x8277, .index = 53}, {.codepoint = 0x8276, .index = 52},
        {.codepoint = 0x8275, .index = 51}, {.codepoint = 0x8274, .index = 50}, {.codepoint = 0x8273, .index = 49},
        {.codepoint = 0x8172, .index = 28}, {.codepoint = 0x8272, .index = 48}, {.codepoint = 0x8171, .index = 26},
        {.codepoint = 0x8271, .index = 47}, {.codepoint = 0x8270, .index = 46}, {.codepoint = 0x826f, .index = 45},
        {.codepoint = 0x826e, .index = 44}, {.codepoint = 0x826d, .index = 43}, {.codepoint = 0x826c, .index = 42},
        {.codepoint = 0x826b, .index = 41}, {.codepoint = 0x816a, .index = 7},  {.codepoint = 0x826a, .index = 40},
        {.codepoint = 0x8169, .index = 6},  {.codepoint = 0x8269, .index = 39}, {.codepoint = 0x8168, .index = 2},
        {.codepoint = 0x8268, .index = 38}, {.codepoint = 0x8167, .index = 2},  {.codepoint = 0x8267, .index = 37},
        {.codepoint = 0x8166, .index = 5},  {.codepoint = 0x8266, .index = 36}, {.codepoint = 0x8165, .index = 5},
        {.codepoint = 0x8265, .index = 35}, {.codepoint = 0x8264, .index = 34}, {.codepoint = 0x8263, .index = 33},
        {.codepoint = 0x8262, .index = 32}, {.codepoint = 0x8261, .index = 31}, {.codepoint = 0x8260, .index = 30},
        {.codepoint = 0x815e, .index = 13}, {.codepoint = 0x8258, .index = 23}, {.codepoint = 0x8257, .index = 22},
        {.codepoint = 0x8256, .index = 21}, {.codepoint = 0x8255, .index = 20}, {.codepoint = 0x8254, .index = 19},
        {.codepoint = 0x8253, .index = 18}, {.codepoint = 0x8252, .index = 17}, {.codepoint = 0x8151, .index = 56},
        {.codepoint = 0x8251, .index = 16}, {.codepoint = 0x8250, .index = 15}, {.codepoint = 0x824f, .index = 14},
        {.codepoint = 0x8149, .index = 1},  {.codepoint = 0x8147, .index = 25}, {.codepoint = 0x8146, .index = 24},
        {.codepoint = 0x8143, .index = 10}, {.codepoint = 0x8142, .index = 12}, {.codepoint = 0x8140, .index = 0},
        {.codepoint = 0x8196, .index = 8},  {.codepoint = 0x8194, .index = 3},  {.codepoint = 0x8193, .index = 4},
        {.codepoint = 0x8181, .index = 27}, {.codepoint = 0x61, .index = 30},   {.codepoint = 0x62, .index = 31},
        {.codepoint = 0x63, .index = 32},   {.codepoint = 0x64, .index = 33},   {.codepoint = 0x65, .index = 34},
        {.codepoint = 0x66, .index = 35},   {.codepoint = 0x67, .index = 36},   {.codepoint = 0x68, .index = 37},
        {.codepoint = 0x69, .index = 38},   {.codepoint = 0x6a, .index = 39},   {.codepoint = 0x6b, .index = 40},
        {.codepoint = 0x6c, .index = 41},   {.codepoint = 0x6d, .index = 42},   {.codepoint = 0x6e, .index = 43},
        {.codepoint = 0x6f, .index = 44},   {.codepoint = 0x70, .index = 45},   {.codepoint = 0x71, .index = 46},
        {.codepoint = 0x72, .index = 47},   {.codepoint = 0x73, .index = 48},   {.codepoint = 0x74, .index = 49},
        {.codepoint = 0x75, .index = 50},   {.codepoint = 0x76, .index = 51},   {.codepoint = 0x77, .index = 52},
        {.codepoint = 0x78, .index = 53},   {.codepoint = 0x79, .index = 54},   {.codepoint = 0x7a, .index = 55},
    };

    static Glyph5PX myGlyphs[58] = {
        {.width = 3, .rows = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},
        {.width = 2, .rows = {0x7f, 0x7f, 0x7f, 0xff, 0x7f, 0xff}},
        {.width = 4, .rows = {0x5f, 0x5f, 0xff, 0xff, 0xff, 0xff}},
        {.width = 6, .rows = {0xaf, 0x07, 0xaf, 0x07, 0xaf, 0xff}},
        {.width = 5, .rows = {0x6f, 0xdf, 0xbf, 0x6f, 0xff, 0xff}},
        {.width = 2, .rows = {0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff}},
        {.width = 3, .rows = {0xbf, 0x7f, 0x7f, 0x7f, 0xbf, 0xff}},
        {.width = 3, .rows = {0x7f, 0xbf, 0xbf, 0xbf, 0x7f, 0xff}},
        {.width = 4, .rows = {0x5f, 0xbf, 0x5f, 0xff, 0xff, 0xff}},
        {.width = 4, .rows = {0xff, 0xbf, 0x1f, 0xbf, 0xff, 0xff}},
        {.width = 2, .rows = {0xff, 0xff, 0xff, 0x7f, 0x7f, 0xff}},
        {.width = 4, .rows = {0xff, 0xff, 0x1f, 0xff, 0xff, 0xff}},
        {.width = 2, .rows = {0xff, 0xff, 0xff, 0x7f, 0xff, 0xff}},
        {.width = 3, .rows = {0xff, 0xbf, 0xbf, 0x7f, 0x7f, 0xff}},
        {.width = 4, .rows = {0x1f, 0x5f, 0x5f, 0x5f, 0x1f, 0xff}},
        {.width = 4, .rows = {0xbf, 0x3f, 0xbf, 0xbf, 0x1f, 0xff}},
        {.width = 4, .rows = {0x1f, 0xdf, 0x1f, 0x7f, 0x1f, 0xff}},
        {.width = 4, .rows = {0x1f, 0xdf, 0x1f, 0xdf, 0x1f, 0xff}},
        {.width = 4, .rows = {0x5f, 0x5f, 0x1f, 0xdf, 0xdf, 0xff}},
        {.width = 4, .rows = {0x1f, 0x7f, 0x1f, 0xdf, 0x1f, 0xff}},
        {.width = 4, .rows = {0x1f, 0x7f, 0x1f, 0x5f, 0x1f, 0xff}},
        {.width = 4, .rows = {0x1f, 0xdf, 0xdf, 0xdf, 0xdf, 0xff}},
        {.width = 4, .rows = {0x1f, 0x5f, 0x1f, 0x5f, 0x1f, 0xff}},
        {.width = 4, .rows = {0x1f, 0x5f, 0x1f, 0xdf, 0x1f, 0xff}},
        {.width = 2, .rows = {0xff, 0x7f, 0xff, 0x7f, 0xff, 0xff}},
        {.width = 2, .rows = {0xff, 0x7f, 0xff, 0x7f, 0x7f, 0xff}},
        {.width = 4, .rows = {0xdf, 0xbf, 0x7f, 0xbf, 0xdf, 0xff}},
        {.width = 4, .rows = {0xff, 0x1f, 0xff, 0x1f, 0xff, 0xff}},
        {.width = 4, .rows = {0x7f, 0xbf, 0xdf, 0xbf, 0x7f, 0xff}},
        {.width = 4, .rows = {0x1f, 0xdf, 0x9f, 0xff, 0xbf, 0xff}},
        {.width = 4, .rows = {0xbf, 0x5f, 0x1f, 0x5f, 0x5f, 0xff}},
        {.width = 4, .rows = {0x3f, 0x5f, 0x3f, 0x5f, 0x3f, 0xff}},
        {.width = 4, .rows = {0x9f, 0x7f, 0x7f, 0x7f, 0x9f, 0xff}},
        {.width = 4, .rows = {0x3f, 0x5f, 0x5f, 0x5f, 0x3f, 0xff}},
        {.width = 4, .rows = {0x1f, 0x7f, 0x1f, 0x7f, 0x1f, 0xff}},
        {.width = 4, .rows = {0x1f, 0x7f, 0x1f, 0x7f, 0x7f, 0xff}},
        {.width = 5, .rows = {0x9f, 0x7f, 0x4f, 0x6f, 0x9f, 0xff}},
        {.width = 4, .rows = {0x5f, 0x5f, 0x1f, 0x5f, 0x5f, 0xff}},
        {.width = 2, .rows = {0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0xff}},
        {.width = 3, .rows = {0xbf, 0xbf, 0xbf, 0xbf, 0x3f, 0xff}},
        {.width = 4, .rows = {0x5f, 0x5f, 0x3f, 0x5f, 0x5f, 0xff}},
        {.width = 4, .rows = {0x7f, 0x7f, 0x7f, 0x7f, 0x1f, 0xff}},
        {.width = 6, .rows = {0x77, 0x27, 0x57, 0x77, 0x77, 0xff}},
        {.width = 5, .rows = {0x6f, 0x2f, 0x0f, 0x4f, 0x6f, 0xff}},
        {.width = 4, .rows = {0xbf, 0x5f, 0x5f, 0x5f, 0xbf, 0xff}},
        {.width = 4, .rows = {0x3f, 0x5f, 0x1f, 0x7f, 0x7f, 0xff}},
        {.width = 5, .rows = {0xbf, 0x5f, 0x5f, 0x5f, 0x8f, 0xff}},
        {.width = 4, .rows = {0x3f, 0x5f, 0x3f, 0x5f, 0x5f, 0xff}},
        {.width = 4, .rows = {0x9f, 0x7f, 0xbf, 0xdf, 0x3f, 0xff}},
        {.width = 4, .rows = {0x1f, 0xbf, 0xbf, 0xbf, 0xbf, 0xff}},
        {.width = 4, .rows = {0x5f, 0x5f, 0x5f, 0x5f, 0x1f, 0xff}},
        {.width = 4, .rows = {0x5f, 0x5f, 0x5f, 0x1f, 0xbf, 0xff}},
        {.width = 6, .rows = {0x77, 0x77, 0x57, 0x07, 0xaf, 0xff}},
        {.width = 4, .rows = {0x5f, 0x5f, 0xbf, 0x5f, 0x5f, 0xff}},
        {.width = 4, .rows = {0x5f, 0x5f, 0x1f, 0xbf, 0xbf, 0xff}},
        {.width = 4, .rows = {0x1f, 0xdf, 0xbf, 0x7f, 0x1f, 0xff}},
        {.width = 4, .rows = {0xff, 0xff, 0xff, 0xff, 0x1f, 0xff}},
        {.width = 4, .rows = {0x5f, 0xbf, 0x5f, 0xbf, 0x5f, 0xff}},
    };

    Glyph5PX* getGlyph5px(uint16_t codepoint)
    {
        for (int32_t i = 0; i < myFont.mappingCount; i++)
        {
            if (myMapping[i].codepoint == codepoint) return &myGlyphs[myMapping[i].index];
        }

        return &myGlyphs[myFont.mappingCount - 1];
    }

    static uint8_t getWidthCustom(uint16_t codepoint)
    {
        return getGlyph5px(codepoint)->width;
    }

    static uint16_t getRowCustom(uint16_t codepoint, uint8_t row)
    {
        if (row > myFont.height) return 0;

        return getGlyph5px(codepoint)->rows[row] << 8;
    }

    static uint16_t getCodePointCustom(const uint8_t* string, uint32_t index)
    {
        const uint8_t* ptr = jis_at_index(string, index);
        uint8_t firstByte  = *ptr;
        return firstByte;
    }

    Font myFont5px = {
        .height       = 6,
        .getWidth     = getWidthCustom,
        .getRow       = getRowCustom,
        .getCodePoint = getCodePointCustom,
    };
}