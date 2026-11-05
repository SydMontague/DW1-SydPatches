#include "Sound.hpp"
#include "../Sound.hpp"
#include "Operands.hpp"

extern "C"
{
    void EFE_clearSoundChannels()
    {
        // Clearing forgets ownership without stopping playback.
        clearSoundOwners();
    }

    // A null owner retires every owned voice; native stop has no argument.
    [[gnu::optimize("Os")]]
    void EFE_retireSubEffectSounds(EFESubEffect* owner)
    {
        for (uint32_t voice = 10; voice < 24; ++voice) {
            auto* current = getSoundOwner(voice);
            if (current != nullptr && (owner == nullptr || current == owner)) stopOwnedSound(voice, current);
        }
    }

    void EFE_stopSounds()
    {
        EFE_retireSubEffectSounds(nullptr);
    }

    [[gnu::optimize("Os")]]
    void EFE_playSound()
    {
        auto id = EFEOperands::pop();
        if (id < 0) return;
        if (id == 0x12 || id == 0x20 || id == 0x21 || id == 0x22)
            playOwnedSound(8, id, EFE_CURRENT_SUBEFFECT);
        else
            playSound(8, id);
    }

    [[gnu::optimize("Os")]]
    void EFE_tickSounds()
    {
        for (uint32_t voice = 10; voice < 24; ++voice) {
            auto* owner = getSoundOwner(voice);
            if (owner != nullptr && owner->instance->frame < 0) stopOwnedSound(voice, owner);
        }
    }
}
