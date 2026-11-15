#include "Engine.hpp"
extern "C"
{
    [[gnu::optimize("Os")]]
    int32_t EFE_runSlotScript(int32_t index)
    {
        EFE_INSTANCE_BASE     = nullptr;
        auto& slot            = EFE_SLOT_BASE[index];
        EFE_CURRENT_SLOT      = &slot;
        EFE_SCRIPT_BASE       = slot.activeFile;
        EFE_CURRENT_SUBEFFECT = slot.subEffects;
        EFE_SCRIPT_RESULT     = -1;
        return EFE_runScript(slot.runInstruction);
    }
}
