#include "../Entity.hpp"
#include "../Model.hpp"
#include "Transform.hpp"

using namespace EFETransform;

extern "C"
{
    void EFE_ENTRY(faceTargetEntity)()
    {
        const auto& sub = *EFE_CURRENT_SUBEFFECT;
        if (sub.targetEntity == nullptr) return;
        entityLookAtLocation(sub.sourceEntity, &sub.targetEntity->posData->location);
        auto& root = *sub.sourceEntity->posData;
        projectPosition(&root.posMatrix, &root.location, &root.rotation, &root.scale);
    }

    void EFE_ENTRY(copyTargetEntityPosition)()
    {
        auto* out = popPointer<dtl::array<int32_t, 3>>();
        *out      = EFE_CURRENT_SUBEFFECT->targetEntity->posData->posMatrix.work.t;
    }

    // Keep the nine-slot scan as a loop in the overlay.
    [[gnu::optimize("Os")]]
    void EFE_ENTRY(centerTransformOnEntities)()
    {
        auto& instance    = *reinterpret_cast<EFEInstance*>(EFE_INSTANCE_BASE);
        instance.position = {};
        instance.rotation = {};
        int count         = 0;
        for (int i = 1; i < ENTITY_TABLE.table.size(); ++i) {
            const auto* entity = ENTITY_TABLE.table[i];
            if (!entity || entity == EFE_CURRENT_SUBEFFECT->sourceEntity || !entity->isOnMap) continue;
            ++count;
            for (int axis = 0; axis < 3; ++axis)
                instance.position[axis] += entity->posData->posMatrix.work.t[axis];
        }
        for (auto& coordinate : instance.position)
            coordinate /= count;
    }
}
