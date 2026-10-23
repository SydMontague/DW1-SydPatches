#include "../EFE/Control.hpp"
#include "../extern/STD.hpp"

extern "C"
{
    void STD_initializeEFESubOpcodeTable()
    {
        EFEControl::initializeSubOpcodes(STD_EFE_SUBOPCODE_ENTRIES, STD_EFE_SUBOPCODES);
    }

    void STD_dispatchEFEOpcode(int32_t opcode)
    {
        EFEControl::dispatchOpcode(opcode, STD_EFE_PRIMARY_OPCODES);
    }

    void STD_dispatchEFESubOpcode()
    {
        EFEControl::dispatchSubOpcode(STD_EFE_SUBOPCODES);
    }
}
