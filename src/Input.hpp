#pragma once

#include "extern/dw1.hpp"

extern "C"
{
    /*
     * Checks if a button has been pressed and consumes it (i.e. subsequent checks for the same button within the same
     * tick return false).
     */
    bool isKeyDown(uint16_t keyMask);
    uint32_t processInput();
}

bool isKeyDownPolled(InputButtons button);
bool isKeyPressed(InputButtons button);