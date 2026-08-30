#include "../Fade.hpp"
#include "../GameObjects.hpp"
#include "../Helper.hpp"
#include "../Input.hpp"
#include "../Math.hpp"
#include "../Sound.hpp"
#include "../UIBox.hpp"
#include "../UIElements.hpp"
#include "../extern/VS.hpp"

namespace
{
    struct SelectBox
    {
        uint8_t optionCount{0};
        uint8_t selection{0};
        uint8_t animFrame{0};
        bool hasSelected{false};
        uint8_t timer{0};
    };

    struct VSSelectSprite
    {
        int16_t clutY;
        uint8_t uvX;
        uint8_t uvY;
        uint8_t width;
        uint8_t height;
        int8_t posX;
        int8_t posY;
    };

    constexpr dtl::array<VSSelectSprite, 8> MODE_SPRITES{{
        {.clutY = 489, .uvX = 120, .uvY = 0, .width = 124, .height = 14, .posX = 28, .posY = 7},
        {.clutY = 494, .uvX = 0, .uvY = 136, .width = 40, .height = 48, .posX = 14, .posY = 37},
        {.clutY = 494, .uvX = 80, .uvY = 136, .width = 40, .height = 48, .posX = 70, .posY = 37},
        {.clutY = 495, .uvX = 160, .uvY = 136, .width = 40, .height = 48, .posX = 126, .posY = 37},
        {.clutY = 500, .uvX = 0, .uvY = 232, .width = 52, .height = 23, .posX = 10, .posY = 96},
        {.clutY = 500, .uvX = 52, .uvY = 232, .width = 52, .height = 23, .posX = 65, .posY = 96},
        {.clutY = 500, .uvX = 104, .uvY = 232, .width = 52, .height = 23, .posX = 122, .posY = 96},
        {.clutY = 494, .uvX = 120, .uvY = 76, .width = 52, .height = 60, .posX = 8, .posY = 31},
    }};
    constexpr dtl::array<VSSelectSprite, 8> MAP_SPRITES = {{
        {.clutY = 490, .uvX = 120, .uvY = 14, .width = 124, .height = 14, .posX = 28, .posY = 7},
        {.clutY = 496, .uvX = 0, .uvY = 184, .width = 40, .height = 48, .posX = 14, .posY = 37},
        {.clutY = 497, .uvX = 80, .uvY = 184, .width = 40, .height = 48, .posX = 70, .posY = 37},
        {.clutY = 498, .uvX = 160, .uvY = 184, .width = 40, .height = 48, .posX = 126, .posY = 37},
        {.clutY = 491, .uvX = 120, .uvY = 52, .width = 52, .height = 24, .posX = 8, .posY = 96},
        {.clutY = 491, .uvX = 120, .uvY = 28, .width = 52, .height = 24, .posX = 64, .posY = 96},
        {.clutY = 491, .uvX = 172, .uvY = 28, .width = 48, .height = 24, .posX = 122, .posY = 96},
        {.clutY = 494, .uvX = 120, .uvY = 76, .width = 52, .height = 60, .posX = 8, .posY = 31},
    }};

    SelectBox data;

    void renderModeSelectBox(int32_t)
    {
        auto prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        for (auto i = 0; i < 8; i++) {
            auto& entry = MODE_SPRITES[i];
            libgpu_SetPolyFT4(prim);
            prim->r0   = 128;
            prim->g0   = 128;
            prim->b0   = 128;
            prim->clut = getClut(48, entry.clutY);

            if (i > 0 && i < 4 && data.selection != (i - 1)) {
                prim->r0 = 64;
                prim->g0 = 64;
                prim->b0 = 64;
            }

            if (i == 4 && data.selection != 0) prim->clut = getClut(48, 501);
            if (i == 5 && data.selection != 1) prim->clut = getClut(48, 501);
            if (i == 6 && data.selection != 2) prim->clut = getClut(48, 501);

            prim->tpage = 7;

            if (i == data.selection + 1 && i > 0 && i < 4) {
                setUVDataPolyFT4(prim, entry.uvX + data.animFrame * 40, entry.uvY, entry.width, entry.height);
                if (i == 3 && data.animFrame == 1) prim->clut = getClut(48, 495);
            }
            else
                setUVDataPolyFT4(prim, entry.uvX, entry.uvY, entry.width, entry.height);

            auto posX = entry.posX - 0x5A;
            if (i == 7) posX += data.selection * 0x38;

            setPosDataPolyFT4(prim, posX, entry.posY - 0x46, entry.width, entry.height);
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 6, prim);
            prim++;
        }
        libgs_GsSetWorkBase(prim);

        UIBox{RECT{.x = -90, .y = -70, .width = 180, .height = 128}, UIBox::DEFAULT_COLOR, false}.render(6);
    }

    void renderMapSelectBox(int32_t)
    {

        auto prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        for (auto i = 0; i < 8; i++) {
            auto& entry = MAP_SPRITES[i];
            libgpu_SetPolyFT4(prim);
            prim->r0 = 128;
            prim->g0 = 128;
            prim->b0 = 128;

            prim->clut = getClut(48, entry.clutY);
            setUVDataPolyFT4(prim, entry.uvX, entry.uvY, entry.width, entry.height);

            if (i == 1 && data.selection != 0) {
                setUVDataPolyFT4(prim, entry.uvX + 40, entry.uvY, entry.width, entry.height);
                prim->clut = getClut(48, 499);
            }
            if (i == 2 && data.selection != 1) {
                setUVDataPolyFT4(prim, entry.uvX + 40, entry.uvY, entry.width, entry.height);
                prim->clut = getClut(48, 499);
            }
            if (i == 3 && data.selection != 2) {
                setUVDataPolyFT4(prim, entry.uvX + 40, entry.uvY, entry.width, entry.height);
                prim->clut = getClut(48, 499);
            }

            if (i > 3 && i < 7 && data.selection != (i - 4)) prim->clut = getClut(48, 492);

            prim->tpage = 7;

            auto posX = entry.posX - 0x5A;
            if (i == 7) posX += data.selection * 0x38;

            setPosDataPolyFT4(prim, posX, entry.posY - 0x46, entry.width, entry.height);
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 6, prim);
            prim++;
        }
        libgs_GsSetWorkBase(prim);

        UIBox{RECT{.x = -90, .y = -70, .width = 180, .height = 128}, UIBox::DEFAULT_COLOR, false}.render(6);
    }

    bool isKeyPressedByAnyPlayer(InputButtons button)
    {
        return isKeyDownPolled(button) || isKeyDownPolledP2(button);
    }

    void tickSelectBox(int32_t)
    {
        if (data.hasSelected) return;

        if (isKeyPressedByAnyPlayer(InputButtons::BUTTON_CROSS)) {
            playSound(0, 3);
            data.hasSelected = true;
            return;
        }

        if ((++data.timer % 8) == 0) data.animFrame = (data.animFrame + 1) % 2;

        if (data.optionCount == 0) return;

        if (isKeyPressedByAnyPlayer(InputButtons::BUTTON_LEFT)) {
            data.selection = ring(data.selection - 1, 0, data.optionCount);
            playSound(0, 2);
            data.animFrame = 0;
        }
        if (isKeyPressedByAnyPlayer(InputButtons::BUTTON_RIGHT)) {
            data.selection = ring(data.selection + 1, 0, data.optionCount);
            playSound(0, 2);
            data.animFrame = 0;
        }
    }
} // namespace

void VS__tickSelectMode()
{
    constexpr dtl::array<uint8_t, 3> BATTLE_NUM_SELECTION{1, 3, 5};

    auto p1Count = 0;
    auto p2Count = 0;
    for (auto& entry : *VS_DIGIMON_P1_PTR)
        if (entry.digimonId != 0) p1Count++;
    for (auto& entry : *VS_DIGIMON_P2_PTR)
        if (entry.digimonId != 0) p2Count++;

    data = {};
    if (p1Count > 5 && p2Count > 5)
        data.optionCount = 3;
    else if (p1Count > 3 && p2Count > 3)
        data.optionCount = 2;
    else
        data.optionCount = 1;

    addObject(ObjectID::VS_SELECT_BOX, 0, tickSelectBox, renderModeSelectBox);
    fadeFromBlack(5);
    for (auto i = 0; i < 6; i++)
        VS__tickFrame();

    while (!data.hasSelected)
        VS__tickFrame();

    fadeToBlack(5);
    for (auto i = 0; i < 6; i++)
        VS__tickFrame();

    VS_NUM_BATTLES = BATTLE_NUM_SELECTION[data.selection];
    removeObject(ObjectID::VS_SELECT_BOX, 0);
}

void VS__tickSelectMap()
{
    data = {.optionCount = 3};
    addObject(ObjectID::VS_SELECT_BOX, 0, tickSelectBox, renderMapSelectBox);
    fadeFromBlack(5);
    for (auto i = 0; i < 6; i++)
        VS__tickFrame();

    while (!data.hasSelected)
        VS__tickFrame();

    fadeToBlack(5);
    for (auto i = 0; i < 6; i++)
        VS__tickFrame();

    VS_SELECTED_STAGE = data.selection;
    removeObject(ObjectID::VS_SELECT_BOX, 0);
}
