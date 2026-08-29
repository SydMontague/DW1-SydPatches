#include "../AtlasFont.hpp"
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
