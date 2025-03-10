#pragma once

#include "stddef.hpp"

extern "C"
{
    extern uint32_t RNG_STATE;

    extern void* memcpy(void* dest, void* src, uint32_t length);
    extern void* memset(void* dest, int fillbyte, uint32_t length);
    extern uint32_t strlen(uint8_t* string);
    extern uint8_t* strncpy(uint8_t* dest, uint8_t* src, uint32_t byte_cnt);
    extern char* strcpy(char* dest, const char* src);
    extern uint8_t* sprintf(uint8_t* buffer, const char* format, ...);
    extern void printf(const char* string, ...);
    extern int32_t rand();
}