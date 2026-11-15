#include "Engine.hpp"
#include "../GameObjects.hpp"
#include "../extern/libgpu.hpp"

namespace
{
    [[gnu::optimize("Os")]]
    void unloadSlot(int32_t index)
    {
        EFE_CURRENT_SLOT = &EFE_SLOT_BASE[index];
        auto& slot       = *EFE_CURRENT_SLOT;
        // Vanilla leaves these references live. Retire instances and UV records
        // before their file storage can be reused by the next engine.
        for (int32_t i = 0; i < slot.subEffectCount; i++) {
            auto& sub = slot.subEffects[i];
            if (sub.tickInstruction == nullptr) continue;
            sub.instance->frame = -1;
            sub.tickInstruction = nullptr;
        }
        for (int32_t i = 0; i < slot.uvCount; i++) {
            auto& uv = slot.uvRecords[i];
            if (uv.unk0 == 0) continue;
            uv.unk0         = 0;
            uv.numKeyframes = 0;
        }
        unloadModel(slot.fileId, EntityType::EFE);
        EFE_CURRENT_SLOT->activeFile = nullptr;
        EFE_CURRENT_SLOT->uvCount    = 0;
    }

    [[gnu::optimize("Os")]]
    void unloadAllSlots()
    {
        // Files callbacks and GPU transfers can still use slot storage. Finish
        // them here, at its owner, before any borrowed pointers are invalidated.
        EFE_finishLoading();
        libgpu_DrawSync(0);
        EFE_stopSounds();
        // Vanilla stop leaves ledger entries behind; drop borrows before releasing files.
        EFE_clearSoundChannels();
        // Particle owners borrow instances from the files about to be released.
        EFE_initializeParticleEmitters(reinterpret_cast<uint8_t*>(EFE_SLOT_BASE) + 644);
        for (int32_t i = 0; i < 16; i++) {
            if (EFE_LOADED_MOVE_DATA[i] == -1) continue;
            unloadSlot(i);
            EFE_LOADED_MOVE_DATA[i] = -1;
        }
        EFE_FILE_CURSOR = EFE_FILE_BASE;
    }
} // namespace

extern "C"
{
    [[gnu::optimize("Os")]]
    void EFE_removeEngine()
    {
        if (EFE_SLOT_BASE == nullptr) return;
        // Vanilla callers unload before removing the engine. Own both steps here
        // so repeated initialization also releases the previous engine's files.
        unloadAllSlots();
        EFE_removeAllAuraProjectiles();
        // Resident flash callbacks also borrow the engine's prefix storage.
        for (int32_t i = 0; i < 12; i++) {
            removeObject(ObjectID::EFE_FLASH, i);
            EFE_FLASH_DATA[i].progress = -1;
        }
        removeObject(ObjectID::EFE_ENGINE, 0);
        EFE_SLOT_BASE = nullptr;
    }
}
