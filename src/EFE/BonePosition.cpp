#include "../Math.hpp"
#include "Transform.hpp"

using namespace EFETransform;

namespace
{
    void placeAtBone(const Entity& entity, int32_t bone, int32_t distance)
    {
        auto& instance       = *reinterpret_cast<EFEInstance*>(EFE_INSTANCE_BASE);
        const auto& position = entity.posData[bone].posMatrix.work.t;
        auto yaw             = entity.posData->rotation.y;
        auto phase           = yaw >> 3;
        instance.rotation    = {0, yaw, 0};
        instance.position    = {position[0] + scaledComponent(distance, sin(-phase)),
                                position[1],
                                position[2] + scaledComponent(distance, sin(-((phase + 128) & 511)))};
    }
} // namespace

extern "C"
{
    void EFE_ENTRY(setTransformToSourceBone)()
    {
        placeAtBone(*EFE_CURRENT_SUBEFFECT->sourceEntity, pop(), 25);
    }

    void EFE_ENTRY(setTransformToTargetBone)()
    {
        auto radius = pop();
        placeAtBone(*EFE_CURRENT_SUBEFFECT->targetEntity, pop(), radius);
    }

    void EFE_ENTRY(getScatteredSpawnPosition)()
    {
        auto radius      = pop();
        auto* out        = popPointer<int32_t>();
        const auto& sub  = *EFE_CURRENT_SUBEFFECT;
        const auto& base = EFE_PARENT_INSTANCE ? EFE_PARENT_INSTANCE->position
                                               : sub.sourceEntity->posData[sub.boneOffset->boneId].posMatrix.work.t;
        for (int i = 0; i < 3; ++i)
            out[i] = base[i];
        if (radius > 0) {
            for (int i = 0; i < 3; ++i)
                out[i] += random2(radius) - (radius >> 1);
        }
    }
}
