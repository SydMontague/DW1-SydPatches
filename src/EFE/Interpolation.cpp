#include "../Math.hpp"
#include "../extern/dtl/algorithm.hpp"
#include "Operands.hpp"

using namespace EFEOperands;

extern "C"
{
    void EFE_ENTRY(interpolateVector)()
    {
        auto* out      = popPointer<int32_t>();
        auto time      = pop();
        auto endTime   = pop();
        auto startTime = pop();
        auto* end      = popPointer<int32_t>();
        auto* start    = popPointer<int32_t>();
        for (int i = 0; i < 3; ++i)
            out[i] = lerp(start[i], end[i], startTime, endTime, time);
    }

    void EFE_ENTRY(interpolateValue)()
    {
        auto end   = pop();
        auto start = pop();
        auto time  = pop();
        auto hi    = pop();
        auto lo    = pop();
        auto* out  = popPointer<int32_t>();
        if (hi < lo) dtl::swap(lo, hi);
        time = clamp(time, lo, hi);
        *out = lo == hi ? start : start + (time - lo) * (end - start) / (hi - lo);
    }
}
