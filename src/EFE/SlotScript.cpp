#include "Engine.hpp"
#include "Sound.hpp"
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

    [[gnu::optimize("Os")]]
    void EFE_initializeSubEffectInstructions()
    {
        auto* render = reinterpret_cast<int16_t*>(EFEOperands::pop());
        auto* tick   = reinterpret_cast<int16_t*>(EFEOperands::pop());
        auto* sub    = EFE_CURRENT_SUBEFFECT;
        EFE_retireSubEffectSounds(sub);
        reinterpret_cast<EFEInstance*>(EFE_INSTANCE_BASE)->frame = 0;
        sub->tickInstruction                                     = tick;
        sub->renderInstruction                                   = render;
    }
}
