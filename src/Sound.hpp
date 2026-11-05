#pragma once
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

struct EFESubEffect;

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
    int32_t isSoundBufferLoadingDone(int32_t id);
    int32_t readVBALLSection(int32_t id, int32_t soundId);
    int32_t loadMapSounds2(int32_t mapSoundId);
    int32_t isSoundLoaded(bool isAsync, int32_t soundId);
    int32_t loadVLALL(int16_t type, uint8_t* buffer);
    int32_t loadSB();
}

void clearSoundOwners();
EFESubEffect* getSoundOwner(uint32_t voice);
void stopOwnedSound(uint32_t voice, EFESubEffect* owner);
void playOwnedSound(int32_t vabId, uint32_t value, EFESubEffect* owner);
