#include "AtlasFont.hpp"

#include "Font.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "Timestamp.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dtl/runtime_array.hpp"

namespace
{
    constexpr auto CLUT_X = 208;
    constexpr auto CLUT_Y = 489;

    AtlasFont fontAtlas5px;
    AtlasFont fontAtlas7px;
    AtlasFont fontAtlasNumbers;
    AtlasFont fontAtlasVanilla;

    struct JISIterator
    {
        const uint8_t* string;

        uint16_t operator*()
        {
            auto val1 = string[0];
            if ((val1 & 0x80) == 0) return val1;

            return val1 << 8 | string[1];
        }

        JISIterator& operator++()
        {
            auto val1 = string[0];
            string += ((val1 & 0x80) != 0) + 1;
            return *this;
        }

        JISIterator operator++(int)
        {
            JISIterator old = *this;
            auto val1       = string[0];
            string += ((val1 & 0x80) != 0) + 1;
            return old;
        }
    };

    AtlasGlyph predrawGlyph(Font* font, int32_t index, int16_t x, int16_t y)
    {
        dtl::array<uint8_t, 16 * 16> area{};

        uint8_t font_height   = font->height;
        uint8_t glyph_width   = font->getGlyphWidth(index);
        auto vram_width       = ((glyph_width + 3) / 4) * 4;
        uint8_t* render_start = area.data();

        for (int i = 0; i < font->height; i++)
        {
            uint16_t row_data = font->getGlyphRow(index, i);
            uint8_t* draw_row = render_start + (vram_width * i) / 2;

            for (int j = 0; j < glyph_width; j += 2)
            {
                uint8_t tmp = 0;
                if ((row_data & 0x8000) == 0) tmp = 1;
                if ((row_data & 0x4000) == 0) tmp = tmp | 0x10;
                *draw_row = tmp;
                draw_row++;
                row_data = row_data << 2;
            }
        }

        for (int32_t i = 1; i < font->height; i++)
        {
            uint8_t* src_row = render_start + (vram_width * (i - 1)) / 2;
            uint8_t* dst_row = render_start + (vram_width * i) / 2;
            for (int32_t j = 1; j < glyph_width; j++)
            {
                auto y_dst = i / 2;
                auto x_dst = j / 2;
                auto y_src = (i - 1) / 2;
                auto x_src = (j - 1) / 2;

                auto src        = src_row[x_src];
                auto& dst       = dst_row[x_dst];
                auto dst_offset = 4 * (j % 2);
                auto src_offset = 4 * ((j % 2) ^ 1);

                if ((src & (0x1 << src_offset)) != 0 && (dst & (0x1 << dst_offset)) == 0) dst |= 2 << dst_offset;
            }
        }

        const RECT rect{
            .x      = x,
            .y      = y,
            .width  = static_cast<int16_t>((glyph_width + 3) / 4),
            .height = font_height,
        };
        libgpu_LoadImage(&rect, reinterpret_cast<uint32_t*>(render_start));
        libgpu_DrawSync(0);

        return {rect, glyph_width};
    }

    constexpr SPRT getSPRT(const RECT& data, int32_t x, int32_t y, RGB8 color, uint8_t baseClut)
    {
        SPRT prim;
        prim.tag.num = 4;
        prim.code    = 0x64;
        prim.r       = (color.red + 1) / 2;
        prim.g       = (color.green + 1) / 2;
        prim.b       = (color.blue + 1) / 2;
        prim.clut    = getClut(CLUT_X, CLUT_Y + baseClut);
        prim.x       = x;
        prim.y       = y;
        prim.u       = 4 * (data.x % 64);
        prim.v       = data.y % 256;
        prim.width   = data.width * 4;
        prim.height  = data.height;

        return prim;
    }

} // namespace

const AtlasFont& getAtlas5px()
{
    return fontAtlas5px;
}

const AtlasFont& getAtlas7px()
{
    return fontAtlas7px;
}

const AtlasFont& getAtlasNumbers()
{
    return fontAtlasNumbers;
}

const AtlasFont& getAtlasVanilla()
{
    return fontAtlasVanilla;
}

void initFonts()
{
    // move stat icons to new space, so that we have enough space for the vanilla font
    RECT src{
        .x      = 356,
        .y      = 186,
        .width  = 68 / 4,
        .height = 11,
    };
    libgpu_MoveImage(&src, 334, 225);

    // use space in ETCTIM8
    fontAtlasVanilla.init(&vanillaFont, 351, 176);
    fontAtlas5px.init(&myFont5px, 350, 242);
    // use space in ETCTIM5
    fontAtlas7px.init(&myFont7px, 768, 432);
    // use space below ETCTIM2
    fontAtlasNumbers.init(&fixedNumbersFont, 530, 418);
}

AtlasFont::AtlasFont() {}

void AtlasFont::init(Font* f, int32_t x, int32_t y, int32_t maxWidth)
{
    glyphs = dtl::runtime_array<AtlasGlyph>(f->glyph_count);
    font   = f;
    tpage  = (y / 256) * 16 + (x / 64);

    const auto end_x = min(((x / 64) + 1) * 64, x + maxWidth);
    auto current_x   = x;

    for (int32_t i = 0; i < f->glyph_count; i++)
    {
        auto width = f->getGlyphWidth(i);

        if (current_x + ((width + 3) / 4) > end_x)
        {
            current_x = x;
            y += f->height;
        }

        glyphs[i] = predrawGlyph(f, i, current_x, y);
        current_x += (width + 3) / 4;
    }
}

int32_t AtlasFont::getWidth(const uint8_t* string) const
{
    auto size = jis_len(string);
    JISIterator itr{string};
    auto width = 0;

    for (int32_t i = 0; i < size; i++)
    {
        auto index = font->getGlyphIndex(*itr++);
        width += glyphs[index].width;
    }

    return width;
}

Position AtlasFont::getPosition(const uint8_t* string, const RenderSettings& settings) const
{
    int16_t x = settings.x;
    int16_t y = settings.y;
    if (settings.width > 0 && settings.alignX != AlignmentX::LEFT)
    {
        auto width = getWidth(string);
        switch (settings.alignX)
        {
            case AlignmentX::LEFT: x += 0; break;
            case AlignmentX::CENTER: x += (settings.width - width) / 2; break;
            case AlignmentX::RIGHT: x += (settings.width - width); break;
        }
    }

    if (settings.height > 0)
    {
        switch (settings.alignY)
        {
            case AlignmentY::TOP: y += 0; break;
            case AlignmentY::CENTER: y += (settings.height - font->height) / 2; break;
            case AlignmentY::BOTTOM: y += (settings.height - font->height); break;
        }
    }

    return {x, y};
}

void AtlasFont::renderSlow(const char* string, int32_t depth, const RenderSettings& settings) const
{
    renderSlow(reinterpret_cast<const uint8_t*>(string), depth, settings);
}

void AtlasFont::renderSlow(const uint8_t* string, int32_t depth, const RenderSettings& settings) const
{
    auto pos  = getPosition(string, settings);
    auto size = jis_len(string);
    JISIterator itr{string};

    auto* prim2 = reinterpret_cast<SPRT*>(libgs_GsGetWorkBase());
    for (int32_t i = 0; i < size; i++)
    {
        auto index = font->getGlyphIndex(*itr++);
        auto glyph = glyphs[index];
        prim2[i]   = getSPRT(glyph.rect, pos.x, pos.y, settings.color, settings.baseClut);
        pos.x += glyph.width;
        addPrim(ACTIVE_ORDERING_TABLE->origin + depth, reinterpret_cast<GsOT_TAG*>(prim2 + i));
    }

    auto* prim   = reinterpret_cast<DR_TPAGE*>(prim2 + size);
    prim->tag[3] = 1;
    prim->code   = 0xe1000400 | tpage;
    addPrim(ACTIVE_ORDERING_TABLE->origin + depth, reinterpret_cast<GsOT_TAG*>(&prim->tag));
    libgs_GsSetWorkBase(prim + 1);
}

AtlasString AtlasFont::render(const char* string, const RenderSettings& settings) const
{
    return render(reinterpret_cast<const uint8_t*>(string), settings);
}

AtlasString AtlasFont::render(const uint8_t* string, const RenderSettings& settings) const
{
    auto pos  = getPosition(string, settings);
    auto size = jis_len(string);
    JISIterator itr{string};
    dtl::runtime_array<SPRT> data(size);
    auto totalWidth = 0;

    for (int32_t i = 0; i < size; i++)
    {
        auto index = font->getGlyphIndex(*itr++); 
        const auto& glyph = glyphs[index];
        data[i]    = getSPRT(glyph.rect, pos.x + totalWidth, pos.y, settings.color, settings.baseClut);
        totalWidth += glyph.width;
    }
    return AtlasString(dtl::move(data), tpage, totalWidth, font->height);
}

void AtlasString::enableShadow(bool enabled)
{
    for (auto& entry : data)
        entry.clut = getClut(CLUT_X, CLUT_Y + enabled);
}

void AtlasString::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    for (auto& entry : data)
    {
        entry.r = (r + 1) / 2;
        entry.g = (g + 1) / 2;
        entry.b = (b + 1) / 2;
    }
}

void AtlasString::setColor(RGB8 color)
{
    setColor(color.red, color.green, color.blue);
}

void AtlasString::setAlignment(RECT rect, AlignmentX alignX, AlignmentY alignY)
{
    auto x = rect.x;
    auto y = rect.y;

    switch (alignX)
    {
        case AlignmentX::LEFT: x += 0; break;
        case AlignmentX::CENTER: x += (rect.width - width) / 2; break;
        case AlignmentX::RIGHT: x += (rect.width - width); break;
    }

    switch (alignY)
    {
        case AlignmentY::TOP: y += 0; break;
        case AlignmentY::CENTER: y += (rect.height - height) / 2; break;
        case AlignmentY::BOTTOM: y += (rect.height - height); break;
    }

    setPosition(x, y);
}

void AtlasString::setPosition(int32_t x, int32_t y)
{
    const auto old_x = data[0].x;

    for (auto& entry : data)
    {
        entry.x = x + (entry.x - old_x);
        entry.y = y;
    }
}

void AtlasString::render(int32_t depth) const
{
    if(data.size() == 0) return;

    auto* sprites = reinterpret_cast<SPRT*>(libgs_GsGetWorkBase());
    dtl::copy(data.begin(), data.end(), sprites);
    for (int32_t i = 0; i < data.size(); i++)
        addPrim(ACTIVE_ORDERING_TABLE->origin + depth, &sprites[i].tag);

    auto* tpagePrim   = reinterpret_cast<DR_TPAGE*>(sprites + data.size());
    tpagePrim->tag[3] = 1;
    tpagePrim->code   = 0xe1000400 | tpage;
    addPrim(ACTIVE_ORDERING_TABLE->origin + depth, reinterpret_cast<GsOT_TAG*>(&tpagePrim->tag));
    libgs_GsSetWorkBase(tpagePrim + 1);
}

int32_t AtlasString::getWidth() const
{
    return width;
}