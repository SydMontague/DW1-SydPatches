#pragma once

#include "Helpers.hpp"

namespace EFEControl
{
    using EFEHelpers::advanceInstruction;
    using EFEHelpers::getWord;
    using EFEHelpers::readMemory;
    using EFEHelpers::readVariable;
    using EFEHelpers::variableAddress;

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

    inline void branch(const dtl::array<EFEComparison, 6>& comparisons)
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

    inline void dispatchOpcode(int32_t opcode, const dtl::array<EFEOpcodeHandler, 18>& handlers)
    {
        handlers[opcode]();
    }

    inline void dispatchSubOpcode(const dtl::array<EFEOpcodeHandler, 97>& handlers)
    {
        advanceInstruction(2);
        int32_t opcode = EFE_CURRENT_OPCODE;
        handlers[opcode >> 8]();
    }

    inline void initializeSubOpcodes(const dtl::array<EFEOpcodeEntry, 97>& entries,
                                     dtl::array<EFEOpcodeHandler, 97>& handlers)
    {
        for (const volatile auto& entry : entries) {
            uint32_t opcode = entry.opcode;
            // Keep malformed descriptors from writing outside the dispatch table.
            if (opcode >= handlers.size()) return;
            handlers[opcode] = entry.handler;
        }
    }

    inline int32_t run(int16_t* instruction, const dtl::array<EFEOpcodeHandler, 18>& handlers)
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
