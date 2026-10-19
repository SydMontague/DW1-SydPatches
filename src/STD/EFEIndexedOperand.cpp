#include "../EFE/Operands.hpp"

extern "C"
{
    void STD_loadEFEIndexedVariable()
    {
        EFEOperands::loadIndexedVariable();
    }
}
