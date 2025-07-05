#include "extern/stddef.hpp"

extern "C"
{
    void tickGameClock(int32_t instanceId);
    void renderGameClock(int32_t instanceId);
    void addClock();
    void advanceToTime(uint32_t hour, uint32_t minute);
    void updateMinuteHand(int32_t hour, int32_t minute);
    void startGameTime();
    void stopGameTime();
}