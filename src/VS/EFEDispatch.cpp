#include "../EFE/Control.hpp"
#include "../extern/VS.hpp"

extern "C"
{
    void VS_initializeEFESubOpcodeTable()
    {
        EFEControl::initializeSubOpcodes(VS_EFE_SUBOPCODE_ENTRIES, VS_EFE_SUBOPCODES);
    }

    void VS_dispatchEFEOpcode(int32_t opcode)
    {
        EFEControl::dispatchOpcode(opcode, VS_EFE_PRIMARY_OPCODES);
    }

    void VS_dispatchEFESubOpcode()
    {
        EFEControl::dispatchSubOpcode(VS_EFE_SUBOPCODES);
    }
}
