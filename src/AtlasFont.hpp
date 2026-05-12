#pragma once

#include "Font.hpp"
#include "UIElements.hpp"
#include "extern/dtl/runtime_array.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"

constexpr RGB8 TEXT_COLOR_WHITE  = {.red = 0xd6, .green = 0xd6, .blue = 0xd6};
constexpr RGB8 TEXT_COLOR_BLUE   = {.red = 0x29, .green = 0x8e, .blue = 0xd6};
constexpr RGB8 TEXT_COLOR_YELLOW = {.red = 0xFF, .green = 0xFF, .blue = 0x53};

struct AtlasString
{
    dtl::runtime_array<SPRT> data{};
    uint8_t tpage;
    uint8_t width;
    uint8_t height;

    AtlasString() {};
    AtlasString(dtl::runtime_array<SPRT> data, uint8_t tpage, uint8_t width, uint8_t height)
        : data(dtl::move(data))
        , tpage(tpage)
        , width(width)
        , height(height)
    {
    }

    void render(int32_t depth) const;
    int32_t getWidth() const;
    void enableShadow(bool enabled);
    void setColor(RGB8 color);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setPosition(int32_t x, int32_t y);
    void setAlignment(RECT rect, AlignmentX alignX, AlignmentY alignY);
};

struct AtlasGlyph
{
    RECT rect;
    uint8_t width;
};

struct RenderSettings
{
    int16_t x;
    int16_t y;
    bool hasShadow    = false;
    RGB8 color        = TEXT_COLOR_WHITE;
    int16_t width     = 0;
    int16_t height    = 0;
    AlignmentX alignX = AlignmentX::LEFT;
    AlignmentY alignY = AlignmentY::TOP;
};

struct AtlasFont
{
public:
    AtlasFont();
    void init(Font* font, int32_t x, int32_t y, int32_t maxWidth = 64);

    AtlasString render(const char* string, const RenderSettings& settings) const;
    AtlasString render(const uint8_t* string, const RenderSettings& settings) const;
    void renderSlow(const char* string, int32_t depth, const RenderSettings& settings) const;
    void renderSlow(const uint8_t* string, int32_t depth, const RenderSettings& settings) const;

private:
    int32_t getWidth(const uint8_t* string) const;
    Position getPosition(const uint8_t* string, const RenderSettings& settings) const;

    Font* font;
    dtl::runtime_array<AtlasGlyph> glyphs;
    uint8_t tpage;
};

const AtlasFont& getAtlas5px();
const AtlasFont& getAtlas7px();
const AtlasFont& getAtlasVanilla();
const AtlasFont& getAtlasNumber();
void initFonts();