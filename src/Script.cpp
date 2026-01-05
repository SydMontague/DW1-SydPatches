
#include "Tournament.hpp"
#include "extern/dw1.hpp"

extern "C"
{
    int32_t getScriptSyncBit()
    {
        return SOME_SCRIPT_SYNC_BIT;
    }

    void scriptStartTournament()
    {
        if (SCRIPT_STATE_2 == 3) ACTIVE_INSTRUCTION = 0;
        if (SCRIPT_STATE_2 == 2)
        {
            closeBox(0);
            startTournament();
            ACTIVE_INSTRUCTION = 0;
        }
        if (SCRIPT_STATE_2 == 1) return;
        if (SCRIPT_STATE_2 == 0) SCRIPT_STATE_2 = 2;
    }
}