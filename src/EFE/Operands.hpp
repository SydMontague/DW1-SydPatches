#pragma once

#include "../extern/libc.hpp"
#include "Helpers.hpp"

namespace EFEOperands
{
    using EFEHelpers::advanceInstruction;
    using EFEHelpers::getWord;
    using EFEHelpers::readMemory;
    using EFEHelpers::readVariable;
    using EFEHelpers::variableAddress;
    using EFEHelpers::writeWord;

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
