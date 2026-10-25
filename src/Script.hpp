#pragma once

#include "extern/dtl/types.hpp"
#include "extern/dw1.hpp"

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
    uint8_t scriptIdToEntityId(uint8_t scriptId);
    int16_t enforceStatsLimits(ScriptStats type, int16_t value);
}

void triggerSeadramonCutscene();
void checkArenaMap(int32_t mapId);
uint8_t readScriptUByte();
uint16_t readScriptUShort();
int16_t readScriptShort();
int32_t readScriptInt();
