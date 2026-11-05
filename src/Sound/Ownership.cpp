#include "../Sound.hpp"
#include "../extern/libapi.hpp"
#include "../extern/libsnd.hpp"
#include "VoiceOwners.hpp"

[[gnu::optimize("Os")]]
void clearSoundOwners()
{
    for (auto& owner : SoundInternal::soundOwners)
        owner = nullptr;
}

// The main thread retires subeffects before reuse. IRQs only clear this snapshot.
// Keep ownership through queued starts and natural completion until reuse or retirement.
EFESubEffect* getSoundOwner(uint32_t voice)
{
    return SoundInternal::soundOwners[voice - 10];
}

[[gnu::optimize("Os")]]
void stopOwnedSound(uint32_t voice, EFESubEffect* owner)
{
    // An interrupt may have reused this voice since the caller inspected its owner.
    auto enabled  = libapi_EnterCriticalSection();
    auto& current = SoundInternal::soundOwners[voice - 10];
    if (owner != nullptr && current == owner) {
        current = nullptr;
        libsnd_SsUtAutoVol(voice, 0x7f, 0, 6);
        libsnd_SsUtKeyOffV(voice);
    }
    // Preserve a caller that already had interrupts disabled.
    if (enabled) libapi_ExitCriticalSection();
}
