#include "extern/dw1.hpp"
#include "extern/libgte.hpp"

extern "C"
{
    int32_t getDistanceSquared(Vector* pos1, Vector* pos2);

    void jis_concat_glyph(uint8_t* string, const uint8_t* glyph);
    const uint8_t* jis_at_index(const uint8_t* string, uint32_t index);
    int32_t jis_len(const uint8_t* str);
    void jis_strncpy(uint8_t* dest, uint8_t* src, int32_t byte_cnt);

    void clearTextSubArea2(int16_t x, int16_t y, int16_t width, int16_t height);
}