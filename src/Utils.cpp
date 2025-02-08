#include "Utils.hpp"

#include "Helper.hpp"
#include "extern/dw1.hpp"
#include "extern/libgte.hpp"

extern "C"
{
    constexpr bool jis_is_two_byte(uint8_t first)
    {
        return (first & 0x80) != 0;
    }

    int32_t getDistanceSquared(Vector* pos1, Vector* pos2)
    {
        int32_t diffX = pos1->x - pos2->x;
        int32_t diffZ = pos1->z - pos2->z;
        return diffX * diffX + diffZ * diffZ;
    }

    void jis_concat_glyph(uint8_t* string, const uint8_t* glyph)
    {
        string    = string + strlen(string);
        string[0] = glyph[0];

        int idx = 1;
        if (jis_is_two_byte(glyph[0]))
        {
            string[1] = glyph[1];
            idx++;
        }
        string[idx] = 0;
    }

    const uint8_t* jis_at_index(const uint8_t* string, uint32_t index)
    {
        for (int i = 0; i < index; i++)
        {
            uint8_t firstByte = *string;
            if (firstByte == 0) return string;

            string += jis_is_two_byte(firstByte) + 1;
        }
        return string;
    }

    int32_t jis_len(const uint8_t* str)
    {
        int32_t glyphs = 0;
        int32_t bytes  = 0;

        while (str[0] != 0)
        {
            glyphs++;
            bytes += jis_is_two_byte(str[0]) + 1;
            str += jis_is_two_byte(str[0]) + 1;
        }
        asm volatile("move $v1, %0\n\t" : : "r"(bytes));

        return glyphs;
    }

    void jis_strncpy(uint8_t* dest, uint8_t* src, int32_t byte_cnt)
    {
        uint8_t* end = dest + byte_cnt;
        strncpy(dest, src, byte_cnt);
        end[-1] = 0;
        if (!jis_is_two_byte(end[-2])) end[-2] = 0;
    }

    void clearTextSubArea2(int16_t x, int16_t y, int16_t width, int16_t height)
    {
        RECT rect = {
            .x      = x,
            .y      = y,
            .width  = width,
            .height = height,
        };
        clearTextSubArea(&rect);
    }
}
