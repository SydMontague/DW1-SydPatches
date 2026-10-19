#pragma once

#include "../extern/dw1.hpp"
#include "../extern/libc.hpp"

namespace EFEOperands
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

    static inline void push(uint32_t value)
    {
        auto* destination = EFE_DATA_STACK;
        EFE_DATA_STACK    = destination + 4;
        writeWord(destination, value);
    }

    static inline int32_t pop()
    {
        auto* source   = EFE_DATA_STACK - 4;
        EFE_DATA_STACK = source;
        return readMemory<int32_t>(source);
    }

    // setupLoadedEFEFile runs up to four instructions per callback, then returns.
    // Its instruction pointer and stack must persist between callback invocations.
    inline void loadImmediate()
    {
        EFE_SCRIPT_REGISTER = getWord(1);
        advanceInstruction(6);
    }

    inline void loadVariable()
    {
        auto width   = EFE_CURRENT_OPCODE & 0xF00;
        auto offset  = getWord(1);
        auto address = variableAddress(offset, getWord(2) == 0);

        EFE_SCRIPT_REGISTER = readVariable(address, width);
        advanceInstruction(6);
    }

    inline void loadRandom()
    {
        EFE_SCRIPT_REGISTER = rand();
        advanceInstruction(2);
    }

    inline void loadIndexedVariable()
    {
        auto opcode      = EFE_CURRENT_OPCODE;
        auto indexOffset = getWord(3);
        auto indexSource = variableAddress(indexOffset, getWord(4) == 0);
        auto index       = readVariable(indexSource, (opcode & 0xF000) >> 4);
        auto width       = opcode & 0xF00;
        auto stride      = 1;
        if (width == 0x400)
            stride = 4;
        else if (width == 0x200)
            stride = 2;

        // Supported US scripts keep this signed byte offset within [-65480, 262328].
        auto offset  = getWord(1) + index * stride;
        auto address = variableAddress(offset, false);

        EFE_SCRIPT_REGISTER = readVariable(address, width);
        advanceInstruction(10);
    }

    inline void pushImmediate()
    {
        push(getWord(1));
        advanceInstruction(6);
    }

    inline void pushVariable()
    {
        auto offset  = getWord(1);
        auto address = variableAddress(offset, getWord(2) == 0);
        auto width   = EFE_CURRENT_OPCODE & 0xF00;

        push(readVariable(address, width));
        advanceInstruction(6);
    }

    inline void pushAddress()
    {
        auto offset  = getWord(1);
        auto address = variableAddress(offset, getWord(2) == 0);

        push(address);
        advanceInstruction(6);
    }

    inline void popToVariable()
    {
        auto offset  = getWord(1);
        auto address = variableAddress(offset, getWord(2) == 0);
        auto value   = pop();

        writeWord(address, value);
        advanceInstruction(6);
    }
} // namespace EFEOperands
