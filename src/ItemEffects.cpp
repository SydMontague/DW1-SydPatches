#include "Font.hpp"
#include "extern/dw1.hpp"

extern "C"
{
    bool handleMedicineHealing(int32_t injuryHealChance, int32_t sicknessHealChance)
    {
        if (PARTNER_PARA.condition.isInjured && random(3) < injuryHealChance)
        {
            PARTNER_PARA.condition.isInjured = false;
            PARTNER_PARA.injuryTimer         = 0;
        }

        if (PARTNER_PARA.condition.isSick && random(10) < sicknessHealChance)
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

        PARTNER_PARA.poopLevel         = RAISE_DATA[static_cast<uint32_t>(PARTNER_ENTITY.type)].poopTimer;
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
        auto cap       = RAISE_DATA[static_cast<uint32_t>(PARTNER_ENTITY.type)].energyCap;
        auto new_value = PARTNER_PARA.energyLevel + amount;
        if (new_value < 0) new_value = 0;
        if (new_value > cap) new_value = cap;
        PARTNER_PARA.energyLevel = new_value;
    }

    void addTiredness(int32_t amount)
    {
        auto new_value = PARTNER_PARA.tiredness + amount;
        if (new_value < 0) new_value = 0;
        if (new_value > 100) new_value = 100;
        PARTNER_PARA.tiredness = new_value;
    }

    void reduceTiredness(int32_t amount)
    {
        addTiredness(-amount);
    }

    void addHappiness(int32_t amount)
    {
        auto new_value = PARTNER_PARA.happiness + amount;
        if (new_value < -100) new_value = -100;
        if (new_value > 100) new_value = 100;
        PARTNER_PARA.happiness = new_value;
    }

    void addDiscipline(int32_t amount)
    {
        auto new_value = PARTNER_PARA.discipline + amount;
        if (new_value < 0) new_value = 0;
        if (new_value > 100) new_value = 100;
        PARTNER_PARA.discipline = new_value;
    }

    void addWeight(int32_t amount)
    {
        auto new_weight = PARTNER_PARA.weight + amount;
        if (new_weight < 1) new_weight = 1;
        if (new_weight > 99) new_weight = 99;
        PARTNER_PARA.weight = new_weight;
    }

    void decreasePoopLevel()
    {
        PARTNER_PARA.poopLevel--;
    }

    void setTrainingBoost(uint16_t flag, uint16_t value, uint16_t duration)
    {
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
        uint8_t buffer[40];
        sprintf(buffer, "%s is sick", PARTNER_ENTITY.name);
        drawStringNew(&vanillaFont, buffer, 704, 256 + 0x78);
    }
}