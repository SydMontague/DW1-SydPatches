#include "Input.hpp"

#include "extern/dw1.hpp"

bool isKeyDownPolled(InputButtons button)
{
    return ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & button) != 0;
}

bool isKeyPressed(InputButtons button)
{
    return (POLLED_INPUT & button) != 0;
}