#pragma once

#include "stddef.hpp"

extern "C"
{
    void libapi_InitHeap3(void* start, size_t size);
    extern void* libapi_malloc3(uint32_t size);
    extern void libapi_free3(void* ptr);

    // Returns nonzero only when interrupts were enabled before this call.
    int32_t libapi_EnterCriticalSection();
    void libapi_ExitCriticalSection();
}
