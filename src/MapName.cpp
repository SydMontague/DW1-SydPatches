
#include "AtlasFont.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "MapData.hpp"
#include "UIElements.hpp"
#include "extern/dw1.hpp"

namespace
{
    constexpr RenderSettings settings{
        .x      = -SCREEN_WIDTH / 2,
        .y      = -SCREEN_HEIGHT / 2,
        .width  = SCREEN_WIDTH,
        .height = SCREEN_HEIGHT,
        .alignX = AlignmentX::CENTER,
        .alignY = AlignmentY::CENTER,
    };

    void renderMapName(int32_t mapId)
    {
        getAtlasVanilla().renderSlow(MAP_NAME_PTR[MAP_ENTRIES[mapId].loadingName], 0, settings);
    }
} // namespace

void addMapNameObject(int32_t mapId)
{
    // vanilla draws the string in here, but we can also do it within the object render
    addObject(ObjectID::MAP_NAME, mapId, nullptr, renderMapName);
}