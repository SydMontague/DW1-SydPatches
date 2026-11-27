#include "Loading.hpp"
#include "ModelPolygon.hpp"

namespace
{
    struct FaceColor
    {
        RGB8 rgb;
        uint8_t flags;
    };
} // namespace

extern "C"
{
    void EFE_ENTRY(setTMDObjectColor)(int32_t index, const int32_t* color, TMDModel* model)
    {
        const auto& object = model->objects[index];
        auto* primitive    = object.primitive_top;
        const RGB8 rgb{static_cast<uint8_t>(color[0]), static_cast<uint8_t>(color[1]), static_cast<uint8_t>(color[2])};
        for (uint32_t i = 0; i < object.n_primitive; ++i) {
            const auto header = *primitive;
            const auto mode   = header >> 24;
            if (mode == 0x2D || mode == 0x2F) {
                // These textured quads store one face color after their four UV words.
                auto* face = reinterpret_cast<FaceColor*>(primitive + 5);
                // EFEDAT uses bit 0 of the spare fourth color byte to keep a face's authored RGB.
                if ((face->flags & 1) == 0) face->rgb = rgb;
            }
            // Vanilla advanced only for modes 0x2D and 0x2F, so a different polygon left later quads uncolored.
            primitive += modelPolygonWords(header);
        }
    }

    void EFE_ENTRY(setEFEModelObjectColor)()
    {
        const auto* color = EFEOperands::popPointer<int32_t>();
        const auto index  = EFEOperands::pop();
        EFE_ENTRY(setTMDObjectColor)(index, color, EFE_CURRENT_SLOT->model->modelPtr);
    }
}
