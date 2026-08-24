#include "../Fade.hpp"
#include "../GameObjects.hpp"
#include "../Sound.hpp"
#include "../extern/VS.hpp"
#include "../extern/dtl/array.hpp"
#include "../extern/dw1.hpp"

extern "C"
{
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
