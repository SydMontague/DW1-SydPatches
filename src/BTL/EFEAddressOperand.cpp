#include "../EFE/Operands.hpp"

extern "C"
{
    void BTL_pushEFEVariableAddress()
    {
        EFEOperands::pushAddress();
    }
}
