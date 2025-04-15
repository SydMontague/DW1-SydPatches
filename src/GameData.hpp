#pragma once
#include "extern/stddef.hpp"

struct SleepPattern
{
    uint8_t sleepyHour;
    uint8_t sleepyMinute;
    uint8_t wakeupHour;
    uint8_t wakeupMinute;
    uint8_t awakeHours;
    uint8_t sleepyHours;
};

const extern dtl::array<SleepPattern, 8> SLEEP_PATTERN;