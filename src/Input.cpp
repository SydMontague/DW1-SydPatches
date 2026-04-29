#include "Input.hpp"

#include "extern/dw1.hpp"

namespace
{
    constexpr uint16_t REPEAT_FIRST_FIRE = 6;
    constexpr uint16_t REPEAT_WRAP       = 8;
    static_assert(REPEAT_WRAP > REPEAT_FIRST_FIRE);

    inline uint32_t tickAutoRepeat(uint32_t held, uint32_t prev)
    {
        if (held != prev)
        {
            INPUT_REPEAT_COUNTER = 0;
            INPUT_FRESH_MASK |= INPUT_REPEAT_MASK;
            return 0;
        }

        const uint16_t cnt = INPUT_REPEAT_COUNTER;
        uint32_t      fire = 0;
        if (cnt == REPEAT_WRAP)
        {
            INPUT_REPEAT_COUNTER = REPEAT_FIRST_FIRE;
            fire                 = held;
        }
        else if (cnt == REPEAT_FIRST_FIRE)
        {
            fire = held;
        }
        INPUT_REPEAT_COUNTER = static_cast<uint16_t>(INPUT_REPEAT_COUNTER + 1);
        return fire;
    }
}

bool isKeyDownPolled(InputButtons button)
{
    return ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & button) != 0;
}

bool isKeyPressed(InputButtons button)
{
    return (POLLED_INPUT & button) != 0;
}

extern "C"
{
    void inputInit()
    {
        INPUT_PENDING_MASK   = 0;
        INPUT_FRESH_MASK     = 0xFFFFFFFFu;
        INPUT_REPEAT_MASK    = 0;
        INPUT_REPEAT_COUNTER = 0;
    }

    bool isKeyDown(uint16_t keyMask)
    {
        if ((INPUT_PENDING_MASK & keyMask) == 0) return false;
        INPUT_FRESH_MASK &= ~static_cast<uint32_t>(keyMask);
        return true;
    }

    uint32_t processInput()
    {
        const uint32_t polled = POLLED_INPUT;
        INPUT_FRESH_MASK |= ~polled;

        const uint32_t held = polled & INPUT_REPEAT_MASK;
        if (held == 0) INPUT_REPEAT_COUNTER = 0;

        const uint32_t fire = (INPUT_REPEAT_MASK != 0)
            ? tickAutoRepeat(held, POLLED_INPUT_PREVIOUS & INPUT_REPEAT_MASK)
            : 0u;

        const uint32_t result = fire | (polled & INPUT_FRESH_MASK);
        INPUT_PENDING_MASK    = result;
        return result;
    }
}
