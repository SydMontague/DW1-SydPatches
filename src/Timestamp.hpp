#pragma once

#include "extern/stddef.hpp"

extern "C" void initTimestamp();

/*
 * Gets the number of ticks since initialization of the timer. One tick equals 1.25ms.
 */
uint32_t getTickCount();

/*
 * Gets the current time in µs since intialization of the timer.
 */
uint64_t getTimestamp();