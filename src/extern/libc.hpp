#pragma once

#include "stddef.hpp"

extern "C"
{
    struct jmpbuffer
    {
        uint32_t pc;
        uint32_t sp;
        uint32_t fp;
        uint32_t r16;
        uint32_t r17;
        uint32_t r18;
        uint32_t r19;
        uint32_t r20;
        uint32_t r21;
        uint32_t r22;
        uint32_t r23;
        uint32_t gp;
    };

    extern uint32_t RNG_STATE;

    extern void* memcpy(void* dest, const void* src, uint32_t length);
    extern void* memmove(void* dest, const void* src, uint32_t length);
    extern void* memset(void* dest, int fillbyte, uint32_t length);
    extern uint32_t strlen(const uint8_t* string);
    extern uint8_t* strncpy(uint8_t* dest, const uint8_t* src, uint32_t byte_cnt);
    extern char* strcpy(uint8_t* dest, const uint8_t* src);
    extern uint8_t* sprintf(uint8_t* buffer, const char* format, ...);
    extern void printf(const char* string, ...);
    extern int32_t rand();
    void libc_exit(int32_t code);
    void bzero(void* pointer, int32_t size);
    int32_t libc_setjmp(jmpbuffer* buffer);
    void libc_longjmp(const jmpbuffer* buffer, int32_t retVal);
}

static_assert(sizeof(jmpbuffer) == 48);
