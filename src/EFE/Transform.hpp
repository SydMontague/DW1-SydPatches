#pragma once

#include "../extern/libgte.hpp"
#include "Operands.hpp"
#include "Types.hpp"

namespace EFETransform
{
    using EFEHelpers::toShortVector;
    using EFEOperands::pop;
    using EFEOperands::popPointer;

    inline void writeVector(int32_t* words, const SVector& value)
    {
        words[0] = value.x;
        words[1] = value.y;
        words[2] = value.z;
    }

    inline int32_t scaledComponent(int32_t scale, int32_t component)
    {
        return static_cast<int32_t>(static_cast<uint32_t>(scale) * component) >> 12;
    }
} // namespace EFETransform
