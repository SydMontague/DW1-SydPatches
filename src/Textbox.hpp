#pragma once

#include "AtlasFont.hpp"
#include "UIBox.hpp"

// Text must be non-null and NUL-terminated. Descriptors and strings are borrowed only for the call.
struct TextboxLine
{
    const char* text = "";
    RGB8 color       = TEXT_COLOR_WHITE;
};

// Owners retain the concrete type; textboxes own the rendered glyphs.
class Textbox
{
public:
    Textbox(RECT bounds,
            const TextboxLine* lines,
            uint32_t lineCount,
            dtl::optional<RECT> origin = {},
            RGB8 color                 = UIBox::DEFAULT_COLOR,
            bool semiTrans             = false);
    void tick();
    void close(dtl::optional<RECT> target = {});
    void replacePage(const TextboxLine* lines, uint32_t lineCount);
    void render(int32_t depth);
    bool isOpened();
    bool isClosed();

protected:
    void renderText(int32_t depth);
    UIBox frame;
    RECT bounds;
    dtl::optional<RECT> origin;
    dtl::runtime_array<AtlasString> text;
};

class ConfirmTextbox : public Textbox
{
public:
    using Textbox::Textbox;
    bool tick();
    void render(int32_t depth);

private:
    uint8_t indicatorCounter{};
};

class SelectTextbox : public Textbox
{
public:
    // Each option names a valid logical row; the widest option determines the cursor width.
    SelectTextbox(RECT bounds,
                  const TextboxLine* lines,
                  uint32_t lineCount,
                  dtl::runtime_array<uint32_t> options,
                  dtl::optional<RECT> origin = {},
                  RGB8 color                 = UIBox::DEFAULT_COLOR,
                  bool semiTrans             = false,
                  uint32_t initial           = 0);
    dtl::optional<uint32_t> tick();
    void render(int32_t depth);
    uint32_t hoveredOption() const { return selected; }

private:
    using Textbox::replacePage;
    uint32_t selectedRow() const { return options[selected]; }
    dtl::runtime_array<uint32_t> options;
    uint32_t selected;
    int32_t cursorWidth{};
};
