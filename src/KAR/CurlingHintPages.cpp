#include "Curling.hpp"

#include "../VanillaText.hpp"
#include "../extern/dtl/array.hpp"

extern const dtl::array<const char*, 61> KAR_PENGUINMON_HINT_STRINGS;
extern const dtl::array<const char*, 61> KAR_METALMAMEMON_HINT_STRINGS;

namespace
{
    constexpr dtl::array<int8_t, 10> HINT_OFFSETS = {0, 12, 20, 24, 40, 44, 48, 52, 56, 60};
} // namespace

int32_t KAR_drawHintPage(int32_t page, int8_t line, bool penguinmon)
{
    setTextColor(line % 4 == 0 ? 7 : 1);
    const auto& strings = penguinmon ? KAR_PENGUINMON_HINT_STRINGS : KAR_METALMAMEMON_HINT_STRINGS;
    // TODO: Migrate to AtlasStrings when their allocation lifetime is managed.
    drawString(strings[HINT_OFFSETS[page] + line], 0, line * 13 + 1);
    return static_cast<int8_t>((HINT_OFFSETS[page + 1] - HINT_OFFSETS[page]) / 4);
}
