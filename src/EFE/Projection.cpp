#include "../Math.hpp"
#include "../Matrix.hpp"
#include "Transform.hpp"

using namespace EFETransform;

extern "C"
{
    void EFE_ENTRY(getCameraRotation)()
    {
        auto* out = popPointer<int32_t>();
        Matrix inverse;
        SVector rotation;
        libgte_TransposeMatrix(&libgs_REFERENCE_MATRIX, &inverse);
        matrixToEuler2(&inverse, &rotation);
        writeVector(out, rotation);
    }

    void EFE_ENTRY(convertToViewSpace)()
    {
        auto rotation     = toShortVector(popPointer<int32_t>());
        auto* position    = popPointer<dtl::array<int32_t, 3>>();
        auto* outRotation = popPointer<int32_t>();
        auto* outPosition = popPointer<dtl::array<int32_t, 3>>();
        Matrix world, view;
        world.t = *position;
        libgte_RotMatrixYXZ(&rotation, &world);
        libgs_GsMulCoord0(&libgs_REFERENCE_MATRIX, &world, &view);
        matrixToEuler2(&view, &rotation);
        *outPosition = view.t;
        writeVector(outRotation, rotation);
    }

    void EFE_ENTRY(projectPositionToScreen)()
    {
        auto* depth    = popPointer<int32_t>();
        auto* y        = popPointer<int32_t>();
        auto* x        = popPointer<int32_t>();
        auto* position = popPointer<int32_t>();
        auto projected = getMapPosition(position[0], position[1], position[2]);
        *depth         = projected.depth >> 4;
        *x             = projected.screenX;
        *y             = projected.screenY;
    }
}
