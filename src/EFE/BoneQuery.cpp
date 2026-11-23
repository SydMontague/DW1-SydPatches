#include "../Matrix.hpp"
#include "../Model.hpp"
#include "Transform.hpp"

using namespace EFETransform;

namespace
{
    void writeBoneTransform(Matrix& matrix, dtl::array<int32_t, 3>& positionOut, int32_t* rotationOut)
    {
        SVector rotation;
        matrixToEuler2(&matrix, &rotation);
        writeVector(rotationOut, rotation);
        positionOut = matrix.t;
    }
} // namespace

extern "C"
{
    void EFE_ENTRY(getSourceBoneTransform)()
    {
        auto* rotationOut = popPointer<int32_t>();
        auto* positionOut = popPointer<dtl::array<int32_t, 3>>();
        auto bone         = pop();
        Matrix matrix;
        calculateBoneMatrix(EFE_CURRENT_SUBEFFECT->sourceEntity, bone, &matrix);
        writeBoneTransform(matrix, *positionOut, rotationOut);
    }

    void EFE_ENTRY(getTargetBoneTransform)()
    {
        auto* rotationOut = popPointer<int32_t>();
        auto* positionOut = popPointer<dtl::array<int32_t, 3>>();
        auto bone         = pop();
        auto* nodes       = EFE_CURRENT_SUBEFFECT->targetEntity->posData;
        // Vanilla refreshes the root's local transform while retaining its cached-coordinate flag.
        auto& root = nodes[0];
        libgte_RotMatrix(&root.rotation, &root.posMatrix.coord);
        libgte_ScaleMatrix(&root.posMatrix.coord, &root.scale);
        libgte_TransMatrix(&root.posMatrix.coord, &root.location);
        Matrix matrix;
        calculatePosition(&nodes[bone].posMatrix, &matrix);
        writeBoneTransform(matrix, *positionOut, rotationOut);
    }
}
