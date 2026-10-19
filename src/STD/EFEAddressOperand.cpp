#include "../EFE/Operands.hpp"

extern "C"
{
    void STD_pushEFEVariableAddress()
    {
        EFEOperands::pushAddress();
    }
}
