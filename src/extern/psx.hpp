#pragma once
#include "stddef.hpp"

template<uint32_t reg> struct GTEControlRegister
{
    inline operator uint32_t()
    {
        uint32_t value;
        asm volatile("cfc2 %0, $%1" : "=r"(value) : "I"(reg));
        return value;
    }

    void operator=(uint32_t val) { asm volatile("ctc2 %0, $%1" ::"r"(val), "I"(reg)); }
};

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

    register uint32_t* gp asm("gp");
    register uint32_t* sp asm("sp");

    register uint32_t cop0_status asm("c0r12");

    static GTEControlRegister<0> gte_rot1;
    static GTEControlRegister<1> gte_rot2;
    static GTEControlRegister<2> gte_rot3;
    static GTEControlRegister<3> gte_rot4;
    static GTEControlRegister<4> gte_rot5;
    static GTEControlRegister<5> gte_transX;
    static GTEControlRegister<6> gte_transY;
    static GTEControlRegister<7> gte_transZ;
    static GTEControlRegister<8> gte_light_source1;
    static GTEControlRegister<9> gte_light_source2;
    static GTEControlRegister<10> gte_light_source3;
    static GTEControlRegister<11> gte_light_source4;
    static GTEControlRegister<12> gte_light_source5;
    static GTEControlRegister<13> gte_backgroundR;
    static GTEControlRegister<14> gte_backgroundG;
    static GTEControlRegister<15> gte_backgroundB;
    static GTEControlRegister<16> gte_light_color1;
    static GTEControlRegister<17> gte_light_color2;
    static GTEControlRegister<18> gte_light_color3;
    static GTEControlRegister<19> gte_light_color4;
    static GTEControlRegister<20> gte_light_color5;
    static GTEControlRegister<21> gte_farR;
    static GTEControlRegister<22> gte_farG;
    static GTEControlRegister<23> gte_farB;
    static GTEControlRegister<24> gte_screenX;
    static GTEControlRegister<25> gte_screenY;
    static GTEControlRegister<26> gte_plane_distance;
}

static_assert(sizeof(TimerConfig) == 4);
static_assert(sizeof(Timer) == 0x10);