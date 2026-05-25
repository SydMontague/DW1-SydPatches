#pragma once

#include "extern/stddef.hpp"

inline constexpr bool isDebugEnabled()
{
    return true;
}

void addDebugMenu();
void removeDebugMenu();