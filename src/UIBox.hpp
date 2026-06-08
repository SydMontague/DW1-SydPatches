#pragma once
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"

inline constexpr RGB8 UIBOX_DEFAULT_COLOR = {0x2D, 0x38, 0x40};

// How a UIBox interior is filled. The blended modes use the GPU's
// semi-transparency; DOUBLE submits the fill twice for a 75/25 blend.
enum class UIBoxFill
{
    NONE,    // caller draws its own fill
    OPAQUE,
    SEMI,    // 50/50 blend
    DOUBLE,  // 75/25 blend
};

// Defined at namespace scope (rather than nested in UIBox) so its default
// member initializers can be used for a defaulted `Style style = {}` parameter;
// a nested class can't be value-initialized from within its own enclosing class.
struct UIBoxStyle
{
    RGB8      color          = UIBOX_DEFAULT_COLOR;
    UIBoxFill fill           = UIBoxFill::SEMI;
    bool      drawBorder     = true;   // false → caller draws own border
    bool      playOpenSound  = true;
    bool      playCloseSound = true;
};

struct UIBox
{
    static inline constexpr RGB8 DEFAULT_COLOR = UIBOX_DEFAULT_COLOR;
    using Fill  = UIBoxFill;
    using Style = UIBoxStyle;

    enum class State
    {
        CLOSED,
        OPENING,
        OPENED,
        CLOSING,
    };

    UIBox() = default;
    UIBox(RECT final, Style style = {}, dtl::optional<RECT> start = {});

    void open();
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

    // Geometry-only update for an already-constructed box. Preserves the
    // animation `start`, so a live resize doesn't break the close animation.
    void resize(RECT newFinal) { final = newFinal; }

protected:
    // For subclasses (e.g. LabeledUIBox) that draw over the box's own fill.
    RGB8 fillColor() const { return style.color; }

private:
    inline void renderOpeningClosing(int32_t depth);
    inline void renderOpened(int32_t depth);

    dtl::optional<RECT> start{};
    RECT final{};
    State state{UIBox::State::CLOSED};
    Style style{};
    int16_t frame{0};
};
