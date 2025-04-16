#include "stddef.hpp"

extern "C"
{
    using IRQCallback = void (*)();

    void libetc_InterruptCallback(uint32_t irq, IRQCallback callback);
    void libetc_ResetCallback();
}