#include "../Math.hpp"
#include "Transform.hpp"

using namespace EFETransform;

extern "C"
{
    void EFE_ENTRY(calculateSine)()
    {
        auto* out  = popPointer<int32_t>();
        auto scale = pop();
        *out       = scaledComponent(scale, libgte_rsin(pop() & 0xFFF));
    }

    void EFE_ENTRY(calculateCosine)()
    {
        auto* out  = popPointer<int32_t>();
        auto scale = pop();
        *out       = scaledComponent(scale, libgte_rcos(pop() & 0xFFF));
    }

    void EFE_ENTRY(calculatePolarOffset)()
    {
        auto radius = pop();
        auto angle  = 128U - pop();
        auto* out   = popPointer<int32_t>();
        out[2]      = scaledComponent(radius, sin(angle));
        out[0]      = scaledComponent(radius, cos(angle));
    }
}
