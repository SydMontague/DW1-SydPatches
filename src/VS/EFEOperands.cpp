#include "../EFE/Operands.hpp"

extern "C"
{
    void VS_loadEFEImmediate()
    {
        EFEOperands::loadImmediate();
    }

    void VS_loadEFEVariable()
    {
        EFEOperands::loadVariable();
    }

    void VS_loadEFERandomValue()
    {
        EFEOperands::loadRandom();
    }

    void VS_pushEFEImmediate()
    {
        EFEOperands::pushImmediate();
    }

    void VS_pushEFEVariable()
    {
        EFEOperands::pushVariable();
    }

    void VS_popEFEValueToVariable()
    {
        EFEOperands::popToVariable();
    }
}
