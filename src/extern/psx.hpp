#pragma once
#include "stddef.hpp"

extern "C"
{
    struct TimerConfig
    {
        uint8_t syncEnable        : 1;
        uint8_t mode              : 2;
        uint8_t resetMode         : 1;
        uint8_t irqOnTarget       : 1;
        uint8_t irqOnOverflow     : 1;
        uint8_t irqOnceRepeatMode : 1;
        uint8_t irqPulseToggle    : 1;
        uint8_t clockSource       : 2;
        uint8_t interruptRequest  : 1;
        uint8_t reachedTarget     : 1;
        uint8_t reachedOverflow   : 1;
        uint8_t unknown           : 3;
        uint16_t garbage;
    };

    struct Timer
    {
        uint16_t currentValue;
        uint16_t padding1;
        TimerConfig config;
        uint16_t targetValue;
        uint16_t padding2;
        uint32_t padding3;
    };

    extern Timer TIMER0;
    extern Timer TIMER1;
    extern Timer TIMER2;

    extern uint32_t SCRATCHPAD;
}

static_assert(sizeof(TimerConfig) == 4);
static_assert(sizeof(Timer) == 0x10);