#pragma once
#include "dw1.hpp"

struct TournamentConfig
{
    int8_t id;
    dtl::array<uint8_t, 7> participants;
};

extern "C"
{
    extern EFEOpcodeHandler const STD_EFE_PRIMARY_OPCODES[18];
    extern EFEOpcodeEntry const STD_EFE_SUBOPCODE_ENTRIES[97];
    extern EFEOpcodeHandler STD_EFE_SUBOPCODES[97];
    extern EFEComparison const STD_EFE_COMPARISONS[6];

    void STD_Tamer_tickTournament(int32_t instanceId);
    void STD_NPCEntity_tickTournament(int32_t instanceId);
    void STD_Partner_tickTournament(int32_t instanceId);
    int16_t STD_tournamentMain(TournamentConfig* config);
}
