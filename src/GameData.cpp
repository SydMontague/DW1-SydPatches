#include "GameData.hpp"

const dtl::array<SleepPattern, 8> SLEEP_PATTERN = {{
    {.sleepyHour = 16, .sleepyMinute = 0, .wakeupHour = 1, .wakeupMinute = 0, .awakeHours = 15, .sleepyHours = 9},
    {.sleepyHour = 19, .sleepyMinute = 0, .wakeupHour = 4, .wakeupMinute = 0, .awakeHours = 15, .sleepyHours = 9},
    {.sleepyHour = 22, .sleepyMinute = 0, .wakeupHour = 7, .wakeupMinute = 0, .awakeHours = 15, .sleepyHours = 9},
    {.sleepyHour = 2, .sleepyMinute = 0, .wakeupHour = 11, .wakeupMinute = 0, .awakeHours = 15, .sleepyHours = 9},
    {.sleepyHour = 7, .sleepyMinute = 0, .wakeupHour = 16, .wakeupMinute = 0, .awakeHours = 15, .sleepyHours = 9},
    {.sleepyHour = 10, .sleepyMinute = 0, .wakeupHour = 19, .wakeupMinute = 0, .awakeHours = 15, .sleepyHours = 9},
    {.sleepyHour = 0, .sleepyMinute = 0, .wakeupHour = 0, .wakeupMinute = 0, .awakeHours = 4, .sleepyHours = 1},
    {.sleepyHour = 0, .sleepyMinute = 0, .wakeupHour = 0, .wakeupMinute = 0, .awakeHours = 8, .sleepyHours = 3},
}};
