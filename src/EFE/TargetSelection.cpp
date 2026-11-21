#include "Operands.hpp"

namespace
{
    // The combat roster starts after the tamer in slot zero.
    constexpr int32_t firstDigimon = 1;

    inline DigimonEntity* getLivingDigimon(int32_t index)
    {
        auto* entity = static_cast<DigimonEntity*>(ENTITY_TABLE.table[index]);
        if (entity == nullptr || !entity->isOnMap || entity->stats.currentHP <= 0) return nullptr;
        return entity;
    }
} // namespace

extern "C"
{
    void EFE_ENTRY(selectNextTargetEntity)()
    {
        auto* out = EFEOperands::popPointer<int32_t>();
        for (; EFE_TARGET_INDEX < static_cast<int32_t>(ENTITY_TABLE.table.size()); ++EFE_TARGET_INDEX) {
            auto* entity = getLivingDigimon(EFE_TARGET_INDEX);
            if (entity == nullptr || entity == EFE_CURRENT_SUBEFFECT->sourceEntity || !entity->isOnScreen) continue;

            EFE_CURRENT_SUBEFFECT->targetEntity = entity;
            *out                                = EFE_TARGET_INDEX++;
            return;
        }
        *out = -1;
    }

    [[gnu::optimize("Os")]]
    void EFE_ENTRY(selectRandomTargetEntity)()
    {
        const bool includeSource = EFEOperands::pop() != 0;
        int32_t remaining        = rand() % 8u + 1;
        while (true) {
            if (++EFE_TARGET_INDEX >= static_cast<int32_t>(ENTITY_TABLE.table.size())) EFE_TARGET_INDEX = firstDigimon;
            auto* entity = getLivingDigimon(EFE_TARGET_INDEX);
            if (entity == nullptr || (!includeSource && entity == EFE_CURRENT_SUBEFFECT->sourceEntity)) continue;

            if (--remaining == 0) {
                EFE_CURRENT_SUBEFFECT->targetEntity = entity;
                return;
            }
        }
    }
}
