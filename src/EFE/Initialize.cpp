#include "../GameObjects.hpp"
#include "Engine.hpp"

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
        // Vanilla assumes a fresh engine. Release the prior registration and
        // borrowed resources before reusing its arena on repeated initialization.
        EFE_removeEngine();
        arena         = advanceToNextWord(arena);
        EFE_SLOT_BASE = reinterpret_cast<EFELoading::Slot*>(arena);
        for (int32_t i = 0; i < 16; i++) {
            EFE_SLOT_BASE[i].activeFile = nullptr;
            EFE_SLOT_BASE[i].uvCount    = 0;
        }
        // Particles return arena + 9156; flash reserves 12 word-aligned records.
        // Aura aligns internally and returns another 288 bytes; keep vanilla padding.
        arena           = advanceToNextWord(arena + sizeof(EFELoading::Slot) * 16);
        arena           = EFE_initializeParticleEmitters(arena);
        arena           = advanceToNextWord(arena);
        arena           = initializeFlashData(arena);
        arena           = EFE_initializeAuraProjectiles(arena);
        arena           = advanceToNextWord(arena);
        EFE_FILE_BASE   = arena;
        EFE_FILE_CURSOR = arena;
        EFE_initializeSubOpcodes();
        EFE_clearSoundChannels();
        addObject(ObjectID::EFE_ENGINE, 0, EFE_tickEngine, EFE_renderEngine);
        return arena + 0x41000;
    }
}
