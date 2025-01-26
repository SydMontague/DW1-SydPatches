#include "stddef.hpp"

extern "C"
{
    extern void* libapi_malloc3(uint32_t size);
    extern void libapi_free3(void* ptr);
}