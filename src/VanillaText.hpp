#include "extern/stddef.hpp"

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