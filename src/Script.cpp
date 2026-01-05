
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

    void checkTournamentMedalConditions() {
        // in vanilla this is leftover code that is duplicated elsewhere
        // removing this fixes the ability to get the 100 tournament won medal by winning just 100 battles
        return;
    }
    
    void scriptCheckTournamentMedal()
    {
        ACTIVE_INSTRUCTION = 0;
        // in vanilla this is leftover code that is duplicated elsewhere
        // removing this fixes the ability to get the 100 tournament won medal by winning just 100 battles
        return;
    }
}