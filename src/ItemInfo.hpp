#pragma once

#include "extern/dw1.hpp"

struct ItemInfo
{
    const char* headline;
    const char* detail;
    const char* desc;
};

// Returned by value so the header and desc tables can live in separate binary
// regions (each lib is too big to share one).
ItemInfo getItemInfo(ItemType type);

// Desc strings split A/B (items 0..63 / 64..127) for the same reason.
extern "C" const char* const ITEM_DESC_TABLE_A[64];
extern "C" const char* const ITEM_DESC_TABLE_B[64];
