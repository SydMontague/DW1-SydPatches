#include "../EFE/Control.hpp"
#include "../extern/VS.hpp"

extern "C"
{
    void VS_jumpEFEScript()
    {
        EFEControl::jump();
    }

    void VS_stopEFEScript()
    {
        EFEControl::stop();
    }

    void VS_branchEFEOnComparison()
    {
        EFEControl::branch(VS_EFE_COMPARISONS);
    }

    void VS_returnFromEFESubroutine()
    {
        EFEControl::returnFromSubroutine();
    }
}
