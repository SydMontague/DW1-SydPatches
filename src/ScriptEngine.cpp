
#include "Camera.hpp"
#include "GameTime.hpp"
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
#include "extern/dw1.hpp"
#include "extern/libc.hpp"

namespace
{
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
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
        wrapper<scriptInstruction10to27>,
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
        wrapper<scriptInstructionFBtoFF>,
        wrapper<scriptInstructionFBtoFF>,
        wrapper<scriptInstructionFBtoFF>,
        wrapper<scriptInstructionFBtoFF>,
        wrapper<scriptInstructionFBtoFF>,
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
