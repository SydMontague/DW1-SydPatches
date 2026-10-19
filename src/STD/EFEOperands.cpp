#include "../EFE/Operands.hpp"

extern "C"
{
    void STD_loadEFEImmediate()
    {
        EFEOperands::loadImmediate();
    }

    void STD_loadEFEVariable()
    {
        EFEOperands::loadVariable();
    }

    void STD_loadEFERandomValue()
    {
        EFEOperands::loadRandom();
    }

    void STD_pushEFEImmediate()
    {
        EFEOperands::pushImmediate();
    }

    void STD_pushEFEVariable()
    {
        EFEOperands::pushVariable();
    }

    void STD_popEFEValueToVariable()
    {
        EFEOperands::popToVariable();
    }
}
