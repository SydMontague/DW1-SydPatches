#include "Timestamp.hpp"

#include "GameObjects.hpp"
#include "extern/libc.hpp"
#include "extern/libetc.hpp"
#include "extern/psx.hpp"

static uint32_t irqCount = 0;

static void timer0irq()
{
    irqCount++;
}

uint32_t getTickCount()
{
    return irqCount;
}

uint64_t getTimestamp()
{
    auto currentValue = TIMER2.currentValue;
    uint64_t us       = (irqCount * 1250LL) + (currentValue * 1250) / 42336;

    return us;
}

extern "C" void initTimestamp()
{
    TIMER0.config.syncEnable        = 0;
    TIMER0.config.resetMode         = 1;
    TIMER0.config.irqOnTarget       = 1;
    TIMER0.config.irqOnOverflow     = 0;
    TIMER0.config.irqOnceRepeatMode = 1;
    TIMER0.config.irqPulseToggle    = 0;
    TIMER0.config.clockSource       = 0;     // system clock
    TIMER0.targetValue              = 42336; // 33868800 Hz / 42336 -> 800 ticks per second
    libetc_InterruptCallback(4, timer0irq);
}
