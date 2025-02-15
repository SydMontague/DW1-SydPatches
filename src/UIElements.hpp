#pragma once
#include "Font.hpp"
#include "extern/stddef.hpp"
#include "Helper.hpp"

extern "C"
{
    enum class AlignmentX : uint8_t
    {
        LEFT,
        CENTER,
        RIGHT,
    };

    enum class AlignmentY : uint8_t
    {
        TOP,
        CENTER,
        BOTTOM,
    };

    struct TextSprite
    {
        Font* font;
        const char* string;

        uint16_t uvX;
        uint16_t uvY;
        uint16_t uvWidth;
        uint16_t uvHeight;

        int16_t posX;
        int16_t posY;
        uint16_t boxWidth;
        uint16_t boxHeight;

        AlignmentX alignmentX : 4;
        AlignmentY alignmentY : 4;
        int8_t color;
        uint8_t layer;
        uint8_t hasShadow;
    };

    struct IconSprite
    {
        int16_t posX;
        int16_t posY;
        int16_t width;
        int16_t height;
        int16_t uvX;
        int16_t uvY;
        uint8_t flag;
        int8_t texture_page;
        uint16_t clut;
    };

    static_assert(sizeof(IconSprite));

    struct Inset
    {
        int16_t posX;
        int16_t posY;
        int16_t width;
        int16_t height;
    };

    void drawTextSprite(TextSprite& entry);
    void renderTextSprite(TextSprite& entry);
    void initSpecialSprite(IconSprite& sprite, Special special);
}