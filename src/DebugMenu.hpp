#pragma once

#include "extern/stddef.hpp"

inline constexpr bool isDebugEnabled()
{
    return false;
}

void addDebugMenu();
void removeDebugMenu();