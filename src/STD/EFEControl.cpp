#include "../EFE/Control.hpp"
#include "../extern/STD.hpp"

extern "C"
{
    void STD_jumpEFEScript()
    {
        EFEControl::jump();
    }

    void STD_stopEFEScript()
    {
        EFEControl::stop();
    }

    void STD_branchEFEOnComparison()
    {
        EFEControl::branch(STD_EFE_COMPARISONS);
    }

    void STD_returnFromEFESubroutine()
    {
        EFEControl::returnFromSubroutine();
    }
}
