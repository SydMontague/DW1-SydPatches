#include "UIElements.hpp"

#include "extern/dw1.hpp"

extern "C"
{
    constexpr RGB8 UILINE_COLORS[] = {
        {.red = 2, .green = 2, .blue = 2},
        {.red = 144, .green = 217, .blue = 250},
        {.red = 74, .green = 159, .blue = 197},
        {.red = 180, .green = 150, .blue = 105},
    };

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

    void renderTextSprite(TextSprite& entry, int32_t offsetX, int32_t offsetY)
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
}