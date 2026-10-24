#pragma once

#include "Operands.hpp"

namespace EFEControl
{
    using EFEOperands::advanceInstruction;
    using EFEOperands::getWord;
    using EFEOperands::readMemory;
    using EFEOperands::readVariable;
    using EFEOperands::variableAddress;

    // Vanilla engine callers share this mixed-word return stack between callbacks.
    template<class T> static inline void pushReturn(T value)
    {
        static_assert(sizeof(T) == sizeof(uint32_t));
        auto* destination                           = EFE_RETURN_STACK++;
        *reinterpret_cast<volatile T*>(destination) = value;
    }

    template<class T> static inline T popReturn()
    {
        static_assert(sizeof(T) == sizeof(uint32_t));
        return readMemory<T>(--EFE_RETURN_STACK);
    }

    static inline int16_t* scriptInstruction(int32_t offset)
    {
        return reinterpret_cast<int16_t*>(variableAddress(offset, false));
    }

    inline void callSubroutine()
    {
        auto offset = getWord(1);
        pushReturn(EFE_TARGET_INDEX);
        pushReturn(EFE_CURRENT_SUBEFFECT);
        pushReturn(EFE_INSTRUCTION_POINTER + 3);
        EFE_INSTRUCTION_POINTER = scriptInstruction(offset);
    }

    inline void returnFromSubroutine()
    {
        EFE_INSTRUCTION_POINTER = popReturn<int16_t*>();
        if (EFE_INSTRUCTION_POINTER == nullptr) return;

        EFE_CURRENT_SUBEFFECT = popReturn<EFESubEffect*>();
        EFE_INSTANCE_BASE     = reinterpret_cast<uint8_t*>(EFE_CURRENT_SUBEFFECT->instance);
        EFE_PARENT_INSTANCE   = EFE_CURRENT_SUBEFFECT->parentInstance;
        EFE_TARGET_INDEX      = popReturn<int32_t>();
    }

    inline void jump()
    {
        EFE_INSTRUCTION_POINTER = scriptInstruction(getWord(1));
    }

    inline void stop()
    {
        EFE_INSTRUCTION_POINTER = nullptr;
        EFE_CURRENT_OPCODE &= 0xFF;
    }

    inline void branch(const EFEComparison* comparisons)
    {
        int32_t opcode = EFE_CURRENT_OPCODE;
        auto offset    = getWord(1);
        auto address   = variableAddress(offset, getWord(2) == 0);
        auto value     = readVariable(address, opcode & 0xF00);

        if (comparisons[opcode >> 12](value) == 0)
            EFE_INSTRUCTION_POINTER = scriptInstruction(getWord(3));
        else
            advanceInstruction(10);
    }

    inline void dispatchOpcode(int32_t opcode, const EFEOpcodeHandler* handlers)
    {
        handlers[opcode]();
    }

    inline void dispatchSubOpcode(const EFEOpcodeHandler* handlers)
    {
        advanceInstruction(2);
        int32_t opcode = EFE_CURRENT_OPCODE;
        handlers[opcode >> 8]();
    }

    inline void initializeSubOpcodes(const EFEOpcodeEntry* entries, EFEOpcodeHandler* handlers)
    {
        for (uint32_t i = 0; i < 97; ++i) {
            const volatile auto& entry = entries[i];
            if (entry.opcode >= 97) libc_exit(1);
            // libc_exit is an ordinary service; preserve these reads if it returns.
            auto handler           = entry.handler;
            handlers[entry.opcode] = handler;
        }
    }

    inline int32_t run(int16_t* instruction, const EFEOpcodeHandler* handlers)
    {
        EFE_DATA_STACK          = EFE_SCRIPT_MEM1_DATA;
        EFE_RETURN_STACK        = EFE_RETURN_STACK_BASE;
        EFE_INSTRUCTION_POINTER = instruction;
        pushReturn(static_cast<int16_t*>(nullptr));

        while (EFE_INSTRUCTION_POINTER != nullptr) {
            EFE_CURRENT_OPCODE = getWord(0);
            dispatchOpcode(EFE_CURRENT_OPCODE & 0xFF, handlers);
        }

        auto* sub = EFE_CURRENT_SUBEFFECT;
        if (sub->instance->frame == -1) sub->tickInstruction = nullptr;
        return EFE_SCRIPT_RESULT;
    }
} // namespace EFEControl
