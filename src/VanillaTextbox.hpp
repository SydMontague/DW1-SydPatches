#pragma once
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    void initializeTextbox();
    void tickTextboxHandling(int32_t mode);
    void triggerBoxCloseFlag(int32_t id);
    void closeBox(int32_t id);
    void createTextbox(int32_t id,
                       uint8_t flags,
                       const RECT* textboxPos,
                       const RECT* textboxOrigin,
                       TickFunction tickFunction,
                       RenderFunction renderFunction);
    void registerTextbox(int id, uint32_t lineOffset, uint32_t lineCount, bool isDoubleBuffered, VRAMMode vramMode);
    void setupDialogueBox(int32_t speaker);
    bool setupBoxOrigin(int32_t speaker, RECT* result);
    void renderConfirmPrompt(int32_t instanceId);
}

void closeTextbox(int32_t id, RECT* final);
