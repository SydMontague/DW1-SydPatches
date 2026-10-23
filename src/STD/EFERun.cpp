#include "../EFE/Control.hpp"
#include "../extern/STD.hpp"

extern "C"
{
    void STD_callEFESubroutine()
    {
        EFEControl::callSubroutine();
    }

    int32_t STD_runEFEScript(int16_t* instruction)
    {
        return EFEControl::run(instruction, STD_EFE_PRIMARY_OPCODES);
    }
}
