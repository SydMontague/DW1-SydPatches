#include "extern/dw1.hpp"

extern "C"
{
    int32_t getNumMasteredMoves();

    DigimonType getFreshEvolutionTarget(DigimonType type);
    DigimonType getInTrainingEvolutionTarget(DigimonType current);
    DigimonType getRegularEvolutionTarget(DigimonType current);
    DigimonType handleSpecialEvolutionPraise(int32_t mode, PartnerEntity* entity);
    DigimonType handleSpecialEvolutionSleep(int32_t mode, PartnerEntity* entity);
}