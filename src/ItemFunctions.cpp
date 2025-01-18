#include "ItemFunctions.hpp"

#include "extern/dw1.hpp"
#include "ItemEffects.hpp"
#include "Helper.hpp"

extern "C"
{

    void handleAutopilot(int32_t itemId)
    {
        if (!IS_SCRIPT_PAUSED) return;

        removeTamerItem();
        callScriptSection(0, 1245, 0);
    }

    void handleOffChip(int32_t itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.off, 50, 999);
    }

    void handleDefChip(int32_t itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.def, 50, 999);
    }

    void handleSpeedChip(int32_t itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.speed, 50, 999);
    }

    void handleBrainChip(int32_t itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.brain, 50, 999);
    }

    void handleHPChip(int32_t itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.hp, 500, 9999);
    }

    void handleMPChip(int32_t itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.mp, 500, 9999);
    }

    void handleDVChipA(int32_t itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.off, 100, 999);
        addWithLimit(&PARTNER_ENTITY.stats.brain, 100, 999);
        addLifetime(-24);
        addTamerLevel(10, -1);
    }

    void handleDVChipD(int32_t itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.def, 100, 999);
        addWithLimit(&PARTNER_ENTITY.stats.speed, 100, 999);
        addLifetime(-24);
        addTamerLevel(10, -1);
    }

    void handleDVChipE(int32_t itemId)
    {
        addWithLimit(&PARTNER_ENTITY.stats.hp, 1000, 9999);
        addWithLimit(&PARTNER_ENTITY.stats.mp, 1000, 9999);
        addLifetime(-24);
        addTamerLevel(10, -1);
    }

    void handlePortaPottyItem(int32_t itemId)
    {
        handlePortaPotty();
    }

    void handleBandage(int32_t itemId)
    {
        if (handleMedicineHealing(3, 2)) addHealingParticleEffect(&PARTNER_ENTITY, 0);
    }

    void handleMedicine(int32_t itemId)
    {
        if (handleMedicineHealing(3, 10)) addHealingParticleEffect(&PARTNER_ENTITY, 0);
    }

    // TODO function for every item
    void handleFood(int32_t itemId)
    {
        struct FoodData
        {
            uint8_t itemId;
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
                // Steak
                .itemId         = 0x7a,
                .energy         = 32,
                .sicknessChance = 20,
                .healedHP       = 1000,
            },
            {
                // Rain Plant
                .itemId   = 0x79,
                .healedMP = 1000,
            },
            {
                // Chain Melon
                .itemId         = 0x46,
                .energy         = 50,
                .tiredness      = -50,
                .happiness      = 50,
                .weight         = 3,
                .sicknessChance = 3,
                .lifetime       = 20,
            },
            {
                // HappyMushroom
                .itemId         = 0x45,
                .energy         = 15,
                .tiredness      = -30,
                .happiness      = 30,
                .weight         = 1,
                .sicknessChance = 30,
            },
            {
                // Moldy Meat
                .itemId         = 0x44,
                .energy         = 30,
                .weight         = 2,
                .sicknessChance = 100,
            },
            {
                // DigiSeabass
                .itemId         = 0x43,
                .energy         = 35,
                .weight         = 4,
                .sicknessChance = 20,
                .lifetime       = 3,
                .healedHP       = 9999,
                .healedMP       = 9999,
            },
            {
                // Digicatfish
                .itemId = 0x42,
                .energy = 49,
                .weight = 5,
            },
            {
                // Black Trout
                .itemId       = 0x41,
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
                // DigiTrout
                .itemId = 0x40,
                .energy = 22,
                .weight = 2,
            },
            {
                // Digisnapper
                .itemId = 0x3f,
                .energy = 12,
                .weight = 1,
            },
            {
                // DigiAnchovy
                .itemId = 0x3e,
                .energy = 8,
                .weight = 1,
            },
            {
                // Calm Berry
                .itemId  = 0x3d,
                .energy  = 20,
                .weight  = 2,
                .addedMP = 200,
            },
            {
                // Muscle Yam
                .itemId  = 0x3c,
                .energy  = 20,
                .weight  = 2,
                .addedHP = 200,
            },
            {
                // Sage Fruit
                .itemId     = 0x3b,
                .energy     = 20,
                .weight     = 2,
                .addedBrain = 20,
            },
            {
                // Speed Leaf
                .itemId     = 0x3a,
                .energy     = 20,
                .weight     = 2,
                .addedSpeed = 20,
            },
            {
                // Power Ice
                .itemId       = 0x39,
                .energy       = 20,
                .weight       = 2,
                .addedDefense = 20,
            },
            {
                // Power Fruit
                .itemId       = 0x38,
                .energy       = 20,
                .weight       = 2,
                .addedOffense = 20,
            },
            {
                // Orange bana
                .itemId   = 0x37,
                .energy   = 24,
                .weight   = 2,
                .healedHP = 1000,
                .healedMP = 1000,
            },
            {
                // Pricklypear
                .itemId = 0x36,
                .energy = 33,
                .weight = -5,
            },
            {
                // Super veggy
                .itemId   = 0x35,
                .energy   = 16,
                .weight   = 2,
                .healedMP = 9999,
            },
            {
                // Sweet Nut
                .itemId   = 0x34,
                .energy   = 20,
                .weight   = 2,
                .healedHP = 9999,
            },
            {
                // Big Berry
                .itemId = 0x33,
                .energy = 100,
                .weight = 10,
            },
            {
                // Gold Acorn
                .itemId = 0x32,
                .energy = 40,
                .weight = 4,
            },
            {
                // Red Berry
                .itemId     = 0x31,
                .energy     = 25,
                .tiredness  = -20,
                .happiness  = 20,
                .discipline = 20,
                .weight     = 2,
            },
            {
                // Blue Apple
                .itemId    = 0x30,
                .energy    = 30,
                .happiness = 50,
                .weight    = 3,
            },
            {
                // Digipine
                .itemId        = 0x2f,
                .energy        = 22,
                .weight        = 2,
                .trainFlag     = 0x3F,
                .trainValue    = 15,
                .trainDuration = 6,
            },
            {
                // Black Trout
                .itemId       = 0x2e,
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
                // Ice Mushroom
                .itemId     = 0x2d,
                .energy     = 19,
                .discipline = 50,
                .weight     = 2,
            },
            {
                // Digimushroom
                .itemId = 0x2c,
                .energy = 12,
                .weight = 1,
            },
            {
                // Spiny green
                .itemId    = 0x2b,
                .energy    = 9,
                .tiredness = -50,
                .weight    = 1,
            },
            {
                // Hawk radish
                .itemId        = 0x2a,
                .energy        = 15,
                .weight        = 3,
                .trainFlag     = 0x16,
                .trainValue    = 12,
                .trainDuration = 6,
            },
            {
                // Supercarrot
                .itemId        = 0x29,
                .energy        = 10,
                .weight        = -2,
                .trainFlag     = 0x29,
                .trainValue    = 12,
                .trainDuration = 6,
            },
            {
                // Sirloin
                .itemId    = 0x28,
                .energy    = 35,
                .tiredness = -5,
                .happiness = 3,
                .weight    = 3,
            },
            {
                // Giant Meat
                .itemId = 0x27,
                .energy = 24,
                .weight = 2,
            },
            {
                // Meat
                .itemId = 0x26,
                .energy = 12,
                .weight = 1,
            },
            {
                // None
                .itemId = 0xFF,
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