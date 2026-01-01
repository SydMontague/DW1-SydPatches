#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

constexpr dtl::array<RGB8, 17> TEXT_COLORS{{
    {0x80, 0x80, 0x80},
    {0x19, 0x55, 0x80},
    {0x69, 0xc2, 0xff},
    {0xff, 0x96, 0x46},
    {0xc8, 0xb4, 0x32},
    {0x1e, 0x80, 0x80},
    {0xd0, 0x1e, 0x50},
    {0x1e, 0xff, 0x1e},
    {0x50, 0x50, 0x50},
    {0x6e, 0x6e, 0x6e},
    {0x46, 0x46, 0x46},
    {0x00, 0x80, 0x00},
    {0x80, 0x00, 0x80},
    {0x40, 0x40, 0x40},
    {0x70, 0x44, 0x2c},
    {0x48, 0x54, 0x7c},
    {0x7c, 0x4c, 0x68},

}};

extern "C"
{
    void drawEntityText(int32_t color, int32_t digitCount, int32_t x, int32_t y, int32_t value, int32_t layer);
    [[deprecated("renderStringNew")]] void renderString(int32_t colorId,
                                                        int16_t posX,
                                                        int16_t posY,
                                                        int16_t uvWidth,
                                                        int16_t uvHeight,
                                                        int16_t uvX,
                                                        int16_t uvY,
                                                        int32_t offset,
                                                        int32_t hasShadow);
}