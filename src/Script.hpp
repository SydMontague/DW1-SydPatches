#pragma once

#include "extern/dtl/types.hpp"

extern "C"
{
    bool isPartnerBaby();
    void dailyPStatTrigger();
    void showMapheadTextbox(int32_t line, int32_t speaker, int32_t textboxId, int32_t sectionId);
    bool flipTextboxPage(int32_t id);
    void showMapheadSelection(int32_t line,
                              int32_t speaker,
                              int32_t selectionCount,
                              uint32_t* usePreviousSelection,
                              uint16_t sectionId);
    uint32_t showTextbox(int32_t id, uint32_t speaker);
}

void triggerSeadramonCutscene();
void checkArenaMap(int32_t mapId);
