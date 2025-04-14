#include "ItemFunctions.hpp"

#include "Effects.hpp"
#include "Helper.hpp"
#include "ItemEffects.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "Tamer.hpp"
#include "extern/BTL.hpp"
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

        constexpr FoodData foodData[36] = {
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

    DigimonType getEvoItemTarget(ItemType item)
    {
        switch (item)
        {
            case ItemType::GREY_CLAWS: return DigimonType::GREYMON;
            case ItemType::FIREBALL: return DigimonType::MERAMON;
            case ItemType::FLAMEWING: return DigimonType::BIRDRAMON;
            case ItemType::IRON_HOOF: return DigimonType::CENTARUMON;
            case ItemType::MONO_STONE: return DigimonType::MONOCHROMON;
            case ItemType::STEEL_DRILL: return DigimonType::DRIMOGEMON;
            case ItemType::WHITE_FANG: return DigimonType::TYRANNOMON;
            case ItemType::BLACK_WING: return DigimonType::DEVIMON;
            case ItemType::SPIKE_CLUB: return DigimonType::OGREMON;
            case ItemType::FLAMEINGMANE: return DigimonType::LEOMON;
            case ItemType::WHITE_WING: return DigimonType::ANGEMON;
            case ItemType::TORN_TATTER: return DigimonType::BAKEMON;
            case ItemType::ELECTRO_RING: return DigimonType::INVALID;
            case ItemType::RAINBOWHORN: return DigimonType::AIRDRAMON;
            case ItemType::ROOSTER: return DigimonType::KOKATORIMON;
            case ItemType::UNIHORN: return DigimonType::UNIMON;
            case ItemType::HORN_HELMET: return DigimonType::KABUTERIMON;
            case ItemType::SCISSOR_JAW: return DigimonType::KUWAGAMON;
            case ItemType::FERTILIZER: return DigimonType::VEGIEMON;
            case ItemType::KOGA_LAWS: return DigimonType::NINJAMON;
            case ItemType::WATERBOTTLE: return DigimonType::SEADRAMON;
            case ItemType::NORTH_STAR: return DigimonType::WHAMON;
            case ItemType::RED_SHELL: return DigimonType::SHELLMON;
            case ItemType::HARD_SCALE: return DigimonType::COELAMON;
            case ItemType::BLUECRYSTAL: return DigimonType::GARURUMON;
            case ItemType::ICE_CRYSTAL: return DigimonType::FRIGIMON;
            case ItemType::HAIR_GROWER: return DigimonType::MOJYAMON;
            case ItemType::SUNGLASSES: return DigimonType::NANIMON;
            case ItemType::METAL_PART: return DigimonType::METALGREYMON;
            case ItemType::FATAL_BONE: return DigimonType::SKULLGREYMON;
            case ItemType::CYBER_PART: return DigimonType::ANDROMON;
            case ItemType::MEGA_HAND: return DigimonType::MEGADRAMON;
            case ItemType::SILVER_BALL: return DigimonType::MAMEMON;
            case ItemType::METAL_ARMOR: return DigimonType::METALMAMEMON;
            case ItemType::CHAINSAW: return DigimonType::GIROMON;
            case ItemType::SMALL_SPEAR: return DigimonType::PIXIMON;
            case ItemType::X_BANDAGE: return DigimonType::MONZAEMON;
            case ItemType::RAY_GUN: return DigimonType::VADEMON;
            case ItemType::GOLD_BANANA: return DigimonType::ETEMON;
            case ItemType::MYSTY_EGG: return DigimonType::DIGITAMAMON;
            case ItemType::RED_RUBY: return DigimonType::PHOENIXMON;
            case ItemType::BETTLEPEARL: return DigimonType::HERCULESKABUTERIMON;
            case ItemType::CORAL_CHARM: return DigimonType::MEGASEADRAMON;
            case ItemType::MOON_MIRROR: return DigimonType::WEREGARURUMON;
            case ItemType::GIGA_HAND: return DigimonType::GIGADRAMON;
            case ItemType::NOBLE_MANE: return DigimonType::PANJYAMON;
            case ItemType::METAL_BANANA: return DigimonType::METALETEMON;

            default: return DigimonType::INVALID;
        }
    }

    void handleEvoItem(ItemType item)
    {
        auto target = getEvoItemTarget(item);

        if (target == DigimonType::INVALID) return;

        EVOLUTION_TARGET = static_cast<int16_t>(target);
        HAS_USED_EVOITEM = true;
        removeTamerItem();
        closeInventoryBoxes();
        Tamer_setState(6);
        Partner_setState(13);
    }

    static void handleVarious(ItemType item)
    {
        if (PARTNER_ENTITY.stats.currentHP == 0) return;

        BTL_healStatusEffect(true);
    }

    static void handleDoubleFloppy(ItemType item)
    {
        if (PARTNER_ENTITY.stats.currentHP == 0) return;

        addWithLimit(&PARTNER_ENTITY.stats.currentHP, 1500, PARTNER_ENTITY.stats.hp);
        addWithLimit(&PARTNER_ENTITY.stats.currentMP, 1500, PARTNER_ENTITY.stats.mp);
        addHealingParticleEffect(&PARTNER_ENTITY, 0);
        if (GAME_STATE == 1)
        {
            addEntityText(&PARTNER_ENTITY, 0, 11, 1500, 1);
            addEntityText(&PARTNER_ENTITY, 0, 11, 1500, 2);
        }
    }

    static void handleOmnipotent(ItemType item)
    {
        if (PARTNER_ENTITY.stats.currentHP == 0) return;

        BTL_healStatusEffect(false);
        handleDoubleFloppy(item);
    }

    static void handleProtection(ItemType item)
    {
        COMBAT_DATA_PTR->fighter[0].flags.isProtected = true;
    }

    static void handleRestoreItem(int32_t healAmount, bool removeStatus)
    {
        if (PARTNER_ENTITY.stats.currentHP == 0) startAnimation(&PARTNER_ENTITY, 0x2C);

        addWithLimit(&PARTNER_ENTITY.stats.currentHP, healAmount, PARTNER_ENTITY.stats.hp);
        addHealingParticleEffect(&PARTNER_ENTITY, 1);

        if (GAME_STATE == 1)
        {
            BTL_removeDeathCountdown();
            addEntityText(&PARTNER_ENTITY, 0, 11, healAmount, 1);
            if (removeStatus) BTL_healStatusEffect(false);
        }
    }

    static void handleRestore(ItemType item)
    {
        handleRestoreItem(PARTNER_ENTITY.stats.hp / 2, false);
    }

    static void handleSuperRestore(ItemType item)
    {
        handleRestoreItem(9999, true);
    }

    static void handleHPHealingItem(int32_t amount, int32_t particleEffect)
    {
        if (PARTNER_ENTITY.stats.currentHP == 0) return;

        addWithLimit(&PARTNER_ENTITY.stats.currentHP, amount, PARTNER_ENTITY.stats.hp);
        addHealingParticleEffect(&PARTNER_ENTITY, particleEffect);

        if (GAME_STATE == 1) addEntityText(&PARTNER_ENTITY, 0, 11, amount, 1);
    }

    static void handleMPHealingItem(int32_t amount, int32_t particleEffect)
    {
        if (PARTNER_ENTITY.stats.currentHP == 0) return;

        addWithLimit(&PARTNER_ENTITY.stats.currentHP, amount, PARTNER_ENTITY.stats.hp);
        addHealingParticleEffect(&PARTNER_ENTITY, particleEffect);

        if (GAME_STATE == 1) addEntityText(&PARTNER_ENTITY, 0, 11, amount, 2);
    }

    static void handleSmallHP(ItemType item)
    {
        handleHPHealingItem(500, 0);
    }

    static void handleMediumHP(ItemType item)
    {
        handleHPHealingItem(1500, 0);
    }

    static void handleLargeHP(ItemType item)
    {
        handleHPHealingItem(5000, 1);
    }

    static void handleSuperHP(ItemType item)
    {
        handleHPHealingItem(9999, 1);
    }

    static void handleSmallMP(ItemType item)
    {
        handleMPHealingItem(500, 0);
    }

    static void handleMediumMP(ItemType item)
    {
        handleMPHealingItem(1500, 0);
    }

    static void handleLargeMP(ItemType item)
    {
        handleMPHealingItem(5000, 1);
    }

    static void handleNothing(ItemType item) {}

    void fillItemTable()
    {
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::SMALL_RECOVERY)]  = handleSmallHP;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::MEDIUM_RECOVERY)] = handleMediumHP;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::LARGE_RECOVERY)]  = handleLargeHP;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::SUPER_RECOVERY)]  = handleSuperHP;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::MP_FLOPPY)]       = handleSmallMP;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::MEDIUM_MP)]       = handleMediumMP;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::LARGE_MP)]        = handleLargeMP;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::DOUBLE_FLOPPY)]   = handleDoubleFloppy;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::VARIOUS)]         = handleVarious;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::OMNIPOTENT)]      = handleOmnipotent;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::PROTECTION)]      = handleProtection;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::RESTORE)]         = handleRestore;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::SUPER_RESTORE)]   = handleSuperRestore;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::BANDAGE)]         = handleBandage;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::MEDICINE)]        = handleMedicine;

        // TODO discs

        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::AUTOPILOT)]       = handleAutopilot;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::OFFENSE_CHIP)]    = handleOffChip;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::DEFENSE_CHIP)]    = handleDefChip;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::BRAIN_SHIP)]      = handleBrainChip;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::SPEED_CHIP)]      = handleSpeedChip;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::HP_CHIP)]         = handleHPChip;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::MP_CHIP)]         = handleMPChip;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::DV_CHIP_A)]       = handleDVChipA;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::DV_CHIP_D)]       = handleDVChipD;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::DV_CHIP_E)]       = handleDVChipE;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::PORTA_POTTY)]     = handlePortaPottyItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::TRAINING_MANUAL)] = handleNothing;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::REST_PILLOW)]     = handleNothing;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::ENEMY_REPEL)]     = handleNothing;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::ENEMY_BELL)]      = handleNothing;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::HEALTH_SHOE)]     = handleNothing;

        for (uint32_t type = static_cast<uint32_t>(ItemType::MEAT);
             type <= static_cast<uint32_t>(ItemType::CHAIN_MELON);
             type++)
        {
            ITEM_FUNCTIONS[type] = handleFood;
        }

        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::GREY_CLAWS)]   = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::FIREBALL)]     = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::FLAMEWING)]    = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::IRON_HOOF)]    = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::MONO_STONE)]   = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::STEEL_DRILL)]  = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::WHITE_FANG)]   = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::BLACK_WING)]   = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::SPIKE_CLUB)]   = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::FLAMEINGMANE)] = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::WHITE_WING)]   = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::TORN_TATTER)]  = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::ELECTRO_RING)] = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::RAINBOWHORN)]  = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::ROOSTER)]      = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::UNIHORN)]      = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::HORN_HELMET)]  = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::SCISSOR_JAW)]  = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::FERTILIZER)]   = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::KOGA_LAWS)]    = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::WATERBOTTLE)]  = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::NORTH_STAR)]   = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::RED_SHELL)]    = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::HARD_SCALE)]   = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::BLUECRYSTAL)]  = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::ICE_CRYSTAL)]  = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::HAIR_GROWER)]  = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::SUNGLASSES)]   = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::METAL_PART)]   = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::FATAL_BONE)]   = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::CYBER_PART)]   = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::MEGA_HAND)]    = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::SILVER_BALL)]  = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::METAL_ARMOR)]  = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::CHAINSAW)]     = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::SMALL_SPEAR)]  = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::X_BANDAGE)]    = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::RAY_GUN)]      = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::GOLD_BANANA)]  = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::MYSTY_EGG)]    = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::RED_RUBY)]     = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::BETTLEPEARL)]  = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::CORAL_CHARM)]  = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::MOON_MIRROR)]  = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::BLUE_FLUTE)]   = handleNothing;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::OLD_ROD)]      = handleNothing;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::AMAZING_ROD)]  = handleNothing;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::LEOMONSTONE)]  = handleNothing;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::MANSION_KEY)]  = handleNothing;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::GEAR)]         = handleNothing;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::RAIN_PLANT)]   = handleFood;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::STEAK)]        = handleFood;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::FRIDGE_KEY)]   = handleFood;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::AS_DECODER)]   = handleFood;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::GIGA_HAND)]    = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::NOBLE_MANE)]   = handleEvoItem;
        ITEM_FUNCTIONS[static_cast<uint32_t>(ItemType::METAL_BANANA)] = handleEvoItem;
    }
}