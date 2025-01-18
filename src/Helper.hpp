#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    constexpr bool isWithinTimeframe(int32_t currentTime, int32_t start, int32_t end)
    {
        if (start < end) { return currentTime >= start && currentTime < end; }
        else { return !(currentTime >= end && currentTime < start); }
    }

    constexpr DigimonPara* getDigimonPara(DigimonType type)
    {
        return &DIGIMON_PARA[static_cast<uint32_t>(type)];
    }
    
    constexpr RaisePara* getRaiseData(DigimonType type)
    {
        return &RAISE_DATA[static_cast<uint32_t>(type)];
    }
}

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