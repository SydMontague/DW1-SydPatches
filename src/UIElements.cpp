#include "UIElements.hpp"

#include "Helper.hpp"
#include "extern/dw1.hpp"

extern "C"
{
    constexpr RGB8 UILINE_COLORS[] = {
        {.red = 2, .green = 2, .blue = 2},
        {.red = 144, .green = 217, .blue = 250},
        {.red = 74, .green = 159, .blue = 197},
        {.red = 180, .green = 150, .blue = 105},
    };

    void renderRectPolyFT4(int16_t posX,
                           int16_t posY,
                           uint32_t width,
                           uint32_t height,
                           uint8_t texX,
                           uint8_t texY,
                           uint16_t texturePage,
                           uint16_t clut,
                           int zIndex,
                           char flag)
    {
        POLY_FT4* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyFT4(prim);
        if ((flag & 0x40) != 0) libgpu_SetSemiTrans(prim, 1);

        setPolyFT4UV(prim, texX, texY, width, height);

        if ((flag & 2) != 0)
            setPosDataPolyFT4(prim, posX, posY, width * 2, height * 2);
        else if ((flag & 4) != 0)
        {
            setPosDataPolyFT4(prim, posX, posY, width * 2, height * 2);
            prim->x0 += width;
            prim->x1 -= width;
            prim->x2 += width;
            prim->x3 -= width;
        }
        else
            setPosDataPolyFT4(prim, posX, posY, width, height);

        if ((flag & 1) != 0)
        {
            prim->r0 = 50;
            prim->g0 = 50;
            prim->b0 = 50;
        }
        else
        {
            prim->r0 = 128;
            prim->g0 = 128;
            prim->b0 = 128;
        }

        prim->tpage = texturePage;
        prim->clut  = clut;
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + zIndex, prim);
        libgs_GsSetWorkBase(prim + 1);

        if ((flag & 0x80) != 0)
        {
            renderTrianglePrimitive(0x202020, posX - 1, posY - 1, posX + 13, posY - 1, posX + 13, posY + 12, 2, 0);
            renderTrianglePrimitive(0x202020, posX - 1, posY - 1, posX - 1, posY + 12, posX + 13, posY + 12, 2, 0);
        }
    }

    void drawTextSprite(TextSprite& entry)
    {
        entry.uvWidth = drawStringNew(entry.font, reinterpret_cast<const uint8_t*>(entry.string), entry.uvX, entry.uvY);
        entry.uvHeight = entry.font->height;
    }

    void renderSeperatorLines(const Line* linePtr, int32_t count, int32_t layer)
    {
        for (int32_t i = 0; i < count; i++)
        {
            auto& line = linePtr[i];
            renderLinePrimitive(UILINE_COLORS[line.clut].asUint32(), line.x1, line.y1, line.x2, line.y2, layer, 0);
        }
    }

    void renderTextSprite2(TextSprite& entry, int32_t offsetX, int32_t offsetY)
    {
        switch (entry.alignmentX)
        {
            case AlignmentX::LEFT: offsetX += 0; break;
            case AlignmentX::CENTER: offsetX += (entry.boxWidth - entry.uvWidth) / 2; break;
            case AlignmentX::RIGHT: offsetX += (entry.boxWidth - entry.uvWidth); break;
        }

        switch (entry.alignmentY)
        {
            case AlignmentY::TOP: offsetY += 0; break;
            case AlignmentY::CENTER: offsetY += (entry.boxHeight - entry.uvHeight) / 2; break;
            case AlignmentY::BOTTOM: offsetY += (entry.boxHeight - entry.uvHeight); break;
        }

        renderStringNew(entry.color,
                        entry.posX + offsetX,
                        entry.posY + offsetY,
                        entry.uvWidth,
                        entry.uvHeight,
                        entry.uvX,
                        entry.uvY,
                        entry.layer,
                        entry.hasShadow & 1);

        if ((entry.hasShadow & 2) != 0)
        {
            renderStringNew(0,
                            entry.posX + offsetX + 1,
                            entry.posY + offsetY + 1,
                            entry.uvWidth,
                            entry.uvHeight,
                            entry.uvX,
                            entry.uvY,
                            entry.layer,
                            0);
        }
    }

    void renderTextSprite(TextSprite& entry)
    {
        renderTextSprite2(entry, 0, 0);
    }

    void initSpecialSprite(IconSprite& sprite, Special special)
    {
        uint8_t casted = static_cast<uint8_t>(special);
        sprite.uvX     = 36 + static_cast<uint8_t>(special) * 24;
        sprite.height  = 12;

        switch (special)
        {
            case Special::FIRE:
            case Special::COMBAT:
            case Special::FILTH: sprite.clut = getClut(0x60, 0x1e8 + 0); break;

            case Special::AIR:
            case Special::NATURE:
            case Special::ICE: sprite.clut = getClut(0x60, 0x1e8 + 1); break;

            case Special::MACHINE: sprite.clut = getClut(0x60, 0x1e8 + 2); break;

            case Special::NONE: sprite.height = 0; break;
        }
    }

    bool createMenuBox(int32_t instanceId,
                       int16_t posX,
                       int16_t posY,
                       int16_t width,
                       int16_t height,
                       uint8_t features,
                       BoxTickFunction tickFunc,
                       BoxRenderFunction renderFunc)
    {
        if (UI_BOX_DATA[instanceId].state == 1) return true;

        if (UI_BOX_DATA[instanceId].frame == 0)
        {
            Position pos;
            getEntityScreenPos(&TAMER_ENTITY, 1, &pos);

            RECT final = {.x = posX, .y = posY, .width = width, .height = height};
            RECT start = {
                .x      = static_cast<int16_t>(pos.x - 5),
                .y      = static_cast<int16_t>(pos.y - 5),
                .width  = 10,
                .height = 10,
            };

            createAnimatedUIBox(instanceId, 1, features, &final, &start, tickFunc, renderFunc);
        }
        return false;
    }

    void closeUIBoxIfOpen(int32_t instanceId)
    {
        if (UI_BOX_DATA[instanceId].frame < 5) return;
        if (UI_BOX_DATA[instanceId].state != 1) return;

        // vanilla calls getEntityScreenPos here, but it's results are unused?
        unsetUIBoxAnimated(instanceId, nullptr);
    }

    bool isUIBoxAvailable(int32_t id)
    {
        return UI_BOX_DATA[id].state == 1 || UI_BOX_DATA[id].frame == 0;
    }

    void renderBox(int16_t posX,
                   int16_t posY,
                   uint16_t width,
                   uint16_t height,
                   uint8_t red,
                   uint8_t green,
                   uint8_t blue,
                   uint8_t flag,
                   int32_t layer)
    {
        GsBOXF box;

        box.attribute = 0;
        if ((flag & 1) != 0) box.attribute |= 0x40000000;
        if ((flag & 2) != 0) box.attribute |= 0x20000000;

        box.r      = red;
        box.g      = green;
        box.b      = blue;
        box.x      = posX;
        box.y      = posY;
        box.width  = width;
        box.height = height;
        libgs_GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, layer);
        // vanilla has flag 0x80 here, that draws a 13x14 px border around, only used for moves, but this functionality
        // can be implemented in better ways
    }

    void renderBorderBox(int16_t posX,
                         int16_t posY,
                         int16_t width,
                         int16_t height,
                         uint32_t color1,
                         uint32_t color2,
                         uint8_t red,
                         uint8_t green,
                         uint8_t blue,
                         int32_t layer)
    {
        auto x1 = posX;
        auto x2 = posX + width;
        auto y1 = posY;
        auto y2 = posY + height;

        renderTrianglePrimitive(color1, x1, y2, x1, y1, x2, y1, layer, 0);
        renderTrianglePrimitive(color2, x1, y2, x2, y2, x2, y1, layer, 0);
        renderBox(x1, y1, width, height, red, green, blue, 0, layer);
    }

    void renderDigimonStatsBar(int32_t value, int32_t maxValue, int32_t width, int32_t posX, int32_t posY)
    {
        renderBox(posX, posY, (width * value) / maxValue, 2, 50, 200, 200, 0, 5);
    }

    void Sprite::render(int16_t posX, int16_t posY, uint8_t layer, uint8_t flag) const
    {
        renderRectPolyFT4(posX, posY, width, height, uvX, uvV, texture_page, clut, layer, flag);
    }

    void Inset::render(int32_t order) const
    {
        renderBorderBox(posX, posY, width, height, 0x020202, 0xa08769, 0x35, 0x4B, 0x5C, order);
    }

    void renderMenuTab(int32_t posX, int32_t width, bool isActive)
    {
        auto height = isActive ? 15 : 16;

        renderRectPolyFT4(posX, -104, 7, height, 0xD4, 0x8C, 5, getClut(0x60, 0x1EC), 5, isActive);

        for (int32_t i = 0; i < (width - 14) / 4; i++)
            renderRectPolyFT4(posX + 7 + i * 4, -104, 4, height, 0xE2, 0x8C, 5, getClut(0x60, 0x1EC), 5, isActive);

        renderRectPolyFT4(posX + width - 9, -104, 7, height, 0xDB, 0x8C, 5, getClut(0x60, 0x1EC), 5, isActive);
    }

    void IconSprite::render(int32_t layer) const
    {
        renderRectPolyFT4(posX, posY, width, height, uvX, uvY, texture_page, clut, layer, 0);
    }

    void BorderBox::render() const
    {
        renderBorderBox(posX, posY, width, height, color1, color2, red, green, blue, layer);
    }

    void Line4Points::render(uint32_t color1, uint32_t color2, int32_t layer) const
    {
        // TODO: diagonal lines don't connect neatly to one side
        renderTrianglePrimitive(color1, x1, y1 - 1, x2, y2 - 1, x3, y3 - 1, layer, 0);
        renderLinePrimitive(color1, x3, y3 - 1, x4, y4 - 1, layer, 0);

        renderTrianglePrimitive(color2, x1, y1, x2, y2, x3, y3, layer, 0);
        renderLinePrimitive(color2, x3, y3, x4, y4, layer, 0);

        renderTrianglePrimitive(color1, x1, y1 + 1, x2, y2 + 1, x3, y3 + 1, layer, 0);
        renderLinePrimitive(color1, x3, y3 + 1, x4, y4 + 1, layer, 0);
    }
}
