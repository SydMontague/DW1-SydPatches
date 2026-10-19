#include "../EFE/Operands.hpp"

extern "C"
{
    void VS_pushEFEVariableAddress()
    {
        EFEOperands::pushAddress();
    }
}
