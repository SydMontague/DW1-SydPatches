#include "../Math.hpp"
#include "Operands.hpp"

extern "C"
{
    [[gnu::optimize("Os")]]
    void EFE_ENTRY(findHitEntity)()
    {
        auto* extents      = EFEOperands::popPointer<const int32_t>();
        const int32_t mode = EFEOperands::pop();
        auto* out          = EFEOperands::popPointer<int32_t>();
        *out               = 0;

        const auto* instance = reinterpret_cast<const EFEInstance*>(EFE_INSTANCE_BASE);
        auto center          = EFEHelpers::toShortVector(instance->position.data());
        AABB box{.center = &center, .extent = EFEHelpers::toShortVector(extents)};
        for (EFE_HIT_ENTITY_INDEX = 1;; ++EFE_HIT_ENTITY_INDEX) {
            EFE_HIT_ENTITY_INDEX = findAABBHitEntity(&box, EFE_CURRENT_SUBEFFECT->sourceEntity, EFE_HIT_ENTITY_INDEX);
            if (EFE_HIT_ENTITY_INDEX == -1) return;
            if (mode == 0) {
                const auto* entity = static_cast<DigimonEntity*>(ENTITY_TABLE.table[EFE_HIT_ENTITY_INDEX]);
                if (entity->stats.isHit) continue;
                *out = 1;
            }
            else if (mode == 1) {
                *out = 1;
            }
            return;
        }
    }

    void EFE_ENTRY(setTargetToHitEntity)()
    {
        EFE_CURRENT_SUBEFFECT->targetEntity = ENTITY_TABLE.table[EFE_HIT_ENTITY_INDEX];
    }
}
