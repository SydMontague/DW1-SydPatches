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
}