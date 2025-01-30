#include "Math.hpp"

#include "extern/dw1.hpp"
#include "extern/libc.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    int32_t random(int32_t limit)
    {
        return (limit * rand()) >> 0xF;
    }

    int16_t sin(uint32_t input)
    {
        auto val = input & 0x1FF;

        if (val < 0x80)
            return SIN_LOOKUP_TABLE[val];
        else if (val < 0x100)
            return SIN_LOOKUP_TABLE[0x100 - val];
        else if (val < 0x180)
            return -SIN_LOOKUP_TABLE[val - 0x100];
        else
            return -SIN_LOOKUP_TABLE[0x200 - val];
    }

    int16_t cos(uint32_t input)
    {
        return sin(input + 0x80);
    }

    int16_t atan(int32_t dy, int32_t dx)
    {
        uint32_t generalDirection;
        uint32_t offset;
        uint32_t tmpVal;

        if (dy >= 0 && dy >= 0) generalDirection = 0;
        if (dy < 0 && dx >= 0) generalDirection = 1;
        if (dy < 0 && dx < 0) generalDirection = 2;
        if (dy >= 0 && dx < 0) generalDirection = 3;

        dx = abs(dx);
        dy = abs(dy);

        if (dy < dx)
        {
            if (dx < 1)
                offset = 0;
            else
                offset = (dy << 9) / dx;
            if (offset > 0x1FF) offset = 0x1FF;
            tmpVal = 0x3FF - ATAN_LOOKUP_TABLE[offset];
        }
        else
        {
            if (dy < 1)
                offset = 0;
            else
                offset = (dx << 9) / dy;
            if (offset > 0x1FF) offset = 0x1FF;
            tmpVal = ATAN_LOOKUP_TABLE[offset];
        }

        if (generalDirection == 1 || generalDirection == 2) tmpVal = 0x800 - tmpVal;
        if (generalDirection >= 2) tmpVal = -tmpVal;

        return (tmpVal + 0x800) & 0xFFF;
    }
}