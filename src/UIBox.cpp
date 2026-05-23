#include "UIBox.hpp"

#include "Math.hpp"
#include "UIElements.hpp"

UIBox::UIBox(RECT final, RGB8 color, bool isSemiTrans, dtl::optional<RECT> start)
    : final(final)
    , start(start)
    , color(color)
    , semiTrans(isSemiTrans)
    , state(start.has_value() ? State::OPENING : State::OPENED)
{
}

void UIBox::close()
{
    state = State::CLOSING;
}
void UIBox::open()
{
    state = State::OPENING;
}

UIBox::State UIBox::getState()
{
    return state;
}

void UIBox::tick()
{
    switch (state)
    {
        case State::OPENED:
        case State::CLOSED: break;
        case State::CLOSING:
        {
            if (!start.has_value() || frame-- <= 0) state = State::CLOSED;
            break;
        }
        case State::OPENING:
        {
            if (!start.has_value() || frame++ >= 4) state = State::OPENED;
            break;
        }
    }
}

void UIBox::render(int32_t depth)
{
    switch (state)
    {
        case State::CLOSED: return;
        case State::OPENED: renderOpened(depth); break;
        case State::OPENING:
        case State::CLOSING: renderOpeningClosing(depth); break;
    }
}

void UIBox::renderOpeningClosing(int32_t depth)
{
    if (!start.has_value()) return;

    auto startMinX = start.value().x;
    auto startMaxX = start.value().x + start.value().width;
    auto startMinY = start.value().y;
    auto startMaxY = start.value().y + start.value().height;

    auto finalMinX = final.x;
    auto finalMaxX = final.x + final.width;
    auto finalMinY = final.y;
    auto finalMaxY = final.y + final.height;

    auto x1 = lerp(startMinX, finalMinX, 0, 4, frame);
    auto x2 = lerp(startMaxX, finalMaxX, 0, 4, frame);
    auto y1 = lerp(startMinY, finalMinY, 0, 4, frame);
    auto y2 = lerp(startMaxY, finalMaxY, 0, 4, frame);

    drawLine3P(0x808080, x1, y1, x2, y1, x2, y2, depth, 0);
    drawLine3P(0x808080, x2, y2, x1, y2, x1, y1, depth, 0);
}

void UIBox::renderOpened(int32_t depth)
{
    renderUIBoxBorder(&final, depth);

    GsBOXF box;
    box.attribute = semiTrans != 0 ? 0x40000000 : 0;
    box.r         = color.red;
    box.g         = color.green;
    box.b         = color.blue;
    box.x         = final.x + 4;
    box.y         = final.y + 3;
    box.width     = final.width - 8;
    box.height    = final.height - 3;
    libgs_GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, depth);
}