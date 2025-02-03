#include "Font.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "Partner.hpp"
#include "Tamer.hpp"
#include "constants.hpp"
#include "extern/dw1.hpp"

extern "C"
{
    bool handleMedicineHealing(int32_t injuryHealChance, int32_t sicknessHealChance)
    {
        // vanilla does one roll for each condition and also uses 3 and 10 respectively instead of 100
        // however, this change makes it compatible with poop eating healing
        auto roll = random(100);
        if (PARTNER_PARA.condition.isInjured && roll < injuryHealChance)
        {
            PARTNER_PARA.condition.isInjured = false;
            PARTNER_PARA.injuryTimer         = 0;
        }

        if (PARTNER_PARA.condition.isSick && roll < sicknessHealChance)
        {
            PARTNER_PARA.condition.isSick = false;
            PARTNER_PARA.areaEffectTimer  = 0;
            PARTNER_PARA.sicknessTimer    = 0;
            return true;
        }

        return false;
    }

    void handlePortaPotty()
    {
        if (!PARTNER_PARA.condition.isPoopy) return;

        // vanilla doesn't double the timer here
        PARTNER_PARA.poopLevel         = getRaiseData(PARTNER_ENTITY.type)->poopTimer * 2;
        PARTNER_PARA.condition.isPoopy = false;
        handlePoopWeightLoss(PARTNER_ENTITY.type);
    }

    void addLifetime(int32_t amount)
    {
        auto new_value = PARTNER_PARA.remainingLifetime + amount;
        if (new_value < 0) new_value = 0;
        PARTNER_PARA.remainingLifetime = new_value;
    }

    void addEnergy(int32_t amount)
    {
        auto cap       = getRaiseData(PARTNER_ENTITY.type)->energyCap;
        auto new_value = PARTNER_PARA.energyLevel + amount;
        if (new_value < ENERGY_MIN) new_value = ENERGY_MIN;
        if (new_value > cap) new_value = cap;
        PARTNER_PARA.energyLevel = new_value;
    }

    void addTiredness(int32_t amount)
    {
        auto new_value = PARTNER_PARA.tiredness + amount;
        if (new_value < TIREDNESS_MIN) new_value = TIREDNESS_MIN;
        if (new_value > TIREDNESS_MAX) new_value = TIREDNESS_MAX;
        PARTNER_PARA.tiredness = new_value;
    }

    void reduceTiredness(int32_t amount)
    {
        addTiredness(-amount);
    }

    void addHappiness(int32_t amount)
    {
        auto new_value = PARTNER_PARA.happiness + amount;
        if (new_value < HAPPINESS_MIN) new_value = HAPPINESS_MIN;
        if (new_value > HAPPINESS_MAX) new_value = HAPPINESS_MAX;
        PARTNER_PARA.happiness = new_value;
    }

    void addDiscipline(int32_t amount)
    {
        auto new_value = PARTNER_PARA.discipline + amount;
        if (new_value < DISCIPLINE_MIN) new_value = DISCIPLINE_MIN;
        if (new_value > DISCIPLINE_MAX) new_value = DISCIPLINE_MAX;
        PARTNER_PARA.discipline = new_value;
    }

    void addWeight(int32_t amount)
    {
        auto new_weight = PARTNER_PARA.weight + amount;
        if (new_weight < WEIGHT_MIN) new_weight = WEIGHT_MIN;
        if (new_weight > WEIGHT_MAX) new_weight = WEIGHT_MAX;
        PARTNER_PARA.weight = new_weight;
    }

    void decreasePoopLevel()
    {
        PARTNER_PARA.poopLevel--;
    }

    void setTrainingBoost(uint16_t flag, uint16_t value, uint16_t duration)
    {
        // FIX: prevent non-boost items from clearing the boost
        if (flag == 0) return;

        PARTNER_PARA.trainBoostFlag  = flag;
        PARTNER_PARA.trainBoostValue = value;
        PARTNER_PARA.trainBoostTimer = duration * 1200;
    }

    void handleItemSickness(int32_t chance)
    {
        auto roll = random(100);
        if (roll >= chance) return;
        if (PARTNER_PARA.condition.isSick) return;

        PARTNER_PARA.timesBeingSick++;
        PARTNER_PARA.sicknessTimer = 1;
        if (PARTNER_PARA.condition.isInjured)
        {
            PARTNER_PARA.condition.isInjured = false;
            PARTNER_PARA.injuryTimer         = 0;
        }
        PARTNER_PARA.condition.isSick = true;
        Tamer_setState(20);
        clearTextArea();
        setTextColor(10);
        auto width = drawStringNew(&vanillaFont, PARTNER_ENTITY.name, 704, 256 + 0x78);
        setTextColor(1);
        drawStringNew(&vanillaFont, reinterpret_cast<const uint8_t*>("is sick"), 704 + width, 256 + 0x78);
    }
}