#include "Font.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"
#include "extern/libetc.hpp"
#include "extern/psx.hpp"

extern "C"
{
    uint32_t irqCount = 0;

    void debugOverlayTick(int32_t instanceId)
    {
        RECT area = {
            .x      = 352,
            .y      = 224,
            .width  = 32,
            .height = 32,
        };
        uint8_t buffer[64];
        
        auto previous_color = COLORCODE_LOWBITS;

        setTextColor(1);
        sprintf(buffer, "%u", RNG_STATE);
        libgpu_ClearImage(&area, 0, 0, 0);
        drawStringNew(&myFont5px, buffer, 352, 224);
        sprintf(buffer, "%u", irqCount / 8);
        drawStringNew(&myFont5px, buffer, 352, 230);
        
        setTextColor(previous_color);
    }

    void debugOverlayRender(int32_t instanceId)
    {
        renderStringNew(0, -159, -119, 127, 12, 352, 224, 1, 1);
    }

    void timer0irq()
    {
        irqCount++;
    }

    void initCustomUI()
    {
        TIMER0.config.syncEnable        = 0;
        TIMER0.config.resetMode         = 1;
        TIMER0.config.irqOnTarget       = 1;
        TIMER0.config.irqOnOverflow     = 0;
        TIMER0.config.irqOnceRepeatMode = 1;
        TIMER0.config.irqPulseToggle    = 0;
        TIMER0.config.clockSource       = 0;
        TIMER0.targetValue              = 42336; // 33868800 Hz / 42336 -> 800 ticks per second
        libetc_InterruptCallback(4, timer0irq);

        addObject(ObjectID::DEBUG_OVERLAY, 0, &debugOverlayTick, &debugOverlayRender);
    }
}