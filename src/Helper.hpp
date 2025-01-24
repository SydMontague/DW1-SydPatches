#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    /*
     * Calculates whether a given time is within a given timeframe, where start being larger than end represents the
     * timeframe covering the day-change (or equivalent).
     */
    constexpr bool isWithinTimeframe(int32_t currentTime, int32_t start, int32_t end)
    {
        if (start < end) { return currentTime >= start && currentTime < end; }
        else { return !(currentTime >= end && currentTime < start); }
    }

    /*
     * Get the difference between two timepoints in hours, accounting for the day change.
     */
    constexpr int32_t getTimeDiff(int32_t start, int32_t end)
    {
        // <= instead of < fixes day skip? // TODO decide if we actually want to fix that
        if (start <= end)
            return end - start;
        else
            return (24 - start) + end;
    }

    /*
     * Checks whether a given item is a food item.
     */
    constexpr bool isFoodItem(ItemType item)
    {
        if (item >= ItemType::MEAT && item <= ItemType::CHAIN_MELON) return true;
        if (item == ItemType::STEAK) return true;
        if (item == ItemType::RAIN_PLANT) return true;

        return false;
    }

    /*
     * Convert a tile coordinate to a world coordinate.
     */
    constexpr int32_t tileToPos(int16_t tile)
    {
        return (tile - 50) * 100 + 50;
    }

    /*
     * Check whether a given position is within an rectangular area defined by a center X+Z coordinate and an extent in
     * every direction. This check happens on the XZ 2D plane, used for most of the game.
     */
    constexpr bool isWithinRect(int32_t centerX, int32_t centerZ, int32_t extent, Vector* pos)
    {
        auto x = pos->x;
        auto z = pos->z;

        if (centerX - extent > x) return false;
        if (centerX + extent < x) return false;
        if (centerZ - extent > z) return false;
        if (centerZ + extent < z) return false;

        return true;
    }

    /*
     * Get the happiness lifetime penalty for a given happiness value.
     * The vanilla formula is (1 + (happiness - 80) / 50), for values < 80
     */
    constexpr int32_t getHappinessLifetimePenalty(int32_t happiness)
    {
        if (happiness >= 80) return 0;
        return 1 + (happiness - 80) / -50;
    }

    constexpr DigimonData* getDigimonData(DigimonType type)
    {
        return &DIGIMON_DATA[static_cast<uint32_t>(type)];
    }

    constexpr RaiseData* getRaiseData(DigimonType type)
    {
        return &RAISE_DATA[static_cast<uint32_t>(type)];
    }

    inline void Partner_setState(uint8_t state)
    {
        PARTNER_STATE     = state;
        PARTNER_SUB_STATE = 0;
    }

    inline void Partner_setSubState(uint8_t state)
    {
        PARTNER_SUB_STATE = state;
    }

    inline uint8_t Partner_getState()
    {
        return PARTNER_STATE;
    }

    inline uint8_t Partner_getSubState()
    {
        return PARTNER_SUB_STATE;
    }

    inline void Tamer_setState(uint8_t state)
    {
        TAMER_STATE = state;
    }

    inline uint8_t Tamer_getState()
    {
        return TAMER_STATE;
    }

    inline void Tamer_setSubState(uint8_t state)
    {
        TAMER_SUB_STATE = state;
    }

    inline uint8_t Tamer_getSubState()
    {
        return TAMER_SUB_STATE;
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