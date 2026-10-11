#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../Camera.hpp"
#include "../Input.hpp"
#include "../extern/dw1.hpp"

[[gnu::optimize("Os")]]
void KAR_handleAimScroll()
{
    if (KAR_MATCH_STATE != CurlingMatchState::AIM || KAR_CURRENT_PLAYER != 0) return;

    int32_t dx = 0;
    int32_t dy = 0;
    if (KAR_AIM_VELOCITY >= 10 || KAR_AIM_VELOCITY < -9 || KAR_HINT_STATE != 0) {
        if (isKeyPressed(InputButtons::BUTTON_LEFT)) dx -= 8;
        if (isKeyPressed(InputButtons::BUTTON_RIGHT)) dx += 8;
    }
    if (isKeyPressed(InputButtons::BUTTON_UP)) dy -= 10;
    if (isKeyPressed(InputButtons::BUTTON_DOWN)) dy += 10;

    if (dx != 0 || dy != 0) moveCameraByOffset(dx, dy);
}
