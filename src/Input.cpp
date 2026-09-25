#include "Input.hpp"

#include "extern/dw1.hpp"

namespace
{
    constexpr uint16_t REPEAT_FIRST_FIRE = 6;
    constexpr uint16_t REPEAT_WRAP       = 8;
    static_assert(REPEAT_WRAP > REPEAT_FIRST_FIRE);

    bool isFreshDialogue;
    uint16_t repeatCounter;
    uint32_t repeatMask;
    uint32_t freshMask;
    uint32_t pendingMask;

    inline uint32_t tickAutoRepeat(uint32_t held, uint32_t prev)
    {
        if (held != prev) {
            repeatCounter = 0;
            freshMask |= repeatMask;
            return 0;
        }

        const uint16_t cnt = repeatCounter;
        uint32_t fire      = 0;
        if (cnt == REPEAT_WRAP) {
            repeatCounter = REPEAT_FIRST_FIRE;
            fire          = held;
        }
        else if (cnt == REPEAT_FIRST_FIRE) {
            fire = held;
        }
        repeatCounter = static_cast<uint16_t>(repeatCounter + 1);
        return fire;
    }
} // namespace

bool isKeyDownPolled(InputButtons button)
{
    return ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & button) != 0;
}

bool isKeyDownPolledP2(InputButtons button)
{
    return ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & (button << 0x10)) != 0;
}

bool isKeyPressed(InputButtons button)
{
    return (POLLED_INPUT & button) != 0;
}

extern "C"
{
    void inputInit()
    {
        pendingMask   = 0;
        freshMask     = 0xFFFFFFFFu;
        repeatMask    = 0;
        repeatCounter = 0;
    }

    bool isKeyDown(uint16_t keyMask)
    {
        if ((pendingMask & keyMask) == 0) return false;
        freshMask &= ~static_cast<uint32_t>(keyMask);
        return true;
    }

    uint32_t processInput()
    {
        const uint32_t polled = POLLED_INPUT;
        freshMask |= ~polled;

        const uint32_t held = polled & repeatMask;
        if (held == 0) repeatCounter = 0;

        const uint32_t fire = (repeatMask != 0) ? tickAutoRepeat(held, POLLED_INPUT_PREVIOUS & repeatMask) : 0u;

        const uint32_t result = fire | (polled & freshMask);
        pendingMask           = result;
        return result;
    }

    void setFreshDialogue()
    {
        isFreshDialogue = true;
    }

    bool isXPressedAfterDialogue()
    {
        if (!isFreshDialogue) return 1;

        if (isKeyPressed(InputButtons::BUTTON_CROSS)) return 0;

        isFreshDialogue = 0;
        return 1;
    }

    void setInputRepeatMask(uint32_t mask)
    {
        repeatMask = mask;
    }
}
