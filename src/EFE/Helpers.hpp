#pragma once

#include "../extern/dw1.hpp"

// The overlay build supplies the mode-specific entry name.
#ifndef EFE_ENTRY
#define EFE_ENTRY(name) name
#endif

namespace EFEHelpers
{
    template<class T> static inline T readMemory(auto address)
    {
        return *reinterpret_cast<const volatile T*>(address);
    }

    template<class T> static inline void writeMemory(auto address, T value)
    {
        *reinterpret_cast<volatile T*>(address) = value;
    }

    static inline void writeWord(auto address, uint32_t value)
    {
        writeMemory<uint32_t>(address, value);
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

    static inline void writeVariable(uint32_t address, int32_t width, uint32_t value)
    {
        if (width == 0x400)
            writeWord(address, value);
        else if (width == 0x200)
            writeMemory<uint16_t>(address, value);
        else
            writeMemory<uint8_t>(address, value);
    }
} // namespace EFEHelpers
