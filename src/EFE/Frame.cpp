#include "../Map.hpp"
#include "Engine.hpp"
#include "Sound.hpp"

extern "C"
{
    [[gnu::optimize("Os")]]
    void EFE_tickEngine(int32_t)
    {
        setMapLayerEnabled(1);
        for (auto& flag : EFE_SLOT_FLAGS)
            if (flag == 0) flag = 1;

        // Setup borrows the interpreter between Files callbacks, including its stacks.
        if (!EFE_isSetupScriptActive()) {
            auto* current = EFE_SLOT_BASE;
            for (int32_t slotIndex = 0; slotIndex < 16; ++slotIndex, ++current) {
                EFE_SLOT_INDEX   = slotIndex;
                EFE_CURRENT_SLOT = current;
                auto& slot       = *current;
                if (slot.activeFile == nullptr) continue;
                EFE_SCRIPT_BASE = slot.activeFile;
                int32_t count   = slot.subEffectCount;
                auto* sub       = slot.subEffects;
                for (int32_t subIndex = 0; subIndex < count; ++subIndex, ++sub) {
                    EFE_SUBEFFECT_INDEX   = subIndex;
                    EFE_CURRENT_SUBEFFECT = sub;
                    if (sub->tickInstruction == nullptr) continue;
                    EFE_INSTANCE_BASE   = reinterpret_cast<uint8_t*>(sub->instance);
                    EFE_PARENT_INSTANCE = sub->parentInstance;
                    ++sub->instance->frame;
                    EFE_runScript(sub->tickInstruction);
                }
                EFE_SUBEFFECT_INDEX = count > 0 ? count : 0;
                for (int32_t i = 0; i < slot.uvCount; ++i)
                    EFE_tickUVAnimation(i);
            }
            EFE_SLOT_INDEX   = 16;
            EFE_CURRENT_SLOT = current;
        }
        EFE_tickParticleEmitters();
        EFE_tickSounds();
    }
}
