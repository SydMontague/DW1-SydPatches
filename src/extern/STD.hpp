#include "stddef.hpp"

struct TournamentConfig
{
    int8_t id;
    dtl::array<uint8_t, 7> participants;
};

extern "C"
{
    void STD_Tamer_tickTournament(int32_t instanceId);
    void STD_NPCEntity_tickTournament(int32_t instanceId);
    void STD_Partner_tickTournament(int32_t instanceId);
    int16_t STD_tournamentMain(TournamentConfig* config);
}