#include "../EFE/Control.hpp"
#include "../extern/BTL.hpp"

extern "C"
{
    void BTL_initializeEFESubOpcodeTable()
    {
        EFEControl::initializeSubOpcodes(BTL_EFE_SUBOPCODE_ENTRIES, BTL_EFE_SUBOPCODES);
    }

    void BTL_dispatchEFEOpcode(int32_t opcode)
    {
        EFEControl::dispatchOpcode(opcode, BTL_EFE_PRIMARY_OPCODES);
    }

    void BTL_dispatchEFESubOpcode()
    {
        EFEControl::dispatchSubOpcode(BTL_EFE_SUBOPCODES);
    }
}
