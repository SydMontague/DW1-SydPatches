#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../Sound.hpp"
#include "../Textbox.hpp"
#include "../UIElements.hpp"
#include "../extern/dtl/unique_ptr.hpp"
#include "../extern/dw1.hpp"

namespace
{
    constexpr dtl::array<const char*, 2> PROMPT_STRINGS = {"Start Game", "Explain Game"};
    constexpr dtl::array<uint8_t, 10> HINT_OFFSETS      = {0, 12, 20, 24, 40, 44, 48, 52, 56, 60};

    constexpr dtl::array<const char*, 60> PENGUINMON_HINTS = {{
        "Penguinmon",
        "First of all, please choose",
        "your curling stone.",
        " ",
        "Penguinmon",
        "There are heavy, medium, and",
        "light ones, so think about the",
        "best order to use them.",
        "Penguinmon",
        "Some stones have four pegs.",
        "They stick to where they land",
        "and can become very useful.",
        "Penguinmon",
        "Move the Directional Pad ",
        "left and right to choose the ",
        "direction of your throw.",
        "Penguinmon",
        "Move screen up or down with",
        "Dir. Pad. Use X button to",
        "select throw direction.",
        "Penguinmon",
        "Press X button when power",
        "gauge reaches the desired",
        "level to pick throw strength.",
        "Penguinmon",
        "If your curling stone lands",
        "and touches the GOOD mark,",
        "you get two points.",
        "Penguinmon",
        "If you get it on the blue",
        "line, but outside the center",
        "GOOD mark, one point.",
        "Penguinmon",
        "If you touch the BAD on",
        "bottom right, you'll lose",
        "two points, so be careful.",
        "Penguinmon",
        "And, in the case of a tie,",
        "I win!",
        "So let's start the game!!",
        "Penguinmon",
        " ",
        "You're not that great!",
        " ",
        "Penguinmon",
        "The battle is just beginning!",
        " ",
        " ",
        "Penguinmon",
        "Hey you're pretty good!",
        " ",
        " ",
        "Penguinmon",
        "Yeah! I won!",
        " ",
        " ",
        "Penguinmon",
        "Awh, I lost. ",
        " ",
        " ",
    }};

    constexpr dtl::array<const char*, 60> METALMAMEMON_HINTS = {{
        "MetalMamemon",
        "Yeah. You gotta first pick",
        "the type of curling stone.",
        "",
        "MetalMamemon",
        "I'm a genius, so I can use",
        "various stones in the right",
        "order. It's key to winning.",
        "MetalMamemon",
        "Stones with four pegs stop",
        "and stay where they land,",
        "so they are very valuable.",
        "MetalMamemon",
        "Choose the direction of throw",
        "with the Directional Pad,",
        "by moving it left and right.",
        "MetalMamemon",
        "You can scroll screen moving",
        "Directional Pad up and down.",
        "Press X button to throw it.",
        "MetalMamemon",
        "Press X button when power",
        "gauge reaches the desired ",
        "level to pick throw strength.",
        "MetalMamemon",
        "Points: If you get the",
        "stone on the GOOD mark,",
        "you get two points.",
        "MetalMamemon",
        "If you get it on the blue",
        "line, but outside the center",
        "GOOD mark, one point.",
        "MetalMamemon",
        "If you get on right bottom",
        "BAD mark, it will be minus",
        "two points, so be careful.",
        "MetalMamemon",
        "In the case of a tie, I win!",
        "Start the game!!",
        "",
        "MetalMamemon",
        "I'll win!",
        "",
        "",
        "MetalMamemon",
        "I haven't lost yet!",
        "",
        "",
        "MetalMamemon",
        "I can't lose this match!",
        "",
        "",
        "MetalMamemon",
        "Yeah, I won!",
        "",
        "",
        "MetalMamemon",
        "Awh, I almost won.",
        "",
        "",
    }};

    constexpr RECT FINAL_RECT        = {-130, 42, 262, 59};
    constexpr RECT PROMPT_START_RECT = {75, -5, 10, 10};
    constexpr RECT HINT_START_RECT   = {0, 0, 10, 10};
    constexpr int32_t LINES_PER_PAGE = 4;
    constexpr RGB8 HINT_WHITE        = {216, 216, 216};
    constexpr RGB8 HINT_TITLE        = {255, 255, 0};

    // The overlay loader runs no destructors. Existing start/finish paths reset both concrete owners.
    constinit dtl::unique_ptr<ConfirmTextbox> hint;
    constinit dtl::unique_ptr<SelectTextbox> prompt;
    int32_t pendingPromptChoice{};

    [[gnu::optimize("Os")]]
    dtl::array<TextboxLine, LINES_PER_PAGE> prepareHint(int32_t group)
    {
        const auto& strings = KAR_OPPONENT_ENTITY == 2 ? PENGUINMON_HINTS : METALMAMEMON_HINTS;
        const int32_t first = HINT_OFFSETS[group] + (KAR_HINT_STATE - 1) * LINES_PER_PAGE;
        dtl::array<TextboxLine, LINES_PER_PAGE> page;
        for (int32_t line = 0; line < LINES_PER_PAGE; ++line)
            page[line] = {.text = strings[first + line], .color = line == 0 ? HINT_TITLE : HINT_WHITE};
        return page;
    }
} // namespace

[[gnu::optimize("Os")]]
void KAR_resetHints()
{
    hint.reset();
    prompt.reset();
    pendingPromptChoice = 0;
}

bool KAR_tickHints()
{
    if (hint) return hint->tick();
    if (prompt) {
        const auto choice = prompt->tick();
        if (choice.has_value()) {
            pendingPromptChoice = choice.value() + 1;
            return true;
        }
    }
    return false;
}

void KAR_renderHints()
{
    if (hint) hint->render(6);
    if (prompt) prompt->render(6);
}

bool KAR_isHintBoxOpen()
{
    return (hint && hint->isOpened()) || (prompt && prompt->isOpened());
}

[[gnu::optimize("Os")]]
int32_t KAR_tickYesNoPrompt(bool confirmed)
{
    if (KAR_HINT_STATE == 0) {
        KAR_HINT_STATE = 1;
        return 0;
    }
    if (KAR_HINT_STATE == 1) {
        KAR_resetHints();
        dtl::array<TextboxLine, 2> labels;
        dtl::runtime_array<uint32_t> options(2);
        for (uint32_t i = 0; i < PROMPT_STRINGS.size(); ++i) {
            labels[i]  = {.text = PROMPT_STRINGS[i], .color = HINT_WHITE};
            options[i] = i;
        }
        playSound(0, 0);
        prompt.reset(new SelectTextbox(FINAL_RECT,
                                       labels.data(),
                                       labels.size(),
                                       dtl::move(options),
                                       PROMPT_START_RECT,
                                       RGB8{},
                                       true));
        KAR_HINT_STATE = 3;
        return 0;
    }
    if (confirmed) {
        prompt->close();
        playSound(0, 1);
    }
    if (prompt->isClosed()) {
        const auto result = pendingPromptChoice;
        KAR_resetHints();
        return result;
    }
    KAR_HINT_STATE = 3 + prompt->hoveredOption();
    return 0;
}

[[gnu::optimize("Os")]]
int32_t KAR_tickHintBox(int32_t group, bool confirmed)
{
    if (KAR_HINT_STATE == 0) return 0;
    if (!hint) {
        if (KAR_HINT_STATE != 1) return 0;
        KAR_resetHints();
        const auto page = prepareHint(group);
        hint.reset(new ConfirmTextbox(FINAL_RECT,
                                      page.data(),
                                      page.size(),
                                      group < 4 ? PROMPT_START_RECT : HINT_START_RECT,
                                      RGB8{},
                                      true));
        playSound(0, 0);
        return 1;
    }
    if (hint->isClosed()) {
        KAR_resetHints();
        return 0;
    }
    if (confirmed) {
        ++KAR_HINT_STATE;
        playSound(0, 3);
        const auto pageCount = (HINT_OFFSETS[group + 1] - HINT_OFFSETS[group]) / LINES_PER_PAGE;
        if (KAR_HINT_STATE > pageCount) {
            KAR_HINT_STATE = -1;
            hint->close();
            playSound(0, 1);
        }
        else {
            const auto page = prepareHint(group);
            hint->replacePage(page.data(), page.size());
        }
    }
    return 1;
}
