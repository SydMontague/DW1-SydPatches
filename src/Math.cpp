#include "extern/libc.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    int32_t random(int32_t limit)
    {
        return (limit * rand()) >> 0xF;
    }
}