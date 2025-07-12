#pragma once

#include "UIElements.hpp"
#include "extern/stddef.hpp"

constexpr uint16_t DIGIMON_SPRITE_CLUTS[] = {0x7A07, 0x7A47, 0x7A87, 0x7AC7, 0x7B07, 0x7B47, 0x7B87};

constexpr dtl::array<uint8_t, 67> itemTakeDistance = {
    10, 10, 10, 10, 10, 15, 10, 10, 15, 10, 15, 10, 15, 10, 10, 10, 10, 10, 10, 15, 10, 10, 15,
    10, 15, 10, 25, 10, 10, 10, 10, 10, 10, 10, 10, 15, 15, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 15, 10, 10, 10, 10, 10, 10, 10, 10, 15, 15, 10, 10, 10, 10, 10,
};

constexpr uint8_t getItemTakeDistance(DigimonType type)
{
    return itemTakeDistance[static_cast<uint32_t>(type)];
}

struct DigimonSprite
{
    int16_t uvX;
    int16_t uvV;
    uint8_t clut;
    uint8_t tpage;

    void render(int16_t posX, int16_t posY, uint8_t layer, uint8_t flag, int32_t frame) const;
};

bool hasValidSprite(DigimonType type);

DigimonSprite const& getDigimonSprite(DigimonType type);

extern "C"
{
    DigimonType getOriginalType(DigimonType type);
}