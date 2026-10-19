#include "../EFE/Operands.hpp"

extern "C"
{
    void BTL_loadEFEImmediate()
    {
        EFEOperands::loadImmediate();
    }

    void BTL_loadEFEVariable()
    {
        EFEOperands::loadVariable();
    }

    void BTL_loadEFERandomValue()
    {
        EFEOperands::loadRandom();
    }

    void BTL_pushEFEImmediate()
    {
        EFEOperands::pushImmediate();
    }

    void BTL_pushEFEVariable()
    {
        EFEOperands::pushVariable();
    }

    void BTL_popEFEValueToVariable()
    {
        EFEOperands::popToVariable();
    }
}
