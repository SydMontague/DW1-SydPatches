#pragma once

#include "../extern/dw1.hpp"

namespace EFEHelpers
{
    template<class T> static inline T readMemory(auto address)
    {
        return *reinterpret_cast<const volatile T*>(address);
    }

    static inline void writeWord(auto address, uint32_t value)
    {
        *reinterpret_cast<volatile uint32_t*>(address) = value;
    }

    static inline int32_t getWord(int32_t index)
    {
        return readMemory<int16_t>(EFE_INSTRUCTION_POINTER + index);
    }

    static inline void advanceInstruction(int32_t bytes)
    {
        EFE_INSTRUCTION_POINTER += bytes / 2;
    }

    static inline uint32_t variableAddress(int32_t offset, bool instance)
    {
        auto* base = instance ? EFE_INSTANCE_BASE : EFE_SCRIPT_BASE;
        return reinterpret_cast<uint32_t>(base) + offset;
    }

    static inline int32_t readVariable(uint32_t address, int32_t width)
    {
        if (width == 0x400) return readMemory<int32_t>(address);
        if (width == 0x200) return readMemory<int16_t>(address);
        return readMemory<int8_t>(address);
    }
} // namespace EFEHelpers
