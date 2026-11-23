#include "../Matrix.hpp"
#include "../Model.hpp"
#include "Transform.hpp"

using namespace EFETransform;

namespace
{
    Matrix setBoneOffsetPosition(EFEInstance& instance)
    {
        const auto& offset = *EFE_CURRENT_SUBEFFECT->boneOffset;
        Matrix matrix;
        calculateBoneMatrix(EFE_CURRENT_SUBEFFECT->sourceEntity, offset.boneId, &matrix);
        SVector position = {offset.x, offset.y, offset.z};
        libgte_ApplyMatrixSV(&matrix, &position, &position);
        instance.position = {matrix.t[0] + position.x, matrix.t[1] + position.y, matrix.t[2] + position.z};
        return matrix;
    }
} // namespace

extern "C"
{
    void EFE_ENTRY(setTransformToBoneOffset)()
    {
        auto& instance = *reinterpret_cast<EFEInstance*>(EFE_INSTANCE_BASE);
        setBoneOffsetPosition(instance);
        instance.rotation = {0, EFE_CURRENT_SUBEFFECT->sourceEntity->posData->rotation.y, 0};
    }

    void EFE_ENTRY(setTransformToBoneMatrix)()
    {
        auto& instance = *reinterpret_cast<EFEInstance*>(EFE_INSTANCE_BASE);
        auto matrix    = setBoneOffsetPosition(instance);
        SVector rotation;
        matrixToEuler2(&matrix, &rotation);
        writeVector(instance.rotation.data(), rotation);
    }

    void EFE_ENTRY(initializeEFETransform)()
    {
        if (EFE_PARENT_INSTANCE == nullptr) {
            EFE_ENTRY(setTransformToBoneOffset)();
            return;
        }
        auto& instance    = *reinterpret_cast<EFEInstance*>(EFE_INSTANCE_BASE);
        instance.position = EFE_PARENT_INSTANCE->position;
        instance.rotation = EFE_PARENT_INSTANCE->rotation;
    }
}
