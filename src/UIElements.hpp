#pragma once
#include "Font.hpp"
#include "Helper.hpp"
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

class SimpleTextSprite
{
private:
    uint16_t uvX      = 0;
    uint16_t uvY      = 0;
    uint16_t uvWidth  = 0;
    uint16_t uvHeight = 0;

public:
    constexpr SimpleTextSprite() {};
    constexpr SimpleTextSprite(uint16_t uvX, uint16_t uvY)
        : uvX(uvX)
        , uvY(uvY)
    {
    }

    void draw(Font* font, const uint8_t* string);
    void draw(Font* font, const char* string);
    void render(int32_t posX, int32_t posY, int32_t color, int32_t offset, bool hasShadow);
    void clear();
};

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

    struct Sprite
    {
        int16_t uvX;
        int16_t uvV;
        int16_t width;
        int16_t height;
        int8_t texture_page;
        uint16_t clut;

        void render(int16_t posX, int16_t posY, uint8_t layer, uint8_t flag) const;
    };

    struct BorderBox
    {
        int16_t posX;
        int16_t posY;
        int16_t width;
        int16_t height;
        uint32_t color1;
        uint32_t color2;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t layer;

        void render() const;
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

        void render(int32_t layer) const;
    };

    struct SelectionSprite
    {
        uint8_t uMin;
        uint8_t uMax;
        uint8_t vMin;
        uint8_t vMax;
        int8_t posX;
        int8_t posY;
        uint8_t width;
        uint8_t height;
    };

    struct Inset
    {
        int16_t posX;
        int16_t posY;
        int16_t width;
        int16_t height;

        void render(int32_t order) const;
    };

    struct Line4Points
    {
        int16_t x1;
        int16_t x2;
        int16_t x3;
        int16_t x4;
        int16_t y1;
        int16_t y2;
        int16_t y3;
        int16_t y4;

        void render(uint32_t color1, uint32_t color2, int32_t layer) const;
    };

    void removeStaticUIBox(int32_t id);
    void drawTextSprite(TextSprite& entry);
    void renderTextSprite2(TextSprite& entry, int32_t offsetX, int32_t offsetY);
    void renderTextSprite(TextSprite& entry);
    void initSpecialSprite(IconSprite& sprite, Special special);
    void renderSeperatorLines(const Line* linePtr, int32_t count, int32_t layer);
    void renderRectPolyFT4(int16_t posX,
                           int16_t posY,
                           uint32_t width,
                           uint32_t height,
                           uint8_t texX,
                           uint8_t texY,
                           uint16_t texturePage,
                           uint16_t clut,
                           int zIndex,
                           char flag);
    bool createMenuBox(int32_t instanceId,
                       int16_t posX,
                       int16_t posY,
                       int16_t width,
                       int16_t height,
                       uint8_t features,
                       BoxTickFunction tickFunc,
                       BoxRenderFunction renderFunc);
    bool isUIBoxAvailable(int32_t id);
    void closeUIBoxIfOpen(int32_t instanceId);
    void renderMenuTab(int32_t posX, int32_t width, bool isActive);
    void renderBox(int16_t posX,
                   int16_t posY,
                   uint16_t width,
                   uint16_t height,
                   uint8_t red,
                   uint8_t green,
                   uint8_t blue,
                   uint8_t flag,
                   int32_t layer);
    void renderBorderBox(int16_t posX,
                         int16_t posY,
                         int16_t width,
                         int16_t height,
                         uint32_t color1,
                         uint32_t color2,
                         uint8_t red,
                         uint8_t green,
                         uint8_t blue,
                         int32_t layer);
    void renderDigimonStatsBar(int32_t value, int32_t maxValue, int32_t width, int32_t posX, int32_t posY);
    void createAnimatedUIBox(int32_t instanceId,
                             uint8_t color,
                             uint8_t features,
                             const RECT* finalPos,
                             const RECT* startPos,
                             BoxTickFunction tickFunc,
                             BoxRenderFunction renderFunc);
    void removeAnimatedUIBox(int32_t boxId, RECT* target);

    void drawLine2P(uint32_t color, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int32_t order, uint32_t blend);
    void drawLine3P(uint32_t color,
                          int16_t x0,
                          int16_t y0,
                          int16_t x1,
                          int16_t y1,
                          int16_t x2,
                          int16_t y2,
                          int32_t order,
                          uint32_t blend);
}