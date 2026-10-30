
#include "Camera.hpp"
#include "GameTime.hpp"
#include "Inventory.hpp"
#include "ItemMenu.hpp"
#include "Map.hpp"
#include "MapObjects.hpp"
#include "NPCEntity.hpp"
#include "Script.hpp"
#include "Tamer.hpp"
#include "VanillaText.hpp"
#include "VanillaTextbox.hpp"
#include "extern/DGET.hpp"
#include "extern/DOO2.hpp"
#include "extern/dtl/runtime_array.hpp"
#include "extern/dw1.hpp"
#include "extern/libc.hpp"

namespace
{
    int32_t setSelection(uint32_t)
    {
        SCRIPT_TEXTBOX_SPEAKER_MODE = 1;
        scriptShowSelection();
        return 2;
    }

    int32_t jumpAndLink(uint32_t)
    {
        readScriptUByte();
        auto offset = readScriptUShort();

        ScriptStackEntry entry{
            .scriptPointer   = SCRIPT_POINTER,
            .activeMapScript = ACTIVE_MAP_SCRIPT,
            .type            = 1,
            .trigger         = 0,
        };
        pushScriptStack(&entry);
        SCRIPT_POINTER = ACTIVE_SCRIPT + offset;
        return 1;
    }

    int32_t jumpToFileAndLink(uint32_t)
    {
        readScriptUByte();
        auto script  = readScriptUShort();
        auto section = readScriptUShort();

        ScriptStackEntry entry{
            .scriptPointer   = SCRIPT_POINTER,
            .activeMapScript = ACTIVE_MAP_SCRIPT,
            .type            = 1,
            .trigger         = 0,
        };
        pushScriptStack(&entry);
        ACTIVE_SCRIPT  = getScript(script);
        SCRIPT_POINTER = getScriptSection(ACTIVE_SCRIPT, section);
        return 1;
    }

    int32_t jumpReturn(uint32_t)
    {
        readScriptUByte();
        ScriptStackEntry entry;
        popScriptStack(&entry);
        ACTIVE_SCRIPT  = getScript(entry.activeMapScript);
        SCRIPT_POINTER = entry.scriptPointer;
        return 1;
    }

    int32_t jumpTo(uint32_t)
    {
        readScriptUByte();
        auto offset    = readScriptUShort();
        SCRIPT_POINTER = ACTIVE_SCRIPT + offset;
        return 1;
    }

    int32_t jumpToFile(uint32_t)
    {
        readScriptUByte();
        auto script    = readScriptUShort();
        auto section   = readScriptUShort();
        ACTIVE_SCRIPT  = getScript(script);
        SCRIPT_POINTER = getScriptSection(ACTIVE_SCRIPT, section);
        return 1;
    }

    int32_t scriptSwitch(uint32_t)
    {
        auto stat  = readScriptUByte();
        auto count = readScriptUShort();
        auto value = dtl::min(static_cast<int32_t>(readPStat(stat)), count - 1);

        SCRIPT_POINTER = ACTIVE_SCRIPT + reinterpret_cast<uint16_t*>(SCRIPT_POINTER)[value];
        return 1;
    }

    void scriptIf(uint32_t)
    {
        readScriptUByte();
        scriptIfInstruction();
        libc_longjmp(&SCRIPT_SAVE_STATE, 1); // return 1;
    }

    int32_t scriptShowTextbox(uint32_t)
    {
        readScriptUByte();
        if (SCRIPT_TEXTBOX_SPEAKER_MODE == 2)
            showTextbox(0, 255);
        else
            showTextbox(0, CURRENT_DIALOGUE_OWNER);
        return 2;
    }

    int32_t sciprtSetDialogOwner(uint32_t)
    {
        auto speaker                = readScriptUByte();
        SCRIPT_TEXTBOX_SPEAKER_MODE = 0;
        if (UI_BOX_DATA[0].state != 1) CURRENT_DIALOGUE_OWNER = speaker - 1;
        setDialogueOwner(speaker);
        return 1;
    }

    int32_t scriptSetTrigger(uint32_t)
    {
        readScriptUByte();
        setTrigger(readScriptUShort());
        return 1;
    }

    int32_t scriptUnsetTrigger(uint32_t)
    {
        readScriptUByte();
        unsetTrigger(readScriptUShort());
        return 1;
    }

    int32_t setPStat(uint32_t)
    {
        readScriptUByte();
        auto stat  = readScriptUByte();
        auto value = readScriptUByte();
        writePStat(stat, value);
        return 1;
    }

    int32_t addPStat(uint32_t)
    {
        readScriptUByte();
        auto stat         = readScriptUByte();
        auto value        = readScriptUByte();
        int32_t baseValue = readPStat(stat);
        writePStat(stat, dtl::min(255, baseValue + value));
        return 1;
    }

    int32_t subtractPStat(uint32_t)
    {
        readScriptUByte();
        auto stat         = readScriptUByte();
        auto value        = readScriptUByte();
        int32_t baseValue = readPStat(stat);
        writePStat(stat, dtl::max(0, baseValue - value));
        return 1;
    }

    int32_t storeMapID(uint32_t)
    {
        auto stat = readScriptUByte();
        writePStat(stat, CURRENT_SCREEN_ID);
        return 1;
    }

    int32_t storeDigmonType(uint32_t)
    {
        auto stat = readScriptUByte();
        writePStat(stat, static_cast<uint8_t>(PARTNER_ENTITY.type));
        return 1;
    }

    int32_t scriptSetInventorySize(uint32_t)
    {
        setInventorySize(readScriptUByte());
        return 1;
    }

    int32_t storeRandom(uint32_t)
    {
        readScriptUByte();
        auto stat  = readScriptUByte();
        auto limit = readScriptUByte();
        writePStat(stat, random(limit + 1));
        return 1;
    }

    int32_t storeDate(uint32_t)
    {
        auto stat = readScriptUByte();
        writePStat((stat + 0) % 256, YEAR);
        writePStat((stat + 1) % 256, DAY);
        writePStat((stat + 2) % 256, HOUR);
        writePStat((stat + 3) % 256, MINUTE);
        return 1;
    }

    int32_t setTextboxSize(uint32_t)
    {
        scriptPauseGame(0xFF);
        SCRIPT_TEXTBOX_SPEAKER_MODE = 2;
        scriptSetTextboxSize();
        return 1;
    }

    int32_t fadeoutHud(uint32_t)
    {
        closeBox(readScriptUByte());
        return 2;
    }

    int32_t setScript(uint32_t)
    {
        readScriptUByte(); // skipped
        CURRENT_SCRIPT_ID = readScriptUShort();
        CURRENT_SCREEN_ID = readScriptUShort();

        ScriptStackEntry entry;
        entry.type = 3;
        pushScriptStack(&entry);
        // vanilla sets 0x80134FEC to 1 here, but the value is never used
        resetBGM();
        loadMap(CURRENT_SCREEN_ID);
        return 1;
    }

    int32_t returnFromScriptFile(uint32_t)
    {
        while (true) {
            ScriptStackEntry entry;
            popScriptStack(&entry);

            if (entry.type == 0) {
                IS_SCRIPT_PAUSED = 1;
                return 2;
            }
            if (entry.type == 3) {
                readMapTFS(CURRENT_SCREEN_ID);
                setupMap();
                // vanilla sets 0x80134FEC to 0 here, but the value is never used
                auto script  = getScript(CURRENT_SCRIPT_ID);
                auto section = getScriptSection(script, 0xFE);
                if (section != nullptr) {
                    ACTIVE_SCRIPT  = script;
                    SCRIPT_POINTER = section;
                    return 1;
                }
            }
            else if (entry.type == 4) {
                if (entry.trigger == 0xFF) {
                    ACTIVE_SCRIPT  = MAPHEAD_SCRIPT_PTR;
                    SCRIPT_POINTER = MAPHEAD_SCRIPT_PTR + (*reinterpret_cast<uint16_t*>(MAPHEAD_SCRIPT_PTR) - 2);
                }
                else {
                    ACTIVE_SCRIPT  = getScript(CURRENT_SCRIPT_ID);
                    SCRIPT_POINTER = getScriptSection(ACTIVE_SCRIPT, entry.trigger);
                }
                return 2;
            }
        }
    }

    void tickScriptedMovements()
    {
        for (auto i = 0; i < SCRIPT_MOVEMENT.size(); i++) {
            if (SCRIPT_MOVEMENT[i].type != ScriptMovement::NONE) tickScriptedMovement(i);
        }
    }

    bool isAnyMovementActive()
    {
        for (const auto& movement : SCRIPT_MOVEMENT)
            if (movement.type != ScriptMovement::NONE) return true;
        return false;
    }

    void tickWaitForAnimation()
    {
        if (WAIT_FOR_ENTITY_ID == 0x19) {
            if (SOME_SCRIPT_SYNC_BIT) ACTIVE_INSTRUCTION = 0;
        }
        else if (WAIT_FOR_ENTITY_ID == 0x1A) {
            if (TRN_LOADING_COMPLETE == 0) ACTIVE_INSTRUCTION = 0;
        }
        else if (WAIT_FOR_ENTITY_ID == 0xFF) {
            if (SOME_SCRIPT_SYNC_BIT && !isAnyMovementActive()) ACTIVE_INSTRUCTION = 0;
        }
        else if (SCRIPT_MOVEMENT[WAIT_FOR_ENTITY_ID].type == ScriptMovement::NONE)
            ACTIVE_INSTRUCTION = 0;
    }

    void tickCallRoutine()
    {
        switch (SCRIPT_PARAM_1) {
            case 0: openDiscardItem(); break;
            case 1: openMeritShop(); break;
            case 2: openMojyamonShop(); break;
            case 6: scriptStartTournament(); break;
            case 7:
                if (isTrainingComplete()) ACTIVE_INSTRUCTION = 0;
                break;
            case 8:
                if (isTriggerSet(3))
                    openRecycleShop();
                else
                    openShop();
                break;
            case 9: openJukebox(); break;
            case 10: tickBirdraTransport(); break;
            case 11: tickItemKeeper(); break;
            case 14:
                if (isTriggerSet(3))
                    openSellCardMenu();
                else if (isTriggerSet(4))
                    openBuyCardMenu();
                else
                    rollCardPack();
                break;
            case 18: DGET_initTournamentSchedule(); break;
            case 22: scriptCheckTournamentMedal(); break;
            case 23:
                if (tickOpenChestTray(readPStat(0xFE))) ACTIVE_INSTRUCTION = 0;
                break;
            case 24:
                if (tickCloseChestTray(readPStat(0xFE))) ACTIVE_INSTRUCTION = 0;
                break;
            case 29:
                if (DOO2_tickEggInput()) ACTIVE_INSTRUCTION = 0;
                break;
            case 32: // new name dialogue
                if (newGameStateMachine()) ACTIVE_INSTRUCTION = 0;
                break;
            case 37:
                if (tickRemoveMist()) ACTIVE_INSTRUCTION = 0;
                break;
            case 47: lostAllLives(); break;
            case 48:
                if (tickOpenSaveMachine()) ACTIVE_INSTRUCTION = 0;
                break;
            case 50:
                if (moveAngemonPedestal()) ACTIVE_INSTRUCTION = 0;
                break;
            case 53:
            {
                if (SOME_SCRIPT_SYNC_BIT) {
                    ACTIVE_INSTRUCTION = 0;
                    MAIN_STATE         = 3;
                    // vanilla does a longjmp here, but that seems entirely unnecessary as it works just a fine without
                }
                break;
            }
            case 54:
                if (scriptTickMainMenu()) ACTIVE_INSTRUCTION = 0;
                break;
        }
    }

    int32_t invalidOpcode(uint32_t opcode)
    {
        printf("invalid opcode %2x\n", opcode);
        IS_SCRIPT_PAUSED = 1;
        return 2;
    }

    template<void (*T)(uint32_t)> int32_t wrapper(uint32_t opcode)
    {
        auto val = libc_setjmp(&SCRIPT_SAVE_STATE);
        if (val == 0) T(opcode);

        return val;
    }
    using ScriptOpFn = int32_t (*)(uint32_t);

    dtl::array<ScriptOpFn, 256> SCRIPT_OPS{{
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        setSelection,
        invalidOpcode,
        invalidOpcode,
        jumpAndLink,
        jumpToFileAndLink,
        jumpReturn,
        jumpTo,
        jumpToFile,
        scriptSwitch,
        wrapper<scriptIf>,
        scriptShowTextbox,
        sciprtSetDialogOwner,
        scriptSetTrigger,
        scriptUnsetTrigger,
        setPStat,
        addPStat,
        subtractPStat,
        storeMapID,
        storeDigmonType,
        scriptSetInventorySize,
        storeRandom,
        storeDate,
        setTextboxSize,
        fadeoutHud,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        wrapper<scriptInstruction28to3F>,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        wrapper<scriptInstruction46to58>,
        invalidOpcode,
        wrapper<scriptInstruction5Ato5F>,
        wrapper<scriptInstruction5Ato5F>,
        wrapper<scriptInstruction5Ato5F>,
        wrapper<scriptInstruction5Ato5F>,
        wrapper<scriptInstruction5Ato5F>,
        wrapper<scriptInstruction5Ato5F>,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        wrapper<scriptInstruction64to7E>,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        setScript,
        invalidOpcode,
        invalidOpcode,
        returnFromScriptFile,
        returnFromScriptFile,
    }};
} // namespace

extern "C"
{
    int32_t tickScript()
    {
        if (IS_SCRIPT_PAUSED) return 1;

        tickTextboxHandling(0);
        tickScriptedMovements();

        if (SCRIPT_MAP_CHANGE_ENABLED == 75) {
            if (scriptTickChangeMap(SCRIPT_PARAM_1, SCRIPT_STATE_2, SCRIPT_MAP_CHANGE_SHOW_NAME)) {
                SCRIPT_PAUSED_GAME = 0;
                scriptPauseGame(0xFF);
                SCRIPT_MAP_CHANGE_ENABLED = 0;
            }
        }

        if (ACTIVE_INSTRUCTION == 0x4A) tickWaitForAnimation();
        if (ACTIVE_INSTRUCTION == 0x64) tickCallRoutine();
        if (ACTIVE_INSTRUCTION == 0x67 && DELAY_FRAMES == 0) // TODO move DELAY_FRAMES ticking into script
            ACTIVE_INSTRUCTION = 0;

        if (SCRIPT_MAP_CHANGE_ENABLED != 0) return IS_SCRIPT_PAUSED;
        if (ACTIVE_INSTRUCTION != 0) return IS_SCRIPT_PAUSED;

        while (true) {
            const auto opcode = *SCRIPT_POINTER++;
            const auto val    = SCRIPT_OPS[opcode](opcode);

            if (val == 2) {
                if (IS_SCRIPT_PAUSED) {
                    closeAllTextboxes();
                    writePStat(0, TIMESPEED);
                    if (SCRIPT_PAUSED_GAME) {
                        setMovementEnabled(-1, 0);
                        setCameraFollowPlayer();
                        startGameTime();
                    }
                }
                break;
            }
            if (val == 3) {
                closeAllTextboxes();
                writePStat(0, TIMESPEED);
                SCRIPT_MAP_CHANGE_ENABLED = 75;
                SCRIPT_MAP_CHANGE_STATE   = 0;
                break;
            }
        }

        return IS_SCRIPT_PAUSED;
    }
}
