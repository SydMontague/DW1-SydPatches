#include "AtlasFont.hpp"
#include "GameObjects.hpp"
#include "Timestamp.hpp"

namespace
{
    constexpr RenderSettings RNG_POSITION{.x = -159, .y = -119, .hasShadow = true};
    constexpr RenderSettings TIME_POSITION{.x = -159, .y = -113, .hasShadow = true};

    void debugOverlayRender(int32_t instanceId)
    {
        uint8_t buffer[64];
        sprintf(buffer, "%u", RNG_STATE);
        getAtlas5px().renderSlow(buffer, 1, RNG_POSITION);
        sprintf(buffer, "%u", getTimestamp());
        getAtlas5px().renderSlow(buffer, 1, TIME_POSITION);
    }
} // namespace

extern "C"
{
    void initCustomUI()
    {
        addObject(ObjectID::DEBUG_OVERLAY, 0, nullptr, debugOverlayRender);
    }
}