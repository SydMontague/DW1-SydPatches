

#include "AtlasFont.hpp"
#include "Math.hpp"
#include "UIBox.hpp"
#include "UIElements.hpp"
#include "extern/dtl/optional.hpp"
#include "extern/dtl/unique_ptr.hpp"
#include "extern/dw1.hpp"

namespace
{
    constexpr RECT final{.x = -152, .y = -98, .width = 82, .height = 33};

    constexpr RenderSettings labelSettings{
        .x        = final.x,
        .y        = final.y + 4,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = final.width,
        .height   = 12,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::TOP,
    };
    constexpr RenderSettings valueSettings{
        .x        = final.x,
        .y        = final.y + 17,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = final.width,
        .height   = 12,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::TOP,
    };

    AtlasString label;
    AtlasString value;
    bool useBits;
    int32_t cachedValue;

    void tickShopBitsBox(int32_t)
    {
        auto newValue = useBits ? MONEY : MERIT;
        if (newValue == cachedValue) return;

        cachedValue = newValue;
        value       = getAtlasVanilla().render(format("%d", newValue).data(), valueSettings);
    }

    void renderShopBitsBox(int32_t)
    {
        label.render(4);
        value.render(4);
    }

} // namespace

// TODO: when all callers have been moved to C++, convert the textbox to a regular Object with UIBox and RAII
extern "C"
{
    void createShopBitsBox(int32_t isBits)
    {
        cachedValue = -1;
        useBits     = isBits;
        label       = getAtlasVanilla().render(isBits ? "Bits" : "Merit", labelSettings);

        if (UI_BOX_DATA[2].state == 1) return;

        RECT origin;
        setupBoxOrigin(0xFD, &origin);

        createTextbox(2, 0xE1, &final, &origin, tickShopBitsBox, renderShopBitsBox);
        registerTextbox(2, 8, 1, 0, VRAMMode::FRONT_FULL);
    }
}
