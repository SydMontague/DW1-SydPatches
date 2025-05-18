#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    constexpr RECT rect1{.x = 0, .y = 480, .width = 256, .height = 7};
    constexpr RECT rect2{.x = 272, .y = 480, .width = 16, .height = 1};
    constexpr RECT rect3{.x = 96, .y = 501, .width = 16, .height = 1};
    constexpr RECT rect4{.x = 224, .y = 488, .width = 16, .height = 24};

    void downloadCLUT1(uint32_t* target)
    {
        libgpu_StoreImage(&rect1, target);
        libgpu_DrawSync(0);
    }

    void fadeoutCLUT1(int32_t progress, RGB5551* clut)
    {
        dtl::array<RGB5551, 1792> bla;
        const auto factor = 255 - progress;

        for (int32_t i = 0; i < 256; i++)
        {
            RGB5551 entry = clut[i];

            bla[i].alpha = entry.alpha;
            bla[i].red   = (entry.red * factor) / 255;
            bla[i].green = (entry.green * factor) / 255;
            bla[i].blue  = (entry.blue * factor) / 255;
        }

        for (int32_t i = 1024; i < bla.size(); i++)
        {
            RGB5551 entry = clut[i];

            bla[i].alpha = entry.alpha;
            bla[i].red   = (entry.red * factor) / 255;
            bla[i].green = (entry.green * factor) / 255;
            bla[i].blue  = (entry.blue * factor) / 255;
        }

        libgpu_LoadImage(&rect1, reinterpret_cast<uint32_t*>(bla.data()));
    }

    void downloadCLUT2(uint32_t* target)
    {
        libgpu_StoreImage(&rect2, target);
        libgpu_StoreImage(&rect3, target + 8);
        libgpu_StoreImage(&rect4, target + 16);
        libgpu_DrawSync(0);
    }

    void fadeoutCLUT2(int32_t progress, RGB5551* clut)
    {
        auto factor = 255 - progress;
        dtl::array<RGB5551, 16 * 26> bla;
        for (int32_t i = 0; i < bla.size(); i++)
        {
            RGB5551 entry = clut[i];

            bla[i].alpha = entry.alpha;
            bla[i].red   = (entry.red * factor) / 255;
            bla[i].green = (entry.green * factor) / 255;
            bla[i].blue  = (entry.blue * factor) / 255;
        }

        libgpu_LoadImage(&rect2, reinterpret_cast<uint32_t*>(bla.data()));
        libgpu_LoadImage(&rect2, reinterpret_cast<uint32_t*>(bla.data() + 16));
        libgpu_LoadImage(&rect2, reinterpret_cast<uint32_t*>(bla.data() + 32));
    }
}