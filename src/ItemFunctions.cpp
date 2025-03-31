#include "ItemFunctions.hpp"

#include "Effects.hpp"
#include "Helper.hpp"
#include "ItemEffects.hpp"
#include "Tamer.hpp"
#include "extern/dw1.hpp"

extern "C"
{
    void handleAutopilot(ItemType itemId)
    {
        if (!IS_SCRIPT_PAUSED) return;

        removeTamerItem();
        callScriptSection(0, 1245, 0);
    }

    void handleOffChip(ItemType itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.off, 50, 999);
    }

    void handleDefChip(ItemType itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.def, 50, 999);
    }

    void handleSpeedChip(ItemType itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.speed, 50, 999);
    }

    void handleBrainChip(ItemType itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.brain, 50, 999);
    }

    void handleHPChip(ItemType itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.hp, 500, 9999);
    }

    void handleMPChip(ItemType itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.mp, 500, 9999);
    }

    void handleDVChipA(ItemType itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.off, 100, 999);
        addWithLimit(&PARTNER_ENTITY.stats.brain, 100, 999);
        addLifetime(-24);
        addTamerLevel(10, -1);
    }

    void handleDVChipD(ItemType itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.def, 100, 999);
        addWithLimit(&PARTNER_ENTITY.stats.speed, 100, 999);
        addLifetime(-24);
        addTamerLevel(10, -1);
    }

    void handleDVChipE(ItemType itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.hp, 1000, 9999);
        addWithLimit(&PARTNER_ENTITY.stats.mp, 1000, 9999);
        addLifetime(-24);
        addTamerLevel(10, -1);
    }

    void handlePortaPottyItem(ItemType itemId)
    {
        handlePortaPotty();
    }

    void handleBandage(ItemType itemId)
    {
        if (handleMedicineHealing(100, 20)) addHealingParticleEffect(&PARTNER_ENTITY, 0);
    }

    void handleMedicine(ItemType itemId)
    {
        if (handleMedicineHealing(100, 100)) addHealingParticleEffect(&PARTNER_ENTITY, 0);
    }

    // TODO function for every item
    void handleFood(ItemType itemId)
    {
        struct FoodData
        {
            ItemType itemId;
            int16_t energy;
            int16_t tiredness;
            int16_t happiness;
            int16_t discipline;
            int16_t weight;
            int16_t trainFlag;
            int16_t trainValue;
            int16_t trainDuration;
            int16_t sicknessChance;
            int16_t lifetime;
            int16_t healedHP;
            int16_t healedMP;
            int16_t addedHP;
            int16_t addedMP;
            int16_t addedOffense;
            int16_t addedDefense;
            int16_t addedSpeed;
            int16_t addedBrain;
        };

        FoodData foodData[36] = {
            {
                .itemId         = ItemType::STEAK,
                .energy         = 32,
                .sicknessChance = 20,
                .healedHP       = 1000,
            },
            {
                .itemId   = ItemType::RAIN_PLANT,
                .healedMP = 1000,
            },
            {
                .itemId         = ItemType::CHAIN_MELON,
                .energy         = 50,
                .tiredness      = -50,
                .happiness      = 50,
                .weight         = 3,
                .sicknessChance = 3,
                .lifetime       = 20,
            },
            {
                .itemId         = ItemType::HAPPYMUSHROOM,
                .energy         = 15,
                .tiredness      = -30,
                .happiness      = 30,
                .weight         = 1,
                .sicknessChance = 30,
            },
            {
                .itemId         = ItemType::MOLDY_MEAT,
                .energy         = 30,
                .weight         = 2,
                .sicknessChance = 100,
            },
            {
                .itemId         = ItemType::DIGISEABASS,
                .energy         = 35,
                .weight         = 4,
                .sicknessChance = 20,
                .lifetime       = 3,
                .healedHP       = 9999,
                .healedMP       = 9999,
            },
            {
                .itemId = ItemType::DIGICATFISH,
                .energy = 49,
                .weight = 5,
            },
            {
                .itemId       = ItemType::BLACK_TROUT,
                .energy       = 27,
                .weight       = -2,
                .addedHP      = 10,
                .addedMP      = 10,
                .addedOffense = 1,
                .addedDefense = 1,
                .addedSpeed   = 1,
                .addedBrain   = 1,
            },
            {
                .itemId = ItemType::DIGITROUT,
                .energy = 22,
                .weight = 2,
            },
            {
                .itemId = ItemType::DIGISNAPPER,
                .energy = 12,
                .weight = 1,
            },
            {
                .itemId = ItemType::DIGIANCHOVY,
                .energy = 8,
                .weight = 1,
            },
            {
                .itemId  = ItemType::CALM_BERRY,
                .energy  = 20,
                .weight  = 2,
                .addedMP = 200,
            },
            {
                .itemId  = ItemType::MUSCLE_YAM,
                .energy  = 20,
                .weight  = 2,
                .addedHP = 200,
            },
            {
                .itemId     = ItemType::SAGE_FRUIT,
                .energy     = 20,
                .weight     = 2,
                .addedBrain = 20,
            },
            {
                .itemId     = ItemType::SPEED_LEAF,
                .energy     = 20,
                .weight     = 2,
                .addedSpeed = 20,
            },
            {
                .itemId       = ItemType::POWER_ICE,
                .energy       = 20,
                .weight       = 2,
                .addedDefense = 20,
            },
            {
                .itemId       = ItemType::POWER_FRUIT,
                .energy       = 20,
                .weight       = 2,
                .addedOffense = 20,
            },
            {
                .itemId   = ItemType::ORANGE_BANANA,
                .energy   = 24,
                .weight   = 2,
                .healedHP = 1000,
                .healedMP = 1000,
            },
            {
                .itemId = ItemType::PRICKLYPEAR,
                .energy = 33,
                .weight = -5,
            },
            {
                .itemId   = ItemType::SUPER_VEGGY,
                .energy   = 16,
                .weight   = 2,
                .healedMP = 9999,
            },
            {
                .itemId   = ItemType::SWEET_NUT,
                .energy   = 20,
                .weight   = 2,
                .healedHP = 9999,
            },
            {
                .itemId = ItemType::BIG_BERRY,
                .energy = 100,
                .weight = 10,
            },
            {
                .itemId = ItemType::GOLD_ACORN,
                .energy = 40,
                .weight = 4,
            },
            {
                .itemId     = ItemType::RED_BERRY,
                .energy     = 25,
                .tiredness  = -20,
                .happiness  = 20,
                .discipline = 20,
                .weight     = 2,
            },
            {
                .itemId    = ItemType::BLUE_APPLE,
                .energy    = 30,
                .happiness = 50,
                .weight    = 3,
            },
            {
                .itemId        = ItemType::DIGIPINE,
                .energy        = 22,
                .weight        = 2,
                .trainFlag     = 0x3F,
                .trainValue    = 15,
                .trainDuration = 6,
            },
            {
                .itemId       = ItemType::DELUXE_MUSHROOM,
                .energy       = 38,
                .weight       = 4,
                .addedHP      = 100,
                .addedMP      = 100,
                .addedOffense = 10,
                .addedDefense = 10,
                .addedSpeed   = 10,
                .addedBrain   = 10,
            },
            {
                .itemId     = ItemType::ICE_MUSHROOM,
                .energy     = 19,
                .discipline = 50,
                .weight     = 2,
            },
            {
                .itemId = ItemType::DIGIMUSHROOM,
                .energy = 12,
                .weight = 1,
            },
            {
                .itemId    = ItemType::SPINY_GREEN,
                .energy    = 9,
                .tiredness = -50,
                .weight    = 1,
            },
            {
                .itemId        = ItemType::HAWK_RADISH,
                .energy        = 15,
                .weight        = 3,
                .trainFlag     = 0x16,
                .trainValue    = 12,
                .trainDuration = 6,
            },
            {
                .itemId        = ItemType::SUPERCARROT,
                .energy        = 10,
                .weight        = -2,
                .trainFlag     = 0x29,
                .trainValue    = 12,
                .trainDuration = 6,
            },
            {
                .itemId    = ItemType::SIRLOIN,
                .energy    = 35,
                .tiredness = -5,
                .happiness = 3,
                .weight    = 3,
            },
            {
                .itemId = ItemType::GIANT_MEAT,
                .energy = 24,
                .weight = 2,
            },
            {
                .itemId = ItemType::MEAT,
                .energy = 12,
                .weight = 1,
            },
            {
                .itemId = ItemType::NONE,
            },
        };

        FoodData data = foodData[35];
        for (auto& ptr : foodData)
        {
            if (ptr.itemId == itemId) data = ptr;
        }

        if (getRaiseData(PARTNER_ENTITY.type)->favoriteFood == itemId)
        {
            data.energy = data.energy * 14 / 10;
            data.happiness += 2;
        }

        addEnergy(data.energy);
        addTiredness(-data.tiredness);
        addHappiness(data.happiness);
        addDiscipline(data.discipline);
        addWeight(data.weight);
        decreasePoopLevel();
        setTrainingBoost(data.trainFlag, data.trainValue, data.trainDuration);
        handleItemSickness(data.sicknessChance);
        addLifetime(data.lifetime);

        addWithLimit(&PARTNER_ENTITY.stats.hp, data.addedHP, 9999);
        addWithLimit(&PARTNER_ENTITY.stats.mp, data.addedMP, 9999);
        addWithLimit(&PARTNER_ENTITY.stats.currentHP, data.healedHP, 9999);
        addWithLimit(&PARTNER_ENTITY.stats.currentMP, data.healedMP, 9999);
        addWithLimit(&PARTNER_ENTITY.stats.off, data.addedOffense, 999);
        addWithLimit(&PARTNER_ENTITY.stats.def, data.addedDefense, 999);
        addWithLimit(&PARTNER_ENTITY.stats.speed, data.addedSpeed, 999);
        addWithLimit(&PARTNER_ENTITY.stats.brain, data.addedBrain, 999);
    }

    void fillItemTable()
    {
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::AUTOPILOT)]    = handleAutopilot;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::BANDAGE)]      = handleBandage;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::MEDICINE)]     = handleMedicine;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::OFFENSE_CHIP)] = handleOffChip;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::DEFENSE_CHIP)] = handleDefChip;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::SPEED_CHIP)]   = handleSpeedChip;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::BRAIN_SHIP)]   = handleBrainChip;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::HP_CHIP)]      = handleHPChip;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::MP_CHIP)]      = handleMPChip;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::DV_CHIP_A)]    = handleDVChipA;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::DV_CHIP_D)]    = handleDVChipD;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::DV_CHIP_E)]    = handleDVChipE;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::PORTA_POTTY)]  = handlePortaPottyItem;

        for (uint32_t type = static_cast<uint32_t>(ItemType::MEAT);
             type <= static_cast<uint32_t>(ItemType::CHAIN_MELON);
             type++)
        {
            ITEM_FUNCTIONS[type] = handleFood;
        }
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::RAIN_PLANT)] = handleFood;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::STEAK)]      = handleFood;
    }
}