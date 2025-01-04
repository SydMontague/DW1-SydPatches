#include "extern/dw1.hpp"

extern "C"
{
    bool handleMedicineHealing(int32_t injuryHealChance, int32_t sicknessHealChance);
    void handlePortaPotty();
    void addLifetime(int32_t amount);
    void addEnergy(int32_t amount);
    void addTiredness(int32_t amount);
    void reduceTiredness(int32_t amount);
    void addHappiness(int32_t amount);
    void addDiscipline(int32_t amount);
    void addWeight(int32_t amount);
    void setTrainingBoost(uint16_t flag, uint16_t value, uint16_t duration);
    void handleItemSickness(int32_t chance);
    void decreasePoopLevel();
}