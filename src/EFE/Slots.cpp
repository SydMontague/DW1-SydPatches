#include "Engine.hpp"
#include "Sound.hpp"

extern "C"
{
    // The sole native caller, unload-all, has quiesced Files and drained the GPU.
    [[gnu::optimize("Os")]]
    void EFE_unloadSlot(int32_t index)
    {
        EFE_CURRENT_SLOT = &EFE_SLOT_BASE[index];
        auto& slot       = *EFE_CURRENT_SLOT;
        auto* sub        = slot.subEffects;
        for (int32_t remaining = slot.subEffectCount; remaining > 0; --remaining, ++sub) {
            EFE_retireSubEffectSounds(sub);
            if (sub->tickInstruction == nullptr) continue;
            sub->instance->frame = -1;
            sub->tickInstruction = nullptr;
        }
        for (int32_t i = 0; i < slot.uvCount; ++i) {
            auto& uv = slot.uvRecords[i];
            if (uv.unk0 == 0) continue;
            uv.unk0         = 0;
            uv.numKeyframes = 0;
        }
        unloadModel(slot.fileId, EntityType::EFE);
        EFE_CURRENT_SLOT->activeFile = nullptr;
        EFE_CURRENT_SLOT->uvCount    = 0;
    }
}
