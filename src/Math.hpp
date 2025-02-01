#include "extern/libgte.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    constexpr int32_t abs(int32_t val)
    {
        return val < 0 ? -val : val;
    }

    constexpr int32_t min(int32_t val1, int32_t val2)
    {
        return val1 < val2 ? val1 : val2;
    }

    constexpr int32_t max(int32_t val1, int32_t val2)
    {
        return val1 < val2 ? val2 : val1;
    }

    constexpr int32_t clamp(int32_t val, int32_t min, int32_t max)
    {
        if (min > val) return min;
        if (max < val) return max;
        return val;
    }

    extern int32_t random(int32_t limit);
    extern int16_t sin(uint32_t input);
    extern int16_t cos(uint32_t input);
    extern int16_t atan(int32_t dx, int32_t dy);
    extern void setRotTransMatrix(Matrix* matrix);
}