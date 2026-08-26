#include "../AtlasFont.hpp"
#include "../Fade.hpp"
#include "../GameObjects.hpp"
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

} // namespace
extern "C"
{
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

    void VS__tickSelectDigimon()
    {
        VS_PRESS_START_BOX_CREATED = {-1, -1};

        clearTextArea();
        addObject(ObjectID::VS_SELECT_DIGIMON, 0, VS__tickSelectDigimonPlayer, VS__renderSelectDigimonPlayer);
        addObject(ObjectID::VS_SELECT_DIGIMON, 1, VS__tickSelectDigimonPlayer, VS__renderSelectDigimonPlayer);

        fadeFromBlack(10);

        for (auto i = 0; i < 11; i++)
            VS__tickFrame();

        VS_BOTH_SELECTED = 0;
        while (VS_BOTH_SELECTED == 0)
            VS__tickFrame();

        fadeToBlack(20);
        for (auto i = 0; i < 21; i++)
            VS__tickFrame();

        removeObject(ObjectID::VS_SELECT_DIGIMON, 0);
        removeObject(ObjectID::VS_SELECT_DIGIMON, 1);
    }
}
