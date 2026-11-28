#include "Loading.hpp"

#include "../extern/dtl/algorithm.hpp"
#include "../extern/dtl/array.hpp"
#include "../extern/libgpu.hpp"

namespace
{
    constexpr int PALETTE_SIZE = 16;

    struct ColorComponents
    {
        uint16_t red;
        uint16_t green;
        uint16_t blue;
        uint16_t stp;
    };
} // namespace

extern "C"
{
    [[gnu::optimize("Os")]]
    void EFE_ENTRY(loadClutColors)()
    {
        const auto* source = EFEOperands::popPointer<const ColorComponents>();
        const auto count   = EFEOperands::pop();
        const auto x       = EFEOperands::pop();
        const auto y       = EFEOperands::pop();
        if (count <= 0 || count > PALETTE_SIZE) return;

        alignas(uint32_t) dtl::array<RGB5551, PALETTE_SIZE> colors;
        for (int i = 0; i < count; ++i) {
            const auto& color = source[i];
            colors[i]         = {.red = color.red, .green = color.green, .blue = color.blue, .alpha = color.stp};
        }

        const auto page = static_cast<uint16_t>(EFE_CURRENT_SLOT->model->clutPage);
        alignas(uint32_t) const RECT rect{
            .x      = static_cast<int16_t>(((page & 63) << 4) + x),
            .y      = static_cast<int16_t>((page >> 6) + y),
            .width  = static_cast<int16_t>(count),
            .height = 1,
        };
        libgpu_LoadImage(&rect, colors.data());
        libgpu_DrawSync(0);
    }

    void EFE_ENTRY(addClutLoadPrim)()
    {
        const auto index   = EFEOperands::pop();
        const auto* colors = EFEOperands::popPointer<const RGB5551>();
        const auto y       = EFEOperands::pop();
        const auto depth   = EFEOperands::pop();

        auto* packet = static_cast<DR_LOAD*>(libgs_GsGetWorkBase());
        libgs_GsSetWorkBase(packet + 1);
        const auto page = static_cast<uint16_t>(EFE_CURRENT_SLOT->model->clutPage);
        alignas(uint32_t) const RECT rect{
            .x      = static_cast<int16_t>((page & 63) << 4),
            .y      = static_cast<int16_t>(((page >> 6) & 511) + y),
            .width  = PALETTE_SIZE,
            .height = 1,
        };
        libgpu_SetDrawLoad(packet, &rect);
        const auto* firstColor = colors + index;
        dtl::copy(firstColor, firstColor + PALETTE_SIZE, packet->data);
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + (depth >> 4), packet);
    }
}
