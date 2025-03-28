#include "Helper.hpp"
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

Pair<Entity*, uint8_t> getEntityFromScriptId(uint8_t scriptId);
void renderDigiviceEntity(Entity* entity, int32_t entityId, int32_t refX);
bool hasAttackEquipped(DigimonEntity* entity);