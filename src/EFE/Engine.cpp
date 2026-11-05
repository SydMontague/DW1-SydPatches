#include "Engine.hpp"
#include "../GameObjects.hpp"
#include "../extern/libgpu.hpp"
#include "Sound.hpp"

extern "C"
{
    [[gnu::optimize("Os")]]
    void EFE_unloadAllSlots()
    {
        if (EFE_SLOT_BASE == nullptr) return;
        EFE_quiesceLoading();
        libgpu_DrawSync(0);
        EFE_stopSounds();
        // Particle owners borrow instances from the files about to be released.
        EFE_initializeParticleEmitters(reinterpret_cast<uint8_t*>(EFE_SLOT_BASE) + 644);
        for (int32_t i = 0; i < 16; ++i) {
            if (EFE_LOADED_MOVE_DATA[i] == -1) continue;
            EFE_unloadSlot(i);
            EFE_LOADED_MOVE_DATA[i] = -1;
        }
        EFE_FILE_CURSOR = EFE_FILE_BASE;
    }

    [[gnu::optimize("Os")]]
    void EFE_removeEngine()
    {
        if (EFE_SLOT_BASE == nullptr) return;
        EFE_unloadAllSlots();
        EFE_removeAllAuraProjectiles();
        // Resident flash callbacks also borrow the engine's prefix storage.
        for (int32_t i = 0; i < 12; ++i) {
            removeObject(ObjectID::EFE_FLASH, i);
            EFE_FLASH_DATA[i].progress = -1;
        }
        while (removeObject(ObjectID::EFE_ENGINE, 0))
            ;
        EFE_SLOT_BASE = nullptr;
    }
}
