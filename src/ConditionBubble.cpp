#include "ConditionBubble.hpp"

#include "Effects.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "extern/dw1.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"
#include "extern/stddef.hpp"

// 0 -> nothing
// 1 -> hunger
// 2 -> poop
// 3 -> unused skull
// 4 -> sleeping frame 1
// 5 -> sleeping frame 2
// 6 -> sleeping frame 3
// 7 -> sleeping frame 4
// 8 -> sleeping frame 5
// 9 -> tired
// 10 -> sick
// 11 -> injured
// 12 -> evolving

// TODO refactor the shit out of this

namespace
{
    struct ConditionIcon
    {
        int16_t duration;
        int16_t nextIcon;
        int16_t icon;
    };

    struct ConditionBubble
    {
        int16_t frameCount{-1};
        int16_t activeHeight;
        int16_t baseHeight;
        int16_t frameCount2;
        ConditionIcon* iconPtr;
        uint32_t scale;
        Entity* entity;
    };

    dtl::array<ConditionBubble, 3> data{};

    ConditionIcon icon1{
        .duration = 100,
        .nextIcon = 0,
        .icon     = 1,
    };
    ConditionIcon icon2{
        .duration = 100,
        .nextIcon = 0,
        .icon     = 2,
    };
    ConditionIcon icon3{
        .duration = 100,
        .nextIcon = 0,
        .icon     = 3,
    };
    ConditionIcon icon9{
        .duration = 1000,
        .nextIcon = 0,
        .icon     = 9,
    };
    ConditionIcon icon10{
        .duration = 100,
        .nextIcon = 0,
        .icon     = 10,
    };
    ConditionIcon icon11{
        .duration = 100,
        .nextIcon = 0,
        .icon     = 11,
    };
    ConditionIcon icon12{
        .duration = 100,
        .nextIcon = 0,
        .icon     = 12,
    };

    dtl::array<ConditionIcon, 7> iconSleep{{
        {
            .duration = 10,
            .nextIcon = 1,
            .icon     = 0,
        },
        {
            .duration = 4,
            .nextIcon = 1,
            .icon     = 4,
        },
        {
            .duration = 4,
            .nextIcon = 1,
            .icon     = 5,
        },
        {
            .duration = 8,
            .nextIcon = 1,
            .icon     = 6,
        },
        {
            .duration = 1,
            .nextIcon = 1,
            .icon     = 7,
        },
        {
            .duration = 1,
            .nextIcon = 1,
            .icon     = 8,
        },
        {
            .duration = 1,
            .nextIcon = -5,
            .icon     = 0,
        },
    }};

    dtl::array<ConditionIcon*, 8> CONDITION_BUBBLE_TYPES{
        &icon1,
        &icon2,
        &icon3,
        iconSleep.data(),
        &icon9,
        &icon10,
        &icon11,
        &icon12,
    };

    GsSPRITE spriteCloud{
        .attribute = 0x40000000,
        .x         = 0,
        .y         = 0,
        .width     = 32,
        .height    = 32,
        .tpage     = 0x5F,
        .u         = 0xA0,
        .v         = 0x90,
        .clutX     = 272,
        .clutY     = 484,
        .r         = 128,
        .g         = 128,
        .b         = 128,
        .mx        = 16,
        .my        = 31,
        .scaleX    = 0,
        .scaleY    = 0,
        .rotate    = 0,
    };

    GsSPRITE spriteCloudPop{
        .attribute = 0x40000000,
        .x         = 0,
        .y         = 0,
        .width     = 16,
        .height    = 16,
        .tpage     = 0x5F,
        .u         = 0xD0,
        .v         = 0xB0,
        .clutX     = 272,
        .clutY     = 484,
        .r         = 128,
        .g         = 128,
        .b         = 128,
        .mx        = 8,
        .my        = 8,
        .scaleX    = 0,
        .scaleY    = 0,
        .rotate    = 0,
    };

    GsSPRITE spriteIcon1{
        .attribute = 0x50000000,
        .x         = 0,
        .y         = 0,
        .width     = 16,
        .height    = 16,
        .tpage     = 0x3F,
        .u         = 0x90,
        .v         = 0x80,
        .clutX     = 272,
        .clutY     = 483,
        .r         = 128,
        .g         = 128,
        .b         = 128,
        .mx        = 8,
        .my        = 26,
        .scaleX    = 0,
        .scaleY    = 0,
        .rotate    = 0,
    };

    GsSPRITE spriteIcon2{
        .attribute = 0x50000000,
        .x         = 0,
        .y         = 0,
        .width     = 16,
        .height    = 16,
        .tpage     = 0x3F,
        .u         = 0xD0,
        .v         = 0x90,
        .clutX     = 272,
        .clutY     = 483,
        .r         = 128,
        .g         = 128,
        .b         = 128,
        .mx        = 8,
        .my        = 26,
        .scaleX    = 0,
        .scaleY    = 0,
        .rotate    = 0,
    };

    GsSPRITE spriteIcon3{
        .attribute = 0x50000000,
        .x         = 0,
        .y         = 0,
        .width     = 16,
        .height    = 16,
        .tpage     = 0x3F,
        .u         = 0xE0,
        .v         = 0xB0,
        .clutX     = 272,
        .clutY     = 482,
        .r         = 128,
        .g         = 128,
        .b         = 128,
        .mx        = 8,
        .my        = 26,
        .scaleX    = 0,
        .scaleY    = 0,
        .rotate    = 0,
    };

    GsSPRITE spriteIcon9{
        .attribute = 0x50000000,
        .x         = 0,
        .y         = 0,
        .width     = 16,
        .height    = 16,
        .tpage     = 0x3F,
        .u         = 0xC0,
        .v         = 0x80,
        .clutX     = 272,
        .clutY     = 481,
        .r         = 128,
        .g         = 128,
        .b         = 128,
        .mx        = 8,
        .my        = 23,
        .scaleX    = 0,
        .scaleY    = 0,
        .rotate    = 0,
    };

    GsSPRITE spriteIcon10{
        .attribute = 0x50000000,
        .x         = 0,
        .y         = 0,
        .width     = 16,
        .height    = 16,
        .tpage     = 0x3F,
        .u         = 0xC0,
        .v         = 0x90,
        .clutX     = 272,
        .clutY     = 481,
        .r         = 128,
        .g         = 128,
        .b         = 128,
        .mx        = 8,
        .my        = 26,
        .scaleX    = 0,
        .scaleY    = 0,
        .rotate    = 0,
    };

    GsSPRITE spriteIcon11{
        .attribute = 0x50000000,
        .x         = 0,
        .y         = 0,
        .width     = 16,
        .height    = 16,
        .tpage     = 0x3F,
        .u         = 0xD0,
        .v         = 0xA0,
        .clutX     = 272,
        .clutY     = 483,
        .r         = 128,
        .g         = 128,
        .b         = 128,
        .mx        = 8,
        .my        = 26,
        .scaleX    = 0,
        .scaleY    = 0,
        .rotate    = 0,
    };

    GsSPRITE spriteIcon12{
        .attribute = 0x50000000,
        .x         = 0,
        .y         = 0,
        .width     = 16,
        .height    = 16,
        .tpage     = 0x3F,
        .u         = 0x80,
        .v         = 0xB0,
        .clutX     = 272,
        .clutY     = 482,
        .r         = 128,
        .g         = 128,
        .b         = 128,
        .mx        = 8,
        .my        = 26,
        .scaleX    = 0,
        .scaleY    = 0,
        .rotate    = 0,
    };

    GsSPRITE spriteIcon4{
        .attribute = 0x50000000,
        .x         = 0,
        .y         = 0,
        .width     = 8,
        .height    = 8,
        .tpage     = 0x3F,
        .u         = 0x80,
        .v         = 0xB0,
        .clutX     = 272,
        .clutY     = 482,
        .r         = 128,
        .g         = 128,
        .b         = 128,
        .mx        = 0,
        .my        = 13,
        .scaleX    = 0,
        .scaleY    = 0,
        .rotate    = 0,
    };

    GsSPRITE spriteIcon5{
        .attribute = 0x50000000,
        .x         = 0,
        .y         = 0,
        .width     = 8,
        .height    = 8,
        .tpage     = 0x3F,
        .u         = 0x98,
        .v         = 0x90,
        .clutX     = 272,
        .clutY     = 482,
        .r         = 128,
        .g         = 128,
        .b         = 128,
        .mx        = 8,
        .my        = 19,
        .scaleX    = 0,
        .scaleY    = 0,
        .rotate    = 0,
    };

    GsSPRITE spriteIcon6{
        .attribute = 0x50000000,
        .x         = 0,
        .y         = 0,
        .width     = 8,
        .height    = 8,
        .tpage     = 0x3F,
        .u         = 0x90,
        .v         = 0x90,
        .clutX     = 272,
        .clutY     = 482,
        .r         = 128,
        .g         = 128,
        .b         = 128,
        .mx        = 3,
        .my        = 28,
        .scaleX    = 0,
        .scaleY    = 0,
        .rotate    = 0,
    };

    void tickConditionBubble(int32_t id)
    {
        if (IS_IN_MENU) return;

        auto& entry = data[id];

        if (entry.frameCount < 10)
        {
            auto scale         = lerp(0, 0x6000, 0, 10, entry.frameCount);
            entry.scale        = scale;
            auto height        = lerp(0, -65, 0, 10, entry.frameCount - 10);
            entry.activeHeight = entry.baseHeight + height;
        }
        else if (entry.frameCount < 50)
        {
            entry.scale        = 0x6000;
            entry.activeHeight = entry.baseHeight - 65;
            auto val           = sin(lerp(0, 512, 0, 20, entry.frameCount));
            entry.activeHeight = (entry.baseHeight - 65) + (val * 12) / 4096;
        }
        else if (entry.frameCount == 50)
        {
            entry.activeHeight = entry.baseHeight - 230;
            entry.scale        = 0xf000;
        }
        else
        {
            removeConditionBubble(id);
            return;
        }

        if (entry.iconPtr->duration <= entry.frameCount2)
        {
            entry.frameCount2 = 0;
            entry.iconPtr     = entry.iconPtr + entry.iconPtr->nextIcon;
        }

        entry.frameCount++;
        entry.frameCount2++;
    }

    void renderIcon1(int32_t frame, MapPos mapPos, int32_t scale)
    {
        constexpr dtl::array<uint8_t, 19> offset{0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3};

        spriteIcon1.u  = offset[frame % offset.size()] * 16 + 144;
        spriteIcon1.v  = offset[frame % offset.size()] == 3 ? 0xb0 : 0x80;
        spriteIcon1.my = (((frame % offset.size()) + 1) % 5) == 0 ? 29 : 26;
        renderSprite(&spriteIcon1, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale);
    }

    void renderIcon2(int32_t frame, MapPos mapPos, int32_t scale)
    {
        dtl::array<uint8_t, 13> offset{0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 1};
        spriteIcon2.u = offset[frame % offset.size()] * 16 + 0xd0;
        renderSprite(&spriteIcon2, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale);
    }

    void renderIcon3(int32_t frame, MapPos mapPos, int32_t scale)
    {
        spriteIcon3.u = ((frame / 15) % 2 * 16) + 0xe0;
        renderSprite(&spriteIcon3, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale);
    }

    void renderIcon9(int32_t frame, MapPos mapPos, int32_t scale)
    {
        constexpr dtl::array<uint8_t, 13> offsetU{0, 0, 16, 16, 32, 32, 48, 48, 48, 48, 48, 48, 48};
        constexpr dtl::array<uint8_t, 13> offsetMy{9, 9, 9, 9, 9, 9, 9, 9, 9, 8, 6, 3, 0};

        spriteIcon9.u  = offsetU[frame % offsetU.size()] + 0xc0;
        spriteIcon9.my = offsetMy[frame % offsetMy.size()] + 0x17;
        renderSprite(&spriteIcon9, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale);
    }

    void renderIcon10(int32_t frame, MapPos mapPos, int32_t scale)
    {
        spriteIcon10.v = ((frame / 20) % 2 * 16) + 0x90;
        renderSprite(&spriteIcon10, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale);
    }

    void renderIcon11(int32_t frame, MapPos mapPos, int32_t scale)
    {
        constexpr dtl::array<uint8_t, 20> offset{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0};
        spriteIcon11.u = offset[frame % offset.size()] * 16 + 0xd0;
        renderSprite(&spriteIcon11, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale);
    }

    void renderIcon12(int32_t frame, MapPos mapPos, int32_t scale)
    {
        constexpr dtl::array<uint8_t, 19> offset{0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3};

        spriteIcon12.u = offset[frame % offset.size()] * 16 + 128;
        if (offset[frame % offset.size()] == 0)
        {
            spriteIcon12.v      = 0xb8;
            spriteIcon12.my     = 18;
            spriteIcon12.height = 8;
        }
        else
        {
            spriteIcon12.v      = 0xb0;
            spriteIcon12.my     = 26;
            spriteIcon12.height = 16;
        }
        renderSprite(&spriteIcon12, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale);
    }

    void renderConditionBubble(int32_t id)
    {
        if (IS_IN_MENU) return;

        auto& entry = data[id];
        SVector pos;

        translateConditionFXToEntity(entry.entity, &pos);
        pos.y       = entry.activeHeight + entry.entity->posData->location.y;
        auto mapPos = getMapPosition(pos);
        auto scale  = entry.scale;

        if (entry.entity->isOnScreen)
        {
            auto val1    = entry.frameCount > 50 ? 8 : 32;
            auto yOffset = (val1 * (scale * VIEWPORT_DISTANCE) / mapPos.depth) / 4096;
            auto val2    = (mapPos.depth / 16);
            auto val3    = val2 - 20;

            if (mapPos.screenY + DRAWING_OFFSET_Y < yOffset) mapPos.screenY = yOffset - DRAWING_OFFSET_Y;

            mapPos.depth = max(36, val3) * 16;
            scale        = (scale * val3) / val2;
        }

        if (mapPos.depth <= 512 || mapPos.depth >= 0x10000) return;

        auto frame = entry.frameCount;

        if (frame >= 51)
        {
            renderSprite(&spriteCloudPop, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale);
            return;
        }

        auto iconSprite = entry.iconPtr->icon;
        switch (iconSprite)
        {
            case 1: renderIcon1(frame, mapPos, scale); break;
            case 2: renderIcon2(frame, mapPos, scale); break;
            case 3: renderIcon3(frame, mapPos, scale); break;
            case 4: renderSprite(&spriteIcon4, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale); break;
            case 5:
                renderSprite(&spriteIcon4, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale);
                renderSprite(&spriteIcon5, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale);
                break;
            case 6:
                renderSprite(&spriteIcon4, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale);
                renderSprite(&spriteIcon5, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale);
                renderSprite(&spriteIcon6, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale);
                break;
            case 7:
                renderSprite(&spriteIcon5, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale);
                renderSprite(&spriteIcon6, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale);
                break;
            case 8: renderSprite(&spriteIcon6, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale); break;
            case 9: renderIcon9(frame, mapPos, scale); break;
            case 10: renderIcon10(frame, mapPos, scale); break;
            case 11: renderIcon11(frame, mapPos, scale); break;
            case 12: renderIcon12(frame, mapPos, scale); break;
        }

        renderSprite(&spriteCloud, mapPos.screenX, mapPos.screenY, mapPos.depth, scale, scale);
    }

    int32_t getFreeEntry()
    {
        for (int32_t i = 0; i < data.size(); i++)
            if (data[i].frameCount == -1) return i;

        return -1;
    }

} // namespace

int32_t addConditionBubble(uint32_t type, Entity* entity)
{
    auto index = getFreeEntry();
    if (index == -1) return -1;

    auto& entry        = data[index];
    entry.frameCount   = 0;
    entry.baseHeight   = 20 - getDigimonData(entity->type)->height;
    entry.activeHeight = entry.baseHeight;
    entry.entity       = entity;
    entry.scale        = 0;
    entry.frameCount2  = 0;
    entry.iconPtr      = CONDITION_BUBBLE_TYPES[type];

    addObject(ObjectID::CONDITION_BUBBLE, index, tickConditionBubble, renderConditionBubble);
    return index;
}

void removeConditionBubble(int32_t id)
{
    if (id < 0 || id > 2) return;

    data[id].frameCount = -1;
    removeObject(ObjectID::CONDITION_BUBBLE, id);
}