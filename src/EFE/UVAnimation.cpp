#include "Loading.hpp"
#include "ModelPolygon.hpp"

namespace
{
    // Keep the per-vertex loop compact inside the overlay allocation.
    [[gnu::optimize("Os")]]
    void offsetPrimitiveUVs(const TMD_STRUCT& object, int8_t deltaU, int8_t deltaV)
    {
        auto* primitive = object.primitive_top;
        for (uint32_t i = 0; i < object.n_primitive; ++i) {
            const auto header = *primitive;
            const auto mode   = header >> 24;
            if ((mode & 4) != 0) {
                const int vertices = (mode & 8) != 0 ? 4 : 3;
                for (int vertex = 0; vertex < vertices; ++vertex) {
                    auto* uv = reinterpret_cast<uint8_t*>(primitive + 1 + vertex);
                    uv[0] += deltaU;
                    uv[1] += deltaV;
                }
            }
            primitive += modelPolygonWords(header);
        }
    }
} // namespace

extern "C"
{
    void EFE_ENTRY(initializeUVAnim)()
    {
        const auto tableAddress = EFEOperands::pop();
        const auto value        = EFEOperands::pop();
        const auto index        = EFEOperands::pop();
        auto& slot              = *EFE_CURRENT_SLOT;
        if (index < 0 || index >= slot.uvCount) {
            EFE_SCRIPT_RESULT       = -2;
            EFE_INSTRUCTION_POINTER = nullptr;
            return;
        }

        auto& anim = slot.uvRecords[index];
        anim.unk0  = value;
        if (tableAddress == -1) return;

        auto* table       = reinterpret_cast<int32_t*>(tableAddress);
        anim.numKeyframes = *table;
        anim.uvData       = reinterpret_cast<EFELoading::UVKeyframe*>(table + 1);
        anim.uv           = anim.uvData;
        anim.keyframe     = 0;
        anim.uvFrame      = 0;
        anim.frame        = 1;
        anim.numFrames    = 0;
        for (int i = 0; i < anim.numKeyframes; ++i)
            anim.numFrames += anim.uvData[i].frames;
    }

    void EFE_ENTRY(getUVAnimTimer)()
    {
        const auto index = EFEOperands::pop();
        auto* timer      = EFEOperands::popPointer<int32_t>();
        *timer           = EFE_CURRENT_SLOT->uvRecords[index].frame;
    }

    void EFE_ENTRY(tickEFEUVAnimation)(int32_t index)
    {
        const auto& slot = *EFE_CURRENT_SLOT;
        auto& anim       = slot.uvRecords[index];
        if (anim.numKeyframes == 0) return;

        if (++anim.uvFrame >= anim.uv->frames) {
            anim.uvFrame = 0;
            if (++anim.keyframe >= anim.numKeyframes) anim.keyframe = 0;
            anim.uv = &anim.uvData[anim.keyframe];
            offsetPrimitiveUVs(slot.model->modelPtr->objects[index], anim.uv->deltaU, anim.uv->deltaV);
        }
        ++anim.frame;
        anim.frame %= anim.numFrames;
    }
}
