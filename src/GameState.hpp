#include "extern/dw1.hpp"

extern "C"
{
    void setDigimonRaised(DigimonType type);
}

bool hasDigimonRaised(DigimonType type);
bool hasMedal(Medal medal);
void unlockMedal(Medal medal);
