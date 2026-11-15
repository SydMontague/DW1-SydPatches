#include "../Sound.hpp"
#include "Engine.hpp"

extern "C"
{
    void EFE_stopSounds()
    {
        for (auto& sound : EFE_SOUND_RECORDS) {
            if (sound.mask < 0) continue;
            stopSoundMask(sound.mask);
            sound.mask = -1;
        }
    }
}
