#include "Loading.hpp"

namespace
{
    // TMD polygon sizes in words, by mode bit 0, triangle/quad and flat/Gouraud shading.
    // GsLinkObject4 overwrites the first packet's length in each run with the run count.
    constexpr uint8_t texturedPrimitiveWords[2][2][2] = {
        {{6, 7}, {8, 9}},
        {{7, 9}, {8, 11}},
    };

    constexpr uint8_t untexturedPrimitiveWords[2][2][2] = {
        {{4, 5}, {5, 6}},
        {{4, 6}, {4, 7}},
    };

    // Keep the per-vertex loop compact inside the overlay allocation.
    [[gnu::optimize("Os")]]
    void offsetPrimitiveUVs(const TMD_STRUCT& object, int8_t deltaU, int8_t deltaV)
    {
        auto* primitive = object.primitive_top;
        for (uint32_t i = 0; i < object.n_primitive; ++i) {
            const auto header  = *primitive;
            const auto mode    = header >> 24;
            const bool raw     = (mode & 1) != 0;
            const bool quad    = (mode & 8) != 0;
            const bool gouraud = (mode & 0x10) != 0;
            const int vertices = 3 + quad;
            if ((mode & 4) == 0) {
                const auto flags = header >> 16;
                primitive += untexturedPrimitiveWords[raw][quad][gouraud];
                // Gradated lit polygons store a color for each vertex instead of one for the face.
                if (!raw && (flags & 4) != 0) primitive += vertices - 1;
                continue;
            }

            for (int vertex = 0; vertex < vertices; ++vertex) {
                auto* uv = reinterpret_cast<uint8_t*>(primitive + 1 + vertex);
                uv[0] += deltaU;
                uv[1] += deltaV;
            }
            primitive += texturedPrimitiveWords[raw][quad][gouraud];
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
