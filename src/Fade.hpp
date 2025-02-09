#include "extern/stddef.hpp"

extern "C"
{
    void initializeFadeData();
    void fadeToBlack(int32_t frames);
    void fadeFromBlack(int32_t frames);
    void fadeToWhite(int32_t frames);
    void fadeFromWhite(int32_t frames);
}