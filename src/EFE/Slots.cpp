#include "Loading.hpp"

extern "C"
{
    void EFE_unloadSlot(int32_t index)
    {
        EFE_CURRENT_SLOT = &EFE_SLOT_BASE[index];
        auto& slot       = *EFE_CURRENT_SLOT;
        for (int32_t i = 0; i < slot.uvCount; ++i) {
            auto& uv = slot.uvRecords[i];
            if (uv.unk0 == 0) continue;
            uv.unk0         = 0;
            uv.numKeyframes = 0;
        }
        unloadModel(slot.fileId, EntityType::EFE);
        EFE_CURRENT_SLOT->activeFile = nullptr;
    }

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
