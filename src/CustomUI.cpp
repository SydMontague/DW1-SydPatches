#include "AtlasFont.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Timestamp.hpp"
#include "extern/libc.hpp"

namespace
{
    constexpr RenderSettings RNG_POSITION{.x = -159, .y = -119, .baseClut = 1};
    constexpr RenderSettings TIME_POSITION{.x = -159, .y = -113, .baseClut = 1};

    void debugOverlayRender(int32_t instanceId)
    {
        getAtlas5px().renderSlow(format("%u", RNG_STATE).data(), 1, RNG_POSITION);
        getAtlas5px().renderSlow(format("%u", getTimestamp()).data(), 1, TIME_POSITION);
    }
} // namespace

extern "C"
{
    void initCustomUI()
    {
        addObject(ObjectID::DEBUG_OVERLAY, 0, nullptr, debugOverlayRender);
    }
}