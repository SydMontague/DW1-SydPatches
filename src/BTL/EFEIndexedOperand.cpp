#include "../EFE/Operands.hpp"

extern "C"
{
    void BTL_loadEFEIndexedVariable()
    {
        EFEOperands::loadIndexedVariable();
    }
}
