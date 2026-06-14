#pragma once
#include "dw1.hpp"
#include "libgte.hpp"

extern "C"
{
    extern Vector INTRO_TARGET_POS;
    extern dtl::array<dtl::array<uint8_t, 14>, 116> VS__INTRO_DIGIMON_NAMES;

    void VS__setupIntroCamera(Entity* entity, int32_t distance, int32_t spot);
    void VS__tickFrame();
    void VS__addTimer(int32_t seconds);
    void VS__addHPMPBar(int32_t player);
    void VS__addCommandBar(int32_t player);
}