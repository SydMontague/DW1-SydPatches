#include "../EFE/Operands.hpp"

extern "C"
{
    void VS_loadEFEIndexedVariable()
    {
        EFEOperands::loadIndexedVariable();
    }
}
