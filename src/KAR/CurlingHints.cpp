#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../AtlasFont.hpp"
#include "../Input.hpp"
#include "../Sound.hpp"
#include "../UIElements.hpp"
#include "../extern/dtl/unique_ptr.hpp"
#include "../extern/dw1.hpp"

namespace
{
    constexpr dtl::array<const char*, 2> PROMPT_STRINGS = {"Start Game", "Explain Game"};
    constexpr dtl::array<uint8_t, 10> HINT_OFFSETS      = {0, 12, 20, 24, 40, 44, 48, 52, 56, 60};

    constexpr dtl::array<const char*, 60> PENGUINMON_HINTS = {{
        " Penguinmon ",
        "First of all, please choose",
        "your curling stone.",
        " ",
        " Penguinmon ",
        "There are heavy, medium, and",
        "light ones, so think about the",
        "best order to use them.",
        " Penguinmon ",
        "Some stones have four pegs.",
        "They stick to where they land",
        "and can become very useful.",
        " Penguinmon ",
        "Move the Directional Pad ",
        "left and right to choose the ",
        "direction of your throw.",
        " Penguinmon ",
        "Move screen up or down with",
        "Dir. Pad. Use X button to",
        "select throw direction.",
        " Penguinmon ",
        "Press X button when power",
        "gauge reaches the desired",
        "level to pick throw strength.",
        " Penguinmon ",
        "If your curling stone lands",
        "and touches the GOOD mark,",
        "you get two points.",
        " Penguinmon ",
        "If you get it on the blue",
        "line, but outside the center",
        "GOOD mark, one point.",
        " Penguinmon ",
        "If you touch the BAD on",
        "bottom right, you'll lose",
        "two points, so be careful.",
        " Penguinmon ",
        "And, in the case of a tie,",
        "I win!",
        "So let's start the game!!",
        " Penguinmon ",
        " ",
        "You're not that great!",
        " ",
        " Penguinmon ",
        "The battle is just beginning!",
        " ",
        " ",
        " Penguinmon ",
        "Hey you're pretty good!",
        " ",
        " ",
        " Penguinmon ",
        "Yeah! I won!",
        " ",
        " ",
        " Penguinmon ",
        "Awh, I lost. ",
        " ",
        " ",
    }};

    constexpr dtl::array<const char*, 60> METALMAMEMON_HINTS = {{
        " MetalMamemon ",
        "Yeah. You gotta first pick",
        "the type of curling stone.",
        "",
        " MetalMamemon ",
        "I'm a genius, so I can use",
        "various stones in the right",
        "order. It's key to winning.",
        " MetalMamemon ",
        "Stones with four pegs stop",
        "and stay where they land,",
        "so they are very valuable.",
        " MetalMamemon ",
        "Choose the direction of throw",
        "with the Directional Pad,",
        "by moving it left and right.",
        " MetalMamemon ",
        "You can scroll screen moving",
        "Directional Pad up and down.",
        "Press X button to throw it.",
        " MetalMamemon ",
        "Press X button when power",
        "gauge reaches the desired ",
        "level to pick throw strength.",
        " MetalMamemon ",
        "Points: If you get the",
        "stone on the GOOD mark,",
        "you get two points.",
        " MetalMamemon ",
        "If you get it on the blue",
        "line, but outside the center",
        "GOOD mark, one point.",
        " MetalMamemon ",
        "If you get on right bottom",
        "BAD mark, it will be minus",
        "two points, so be careful.",
        " MetalMamemon ",
        "In the case of a tie, I win!",
        "Start the game!!",
        "",
        " MetalMamemon ",
        "I'll win!",
        "",
        "",
        " MetalMamemon ",
        "I haven't lost yet!",
        "",
        "",
        " MetalMamemon ",
        "I can't lose this match!",
        "",
        "",
        " MetalMamemon ",
        "Yeah, I won!",
        "",
        "",
        " MetalMamemon ",
        "Awh, I almost won.",
        "",
        "",
    }};

    constexpr RECT FINAL_RECT        = {-130, 42, 262, 59};
    constexpr RECT PROMPT_START_RECT = {75, -5, 10, 10};
    constexpr RECT HINT_START_RECT   = {0, 0, 10, 10};
    constexpr int32_t LINES_PER_PAGE = 4;
    constexpr int32_t MAX_HINT_LINES = 16;
    constexpr int32_t LINE_HEIGHT    = 13;
    constexpr int32_t TEXT_DEPTH     = 6;
    // Modulating the white atlas (31) by 108/128 reproduces vanilla's 5-bit white (26).
    constexpr RGB8 HINT_WHITE = {216, 216, 216};
    constexpr RGB8 HINT_TITLE = {255, 255, 0};

    class CurlingText
    {
        dtl::array<AtlasString, MAX_HINT_LINES> lines{};
        uint8_t preparedLines = 0;

        void prepareLine(const char* string, int32_t index, int16_t x, int16_t y, RGB8 color)
        {
            // Blank lines have no visible glyphs and need neither an allocation nor setPosition.
            if (string[0] == '\0' || (string[0] == ' ' && string[1] == '\0')) return;
            lines[index] = getAtlasVanilla().render(string, {.x = x, .y = y, .baseClut = 1, .color = color});
        }

    public:
        void preparePromptLine(int32_t line)
        {
            prepareLine(PROMPT_STRINGS[line], line, -124, 49 + line * LINE_HEIGHT, HINT_WHITE);
        }

        void prepareHintLine(int32_t page, bool penguinmon)
        {
            const int32_t start = HINT_OFFSETS[page];
            if (preparedLines == HINT_OFFSETS[page + 1] - start) return;
            const auto& strings = penguinmon ? PENGUINMON_HINTS : METALMAMEMON_HINTS;
            const int32_t row   = preparedLines % LINES_PER_PAGE;
            prepareLine(strings[start + preparedLines],
                        preparedLines,
                        FINAL_RECT.x + 4,
                        FINAL_RECT.y + 3 + row * LINE_HEIGHT,
                        row == 0 ? HINT_TITLE : HINT_WHITE);
            preparedLines++;
        }

        void renderPrompt() const
        {
            lines[0].render(TEXT_DEPTH);
            lines[1].render(TEXT_DEPTH);
        }

        void renderPage(int32_t page) const
        {
            for (int32_t line = 0; line < LINES_PER_PAGE; line++)
                lines[page * LINES_PER_PAGE + line].render(TEXT_DEPTH);
        }
    };

    // The overlay loader runs no destructors. Release explicitly before each UI close,
    // repeated start and normal match exit; no text allocation survives script resume.
    constinit dtl::unique_ptr<CurlingText> text;

} // namespace

void KAR_resetHints()
{
    text.reset();
}

[[gnu::optimize("Os")]]
int32_t KAR_tickYesNoPrompt()
{
    if (KAR_HINT_STATE < 2) {
        const int32_t line = KAR_HINT_STATE;
        if (!text) text = dtl::make_unique<CurlingText>();
        text->preparePromptLine(line);
        KAR_HINT_STATE++;
    }

    if (UI_BOX_DATA[0].state == 0) {
        if (KAR_HINT_STATE == 2) {
            createAnimatedUIBox(0, 0, 2, &FINAL_RECT, &PROMPT_START_RECT, nullptr, nullptr);
            KAR_HINT_STATE = 3;
        }
        else if (KAR_HINT_STATE == 3 || KAR_HINT_STATE == 4) {
            KAR_resetHints();
            return KAR_HINT_STATE - 2;
        }
    }

    if (UI_BOX_DATA[0].state == 1) {
        if (isKeyDownPolled(InputButtons::BUTTON_CROSS)) {
            removeAnimatedUIBox(0, nullptr);
            KAR_resetHints();
            return 0;
        }
        if (isKeyDownPolled(InputButtons::BUTTON_UP)) {
            KAR_HINT_STATE = KAR_HINT_STATE == 3 ? 4 : 3;
            playSound(0, 2);
        }
        else if (isKeyDownPolled(InputButtons::BUTTON_DOWN)) {
            KAR_HINT_STATE = KAR_HINT_STATE == 3 ? 4 : 3;
            playSound(0, 2);
        }
        if (text) text->renderPrompt();
        renderSelectionCursor(FINAL_RECT.x + 6, FINAL_RECT.y + 7 + (KAR_HINT_STATE - 3) * 13, 94, 13, 6);
    }
    return 0;
}

[[gnu::optimize("Os")]]
int32_t KAR_tickHintBox(int32_t page)
{
    if (KAR_HINT_STATE == 0) return 0;
    const int8_t mode = KAR_HINT_STATE;
    if (UI_BOX_DATA[0].state == 0) {
        if (mode != 1) return 0;
        const RECT* startPosition = page < 4 ? &PROMPT_START_RECT : &HINT_START_RECT;
        createAnimatedUIBox(0, 0, 2, &FINAL_RECT, startPosition, nullptr, nullptr);
        text = dtl::make_unique<CurlingText>();
        return 1;
    }

    const int32_t pageCount = (HINT_OFFSETS[page + 1] - HINT_OFFSETS[page]) / LINES_PER_PAGE;
    if (text) text->prepareHintLine(page, KAR_OPPONENT_ENTITY == 2);

    if (UI_BOX_DATA[0].state == 1) {
        if (isKeyDownPolled(InputButtons::BUTTON_CROSS)) {
            KAR_HINT_STATE++;
            playSound(0, 3);
            if (pageCount < KAR_HINT_STATE) {
                removeAnimatedUIBox(0, nullptr);
                KAR_HINT_STATE = -1;
                KAR_resetHints();
            }
        }
        if (KAR_HINT_STATE != -1) {
            if (text) text->renderPage(KAR_HINT_STATE - 1);
            renderUIBox(0);
        }
    }
    return 1;
}
