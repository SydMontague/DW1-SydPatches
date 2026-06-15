#pragma once
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"

struct UIBox
{
    static inline constexpr RGB8 DEFAULT_COLOR = {0x2D, 0x38, 0x40};

    enum class State
    {
        CLOSED,
        OPENING,
        OPENED,
        CLOSING,
    };

    UIBox() = default;
    UIBox(RECT final, RGB8 color, bool isSemiTrans, dtl::optional<RECT> start = {});

    void close();
    void open();
    void tick();
    void render(int32_t depth);
    State getState();

private:
    inline void renderOpeningClosing(int32_t depth);
    inline void renderOpened(int32_t depth);

    dtl::optional<RECT> start{};
    RECT final{};
    State state{UIBox::State::CLOSED};
    RGB8 color{};
    bool semiTrans{false};
    int16_t frame{0};
};
