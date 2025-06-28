#include "Font.hpp"
#include "GameObjects.hpp"
#include "Timestamp.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"
#include "extern/libetc.hpp"
#include "extern/psx.hpp"

extern "C"
{
    static void debugOverlayTick(int32_t instanceId)
    {
        RECT area = {
            .x      = 352,
            .y      = 224,
            .width  = 32,
            .height = 32,
        };
        uint8_t buffer[64];

        auto previous_color = COLORCODE_LOWBITS;

        setTextColor(1);
        sprintf(buffer, "%u", RNG_STATE);
        libgpu_ClearImage(&area, 0, 0, 0);
        drawStringNew(&myFont5px, buffer, 352, 224);
        sprintf(buffer, "%u", getTimestamp());
        drawStringNew(&myFont5px, buffer, 352, 230);

        setTextColor(previous_color);
    }

    static void debugOverlayRender(int32_t instanceId)
    {
        renderStringNew(0, -159, -119, 127, 12, 352, 224, 1, 1);
    }

    void initCustomUI()
    {
        addObject(ObjectID::DEBUG_OVERLAY, 0, debugOverlayTick, debugOverlayRender);
    }
}