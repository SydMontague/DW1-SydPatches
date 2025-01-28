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

    struct POLY_GT3
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
        uint8_t r1;
        uint8_t g1;
        uint8_t b1;
        uint8_t pad1;
        int16_t x1;
        int16_t y1;
        uint8_t u1;
        uint8_t v1;
        uint16_t tpage;
        uint8_t r2;
        uint8_t g2;
        uint8_t b2;
        uint8_t pad2;
        int16_t x2;
        int16_t y2;
        uint8_t u2;
        uint8_t v2;
        uint16_t pad3;
    };

    struct POLY_GT4
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
        uint8_t r1;
        uint8_t g1;
        uint8_t b1;
        uint8_t pad1;
        int16_t x1;
        int16_t y1;
        uint8_t u1;
        uint8_t v1;
        uint16_t tpage;
        uint8_t r2;
        uint8_t g2;
        uint8_t b2;
        uint8_t pad2;
        int16_t x2;
        int16_t y2;
        uint8_t u2;
        uint8_t v2;
        uint16_t pad3;

        uint8_t r3;
        uint8_t g3;
        uint8_t b3;
        uint8_t pad4;
        int16_t x3;
        int16_t y3;
        uint8_t u3;
        uint8_t v3;
        uint16_t pad5;
    };

    struct LINE_F2
    {
        uint8_t tag[4];
        uint8_t r0;
        uint8_t g0;
        uint8_t b0;
        uint8_t code;
        int16_t x0;
        int16_t y0;
        int16_t x1;
        int16_t y1;
    };

    struct LINE_F3
    {
        uint8_t tag[4];
        uint8_t r0;
        uint8_t g0;
        uint8_t b0;
        uint8_t code;
        int16_t x0;
        int16_t y0;
        int16_t x1;
        int16_t y1;
        int16_t x2;
        int16_t y2;
        uint32_t pad;
    };

    struct LINE_F4
    {
        uint8_t tag[4];
        uint8_t r0;
        uint8_t g0;
        uint8_t b0;
        uint8_t code;
        int16_t x0;
        int16_t y0;
        int16_t x1;
        int16_t y1;
        int16_t x2;
        int16_t y2;
        int16_t x3;
        int16_t y3;
        uint32_t pad;
    };

    constexpr void libgpu_SetPolyGT3(POLY_GT3* prim)
    {
        prim->tag[3] = 9;
        prim->code   = 0x34;
    }

    constexpr void libgpu_SetPolyGT4(POLY_GT4* prim)
    {
        prim->tag[3] = 12;
        prim->code   = 0x3C;
    }

    extern void libgpu_SetPolyFT4(POLY_FT4* prim);
    extern void libgpu_SetLineF2(LINE_F2* prim);
    extern void libgpu_SetLineF3(LINE_F3* prim);
    extern void libgpu_SetLineF4(LINE_F4* prim);
    extern void libgpu_AddPrim(void* ot, void* prim);
    extern void libgpu_ClearImage(RECT* rect, uint32_t red, uint32_t green, uint32_t blue);
    extern int libgpu_LoadImage(RECT* rect, uint32_t* data);
    extern uint16_t libgpu_GetClut(int32_t x, int32_t y);
    extern void libgpu_DrawSync(int32_t mode);
}

// validation
static_assert(sizeof(RECT) == 8);
static_assert(sizeof(POLY_FT4) == 0x28);
static_assert(sizeof(POLY_GT3) == 10 * 4);
static_assert(sizeof(POLY_GT4) == 13 * 4);