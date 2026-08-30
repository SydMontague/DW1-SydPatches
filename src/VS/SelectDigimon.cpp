#include "../AtlasFont.hpp"
#include "../DigimonData.hpp"
#include "../Fade.hpp"
#include "../GameObjects.hpp"
#include "../Input.hpp"
#include "../Math.hpp"
#include "../Sound.hpp"
#include "../UIBox.hpp"
#include "../extern/VS.hpp"
#include "../extern/dtl/array.hpp"
#include "../extern/dw1.hpp"

namespace
{
    dtl::array<bool, 2> pressStartBoxExists;

    void renderPressStartBox(int32_t playerId)
    {
        constexpr int16_t WIDTH  = 108;
        constexpr int16_t HEIGHT = 36;
        constexpr auto y         = 32;
        auto depth               = 6 - playerId;
        int16_t x                = playerId == 0 ? -132 : 22;

        const RenderSettings string1{
            .x      = static_cast<int16_t>(x + 6),
            .y      = static_cast<int16_t>(y + 6),
            .width  = WIDTH - 12,
            .height = 12,
            .alignX = AlignmentX::CENTER,
        };
        const RenderSettings string2{
            .x      = static_cast<int16_t>(x + 6),
            .y      = static_cast<int16_t>(y + 18),
            .width  = WIDTH - 12,
            .height = 12,
            .alignX = AlignmentX::CENTER,
        };

        getAtlasVanilla().renderSlow("Press Start", depth, string1);
        getAtlasVanilla().renderSlow("to begin", depth, string2);
        UIBox(RECT{.x = x, .y = y, .width = WIDTH, .height = HEIGHT}, {0, 0, 0}, true).render(depth);
    }

    void VS__createPressStartToBeginBox(int32_t player)
    {
        if (pressStartBoxExists[player]) return;

        addObject(ObjectID::VS_PRESS_START_BOX, player, nullptr, renderPressStartBox);
        pressStartBoxExists[player] = true;
    }

    void VS__removePressStartToBeginBox(int32_t player)
    {
        if (!pressStartBoxExists[player]) return;

        removeObject(ObjectID::VS_PRESS_START_BOX, player);
        pressStartBoxExists[player] = false;
    }

    bool VS__isAlreadySelected(int32_t player, uint32_t selected)
    {
        const auto& data = player == 0 ? VS__SELECTED_P1 : VS__SELECTED_P2;

        for (auto i = 0; i < VS_NUM_BATTLES; i++) {
            if (data[i] == selected) {
                playSound(0, 11);
                return true;
            }
        }

        return false;
    }

    void VS__handleDigimonSelected(SelectDigimonData* data)
    {
        playSound(0, 3);
        data->unk7 = 0;
        data->unk6 = 0;
        data->selectedMask |= (1 << data->selectedSlot);

        if (data->selectedMask == ((1 << VS_NUM_BATTLES) - 1)) data->lastSlot = data->selectedSlot;
        if (data->selectedSlot < (VS_NUM_BATTLES - 1)) data->selectedSlot++;
    }

    void VS__tickSelectDigimonPlayer(int32_t id)
    {
        auto& slots        = id == 1 ? VS__SELECTED_P2 : VS__SELECTED_P1;
        auto isKeyPolledFn = id == 1 ? isKeyDownPolledP2 : isKeyDownPolled;
        auto& data         = SELECT_DIGIMON_DATA[id];

        data.unk5++;
        if (data.unk5 % 8 == 0) data.unk4 = !data.unk4;

        if (data.selectionState == 2) return;

        if (data.selectionState == 1) {
            constexpr uint16_t MASK = InputButtons::BUTTON_TRIANGLE | InputButtons::BUTTON_CIRCLE |
                                      InputButtons::BUTTON_CROSS | InputButtons::BUTTON_SQUARE |
                                      InputButtons::BUTTON_UP | InputButtons::BUTTON_RIGHT | InputButtons::BUTTON_DOWN |
                                      InputButtons::BUTTON_LEFT;

            if (isKeyPolledFn(InputButtons::BUTTON_START)) {
                playSound(0, 3);
                data.selectionState++;
                data.unk9 = 1;
                VS__removePressStartToBeginBox(id);
            }
            else if (isKeyPolledFn(static_cast<InputButtons>(MASK))) {
                playSound(0, 4);
                data.selectionState--;
                VS__removePressStartToBeginBox(id);
                data.selectedMask &= ~(1 << data.lastSlot);
                slots[data.lastSlot] = -1;
            }
            return;
        }

        if (data.selectionState == 0) {
            if (isKeyPolledFn(InputButtons::BUTTON_LEFT))
                data.listPage = ring(data.listPage - 1, 0, data.listPageCount);
            if (isKeyPolledFn(InputButtons::BUTTON_RIGHT))
                data.listPage = ring(data.listPage + 1, 0, data.listPageCount);

            const auto pageEntries = dtl::min(4, data.slotCount - data.listPage * 4);
            const auto offset      = data.listPage * 4;

            if (isKeyPolledFn(InputButtons::BUTTON_TRIANGLE) && !VS__isAlreadySelected(id, data.slotId[offset])) {
                slots[data.selectedSlot] = data.slotId[offset];
                VS__handleDigimonSelected(&data);
            }
            if (isKeyPolledFn(InputButtons::BUTTON_CROSS) && !VS__isAlreadySelected(id, data.slotId[offset + 3]) &&
                pageEntries >= 4) {
                slots[data.selectedSlot] = data.slotId[offset + 3];
                VS__handleDigimonSelected(&data);
            }
            if (isKeyPolledFn(InputButtons::BUTTON_SQUARE) && !VS__isAlreadySelected(id, data.slotId[offset + 1]) &&
                pageEntries >= 2) {
                slots[data.selectedSlot] = data.slotId[offset + 1];
                VS__handleDigimonSelected(&data);
            }
            if (isKeyPolledFn(InputButtons::BUTTON_CIRCLE) && !VS__isAlreadySelected(id, data.slotId[offset + 2]) &&
                pageEntries >= 3) {
                slots[data.selectedSlot] = data.slotId[offset + 2];
                VS__handleDigimonSelected(&data);
            }

            if (isKeyPolledFn(InputButtons::BUTTON_UP)) {
                playSound(0, 2);
                if (data.selectedSlot != 0) {
                    data.unk7 = 0;
                    data.selectedSlot--;
                }
            }
            if (isKeyPolledFn(InputButtons::BUTTON_DOWN)) {
                playSound(0, 2);
                if (data.selectedSlot < VS_NUM_BATTLES - 1) data.unk7 = 0;
                if (data.selectedSlot < VS_NUM_BATTLES - 1) data.selectedSlot++;
            }

            if (data.unk2 == data.detailPage * 24) {
                if (isKeyPolledFn(InputButtons::BUTTON_L1)) {
                    data.detailScollDir = -1;
                    if (data.detailPage != 0) data.detailPage--;
                }
                if (isKeyPolledFn(InputButtons::BUTTON_R1)) {
                    data.detailScollDir = 1;
                    if (data.detailPage < 5) data.detailPage++;
                }
            }
            else
                data.unk2 += data.detailScollDir * 4;

            if (data.selectedMask == ((1 << VS_NUM_BATTLES) - 1)) {
                VS__createPressStartToBeginBox(id);
                data.selectionState++;
            }
        }
    }

    struct UISlotEntry
    {
        int16_t clutX;
        int16_t clutY;
        uint8_t uvX;
        uint8_t uvY;
        uint8_t width;
        uint8_t height;
        uint8_t tpage;
        int8_t offsetX;
        int8_t offsetY;
    };

    constexpr dtl::array<UISlotEntry, 4> uiSlots{{
        {
            .clutX   = 64,
            .clutY   = 488,
            .uvX     = 0,
            .uvY     = 0,
            .width   = 41,
            .height  = 22,
            .tpage   = 5,
            .offsetX = 9,
            .offsetY = 30,
        },
        {
            .clutX   = 64,
            .clutY   = 488,
            .uvX     = 0,
            .uvY     = 0,
            .width   = 41,
            .height  = 22,
            .tpage   = 5,
            .offsetX = 9,
            .offsetY = 81,
        },
        {
            .clutX   = 64,
            .clutY   = 488,
            .uvX     = 0,
            .uvY     = 0,
            .width   = 41,
            .height  = 22,
            .tpage   = 5,
            .offsetX = 98,
            .offsetY = 30,
        },
        {
            .clutX   = 64,
            .clutY   = 488,
            .uvX     = 0,
            .uvY     = 0,
            .width   = 41,
            .height  = 22,
            .tpage   = 5,
            .offsetX = 98,
            .offsetY = 81,
        },
    }};

    constexpr dtl::array<UISlotEntry, 12> otherUiSlots = {{
        {
            .clutX   = 0,
            .clutY   = 488,
            .uvX     = 0,
            .uvY     = 0,
            .width   = 150,
            .height  = 215,
            .tpage   = 6,
            .offsetX = 0,
            .offsetY = 0,
        },
        {
            .clutX   = 0,
            .clutY   = 490,
            .uvX     = 150,
            .uvY     = 0,
            .width   = 68,
            .height  = 39,
            .tpage   = 6,
            .offsetX = 6,
            .offsetY = 27,
        },
        {
            .clutX   = 0,
            .clutY   = 491,
            .uvX     = 150,
            .uvY     = 39,
            .width   = 68,
            .height  = 39,
            .tpage   = 6,
            .offsetX = 6,
            .offsetY = 68,
        },
        {
            .clutX   = 0,
            .clutY   = 492,
            .uvX     = 150,
            .uvY     = 78,
            .width   = 68,
            .height  = 39,
            .tpage   = 6,
            .offsetX = 76,
            .offsetY = 27,
        },
        {
            .clutX   = 0,
            .clutY   = 493,
            .uvX     = 150,
            .uvY     = 117,
            .width   = 68,
            .height  = 39,
            .tpage   = 6,
            .offsetX = 76,
            .offsetY = 68,
        },
        {
            .clutX   = 0,
            .clutY   = 488,
            .uvX     = 150,
            .uvY     = 156,
            .width   = 20,
            .height  = 10,
            .tpage   = 6,
            .offsetX = 24,
            .offsetY = 2,
        },
        {
            .clutX   = 0,
            .clutY   = 489,
            .uvX     = 0,
            .uvY     = 0,
            .width   = 150,
            .height  = 215,
            .tpage   = 6,
            .offsetX = 0,
            .offsetY = 0,
        },
        {
            .clutX   = 0,
            .clutY   = 490,
            .uvX     = 150,
            .uvY     = 0,
            .width   = 68,
            .height  = 39,
            .tpage   = 6,
            .offsetX = 6,
            .offsetY = 27,
        },
        {
            .clutX   = 0,
            .clutY   = 491,
            .uvX     = 150,
            .uvY     = 39,
            .width   = 68,
            .height  = 39,
            .tpage   = 6,
            .offsetX = 6,
            .offsetY = 68,
        },
        {
            .clutX   = 0,
            .clutY   = 492,
            .uvX     = 150,
            .uvY     = 78,
            .width   = 68,
            .height  = 39,
            .tpage   = 6,
            .offsetX = 76,
            .offsetY = 27,
        },
        {
            .clutX   = 0,
            .clutY   = 493,
            .uvX     = 150,
            .uvY     = 117,
            .width   = 68,
            .height  = 39,
            .tpage   = 6,
            .offsetX = 76,
            .offsetY = 68,
        },
        {
            .clutX   = 0,
            .clutY   = 489,
            .uvX     = 170,
            .uvY     = 156,
            .width   = 20,
            .height  = 10,
            .tpage   = 6,
            .offsetX = 24,
            .offsetY = 2,
        },
    }};

    void setPolyFT4White(POLY_FT4* prim)
    {
        libgpu_SetPolyFT4(prim);
        prim->r0 = 128;
        prim->g0 = 128;
        prim->b0 = 128;
    }

    // TODO refactor and split into sub functions, especially the stat loops can be optimized to only need one loop
    void VS__renderSelectDigimonPlayer(int32_t id)
    {
        constexpr dtl::array<uint8_t, 6> statOffsets{8, 20, 32, 44, 56, 68};
        constexpr dtl::array<int16_t, 6> statLimits{9999, 9999, 999, 999, 999, 999};

        auto digimon           = id == 1 ? VS_DIGIMON_P2_PTR : VS_DIGIMON_P1_PTR;
        auto& data             = SELECT_DIGIMON_DATA[id];
        const auto pageEntries = dtl::min(4, data.slotCount - data.listPage * 4);
        const auto offset      = data.listPage * 4;
        const auto xOffset     = -152 + id * 154;

        // stat bars and values
        for (auto i = 0; i < pageEntries; i++) {
            auto& slot    = uiSlots[i];
            auto& entry   = (*digimon)[data.slotId[offset + i]];
            auto* statPtr = &entry.hp;

            // bars
            for (auto j = 0; j < 6; j++) {
                if (data.unk2 >= statOffsets[j] + 2) continue;
                if (statOffsets[j] > data.unk2 + 22) continue;

                auto value = (statPtr[j] * 34) / statLimits[j];

                auto* prim = reinterpret_cast<POLY_F4*>(libgs_GsGetWorkBase());
                libgpu_SetPolyF4(prim);
                prim->r0 = 0;
                prim->g0 = 255;
                prim->b0 = 255;
                prim->x0 = xOffset + slot.offsetX + 7;
                prim->x1 = xOffset + slot.offsetX + 7 + value;
                prim->x2 = xOffset + slot.offsetX + 7;
                prim->x3 = xOffset + slot.offsetX + 7 + value;
                prim->y0 = slot.offsetY - 109 + statOffsets[j] - data.unk2;
                prim->y1 = slot.offsetY - 109 + statOffsets[j] - data.unk2;
                prim->y2 = slot.offsetY - 107 + statOffsets[j] - data.unk2;
                prim->y3 = slot.offsetY - 107 + statOffsets[j] - data.unk2;
                libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 10, prim);
                libgs_GsSetWorkBase(prim + 1);
            }

            // values
            for (auto j = 0; j < 6; j++) {
                if (data.unk2 > j * 12) continue;
                if (j * 12 > data.unk2 + 15) continue;
                const RenderSettings settings{
                    .x      = static_cast<int16_t>(xOffset + slot.offsetX + 22),
                    .y      = static_cast<int16_t>(slot.offsetY - 109 + j * 12 - data.unk2),
                    .color  = TEXT_COLOR_PURE_WHITE,
                    .width  = 20,
                    .height = 3,
                    .alignX = AlignmentX::RIGHT,
                };
                getAtlas7px().renderSlow(format("%d", statPtr[j]).data(), 10, settings);
            }
            constexpr dtl::array<int16_t, 4> POS_X{54, 54, 80, 80};
            constexpr dtl::array<int16_t, 4> POS_Y{32, 85, 32, 85};

            getDigimonSprite(static_cast<DigimonType>(entry.digimonId))
                .render(xOffset + POS_X[i], POS_Y[i] - 109, 10, 0, data.unk4);

            // TODO missing Digimon name page, disabled in vanilla US
            // TODO missing move name page, disabled in vanilla US

            // icons
            auto* prim  = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
            prim->tpage = slot.tpage;
            prim->clut  = getClut(slot.clutX, slot.clutY);
            setPolyFT4White(prim);
            setUVDataPolyFT4(prim, slot.uvX, slot.uvY + data.unk2, slot.width, slot.height);
            setPosDataPolyFT4(prim, slot.offsetX + xOffset, slot.offsetY - 0x6d, slot.width, slot.height);
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 10, prim);
            libgs_GsSetWorkBase(prim + 1);
        }

        // list page blocks
        if (data.listPageCount > 1) {
            auto offset = xOffset + 0x4B - (data.listPageCount * 11) / 2;

            for (auto i = 0; i < data.listPageCount; i++) {
                auto* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
                setPolyFT4White(prim);
                prim->tpage = 6;
                prim->clut  = getClut(0, 488 + id);

                auto uCoord = i == data.listPage ? 0xbe : 0xc8;
                setUVDataPolyFT4(prim, uCoord, 0x9C, 10, 6);
                setPosDataPolyFT4(prim, offset + 11 * i, -90, 10, 6);
                libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 10, prim);
                libgs_GsSetWorkBase(prim + 1);
            }

            for (auto i = 0; i < 2; i++) {
                auto* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
                setPolyFT4White(prim);
                prim->tpage = 6;
                prim->clut  = getClut(0, 488 + id);
                auto uCoord = i == 0 ? 0xd2 : 0xd6;
                setUVDataPolyFT4(prim, uCoord, 0x9C, 5, 8);
                setPosDataPolyFT4(prim, xOffset + 7 + (i * 0x84), -91, 4, 7);
                libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 10, prim);
                libgs_GsSetWorkBase(prim + 1);
            }
        }

        // selected slot bars
        for (auto i = 0; i < VS_NUM_BATTLES; i++) {
            {
                auto* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
                setPolyFT4White(prim);
                prim->tpage = 6;
                prim->clut  = getClut(0, 495);

                if (i == data.selectedSlot && data.unk9 == 0) {
                    setUVDataPolyFT4(prim, data.unk6 * 12 + 150, 178, 12, 12);

                    if (data.unk7++ % 5 == 0) data.unk6++;
                    if (data.unk7 > 4) data.unk7 = 0;
                    data.unk6 %= 8;
                }
                else {
                    setUVDataPolyFT4(prim, 150 + 12 * i, 166, 12, 12);
                }
                setPosDataPolyFT4(prim, xOffset + 15, 5 + i * 20, 12, 12);
                libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 10, prim);
                libgs_GsSetWorkBase(prim + 1);
            }

            if ((data.selectedMask & (1 << i)) != 0) {
                {
                    auto* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
                    setPolyFT4White(prim);
                    prim->tpage = 6;
                    prim->clut  = getClut(0, 497);
                    setUVDataPolyFT4(prim, 0x98, 0xc8, 16, 16);
                    setPosDataPolyFT4(prim, xOffset + 33, 3 + i * 20, 16, 16);
                    libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 10, prim);
                    libgs_GsSetWorkBase(prim + 1);
                }

                for (auto j = 0; j < 6; j++) {
                    auto* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
                    setPolyFT4White(prim);
                    prim->tpage = 7;
                    prim->clut  = getClut(48, 488);
                    setUVDataPolyFT4(prim, 0x70, 0x48, 8, 8);
                    setPosDataPolyFT4(prim, xOffset + 0x39 + j * 8, 7 + i * 20, 8, 8);
                    libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 10, prim);
                    libgs_GsSetWorkBase(prim + 1);
                }
            }

            auto* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
            setPolyFT4White(prim);
            prim->tpage = 6;
            prim->clut  = getClut(0, 488 + id);
            auto height = (data.selectedMask & (1 << i)) == 0 ? 19 : 20;
            auto yPos   = (data.selectedMask & (1 << i)) == 0 ? 0xec : 0xd8;

            setUVDataPolyFT4(prim, 0, yPos, 0x85, height);
            setPosDataPolyFT4(prim, xOffset + 8, 1 + i * 20, 133, 20);
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 10, prim);
            libgs_GsSetWorkBase(prim + 1);
        }

        // UI backgrounds
        for (auto i = 5; i >= 0; i--) {
            auto* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
            setPolyFT4White(prim);
            auto& slot = otherUiSlots[id * 6 + i];

            prim->tpage = slot.tpage;
            prim->clut  = getClut(slot.clutX, slot.clutY);
            setUVDataPolyFT4(prim, slot.uvX, slot.uvY, slot.width, slot.height);
            setPosDataPolyFT4(prim, slot.offsetX + xOffset, slot.offsetY - 0x6d, slot.width, slot.height);
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 10, prim);
            libgs_GsSetWorkBase(prim + 1);
        }
    }

} // namespace

void VS__tickSelectDigimon()
{
    pressStartBoxExists = {false, false};

    clearTextArea();
    addObject(ObjectID::VS_SELECT_DIGIMON, 0, VS__tickSelectDigimonPlayer, VS__renderSelectDigimonPlayer);
    addObject(ObjectID::VS_SELECT_DIGIMON, 1, VS__tickSelectDigimonPlayer, VS__renderSelectDigimonPlayer);

    fadeFromBlack(10);

    for (auto i = 0; i < 11; i++)
        VS__tickFrame();

    while (SELECT_DIGIMON_DATA[0].selectionState != 2 || SELECT_DIGIMON_DATA[1].selectionState != 2)
        VS__tickFrame();

    fadeToBlack(20);
    for (auto i = 0; i < 21; i++)
        VS__tickFrame();

    removeObject(ObjectID::VS_SELECT_DIGIMON, 0);
    removeObject(ObjectID::VS_SELECT_DIGIMON, 1);
}
