#pragma once

#include "../extern/dtl/array.hpp"
#include "../extern/libgs.hpp"

struct CurlingSprite
{
    int16_t x;
    int16_t y;
    int16_t clutX;
    int16_t clutY;
    int16_t u;
    int16_t v;
    int16_t width;
    int16_t height;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t reserved;
};

extern dtl::array<GsOT, 2> KAR_ORDERING_TABLES;
extern dtl::array<dtl::array<GsOT_TAG, 32>, 2> KAR_ORDERING_TAGS;
