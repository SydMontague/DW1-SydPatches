#include "UIElements.hpp"

#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "Sound.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"

namespace
{
    struct DrawModeSettings
    {
        uint32_t* tag;
        uint32_t pageX       : 4;
        uint32_t pageY       : 1;
        uint32_t blendMode   : 2;
        uint32_t colorMode   : 2;
        bool ditherEnabled   : 1;
        bool allowDrawToDisp : 1;
        bool textureDisabled : 1;
        bool flipX           : 1;
        bool flipY           : 1;
        uint32_t unused      : 10;
        uint8_t code;
    };

    static_assert(sizeof(DrawModeSettings) == 8);

    void setLineBlendingMode(uint32_t mode, int32_t order)
    {
        if (mode == 0) return;

        auto* base            = reinterpret_cast<DrawModeSettings*>(libgs_GsGetWorkBase());
        base->code            = 0x81;
        base->unused          = 0;
        base->flipX           = false;
        base->flipY           = false;
        base->textureDisabled = false;
        base->allowDrawToDisp = true;
        base->ditherEnabled   = true;
        base->colorMode       = 0;
        base->blendMode       = mode;
        base->pageY           = 0;
        base->pageX           = 0;

        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + order, base);
        libgs_GsSetWorkBase(base + 1);
    }
} // namespace

extern "C"
{
    constexpr RGB8 UILINE_COLORS[] = {
        {.red = 8, .green = 8, .blue = 8},       // 2, 2, 2
        {.red = 132, .green = 239, .blue = 250}, // 144, 217, 250
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
            drawLine3P(0x20202, posX - 1, posY - 1, posX + 13, posY - 1, posX + 13, posY + 12, 2, 0);
            drawLine3P(0x20202, posX - 1, posY - 1, posX - 1, posY + 12, posX + 13, posY + 12, 2, 0);
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
            drawLine2P(UILINE_COLORS[line.clut].asUint32(), line.x1, line.y1, line.x2, line.y2, layer, 0);
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
            ScreenPos pos = getScreenPosition(TAMER_ENTITY, 1);
            RECT final    = {.x = posX, .y = posY, .width = width, .height = height};
            RECT start    = {
                   .x      = static_cast<int16_t>(pos.screenX - 5),
                   .y      = static_cast<int16_t>(pos.screenY - 5),
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
        removeAnimatedUIBox(instanceId, nullptr);
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

        drawLine3P(color1, x1, y2, x1, y1, x2, y1, layer, 0);
        drawLine3P(color2, x1, y2, x2, y2, x2, y1, layer, 0);
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
        drawLine3P(color1, x1, y1 - 1, x2, y2 - 1, x3, y3 - 1, layer, 0);
        drawLine2P(color1, x3, y3 - 1, x4, y4 - 1, layer, 0);

        drawLine3P(color2, x1, y1, x2, y2, x3, y3, layer, 0);
        drawLine2P(color2, x3, y3, x4, y4, layer, 0);

        drawLine3P(color1, x1, y1 + 1, x2, y2 + 1, x3, y3 + 1, layer, 0);
        drawLine2P(color1, x3, y3 + 1, x4, y4 + 1, layer, 0);
    }

    constexpr Sprite BORDER_CORNER_SPRITES[] = {
        {.uvX = 0x78, .uvV = 0x90, .width = 4, .height = 4, .texture_page = 5, .clut = getClut(0x60, 0x1ec)},
        {.uvX = 0x7C, .uvV = 0x90, .width = 4, .height = 4, .texture_page = 5, .clut = getClut(0x60, 0x1ec)},
        {.uvX = 0x78, .uvV = 0x94, .width = 4, .height = 4, .texture_page = 5, .clut = getClut(0x60, 0x1ec)},
        {.uvX = 0x7C, .uvV = 0x94, .width = 4, .height = 4, .texture_page = 5, .clut = getClut(0x60, 0x1ec)},
    };

    constexpr uint32_t BORDER_BLACK  = 0x080808; // 0x020202
    constexpr uint32_t BORDER_COLOR1 = 0xcea400; // 0xc59f4a
    constexpr uint32_t BORDER_COLOR2 = 0xffef84; // 0xfad990

    static void tickUIBox(int32_t instanceId)
    {
        if (UI_BOX_DATA[instanceId].state != 1 && UI_BOX_DATA[instanceId].state != 4) return;
        if (UI_BOX_DATA[instanceId].tick == nullptr) return;

        UI_BOX_DATA[instanceId].tick();
    }

    void renderUIBoxBorder(RECT* size, int32_t layer)
    {
        BORDER_CORNER_SPRITES[0].render(size->x, size->y, layer, 0);
        BORDER_CORNER_SPRITES[1].render(size->x + size->width - 4, size->y, layer, 0);
        BORDER_CORNER_SPRITES[2].render(size->x, size->y + size->height - 4, layer, 0);
        BORDER_CORNER_SPRITES[3].render(size->x + size->width - 4, size->y + size->height - 4, layer, 0);

        auto xMin = size->x;
        auto xMax = size->x + size->width - 4;
        auto yMin = size->y;
        auto yMax = size->y + size->height - 3;

        drawLine2P(BORDER_BLACK, xMin + 4, yMin + 0, xMax + 0, yMin + 0, layer, 0);
        // drawLine2P(BORDER_COLOR1, xMin + 4, yMin + 1, xMax + 0, yMin + 1, layer, 0);
        drawLine2P(BORDER_COLOR2, xMin + 4, yMin + 1, xMax + 0, yMin + 1, layer, 0);
        drawLine2P(BORDER_BLACK, xMin + 4, yMin + 2, xMax + 0, yMin + 2, layer, 0);

        drawLine2P(BORDER_BLACK, xMin + 4, yMax + 0, xMax + 0, yMax + 0, layer, 0);
        drawLine2P(BORDER_COLOR2, xMin + 4, yMax + 1, xMax + 0, yMax + 1, layer, 0);
        // drawLine2P(BORDER_COLOR1, xMin + 4, yMax + 2, xMax + 0, yMax + 2, layer, 0);
        drawLine2P(BORDER_BLACK, xMin + 4, yMax + 2, xMax + 0, yMax + 2, layer, 0);

        drawLine2P(BORDER_BLACK, xMin + 0, yMin + 4, xMin + 0, yMax + 0, layer, 0);
        drawLine2P(BORDER_COLOR1, xMin + 1, yMin + 4, xMin + 1, yMax + 0, layer, 0);
        drawLine2P(BORDER_COLOR2, xMin + 2, yMin + 4, xMin + 2, yMax + 0, layer, 0);
        drawLine2P(BORDER_BLACK, xMin + 3, yMin + 4, xMin + 3, yMax + 0, layer, 0);

        drawLine2P(BORDER_BLACK, xMax + 0, yMin + 4, xMax + 0, yMax + 0, layer, 0);
        drawLine2P(BORDER_COLOR2, xMax + 1, yMin + 4, xMax + 1, yMax + 0, layer, 0);
        drawLine2P(BORDER_COLOR1, xMax + 2, yMin + 4, xMax + 2, yMax + 0, layer, 0);
        drawLine2P(BORDER_BLACK, xMax + 3, yMin + 4, xMax + 3, yMax + 0, layer, 0);
    }

    static void renderUIBoxStatic(int32_t instanceId)
    {
        auto& data       = UI_BOX_DATA[instanceId];
        const auto layer = 6 - instanceId;

        if (data.render != nullptr) data.render();

        renderUIBoxBorder(&data.finalPos, layer);

        // horizontal line
        // TODO deprecate, this line should be drawn by the render func
        if ((data.features & 1) != 0)
        {
            auto minX  = data.finalPos.x + 3;
            auto maxX  = data.finalPos.x + data.finalPos.width - 3;
            auto baseY = data.finalPos.y + 13;

            drawLine2P(BORDER_BLACK, minX, baseY + 0, maxX, baseY + 0, layer, 0);
            drawLine2P(BORDER_COLOR2, minX, baseY + 1, maxX, baseY + 1, layer, 0);
            drawLine2P(BORDER_BLACK, minX, baseY + 2, maxX, baseY + 2, layer, 0);
        }

        // scroll bar
        if ((data.features & 4) != 0)
        {
            auto minX = data.finalPos.x;
            auto minY = data.finalPos.y;
            auto maxX = data.finalPos.x + data.finalPos.width;
            auto maxY = data.finalPos.y + data.finalPos.height;

            drawLine3P(0x020202, maxX - 13, maxY - 10, maxX - 13, minY + 13, maxX - 6, minY + 13, layer, 0);
            drawLine3P(0xa08769, maxX - 6, minY + 14, maxX - 6, maxY - 10, maxX - 12, maxY - 10, layer, 0);

            auto barHeight = data.finalPos.height - 25;
            auto rowHeight = (barHeight * data.visibleRows) / data.totalRows;
            auto rowOffset =
                data.finalPos.y + 14 + ((barHeight - rowHeight) * data.rowOffset) / (data.totalRows - data.visibleRows);
            auto barStart = rowOffset + rowHeight;

            drawLine3P(0x020202, maxX - 7, rowOffset, maxX - 7, barStart, maxX - 13, barStart, layer, 0);
            drawLine3P(0xa08769, maxX - 12, barStart, maxX - 12, rowOffset, maxX - 6, rowOffset, layer, 0);

            POLY_F4* prim = reinterpret_cast<POLY_F4*>(libgs_GsGetWorkBase());
            libgpu_SetPolyF4(prim);
            prim[0].r0 = 0x5b;
            prim[0].g0 = 0x70;
            prim[0].b0 = 0x80;
            prim[0].x0 = maxX - 11;
            prim[0].x1 = maxX - 7;
            prim[0].x2 = maxX - 11;
            prim[0].x3 = maxX - 7;
            prim[0].y0 = rowOffset + 1;
            prim[0].y1 = rowOffset + 1;
            prim[0].y2 = rowOffset + rowHeight;
            prim[0].y3 = rowOffset + rowHeight;
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + layer, prim);

            libgpu_SetPolyF4(prim + 1);
            prim[1].r0 = 0x35;
            prim[1].g0 = 0x4b;
            prim[1].b0 = 0x5c;
            prim[1].x0 = maxX - 12;
            prim[1].x1 = maxX - 6;
            prim[1].x2 = maxX - 12;
            prim[1].x3 = maxX - 6;
            prim[1].y0 = minY + 14;
            prim[1].y1 = minY + 14;
            prim[1].y2 = maxY - 10;
            prim[1].y3 = maxY - 10;
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + layer, prim + 1);
            libgs_GsSetWorkBase(prim + 2);
        }

        GsBOXF box;
        if ((data.features & 2) != 0)
            box.attribute = 0x40000000;
        else
            box.attribute = 0;

        constexpr RGB8 boxColors[] = {
            {0, 0, 0},
            {45, 56, 64},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
        };

        box.r      = boxColors[data.color].red;
        box.g      = boxColors[data.color].green;
        box.b      = boxColors[data.color].blue;
        box.x      = data.finalPos.x + 4;
        box.y      = data.finalPos.y + 3;
        box.width  = data.finalPos.width - 8;
        box.height = data.finalPos.height - 3;
        libgs_GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, layer);
    }

    static void renderUIBoxAnim(int32_t instanceId, int32_t frame)
    {
        auto& data       = UI_BOX_DATA[instanceId];
        const auto layer = 6 - instanceId;

        if (data.state == 3 && data.frame == 4) playSound(0, 1);

        auto startMinX = data.startPos.x;
        auto startMaxX = data.startPos.x + data.startPos.width;
        auto startMinY = data.startPos.y;
        auto startMaxY = data.startPos.y + data.startPos.height;

        auto finalMinX = data.finalPos.x;
        auto finalMaxX = data.finalPos.x + data.finalPos.width;
        auto finalMinY = data.finalPos.y;
        auto finalMaxY = data.finalPos.y + data.finalPos.height;

        auto diffMinX = frame * (finalMinX - startMinX);
        auto diffMaxX = frame * (finalMaxX - startMaxX);
        auto diffMinY = frame * (finalMinY - startMinY);
        auto diffMaxY = frame * (finalMaxY - startMaxY);

        auto x1 = startMinX + (diffMinX / 4);
        auto x2 = startMaxX + (diffMaxX / 4);
        auto y1 = startMinY + (diffMinY / 4);
        auto y2 = startMaxY + (diffMaxY / 4);

        drawLine3P(0x808080, x1, y1, x2, y1, x2, y2, layer, 0);
        drawLine3P(0x808080, x2, y2, x1, y2, x1, y1, layer, 0);
    }

    static void renderUIBoxAnimated(int32_t instanceId)
    {
        auto& data = UI_BOX_DATA[instanceId];

        if (data.state == 2 || data.state == 3)
        {
            auto currentFrame = 0;
            if (data.state == 2)
                currentFrame = data.frame++;
            else
                currentFrame = --data.frame;

            renderUIBoxAnim(instanceId, currentFrame);
            if (data.frame > 4) data.state = 1;
            if (data.frame == 0) removeStaticUIBox(instanceId);
        }
        else { renderUIBoxStatic(instanceId); }
    }

    void createStaticUIBox(int32_t id,
                           uint8_t color,
                           uint8_t features,
                           const RECT* pos,
                           BoxTickFunction tickFunc,
                           BoxRenderFunction renderFunc)
    {
        UI_BOX_DATA[id].state    = 1;
        UI_BOX_DATA[id].color    = color;
        UI_BOX_DATA[id].features = features;
        UI_BOX_DATA[id].render   = renderFunc;
        UI_BOX_DATA[id].tick     = tickFunc;
        UI_BOX_DATA[id].finalPos = *pos;
        addObject(ObjectID::UI_BOX, id, tickUIBox, renderUIBoxStatic);
    }

    void createAnimatedUIBox(int32_t instanceId,
                             uint8_t color,
                             uint8_t features,
                             const RECT* finalPos,
                             const RECT* startPos,
                             BoxTickFunction tickFunc,
                             BoxRenderFunction renderFunc)
    {
        playSound(0, 0);
        UI_BOX_DATA[instanceId].state    = 2;
        UI_BOX_DATA[instanceId].color    = color;
        UI_BOX_DATA[instanceId].features = features;
        UI_BOX_DATA[instanceId].startPos = *startPos;
        UI_BOX_DATA[instanceId].finalPos = *finalPos;
        UI_BOX_DATA[instanceId].render   = renderFunc;
        UI_BOX_DATA[instanceId].tick     = tickFunc;
        addObject(ObjectID::UI_BOX, instanceId, tickUIBox, renderUIBoxAnimated);
    }

    void removeStaticUIBox(int32_t id)
    {
        UI_BOX_DATA[id].state = 0;
        UI_BOX_DATA[id].frame = 0;
        removeObject(ObjectID::UI_BOX, id);
    }

    void removeAnimatedUIBox(int32_t id, RECT* target)
    {
        if (target != nullptr) UI_BOX_DATA[id].startPos = *target;

        UI_BOX_DATA[id].state = 3;
    }

    void initializeUIBoxData()
    {
        for (auto& data : UI_BOX_DATA)
        {
            data.frame = 0;
            data.state = 0;
        }
    }

    void drawLine3P(uint32_t color,
                    int16_t x0,
                    int16_t y0,
                    int16_t x1,
                    int16_t y1,
                    int16_t x2,
                    int16_t y2,
                    int32_t order,
                    uint32_t blend)
    {
        auto* line = reinterpret_cast<LINE_F3*>(libgs_GsGetWorkBase());
        line->r0   = static_cast<uint8_t>(color >> 0);
        line->g0   = static_cast<uint8_t>(color >> 8);
        line->b0   = static_cast<uint8_t>(color >> 16);
        line->x0   = x0;
        line->y0   = y0;
        line->x1   = x1;
        line->y1   = y1;
        line->x2   = x2;
        line->y2   = y2;
        libgpu_SetLineF3(line);
        libgpu_SetSemiTrans(line, blend >> 2);
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + order, line);
        libgs_GsSetWorkBase(line + 1);
        setLineBlendingMode(blend, order);
    }

    void drawLine2P(uint32_t color, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int32_t order, uint32_t blend)
    {
        auto* line = reinterpret_cast<LINE_F2*>(libgs_GsGetWorkBase());
        line->r0   = static_cast<uint8_t>(color >> 0);
        line->g0   = static_cast<uint8_t>(color >> 8);
        line->b0   = static_cast<uint8_t>(color >> 16);
        line->x0   = x0;
        line->y0   = y0;
        line->x1   = x1;
        line->y1   = y1;
        libgpu_SetLineF2(line);
        libgpu_SetSemiTrans(line, blend >> 2);
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + order, line);
        libgs_GsSetWorkBase(line + 1);
        setLineBlendingMode(blend, order);
    }

    void setUVDataPolyFT4(POLY_FT4* prim, int16_t uvX, int16_t uvY, int16_t uvWidth, int16_t uvHeight)
    {
        prim->u0 = uvX;
        prim->v0 = uvY;
        prim->u1 = uvX + uvWidth;
        prim->v1 = uvY;
        prim->u2 = uvX;
        prim->v2 = uvY + uvHeight;
        prim->u3 = uvX + uvWidth;
        prim->v3 = uvY + uvHeight;
    }

    void setPosDataPolyFT4(POLY_FT4* prim, int16_t posX, int16_t posY, int16_t width, int16_t height)
    {
        prim->x0 = posX;
        prim->y0 = posY;
        prim->x1 = posX + width;
        prim->y1 = posY;
        prim->x2 = posX;
        prim->y2 = posY + height;
        prim->x3 = posX + width;
        prim->y3 = posY + height;
    }

    void renderSelectionCursor(int16_t x, int16_t y, int16_t width, int16_t height, int32_t depth)
    {
        drawLine3P(0xb0b0b0, x, y + height - 1, x, y, x + width - 1, y, depth, 0);
        drawLine3P(0x121212, x + width, y, x + width, y + height, x, y + height, depth, 0);

        GsBOXF box{
            .attribute = 0x40000000,
            .x         = static_cast<int16_t>(x + 1),
            .y         = static_cast<int16_t>(y + 1),
            .width     = static_cast<uint16_t>(width - 1),
            .height    = static_cast<uint16_t>(height - 1),
            .r         = 0x80,
            .g         = 0x80,
            .b         = 0x80,
        };

        libgs_GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, depth);
    }
}

void SimpleTextSprite::draw(Font* font, const uint8_t* string)
{
    uvWidth  = drawStringNew(font, string, uvX, uvY);
    uvWidth  = min(uvX | 0xFF, uvWidth);
    uvHeight = font->height;
}

void SimpleTextSprite::draw(Font* font, const char* string)
{
    draw(font, reinterpret_cast<const uint8_t*>(string));
}

void SimpleTextSprite::render(int32_t posX, int32_t posY, int32_t color, int32_t offset, bool hasShadow)
{
    renderStringNew(color, posX, posY, uvWidth, uvHeight, uvX, uvY, offset, hasShadow);
}

void SimpleTextSprite::clear()
{
    clearTextSubArea2(uvX, uvY, uvWidth, uvHeight);
}