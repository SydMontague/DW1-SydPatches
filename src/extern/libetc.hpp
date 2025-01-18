#include "stddef.hpp"

extern "C"
{
    using IRQCallback = void (*)();
    extern void libetc_InterruptCallback(uint32_t irq, IRQCallback callback);
}