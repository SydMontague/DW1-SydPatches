#include "UIElements.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    namespace ValueIndex
    {
        enum ValueIndex
        {
            NAME,
            DIGIMON,
            AGE,
            WEIGHT,
            BATTLE,
            CARE,
            ACTIVE_TIME,
            HP,
            MP,
            OFFENSE,
            DEFENSE,
            SPEED,
            BRAINS,
            HAPPINESS,
            DISCIPLINE,
            TIREDNESS,
            VIRUS,
        };
    } // namespace ValueIndex

    namespace SpriteIndex {
        enum SpriteIndex {
            TYPE,
            SPECIAL1,
            SPECIAL2,
            SPECIAL3,
            HAPPINESS,
            DISCIPLINE,
            LIFE1,
            LIFE2,
            LIFE3,
            SICK,
            INJURY,
            HUNGER,
            SLEEP,
            UNHAPPY,
            TIRED,
            POOP,
            HAPPINESS_BAR1,
            HAPPINESS_BAR2,
            DISCIPLINE_BAR1,
            DISCIPLINE_BAR2,
        };
    }
    constexpr int32_t toRelativeX(int32_t in)
    {
        return in - 160;
    }
    constexpr int32_t toRelativeY(int32_t in)
    {
        return in - 120;
    }

    constexpr uint32_t DRAW_OFFSET_X = 704;
    constexpr uint32_t DRAW_OFFSET_Y = 256 + 16;

    constexpr int32_t STATS_OFFSET_X     = toRelativeX(187);
    constexpr int32_t STATS_OFFSET_Y     = toRelativeY(108);
    constexpr int32_t PROFILE_OFFSET_X   = toRelativeX(20);
    constexpr int32_t PROFILE_OFFSET_Y   = toRelativeY(37);
    constexpr int32_t CONDITION_OFFSET_X = toRelativeX(20);
    constexpr int32_t CONDITION_OFFSET_Y = toRelativeY(108);

    extern IconSprite sprites[31];
    extern Line lines[6];
    extern TextSprite badges[3];
    extern TextSprite labels[20];
    extern TextSprite values[17];
}