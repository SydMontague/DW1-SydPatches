
#include "extern/dw1.hpp"

namespace
{
    constexpr dtl::array<ItemType, 78> recycleable{
        ItemType::RESTORE,
        ItemType::SUPER_RESTORE,
        ItemType::OFFENSE_CHIP,
        ItemType::DEFENSE_CHIP,
        ItemType::BRAIN_SHIP,
        ItemType::SPEED_CHIP,
        ItemType::HP_CHIP,
        ItemType::MP_CHIP,
        ItemType::DV_CHIP_A,
        ItemType::DV_CHIP_D,
        ItemType::DV_CHIP_E,
        ItemType::TRAINING_MANUAL,
        ItemType::ENEMY_REPEL,
        ItemType::ENEMY_BELL,
        ItemType::DELUXE_MUSHROOM,
        ItemType::GOLD_ACORN,
        ItemType::POWER_FRUIT,
        ItemType::POWER_ICE,
        ItemType::SPEED_LEAF,
        ItemType::SAGE_FRUIT,
        ItemType::MUSCLE_YAM,
        ItemType::CALM_BERRY,
        ItemType::DIGISEABASS,
        ItemType::CHAIN_MELON,
        ItemType::GREY_CLAWS,
        ItemType::FIREBALL,
        ItemType::FLAMEWING,
        ItemType::IRON_HOOF,
        ItemType::MONO_STONE,
        ItemType::STEEL_DRILL,
        ItemType::BLACK_WING,
        ItemType::SPIKE_CLUB,
        ItemType::FLAMEINGMANE,
        ItemType::WHITE_WING,
        ItemType::TORN_TATTER,
        ItemType::ELECTRO_RING,
        ItemType::RAINBOWHORN,
        /*
         * RAINBOWHORN exists twice in vanilla, making one entry redundant/unused, meanwhile WHITE_FANG is missing.
         * We'using the second RAINBOWHORN slot for WHITE_FANG instead
         */
        ItemType::WHITE_FANG,
        ItemType::ROOSTER,
        ItemType::UNIHORN,
        ItemType::HORN_HELMET,
        ItemType::SCISSOR_JAW,
        ItemType::FERTILIZER,
        ItemType::KOGA_LAWS,
        ItemType::WATERBOTTLE,
        ItemType::NORTH_STAR,
        ItemType::RED_SHELL,
        ItemType::HARD_SCALE,
        ItemType::BLUECRYSTAL,
        ItemType::ICE_CRYSTAL,
        ItemType::HAIR_GROWER,
        ItemType::SUNGLASSES,
        ItemType::METAL_PART,
        ItemType::FATAL_BONE,
        ItemType::CYBER_PART,
        ItemType::MEGA_HAND,
        ItemType::SILVER_BALL,
        ItemType::METAL_ARMOR,
        ItemType::CHAINSAW,
        ItemType::SMALL_SPEAR,
        ItemType::X_BANDAGE,
        ItemType::RAY_GUN,
        ItemType::GOLD_BANANA,
        ItemType::MYSTY_EGG,
        ItemType::RED_RUBY,
        ItemType::BETTLEPEARL,
        ItemType::CORAL_CHARM,
        ItemType::MOON_MIRROR,
        ItemType::BLUE_FLUTE,
        ItemType::OLD_ROD,
        ItemType::AMAZING_ROD,
        ItemType::LEOMONSTONE,
        ItemType::MANSION_KEY,
        ItemType::GEAR,
        ItemType::RAIN_PLANT,
        ItemType::STEAK,
        ItemType::FRIDGE_KEY,
        ItemType::AS_DECODER,
    };
}

int32_t getRecycleId(ItemType type)
{
    for (auto i = 0; i < recycleable.size(); i++)
        if (recycleable[i] == type) return i;

    return 0xFF;
}
