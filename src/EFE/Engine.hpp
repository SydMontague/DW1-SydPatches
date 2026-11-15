#pragma once

#include "Loading.hpp"

extern "C"
{
    void EFE_tickEngine(int32_t);
    void EFE_renderEngine(int32_t);
    uint8_t* EFE_initializeEngine(uint8_t* arena);
    void EFE_removeEngine();
    void EFE_initializeSubOpcodes();
    uint8_t* EFE_initializeParticleEmitters(uint8_t* arena);
    void EFE_tickParticleEmitters();
    void EFE_renderParticleEmitters();
    uint8_t* initializeFlashData(uint8_t* arena);
    uint8_t* EFE_initializeAuraProjectiles(uint8_t* arena);
    void EFE_removeAllAuraProjectiles();
    void EFE_tickUVAnimation(int32_t index);
    void EFE_clearSoundChannels();
    void EFE_stopSounds();
}
