
#include "Effects.hpp"
#include "GameObjects.hpp"
#include "Math.hpp"
#include "extern/dw1.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"

namespace
{
    GsSPRITE MONOCHROMON_BUBBLE_SPRITE{
        .attribute = 0x50000000,
        .x         = 0,
        .y         = 0,
        .width     = 32,
        .height    = 32,
        .tpage     = 10,
        .u         = 128,
        .v         = 0,
        .clutX     = 0,
        .clutY     = 486,
        .r         = 128,
        .g         = 128,
        .b         = 128,
        .mx        = 16,
        .my        = 16,
        .scaleX    = 4096,
        .scaleY    = 4096,
        .rotate    = 0,
    };
    int16_t bubbleTimer;

    void renderMonochromonBubble(int32_t id)
    {
        auto entityId = scriptIdToEntityId(readPStat(0xF7));
        if (entityId == 0xFF) return;

        SVector worldPos;
        translateConditionFXToEntity(ENTITY_TABLE.getEntityById(entityId), &worldPos);
        auto screenPos = getMapPosition(worldPos);

        auto timeFactor = 16 - bubbleTimer / 2;

        MONOCHROMON_BUBBLE_SPRITE.x      = screenPos.screenX;
        MONOCHROMON_BUBBLE_SPRITE.y      = screenPos.screenY - timeFactor;
        MONOCHROMON_BUBBLE_SPRITE.scaleX = timeFactor * 256;
        MONOCHROMON_BUBBLE_SPRITE.scaleY = MONOCHROMON_BUBBLE_SPRITE.scaleX;
        libgs_GsSortSprite(&MONOCHROMON_BUBBLE_SPRITE, ACTIVE_ORDERING_TABLE, screenPos.depth / 16);

        if (bubbleTimer > 0) bubbleTimer--;
    }

} // namespace

extern "C"
{
    void createMonochromonMoodBubble()
    {
        auto speakerId  = readPStat(0xF7);
        auto bubbleType = readPStat(0xF8);

        if (bubbleType >= 5)
            removeObject(ObjectID::MONOCHROMON_BUBBLE, 0);
        else {
            if (scriptIdToEntityId(speakerId) == 0xFF) return;
            MONOCHROMON_BUBBLE_SPRITE.u = bubbleType * 32;
            bubbleTimer                 = 30;
            addObject(ObjectID::MONOCHROMON_BUBBLE, 0, nullptr, renderMonochromonBubble);
        }
    }
}
