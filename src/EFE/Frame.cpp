#include "../Map.hpp"
#include "../Sound.hpp"
#include "Engine.hpp"

namespace
{
    // With the current build flags, inlining saves 68 bytes and fits the overlay allocation.
    [[gnu::always_inline]]
    inline void tickSlots()
    {
        // Setup borrows the interpreter between Files callbacks, including its stacks.
        if (EFE_isSetupScriptActive()) return;
        for (int32_t slotIndex = 0; slotIndex < 16; slotIndex++) {
            EFE_SLOT_INDEX   = slotIndex;
            EFE_CURRENT_SLOT = &EFE_SLOT_BASE[slotIndex];
            auto& slot       = EFE_SLOT_BASE[slotIndex];
            if (slot.activeFile == nullptr) continue;
            EFE_SCRIPT_BASE = slot.activeFile;
            int32_t count   = slot.subEffectCount;
            for (int32_t subIndex = 0; subIndex < count; subIndex++) {
                EFE_SUBEFFECT_INDEX   = subIndex;
                EFE_CURRENT_SUBEFFECT = &slot.subEffects[subIndex];
                auto& sub             = slot.subEffects[subIndex];
                if (sub.tickInstruction == nullptr) continue;
                EFE_INSTANCE_BASE   = reinterpret_cast<uint8_t*>(sub.instance);
                EFE_PARENT_INSTANCE = sub.parentInstance;
                sub.instance->frame++;
                EFE_runScript(sub.tickInstruction);
            }
            EFE_SUBEFFECT_INDEX = count > 0 ? count : 0;
            for (int32_t i = 0; i < slot.uvCount; i++)
                EFE_tickUVAnimation(i);
        }
        EFE_SLOT_INDEX   = 16;
        EFE_CURRENT_SLOT = &EFE_SLOT_BASE[16];
    }

    [[gnu::optimize("Os")]]
    void renderSlots()
    {
        if (EFE_isSetupScriptActive()) return;
        for (int32_t i = 0; i < 16; i++) {
            EFE_CURRENT_SLOT = &EFE_SLOT_BASE[i];
            auto& slot       = EFE_SLOT_BASE[i];
            for (int32_t j = 0; j < slot.uvCount; j++)
                slot.uvRecords[j].unk2 = 0;
            if (slot.activeFile == nullptr) continue;
            EFE_SCRIPT_BASE = slot.activeFile;
            int32_t count   = slot.subEffectCount;
            for (int32_t j = 0; j < count; j++) {
                EFE_CURRENT_SUBEFFECT = &slot.subEffects[j];
                auto& sub             = slot.subEffects[j];
                if (sub.tickInstruction == nullptr) continue;
                EFE_INSTANCE_BASE   = reinterpret_cast<uint8_t*>(sub.instance);
                EFE_PARENT_INSTANCE = sub.parentInstance;
                EFE_runScript(sub.renderInstruction);
            }
        }
        EFE_CURRENT_SLOT = &EFE_SLOT_BASE[16];
    }
} // namespace

extern "C"
{
    [[gnu::optimize("Os")]]
    void EFE_tickEngine(int32_t)
    {
        setMapLayerEnabled(1);
        for (auto& flag : EFE_SLOT_FLAGS)
            if (flag == 0) flag = 1;
        tickSlots();
        EFE_tickParticleEmitters();
        for (auto& sound : EFE_SOUND_RECORDS) {
            if (sound.mask < 0) continue;
            if (sound.owner->instance->frame >= 0) continue;
            stopSoundMask(sound.mask);
            sound.mask = -1;
        }
    }

    [[gnu::optimize("Os")]]
    void EFE_renderEngine(int32_t)
    {
        renderSlots();
        EFE_renderParticleEmitters();
    }
}
