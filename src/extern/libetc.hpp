#include "stddef.hpp"

extern "C"
{
    using IRQCallback = void (*)();

    void libetc_InterruptCallback(uint32_t irq, IRQCallback callback);
    void libetc_ResetCallback();
    int32_t libetc_vsync(int32_t val);
    uint32_t libetc_PadRead(uint16_t controllerId);
}