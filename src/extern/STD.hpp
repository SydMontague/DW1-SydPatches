#pragma once
#include "dw1.hpp"

struct TournamentConfig
{
    int8_t id;
    dtl::array<uint8_t, 7> participants;
};

extern "C"
{
    extern const dtl::array<EFEOpcodeHandler, 18> STD_EFE_PRIMARY_OPCODES;
    extern const dtl::array<EFEOpcodeEntry, 97> STD_EFE_SUBOPCODE_ENTRIES;
    extern dtl::array<EFEOpcodeHandler, 97> STD_EFE_SUBOPCODES;
    extern const dtl::array<EFEComparison, 6> STD_EFE_COMPARISONS;

    void STD_Tamer_tickTournament(int32_t instanceId);
    void STD_NPCEntity_tickTournament(int32_t instanceId);
    void STD_Partner_tickTournament(int32_t instanceId);
    int16_t STD_tournamentMain(TournamentConfig* config);
}
