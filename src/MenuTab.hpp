#include "AtlasFont.hpp"
#include "extern/dtl/types.hpp"

class MenuTab
{
public:
    MenuTab(int16_t x, int16_t y, int16_t width, bool isInactive, const char* string);
    void setState(bool val);
    void render(int32_t depth);

private:
    int16_t x;
    int16_t y;
    int16_t width;
    uint8_t isInactive;
    AtlasString label;
};