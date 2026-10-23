#include "../EFE/Control.hpp"
#include "../extern/BTL.hpp"

extern "C"
{
    void BTL_callEFESubroutine()
    {
        EFEControl::callSubroutine();
    }

    int32_t BTL_runEFEScript(int16_t* instruction)
    {
        return EFEControl::run(instruction, BTL_EFE_PRIMARY_OPCODES);
    }
}
