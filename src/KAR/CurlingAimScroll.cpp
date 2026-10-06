#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../Camera.hpp"
#include "../extern/dw1.hpp"

extern int8_t KAR_HINT_STATE;
extern int16_t KAR_AIM_VELOCITY;

extern "C"
{
    [[gnu::optimize("Os")]]
    void KAR_handleAimScroll()
    {
        if (KAR_MATCH_STATE != 6 || KAR_CURRENT_PLAYER != 0) return;

        int32_t dx = 0;
        int32_t dy = 0;
        if (KAR_AIM_VELOCITY >= 10 || KAR_AIM_VELOCITY < -9 || KAR_HINT_STATE != 0) {
            if (POLLED_INPUT & InputButtons::BUTTON_LEFT) dx -= 8;
            if (POLLED_INPUT & InputButtons::BUTTON_RIGHT) dx += 8;
        }
        if (POLLED_INPUT & InputButtons::BUTTON_UP) dy -= 10;
        if (POLLED_INPUT & InputButtons::BUTTON_DOWN) dy += 10;

        if (dx != 0 || dy != 0) moveCameraByOffset(dx, dy);
    }
}
