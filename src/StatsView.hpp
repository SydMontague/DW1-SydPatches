#pragma once

#include "AtlasFont.hpp"
#include "extern/dtl/array.hpp"

struct DigimonStatsView
{
    // NOLINTNEXTLINE
    __attribute__((optimize("Os"))) DigimonStatsView();
    void tick();
    void render(int32_t depth);

private:
    dtl::array<AtlasString, 40> labels;
};