#include "AtlasFont.hpp"
#include "GameObjects.hpp"
#include "Timestamp.hpp"

namespace
{
    void debugOverlayRender(int32_t instanceId)
    {
        uint8_t buffer[64];
        sprintf(buffer, "%u", RNG_STATE);
        getAtlas5px().renderSlow(buffer, -159, -119, 1, {.hasShadow = true});
        sprintf(buffer, "%u", getTimestamp());
        getAtlas5px().renderSlow(buffer, -159, -113, 1, {.hasShadow = true});
    }
} // namespace

extern "C"
{
    void initCustomUI()
    {
        addObject(ObjectID::DEBUG_OVERLAY, 0, nullptr, debugOverlayRender);
    }
}