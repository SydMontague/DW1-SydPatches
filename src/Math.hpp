#include "extern/stddef.hpp"
#include "extern/libgte.hpp"

extern "C"
{
    constexpr int32_t abs(int32_t val)
    {
        return val < 0 ? -val : val;
    }

    extern int32_t random(int32_t limit);
    extern int16_t sin(uint32_t input);
    extern int16_t cos(uint32_t input);
    extern int16_t atan(int32_t dx, int32_t dy);
    extern void setRotTransMatrix(Matrix* matrix);
}