#include "../EFE/Control.hpp"
#include "../extern/VS.hpp"

extern "C"
{
    void VS_callEFESubroutine()
    {
        EFEControl::callSubroutine();
    }

    int32_t VS_runEFEScript(int16_t* instruction)
    {
        return EFEControl::run(instruction, VS_EFE_PRIMARY_OPCODES);
    }
}
