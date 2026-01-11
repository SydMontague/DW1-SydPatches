#include "Helper.hpp"
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

Pair<Entity*, uint8_t> getEntityFromScriptId(uint8_t scriptId);
void renderDigiviceEntity(Entity* entity, int32_t entityId, int32_t refX);
bool hasAttackEquipped(DigimonEntity* entity);

extern "C"
{
    void getModelTile(Vector* location, int16_t* tileX, int16_t* tileY);
    bool entityIsInEntity(Entity* self, Entity* other);
    void entityLookAtLocation(Entity* entity, Vector* location);
    CollisionCode entityCheckCollision(Entity* ignore, Entity* self, int32_t width, int32_t height);
    void collisionGrace(Entity* ignore, Entity* self, int32_t dx, int32_t dy);
    bool isLinearPathBlocked(int32_t tileX1, int32_t tileY1, int32_t tileX2, int32_t tileY2);
    bool tickEntityWalkTo(uint32_t scriptId1, uint32_t scriptId2, int32_t targetX, int32_t targetZ, bool withCamera);
    void addEntityText(DigimonEntity* entity, int32_t entityId, int8_t color, int32_t value, int8_t icon);
    uint8_t entityGetTechFromAnim(DigimonEntity* entity, uint8_t move);
    void entityLookAtTile(Entity* entity, int8_t tileX, int8_t tileY);
    bool isInvisible(Entity* entity);
    bool entityIsOffScreen(Entity* entity, int16_t width, int16_t height);
}