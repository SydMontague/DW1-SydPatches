#include "../extern/dtl/algorithm.hpp"
#include "Operands.hpp"

using EFEOperands::popPointer;

namespace
{
    inline int32_t divideComponent(int32_t value, int32_t divisor)
    {
        // Preserve the vanilla signed quotient for a zero divisor.
        if (divisor == 0) return value < 0 ? 1 : -1;
        return value / divisor;
    }
} // namespace

extern "C"
{
    void EFE_ENTRY(copyVector)()
    {
        const auto* source = popPointer<const uint32_t>();
        auto* destination  = popPointer<uint32_t>();
        dtl::copy(source, source + 3, destination);
    }

    void EFE_ENTRY(maskVectors)()
    {
        const auto* source = popPointer<const uint32_t>();
        auto* destination  = popPointer<uint32_t>();
        for (auto i = 0; i < 3; i++)
            destination[i] &= source[i];
    }

    void EFE_ENTRY(addVectors)()
    {
        auto* right = popPointer<const uint32_t>();
        auto* left  = popPointer<uint32_t>();
        for (auto i = 0; i < 3; i++)
            left[i] += right[i];
    }

    void EFE_ENTRY(subtractVectors)()
    {
        auto* right = popPointer<const uint32_t>();
        auto* left  = popPointer<uint32_t>();
        for (auto i = 0; i < 3; i++)
            left[i] -= right[i];
    }

    void EFE_ENTRY(multiplyVectors)()
    {
        auto* right = popPointer<const uint32_t>();
        auto* left  = popPointer<uint32_t>();
        for (auto i = 0; i < 3; i++)
            left[i] *= right[i];
    }

    [[gnu::optimize("Os")]]
    void EFE_ENTRY(divideVectors)()
    {
        auto* divisors = popPointer<const int32_t>();
        auto* values   = popPointer<int32_t>();
        for (auto i = 0; i < 3; i++)
            values[i] = divideComponent(values[i], divisors[i]);
    }

    void EFE_ENTRY(shiftVectorsRight)()
    {
        auto* shifts = popPointer<const int32_t>();
        auto* values = popPointer<int32_t>();
        for (auto i = 0; i < 3; i++)
            values[i] >>= shifts[i] & 31;
    }

    void EFE_ENTRY(multiplyVectorByScalar)()
    {
        auto factor  = EFEOperands::pop();
        auto* vector = popPointer<uint32_t>();
        for (auto i = 0; i < 3; i++)
            vector[i] *= factor;
    }

    [[gnu::optimize("Os")]]
    void EFE_ENTRY(divideVectorByScalar)()
    {
        auto divisor = EFEOperands::pop();
        auto* vector = popPointer<int32_t>();
        for (auto i = 0; i < 3; i++)
            vector[i] = divideComponent(vector[i], divisor);
    }

    void EFE_ENTRY(maskVectorByScalar)()
    {
        auto mask    = EFEOperands::pop();
        auto* vector = popPointer<uint32_t>();
        for (auto i = 0; i < 3; i++)
            vector[i] &= mask;
    }
}
