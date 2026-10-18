
#include "extern/dw1.hpp"

extern "C"
{
    void setDigimonRaised(DigimonType type)
    {
        if (type >= DigimonType::PANJYAMON) return;

        setTrigger(0x200 + static_cast<uint32_t>(type));
    }
}

bool hasDigimonRaised(DigimonType type)
{
    if (type >= DigimonType::PANJYAMON) return false;

    return isTriggerSet(0x200 + static_cast<uint32_t>(type));
}

void unlockMedal(Medal medal)
{
    if (medal >= Medal::UNUSED5) return;

    setTrigger(364 + static_cast<uint32_t>(medal));
}

bool hasMedal(Medal medal)
{
    if (medal >= Medal::UNUSED5) return false;

    return isTriggerSet(364 + static_cast<uint32_t>(medal));
}
