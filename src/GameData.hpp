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

struct DoorStruct
{
    dtl::array<uint8_t, 6> modelId;
    dtl::array<int16_t, 6> posX;
    dtl::array<int16_t, 6> posY;
    dtl::array<int16_t, 6> posZ;
    dtl::array<int16_t, 6> rotation;
};

const extern dtl::array<SleepPattern, 8> SLEEP_PATTERN;
const extern dtl::array<DoorStruct, 24> DOOR_MAPDATA;