#include "../EFE/Control.hpp"
#include "../extern/BTL.hpp"

extern "C"
{
    void BTL_jumpEFEScript()
    {
        EFEControl::jump();
    }

    void BTL_stopEFEScript()
    {
        EFEControl::stop();
    }

    void BTL_branchEFEOnComparison()
    {
        EFEControl::branch(BTL_EFE_COMPARISONS);
    }

    void BTL_returnFromEFESubroutine()
    {
        EFEControl::returnFromSubroutine();
    }
}
