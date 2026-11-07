#pragma once

struct EFESubEffect;

extern "C"
{
    void EFE_clearSoundChannels();
    void EFE_stopSounds();
    void EFE_tickSounds();
    void EFE_retireSubEffectSounds(EFESubEffect* owner);
}
