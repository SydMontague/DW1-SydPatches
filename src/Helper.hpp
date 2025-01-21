#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    constexpr bool isWithinTimeframe(int32_t currentTime, int32_t start, int32_t end)
    {
        if (start < end) { return currentTime >= start && currentTime < end; }
        else { return !(currentTime >= end && currentTime < start); }
    }

    constexpr DigimonData* getDigimonData(DigimonType type)
    {
        return &DIGIMON_DATA[static_cast<uint32_t>(type)];
    }

    constexpr RaiseData* getRaiseData(DigimonType type)
    {
        return &RAISE_DATA[static_cast<uint32_t>(type)];
    }

    constexpr int32_t getTimeDiff(int32_t start, int32_t end)
    {
        // <= instead of < fixes day skip? // TODO decide if we actually want to fix that
        if (start <= end)
            return end - start;
        else
            return (24 - start) + end;
    }

    constexpr bool isFoodItem(ItemType item)
    {
        if (item >= ItemType::MEAT && item <= ItemType::CHAIN_MELON) return true;
        if (item == ItemType::STEAK) return true;
        if (item == ItemType::RAIN_PLANT) return true;

        return false;
    }

    constexpr int32_t tileToPos(int16_t tile)
    {
        return (tile - 50) * 100 + 50;
    }

    constexpr bool isWithinRect(int32_t centerX, int32_t centerZ, int32_t radius, Vector* pos)
    {
        auto x = pos->x;
        auto z = pos->z;

        if (centerX - radius > x) return false;
        if (centerX + radius < x) return false;
        if (centerZ - radius > z) return false;
        if (centerZ + radius < z) return false;

        return true;
    }

    constexpr int32_t getHappinessLifetimePenalty(int32_t happiness)
    {
        if (happiness >= 80) return 0;
        return 1 + (happiness - 80) / -50;
    }
}

static_assert(getTimeDiff(9, 18) == 9);
static_assert(getTimeDiff(18, 18) == 0);
static_assert(getTimeDiff(18, 3) == 9);

static_assert(!isWithinTimeframe(8, 9, 18));
static_assert(isWithinTimeframe(9, 9, 18));
static_assert(isWithinTimeframe(10, 9, 18));
static_assert(!isWithinTimeframe(18, 9, 18));
static_assert(!isWithinTimeframe(19, 9, 18));
static_assert(!isWithinTimeframe(23, 9, 18));

static_assert(isWithinTimeframe(6, 18, 9));
static_assert(!isWithinTimeframe(9, 18, 9));
static_assert(!isWithinTimeframe(10, 18, 9));
static_assert(isWithinTimeframe(18, 18, 9));
static_assert(isWithinTimeframe(19, 18, 9));
static_assert(isWithinTimeframe(23, 18, 9));

static_assert(getHappinessLifetimePenalty(81) == 0);
static_assert(getHappinessLifetimePenalty(80) == 0);
static_assert(getHappinessLifetimePenalty(79) == 1);
static_assert(getHappinessLifetimePenalty(31) == 1);
static_assert(getHappinessLifetimePenalty(30) == 2);
static_assert(getHappinessLifetimePenalty(0) == 2);
static_assert(getHappinessLifetimePenalty(-19) == 2);
static_assert(getHappinessLifetimePenalty(-20) == 3);
static_assert(getHappinessLifetimePenalty(-69) == 3);
static_assert(getHappinessLifetimePenalty(-70) == 4);
static_assert(getHappinessLifetimePenalty(-85) == 4);
static_assert(getHappinessLifetimePenalty(-100) == 4);