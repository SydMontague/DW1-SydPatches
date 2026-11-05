#include "Engine.hpp"
extern "C"
{
    [[gnu::optimize("Os")]]
    void EFE_renderEngine(int32_t)
    {
        if (!EFE_isSetupScriptActive()) {
            auto* current = EFE_SLOT_BASE;
            for (int32_t i = 0; i < 16; ++i, ++current) {
                EFE_CURRENT_SLOT = current;
                auto& slot       = *current;
                for (int32_t j = 0; j < slot.uvCount; ++j)
                    slot.uvRecords[j].unk2 = 0;
                if (slot.activeFile == nullptr) continue;
                EFE_SCRIPT_BASE = slot.activeFile;
                int32_t count   = slot.subEffectCount;
                auto* sub       = slot.subEffects;
                for (int32_t j = 0; j < count; ++j, ++sub) {
                    EFE_CURRENT_SUBEFFECT = sub;
                    if (sub->tickInstruction == nullptr) continue;
                    EFE_INSTANCE_BASE   = reinterpret_cast<uint8_t*>(sub->instance);
                    EFE_PARENT_INSTANCE = sub->parentInstance;
                    EFE_runScript(sub->renderInstruction);
                }
            }
            EFE_CURRENT_SLOT = current;
        }
        EFE_renderParticleEmitters();
    }
}
