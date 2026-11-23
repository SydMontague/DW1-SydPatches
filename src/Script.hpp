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
    void scriptStartTournament();
    void scriptCheckTournamentMedal();
    void lostAllLives();
    void scriptSetTextboxSize();
    void setDialogueOwner(int32_t speaker);
    void scriptShowSelection();
    int16_t* getStatsPointer(ScriptStats stat);
    void handleItemLoss();
    uint8_t* getScriptSection(uint8_t* ptr, int32_t section);
    uint8_t* getScript(uint32_t scriptId);
    void runMapHeadScript(uint8_t mapId);
    void callScriptSection(int32_t scriptId, uint32_t scriptSection, uint32_t param);
    void initializeScripts();
    void initializeLoadedNPCModels();
    void writePStat(uint32_t address, uint8_t value);
    uint8_t readPStat(uint32_t address);
    void setTrigger(uint32_t trigger);
    void unsetTrigger(uint32_t trigger);
    bool isTriggerSet(uint32_t trigger);
    void playBGM(uint8_t soundFont);
    void updateBGM();
    void forceUpdateBGM();
    void resetBGM();
    void setCardAmount(uint8_t cardId, int32_t amount);
    int32_t getCardAmount(uint8_t cardId);
    void scriptLoadModel(DigimonType type);
    void scriptUnloadModel(DigimonType type);
}

void scriptPauseGame(int32_t speaker);
void triggerSeadramonCutscene();
void checkArenaMap(int32_t mapId);
uint8_t readScriptUByte();
uint16_t readScriptUShort();
int16_t readScriptShort();
int32_t readScriptInt();
