#pragma once

#include "Font.hpp"
#include "UIElements.hpp"
#include "extern/dtl/runtime_array.hpp"
#include "extern/libgpu.hpp"

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
    bool hasShadow = false;
    uint8_t r      = 0xd6;
    uint8_t g      = 0xd6;
    uint8_t b      = 0xd6;
};

struct AtlasFont
{
public:
    AtlasFont();
    void init(Font* font, int32_t x, int32_t y, int32_t maxWidth = 64);

    AtlasString render(const char* string, int32_t x, int32_t y, RenderSettings settings = {}) const;
    AtlasString render(const uint8_t* string, int32_t x, int32_t y, RenderSettings settings = {}) const;
    void renderSlow(const char* string, int32_t x, int32_t y, int32_t depth, RenderSettings settings = {}) const;
    void renderSlow(const uint8_t* string, int32_t x, int32_t y, int32_t depth, RenderSettings settings = {}) const;

private:
    Font* font;
    dtl::runtime_array<AtlasGlyph> glyphs;
    uint8_t tpage;
};

const AtlasFont& getAtlas5px();
const AtlasFont& getAtlas7px();
const AtlasFont& getAtlasVanilla();
const AtlasFont& getAtlasNumber();
void initFonts();