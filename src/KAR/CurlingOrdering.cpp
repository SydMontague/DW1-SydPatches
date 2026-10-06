#include "Curling.hpp"
#include "CurlingRenderTypes.hpp"

#include "../extern/libgs.hpp"

extern "C"
{
    void KAR_initializeOrderingTables()
    {
        KAR_ORDERING_TABLES[0].length = 5;
        KAR_ORDERING_TABLES[0].origin = KAR_ORDERING_TAGS[0].data();
        KAR_ORDERING_TABLES[1].length = 5;
        KAR_ORDERING_TABLES[1].origin = KAR_ORDERING_TAGS[1].data();
    }
}
