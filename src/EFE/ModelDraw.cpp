#include "Loading.hpp"

#include "../extern/psx.hpp"

namespace
{
    enum class ModelDraw
    {
        World,
        WorldYXZ,
        ScreenSpace,
    };

    void drawModel(ModelDraw mode)
    {
        auto* scale          = EFEOperands::popPointer<Vector>();
        const auto objectId  = EFEOperands::pop();
        const auto& instance = *reinterpret_cast<const EFEInstance*>(EFE_INSTANCE_BASE);
        auto rotation        = EFEHelpers::toShortVector(instance.rotation.data());

        Matrix transform;
        if (mode == ModelDraw::World)
            libgte_RotMatrix(&rotation, &transform);
        else
            libgte_RotMatrixYXZ(&rotation, &transform);
        libgte_ScaleMatrix(&transform, scale);
        transform.t = instance.position;

        if (mode == ModelDraw::ScreenSpace) {
            transform.t[0] += 160 - DRAWING_OFFSET_X;
            transform.t[1] += 120 - DRAWING_OFFSET_Y;

            Matrix inverseView, light;
            libgte_TransposeMatrix(&libgs_REFERENCE_MATRIX, &inverseView);
            libgte_MulMatrix0(&inverseView, &transform, &light);
            libgs_GsSetLightMatrix(&light);
            libgs_GsSetLsMatrix(&transform);
        }
        else {
            Matrix view;
            libgs_GsMulCoord0(&libgs_REFERENCE_MATRIX, &transform, &view);
            if (view.t[2] < -300 || view.t[2] > 65535) return;
            libgs_GsSetLightMatrix(&transform);
            libgs_GsSetLsMatrix(&view);
        }

        auto* objects = EFE_CURRENT_SLOT->model->modelPtr->objects;
        GsDOBJ2 object;
        libgs_GsLinkObject4(reinterpret_cast<uint32_t*>(objects), &object, objectId);
        object.coord2    = nullptr;
        object.attribute = 0;
        libgs_GsSortObject4(&object, ACTIVE_ORDERING_TABLE, 2, SCRATCHPAD.data() + 44);
    }
} // namespace

extern "C"
{
    void EFE_ENTRY(drawTMD)()
    {
        drawModel(ModelDraw::World);
    }

    void EFE_ENTRY(drawTMDYXZ)()
    {
        drawModel(ModelDraw::WorldYXZ);
    }

    void EFE_ENTRY(drawTMDScreenSpace)()
    {
        drawModel(ModelDraw::ScreenSpace);
    }
}
