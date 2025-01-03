#pragma once

using uint8_t  = unsigned char;
using uint16_t = unsigned short;
using uint32_t = unsigned int;
using int8_t   = char;
using int16_t  = short;
using int32_t  = int;

static_assert(sizeof(int32_t) == 4);
static_assert(sizeof(int16_t) == 2);
static_assert(sizeof(int8_t) == 1);
static_assert(sizeof(uint32_t) == 4);
static_assert(sizeof(uint16_t) == 2);
static_assert(sizeof(uint8_t) == 1);
