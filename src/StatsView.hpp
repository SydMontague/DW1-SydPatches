#pragma once

#include "AtlasFont.hpp"
#include "extern/dtl/array.hpp"

struct DigimonStatsView
{
    // NOLINTNEXTLINE
    __attribute__((optimize("Os"))) DigimonStatsView();
    void tick();
    void render();

private:
    dtl::array<AtlasString, 40> labels;
};