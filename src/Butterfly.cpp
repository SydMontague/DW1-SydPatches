
#include "Effects.hpp"
#include "Entity.hpp"
#include "GameObjects.hpp"
#include "Math.hpp"
#include "extern/stddef.hpp"

namespace
{
    struct Butterfly
    {
        uint16_t frameCount = 0xFFFF;
        bool active         = false;
        Entity* entity      = nullptr;
    };

    Butterfly data{};
    GsSPRITE sprite{
        .attribute = 0x50000000,
        .x         = 0,
        .y         = 0,
        .width     = 17,
        .height    = 16,
        .tpage     = 0x3F,
        .u         = 0,
        .v         = 0xB0,
        .clutX     = 272,
        .clutY     = 482,
        .r         = 128,
        .g         = 128,
        .b         = 128,
        .mx        = 8,
        .my        = 8,
        .scaleX    = 0,
        .scaleY    = 0,
        .rotate    = 0,
    };

    void tickButterfly(int32_t id)
    {
        if (IS_IN_MENU == 1) return;
        data.frameCount++;
    }

    void renderButterfly(int32_t id)
    {
        if (IS_IN_MENU == 1) return;

        SVector pos;
        translateConditionFXToEntity(data.entity, &pos);

        const auto val1 = lerp(0, 0x200, 0, 0x3c, data.frameCount);
        const auto val2 = lerp(0, 0x600, 0, 0x3C, data.frameCount);
        const auto posX = sin(val1) * 150;
        const auto posZ = cos(val1) * 150;
        const auto posY = sin(val2) * 50;

        pos.x += (posX / 4096);
        pos.y += (posY / 4096) - 100;
        pos.z += (posZ / 4096);

        auto mapPos = getMapPosition(pos);
        if (mapPos.depth < 512 || mapPos.depth > 65536) return;

        sprite.u = (data.frameCount & 2) * 8;
        renderSprite(&sprite, mapPos.screenX, mapPos.screenY, mapPos.depth, 0x5400, 0x5400);
    }
} // namespace

void addButterfly(Entity* entity)
{
    if (data.active) return;

    data.frameCount = 0;
    data.active     = true;
    data.entity     = entity;
    addObject(ObjectID::BUTTERFLY, 0, tickButterfly, renderButterfly);
}

void removeButterfly()
{
    data.frameCount = 0xFFFF;
    data.active     = false;
    removeObject(ObjectID::BUTTERFLY, 0);
}

bool hasButterfly()
{
    return data.active;
}
