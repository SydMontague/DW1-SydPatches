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

    struct Style
    {
        RGB8 color           = {0x2D, 0x38, 0x40}; // keep in sync with DEFAULT_COLOR
        bool semiTrans       = true;
        bool doubleFill      = false;   // submit fill twice for 75/25 blend
        bool drawBorder      = true;    // false → caller draws own border
        bool drawFill        = true;    // false → caller draws own fill
        bool playOpenSound   = true;
        bool playCloseSound  = true;
    };

    UIBox() = default;
    explicit UIBox(Style style);
    UIBox(RECT final);
    UIBox(RECT final, Style style);
    UIBox(RECT final, Style style, dtl::optional<RECT> start);
    // Back-compat for callers predating Style; preserves no-sound behavior.
    UIBox(RECT final, RGB8 color, bool isSemiTrans, dtl::optional<RECT> start = {});

    void open();
    void open(RECT final, RECT start);
    void close();
    void close(RECT target);
    void closeIfOpen();   // No-op unless currently OPENED; matches old closeUIBoxIfOpen

    void tick();
    void render(int32_t depth);

    State getState() const { return state; }
    bool isOpen()      const { return state == State::OPENED; }
    bool isClosed()    const { return state == State::CLOSED; }
    bool isAnimating() const { return state == State::OPENING || state == State::CLOSING; }

    const RECT& finalPos() const { return final; }
    void resize(RECT newFinal) { final = newFinal; }

private:
    inline void renderOpeningClosing(int32_t depth);
    inline void renderOpened(int32_t depth);

    dtl::optional<RECT> start{};
    RECT final{};
    State state{UIBox::State::CLOSED};
    Style style{};
    int16_t frame{0};
};
