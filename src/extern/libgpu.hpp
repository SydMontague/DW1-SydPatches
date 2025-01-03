#pragma once

#include "stddef.hpp"

extern "C"
{
    struct RECT
    {
        int16_t x;
        int16_t y;
        int16_t width;
        int16_t height;
    };

    struct POLY_FT4
    {
        uint8_t tag[4];
        uint8_t r0;
        uint8_t g0;
        uint8_t b0;
        uint8_t code;
        int16_t x0;
        int16_t y0;
        uint8_t u0;
        uint8_t v0;
        uint16_t clut;
        int16_t x1;
        int16_t y1;
        uint8_t u1;
        uint8_t v1;
        uint16_t tpage;
        int16_t x2;
        int16_t y2;
        uint8_t u2;
        uint8_t v2;
        uint16_t pad1;
        int16_t x3;
        int16_t y3;
        uint8_t u3;
        uint8_t v3;
        uint16_t pad2;
    };

    extern void libgpu_SetPolyFT4(POLY_FT4* prim);
    extern void libgpu_AddPrim(void* ot, void* prim);
    extern void libgpu_ClearImage(RECT* rect, uint32_t red, uint32_t green, uint32_t blue);
    extern int libgpu_LoadImage(RECT* rect, uint32_t* data);
    extern uint16_t libgpu_GetClut(int32_t x, int32_t y);
}

// validation
static_assert(sizeof(RECT) == 8);
static_assert(sizeof(POLY_FT4) == 0x28);