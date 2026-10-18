#include "Textbox.hpp"

#include "Helper.hpp"
#include "Input.hpp"
#include "Math.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"

namespace
{
    constexpr int16_t TEXTBOX_INSET_X     = 5;
    constexpr int16_t TEXTBOX_INSET_Y     = 4;
    constexpr int16_t TEXTBOX_LINE_HEIGHT = 13;

    constexpr dtl::array<Sprite, 3> INDICATOR_FRAMES = {{
        {
            .uvX          = 0x4c,
            .uvV          = 0xb7,
            .width        = 16,
            .height       = 12,
            .texture_page = getTPage(0, 1, 320, 128),
            .clut         = getClut(96, 500),
        },
        {
            .uvX          = 0x5c,
            .uvV          = 0xb7,
            .width        = 16,
            .height       = 12,
            .texture_page = getTPage(0, 1, 320, 128),
            .clut         = getClut(96, 500),
        },
        {
            .uvX          = 0x6c,
            .uvV          = 0xb7,
            .width        = 16,
            .height       = 12,
            .texture_page = getTPage(0, 1, 320, 128),
            .clut         = getClut(96, 500),
        },
    }};
} // namespace

Textbox::Textbox(RECT bounds,
                 const TextboxLine* lines,
                 uint32_t lineCount,
                 dtl::optional<RECT> origin,
                 RGB8 color,
                 bool semiTrans)
    : frame(bounds, color, semiTrans, origin)
    , bounds(bounds)
    , origin(origin)
{
    replacePage(lines, lineCount);
}

void Textbox::replacePage(const TextboxLine* lines, uint32_t lineCount)
{
    text = dtl::runtime_array<AtlasString>(lineCount);
    for (uint32_t i = 0; i < lineCount; ++i)
        text[i] =
            getAtlasVanilla().render(lines[i].text,
                                     {.x = static_cast<int16_t>(bounds.x + TEXTBOX_INSET_X),
                                      .y = static_cast<int16_t>(bounds.y + TEXTBOX_INSET_Y + i * TEXTBOX_LINE_HEIGHT),
                                      .baseClut = 1,
                                      .color    = lines[i].color});
}

void Textbox::tick()
{
    frame.tick();
}

void Textbox::close(dtl::optional<RECT> target)
{
    if (frame.getState() == UIBox::State::CLOSING || isClosed()) return;
    if (target.has_value())
        frame.closeTo(target.value());
    else if (origin.has_value())
        frame.closeTo(origin.value());
    else
        frame.close();
    text = {};
}

bool Textbox::isOpened()
{
    return frame.getState() == UIBox::State::OPENED;
}

bool Textbox::isClosed()
{
    return frame.getState() == UIBox::State::CLOSED;
}

void Textbox::renderText(int32_t depth)
{
    if (!isOpened()) return;
    for (const auto& entry : text)
        entry.render(depth);
}

void Textbox::render(int32_t depth)
{
    renderText(depth);
    frame.render(depth);
}

bool ConfirmTextbox::tick()
{
    if (isOpened()) ++indicatorCounter;
    Textbox::tick();
    return isOpened() && isKeyDownPolled(InputButtons::BUTTON_CROSS);
}

void ConfirmTextbox::render(int32_t depth)
{
    renderText(depth);
    if (isOpened())
        INDICATOR_FRAMES[indicatorCounter / 4 % 3].render(bounds.x + bounds.width - 23,
                                                          bounds.y + bounds.height - 16,
                                                          depth,
                                                          0x40);
    frame.render(depth);
}

SelectTextbox::SelectTextbox(RECT bounds,
                             const TextboxLine* lines,
                             uint32_t lineCount,
                             dtl::runtime_array<uint32_t> options,
                             dtl::optional<RECT> origin,
                             RGB8 color,
                             bool semiTrans,
                             uint32_t initial)
    : Textbox(bounds, lines, lineCount, origin, color, semiTrans)
    , options(dtl::move(options))
    , selected(this->options.size() == 0 ? 0 : dtl::min(initial, this->options.size() - 1))
{
    for (const auto row : this->options)
        cursorWidth = dtl::max(cursorWidth, this->text[row].getWidth() + 2);
}

dtl::optional<uint32_t> SelectTextbox::tick()
{
    Textbox::tick();
    if (!isOpened() || options.size() == 0) return {};
    if (isKeyDownPolled(InputButtons::BUTTON_CROSS)) return selected;
    if (isKeyDownPolled(InputButtons::BUTTON_UP)) {
        selected = ring(static_cast<int32_t>(selected) - 1, 0, static_cast<int32_t>(options.size()));
        playSound(0, 2);
    }
    else if (isKeyDownPolled(InputButtons::BUTTON_DOWN)) {
        selected = ring(static_cast<int32_t>(selected) + 1, 0, static_cast<int32_t>(options.size()));
        playSound(0, 2);
    }
    return {};
}

void SelectTextbox::render(int32_t depth)
{
    renderText(depth);
    if (isOpened() && options.size() != 0)
        renderSelectionCursor(bounds.x + TEXTBOX_INSET_X - 1,
                              bounds.y + TEXTBOX_INSET_Y + selectedRow() * TEXTBOX_LINE_HEIGHT - 1,
                              cursorWidth,
                              TEXTBOX_LINE_HEIGHT,
                              depth);
    frame.render(depth);
}
