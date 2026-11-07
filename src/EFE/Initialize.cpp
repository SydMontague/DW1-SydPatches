#include "../GameObjects.hpp"
#include "Engine.hpp"
#include "Sound.hpp"

namespace
{
    uint8_t* advanceToNextWord(uint8_t* arena)
    {
        // Vanilla reserves a whole word even when the input is already aligned.
        return arena + (4 - (reinterpret_cast<uint32_t>(arena) & 3));
    }
} // namespace

extern "C"
{
    [[gnu::optimize("Os")]]
    uint8_t* EFE_initializeEngine(uint8_t* arena)
    {
        EFE_removeEngine();
        arena         = advanceToNextWord(arena);
        EFE_SLOT_BASE = reinterpret_cast<EFELoading::Slot*>(arena);
        for (int32_t i = 0; i < 16; ++i) {
            EFE_SLOT_BASE[i].activeFile = nullptr;
            EFE_SLOT_BASE[i].uvCount    = 0;
        }
        // The following services preserve word alignment; keep vanilla word padding.
        arena           = arena + 644;
        arena           = EFE_initializeParticleEmitters(arena);
        arena           = arena + 4;
        arena           = initializeFlashData(arena);
        arena           = EFE_initializeAuraProjectiles(arena);
        arena           = arena + 4;
        EFE_FILE_BASE   = arena;
        EFE_FILE_CURSOR = arena;
        EFE_initializeSubOpcodes();
        EFE_clearSoundChannels();
        addObject(ObjectID::EFE_ENGINE, 0, EFE_tickEngine, EFE_renderEngine);
        return arena + 0x41000;
    }
}
