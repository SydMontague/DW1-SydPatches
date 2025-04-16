#pragma once
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    void stopBGM();
    bool playMusic(int32_t font, int32_t track);
    bool loadMapSounds(int32_t mapSoundId);
    void loadPartnerSounds(DigimonType type);
    void loadDigimonSounds(int32_t vabId, DigimonType type);
    uint32_t playSound2(int32_t vabId, uint32_t value);
    void stopSound();
    uint32_t playSound(int32_t vabId, uint32_t val);
    void stopSoundMask(uint32_t mask);
    void finalizeMusic();
    bool initializeMusic();
}