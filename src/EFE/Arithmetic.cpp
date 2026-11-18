#include "../extern/dtl/utility.hpp"
#include "Helpers.hpp"

namespace
{
    uint32_t calculateArithmeticResult(int32_t value, int32_t operand, uint32_t operation)
    {
        switch (operation) {
            case 0: return operand;
            case 1: return static_cast<uint32_t>(value) + operand;
            case 2: return static_cast<uint32_t>(value) - operand;
            case 3: return static_cast<uint32_t>(value) * operand;
            case 4:
                // Vanilla scripts also divide by zero; retain the native quotient.
                if (operand == 0) return value < 0 ? 1 : -1;
                return value / operand;
            case 5:
                if (operand == 0) return value;
                return value % operand;
            case 6: return static_cast<uint32_t>(value) << (static_cast<uint32_t>(operand) & 31);
            case 7: return value >> (static_cast<uint32_t>(operand) & 31);
        }
        dtl::unreachable();
    }
} // namespace

extern "C" void EFE_applyVariableOperator()
{
    auto opcode    = EFE_CURRENT_OPCODE;
    auto width     = opcode & 0xF00;
    auto operation = opcode >> 12;
    auto offset    = EFEHelpers::getWord(1);
    auto address   = EFEHelpers::variableAddress(offset, EFEHelpers::getWord(2) == 0);
    auto value     = EFEHelpers::readVariable(address, width);
    auto operand   = EFE_SCRIPT_REGISTER;

    // Halfword operations use the low signed halfword. Byte division and remainder use the full register.
    if (width == 0x200) operand = static_cast<int16_t>(operand);
    auto result = calculateArithmeticResult(value, operand, operation);

    EFEHelpers::writeVariable(address, width, result);
    EFEHelpers::advanceInstruction(6);
}
