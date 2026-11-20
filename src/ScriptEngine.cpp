
#include "Battle.h"
#include "BitsBox.hpp"
#include "Camera.hpp"
#include "Entity.hpp"
#include "Fade.hpp"
#include "Files.hpp"
#include "GameObjects.hpp"
#include "GameTime.hpp"
#include "Helper.hpp"
#include "Inventory.hpp"
#include "ItemMenu.hpp"
#include "Map.hpp"
#include "MapObjects.hpp"
#include "MeramonShake.hpp"
#include "Model.hpp"
#include "MonochromonMoodBubble.hpp"
#include "NPCEntity.hpp"
#include "NinjamonEffect.hpp"
#include "Partner.hpp"
#include "Script.hpp"
#include "Sound.hpp"
#include "Tamer.hpp"
#include "VanillaText.hpp"
#include "VanillaTextbox.hpp"
#include "constants.hpp"
#include "extern/DGET.hpp"
#include "extern/DOO2.hpp"
#include "extern/EAB.hpp"
#include "extern/TRN.hpp"
#include "extern/TRN2.hpp"
#include "extern/dtl/runtime_array.hpp"
#include "extern/dw1.hpp"
#include "extern/libc.hpp"

namespace
{
    int32_t dateToSeconds(int32_t year, int32_t day, int32_t hour, int32_t minute)
    {
        return ((((year * 30) + day) * 24) + hour) * 60 + minute;
    }

    bool compareValue(uint32_t mode, int32_t val1, int32_t val2)
    {
        switch (mode) {
            case 0: return val1 == val2;
            case 1: return val1 != val2;
            case 2: return val1 >= val2;
            case 3: return val1 <= val2;
            case 4: return val1 > val2;
            case 5: return val1 < val2;
        }
        return false;
    }

    void scriptCompareDate()
    {
        auto stat        = readScriptUByte();
        auto trigger     = readScriptUShort();
        auto compareMode = readScriptUByte();
        auto inputYear   = readScriptUByte();
        auto inputDay    = readScriptUByte();
        auto inputHour   = readScriptUByte();
        auto inputMinute = readScriptUByte();
        readScriptUByte(); // unused;

        auto storedYear   = readPStat((stat + 0) % 256);
        auto storedDay    = readPStat((stat + 1) % 256);
        auto storedHour   = readPStat((stat + 2) % 256);
        auto storedMinute = readPStat((stat + 3) % 256);

        auto storedDate = dateToSeconds(storedYear, storedDay, storedHour, storedMinute);
        auto inputDate  = dateToSeconds(inputYear, inputDay, inputHour, inputMinute);

        if (compareValue(compareMode, storedDate, inputDate))
            setTrigger(trigger);
        else
            unsetTrigger(trigger);
    }

    uint32_t scriptCompareStat()
    {
        auto stat         = static_cast<ScriptStats>(readScriptUByte());
        auto mode         = readScriptUByte();
        int32_t statValue = *getStatsPointer(stat);
        auto scriptValue  = readScriptShort();
        return compareValue(mode, statValue, scriptValue);
    }

    int32_t scriptCompareCard()
    {
        auto cardId      = readScriptUByte();
        auto mode        = readScriptUByte();
        auto scriptValue = readScriptShort();
        auto amount      = getCardAmount(cardId);
        return compareValue(mode, amount, scriptValue);
    }

    int32_t scriptCompareMove()
    {
        auto moveId = readScriptUByte();
        auto mode   = readScriptUByte();
        auto val    = hasMove(moveId);
        return mode == 0 ? val : !val;
    }

    int32_t scriptCompareCondition()
    {
        auto value = readScriptUByte();
        auto mode  = readScriptUByte();

        if (mode == 0)
            return (PARTNER_PARA.condition.raw & value) != 0;
        else
            return (PARTNER_PARA.condition.raw & value) == 0;
    }

    int32_t scriptCompareItemCount()
    {
        auto itemId      = static_cast<ItemType>(readScriptUByte());
        auto mode        = readScriptUByte();
        auto scriptValue = readScriptShort();
        auto amount      = getItemCount(itemId);
        return compareValue(mode, amount, scriptValue);
    }

    int32_t scriptCompareMoney()
    {
        readScriptUByte(); // unused
        auto mode   = readScriptUByte();
        auto amount = readScriptInt();

        return compareValue(mode, MONEY, amount);
    }

    void scriptIfInstruction()
    {
        auto final = false;
        while (true) {
            auto op = readScriptUByte();
            readScriptUByte();

            if (op == 0x19) return;

            bool result = false;
            auto opCode = (op & 0x38) >> 3;
            auto comp   = op & 7;

            switch (opCode) {
                case 0: // 0x00 if trigger
                {
                    auto trigger = readScriptUShort();
                    result       = isTriggerSet(trigger) != static_cast<bool>(comp);
                    break;
                }
                case 1: // 0x08 if pstat
                {
                    auto stat  = readScriptUByte();
                    auto value = readScriptUByte();
                    result     = compareValue(comp, readPStat(stat), value);
                    break;
                }
                case 2: // 0x10 jump if
                {
                    auto val = readScriptUShort();
                    if (!final) continue;
                    SCRIPT_POINTER = ACTIVE_SCRIPT + val;
                    return;
                }
                case 3: // 0x18 jump if not
                {
                    auto val = readScriptUShort();
                    if (final) continue;
                    SCRIPT_POINTER = ACTIVE_SCRIPT + val;
                    return;
                }
                case 4: // 0x20 special compare
                {
                    switch (comp) {
                        case 0: result = scriptCompareStat(); break;
                        case 1: result = scriptCompareCard(); break;
                        case 2: result = scriptCompareMove(); break;
                        case 3: result = scriptCompareCondition(); break;
                        case 4: result = scriptCompareItemCount(); break;
                        case 5: result = scriptCompareMoney(); break;
                    }
                    break;
                }
            }

            if ((op & 0x80) != 0)
                final = (final && result);
            else if ((op & 0x40) != 0)
                final = (final || result);
            else
                final = result;
        }
    }
} // namespace

namespace
{
    int32_t routine0001020608090a0b0e122f()
    {
        ACTIVE_INSTRUCTION  = 100;
        SCRIPT_STATE_2      = 0;
        SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::NONE;
        return 2;
    }
    int32_t routineUnused()
    {
        return 1;
    }
    int32_t routineSetupTraining()
    {
        auto station = readPStat(0xFE);

        if (CURRENT_SCREEN_ID == 107 || CURRENT_SCREEN_ID == 108 || CURRENT_SCREEN_ID == 165 ||
            CURRENT_SCREEN_ID == 99) {
            switch (station) {
                case 0: TRN2_setupHpTraining(CURRENT_SCREEN_ID); break;
                case 1: TRN2_setupOffenseTraining(CURRENT_SCREEN_ID); break;
                case 2: TRN2_setupSpeedTraining(CURRENT_SCREEN_ID); break;
                case 3: TRN2_setupDefenseTraining(CURRENT_SCREEN_ID); break;
                case 4: TRN2_setupMpTraining(CURRENT_SCREEN_ID); break;
            }
        }
        else {
            switch (station) {
                case 0: TRN_setupHpTraining(CURRENT_SCREEN_ID); break;
                case 1: TRN_setupOffenseTraining(CURRENT_SCREEN_ID); break;
                case 2: TRN_setupSpeedTraining(CURRENT_SCREEN_ID); break;
                case 3: TRN_setupDefenseTraining(CURRENT_SCREEN_ID); break;
                case 4: TRN_setupMpTraining(CURRENT_SCREEN_ID); break;
                case 5: TRN_setupBrainsTraining(CURRENT_SCREEN_ID); break;
            }
        }

        ACTIVE_INSTRUCTION  = 100;
        SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::NONE;
        return 2;
    }
    int32_t routineRemoveItem()
    {
        auto item = static_cast<ItemType>(readPStat(0xFE));
        removeItem(item, 1); // vanilla does a NONE check, but the removeItem function does that already
        return 1;
    }
    int32_t routineRemoveStack()
    {
        auto item = static_cast<ItemType>(readPStat(0xFE));
        removeItem(item, 99); // vanilla does a NONE check, but the removeItem function does that already
        return 1;
    }
    int32_t routineCreateBitsBox()
    {
        createShopBitsBox(1);
        return 1;
    }
    int32_t routineCloseBitBox()
    {
        triggerBoxCloseFlag(2);
        return 2;
    }
    int32_t routineSetDirtCartModel()
    {
        setDirtCartModel(readPStat(0xFE));
        return 2;
    }
    int32_t routineDecreaseDirtPileSize()
    {
        decreaseDirtPileSize();
        return 2;
    }
    int32_t routineResetMapObjectAnimation()
    {
        resetMapObjectAnimation(readPStat(0xF7), readPStat(0xF8));
        return 2;
    }
    int32_t routineTournamentMedalConditions()
    {
        // in vanilla this is leftover code that is duplicated elsewhere
        // removing this fixes the ability to get the 100 tournament won medal by winning just 100 battles
        return 1;
    }
    int32_t routine17182532()
    {
        ACTIVE_INSTRUCTION  = 100;
        SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::NONE;
        return 2;
    }
    int32_t routineSetTimespeed()
    {
        TIMESPEED = readPStat(0xFE);
        return 1;
    }
    int32_t routineLoadDirtCartModel()
    {
        loadDirtCartModel();
        return 1;
    }
    int32_t routineLoadDirtPileModel()
    {
        loadDirtPileModel();
        return 1;
    }
    int32_t routineCreateMonochromonBubble()
    {
        createMonochromonMoodBubble();
        return 1;
    }
    int32_t routineOpenEggSelection()
    {
        DOO2_openEggBox();
        ACTIVE_INSTRUCTION  = 100;
        SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::NONE;
        return 2;
    }
    int32_t routineResetShopVariable()
    {
        SHOP_VARIABLE = 0;
        return 1;
    }
    int32_t routineAddToShopVariable()
    {
        auto value = static_cast<int16_t>((readPStat(0xF3) << 8) + readPStat(0xF4));
        SHOP_VARIABLE += value;
        return 1;
    }
    int32_t routineInitializeNamingBuffer()
    {
        auto mode = readPStat(0xFE);
        initializeNamingBuffer(mode);
        return 2;
    }
    int32_t routineRegisterBugTournament()
    {
        setTrigger(0x25);
        setTrigger(0x26);
        setTrigger(0x27);

        writePStat(2, DAY == 22 ? 22 : 149);
        writePStat(3, 22);
        writePStat(4, static_cast<uint8_t>(PARTNER_ENTITY.type));
        return 1;
    }
    int32_t routineCreateMeramonShake()
    {
        createMeramonShake();
        return 1;
    }
    int32_t routineLoadShopLibrary()
    {
        // SHOP_REL is never actually used
        return 1;
    }
    int32_t routineReadMapTFS()
    {
        readMapTFS(CURRENT_SCREEN_ID);
        return 1;
    }
    int32_t routineCreateNinjamonEffect()
    {
        createNinjamonEffect();
        return 1;
    }
    int32_t routineLoadTrainingLibrary()
    {
        loadTrainingLibrary(CURRENT_SCREEN_ID);
        return 1;
    }
    int32_t routineStartMachinedramonSpawn()
    {
        isSoundLoaded(0, 8);
        EAB_startBuildup(ENTITY_TABLE.getEntityById(scriptIdToEntityId(5)));
        return 1;
    }
    int32_t routineSpawnGearbox()
    {
        spawnGearbox();
        return 1;
    }
    int32_t routineSomethingToyTown()
    {
        auto val = readPStat(0xFE);
        somethingToyTown(val);
        return 1;
    }
    int32_t routineSpawnToyTownBoxes()
    {
        spawnToyTownBoxes();
        return 1;
    }
    int32_t routineOpenToyTownBox()
    {
        auto box = readPStat(0xFE);
        openToyTownBox(box);
        return 1;
    }
    int32_t routineFadeToWhite()
    {
        auto frames = readPStat(0xFE);
        fadeToWhite(frames);
        return 1;
    }
    int32_t routineFadeFromWhite()
    {
        auto frames = readPStat(0xFE);
        fadeFromWhite(frames);
        return 1;
    }
    int32_t routineOpenSaveMachine()
    {
        openSaveMachine();
        ACTIVE_INSTRUCTION  = 100;
        SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::NONE;
        return 2;
    }
    int32_t routineSpawnAngemonPedestal()
    {
        spawnAngemonPedestal();
        return 1;
    }

    int32_t routineLoadMachinedramonSpawn()
    {
        addFileReadRequestPath("\\ETCHI\\BOSS_EFE.TMD", &EAB_MODEL_BUFFER, nullptr, nullptr, nullptr);
        loadDynamicLibrary(Overlay::EAB_REL, nullptr, false, nullptr, nullptr);
        readVBALLSection(5, 0x73);
        loadMapSounds2(21);
        return 1;
    }
    int32_t routineLoadEnding()
    {
        loadTextureFile("\\ETCHI\\OP.TIM", nullptr, nullptr);
        loadDynamicLibrary(Overlay::ENDI_REL, nullptr, true, nullptr, nullptr);
        loadMapSounds2(20);
        return 1;
    }
    int32_t routineStartEnding()
    {
        isSoundLoaded(false, 8);
        Tamer_setState(16);
        SOME_SCRIPT_SYNC_BIT = 0;
        ACTIVE_INSTRUCTION   = 100;
        SCRIPT_TEXTBOX_MODE  = ScriptTextboxMode::NONE;
        return 2;
    }
    int32_t routineGameClearSave()
    {
        gameClearSave();
        ACTIVE_INSTRUCTION  = 100;
        SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::NONE;
        return 2;
    }
    int32_t routineSetAnimLoopToOne()
    {
        auto val = readPStat(0xFE);
        setLoopCountToOne(val);
        return 2;
    }
    int32_t routineStoreShopVariable()
    {
        writePStat(0xF3, SHOP_VARIABLE >> 8);
        writePStat(0xF4, SHOP_VARIABLE % 256);
        return 1;
    }

    using ScriptRoutineFn = int32_t (*)();
    dtl::array<ScriptRoutineFn, 57> ROUTINE_OPS{{
        routine0001020608090a0b0e122f,
        routine0001020608090a0b0e122f,
        routine0001020608090a0b0e122f,
        routineUnused,
        routineUnused,
        routineUnused,
        routine0001020608090a0b0e122f,
        routineSetupTraining,
        routine0001020608090a0b0e122f,
        routine0001020608090a0b0e122f,
        routine0001020608090a0b0e122f,
        routine0001020608090a0b0e122f,
        routineRemoveItem,
        routineRemoveStack,
        routine0001020608090a0b0e122f,
        routineCreateBitsBox,
        routineCloseBitBox,
        routineUnused,
        routine0001020608090a0b0e122f,
        routineSetDirtCartModel,
        routineDecreaseDirtPileSize,
        routineResetMapObjectAnimation,
        routineTournamentMedalConditions,
        routine17182532,
        routine17182532,
        routineSetTimespeed,
        routineLoadDirtCartModel,
        routineLoadDirtPileModel,
        routineCreateMonochromonBubble,
        routineOpenEggSelection,
        routineResetShopVariable,
        routineAddToShopVariable,
        routineInitializeNamingBuffer,
        routineRegisterBugTournament,
        routineCreateMeramonShake,
        routineLoadShopLibrary,
        routineReadMapTFS,
        routine17182532,
        routineCreateNinjamonEffect,
        routineLoadTrainingLibrary,
        routineStartMachinedramonSpawn,
        routineSpawnGearbox,
        routineSomethingToyTown,
        routineSpawnToyTownBoxes,
        routineOpenToyTownBox,
        routineFadeToWhite,
        routineFadeFromWhite,
        routine0001020608090a0b0e122f,
        routineOpenSaveMachine,
        routineSpawnAngemonPedestal,
        routine17182532,
        routineLoadMachinedramonSpawn,
        routineLoadEnding,
        routineStartEnding,
        routineGameClearSave,
        routineSetAnimLoopToOne,
        routineStoreShopVariable,
    }};

    int32_t callRoutine(uint32_t)
    {
        auto routine   = readScriptUByte();
        SCRIPT_PARAM_1 = routine;

        if (routine >= ROUTINE_OPS.size()) return 1;

        auto result = ROUTINE_OPS[routine]();
        return result;
    }

} // namespace

namespace
{
    void secondsToDate(int32_t seconds, uint8_t* year, uint8_t* day, uint8_t* hour, uint8_t* minute)
    {
        seconds = dtl::min(seconds, 11015999); // 255 years

        *minute       = seconds % 60;
        *hour         = (seconds / 60) % 24;
        auto localDay = (seconds / (60 * 24));
        *day          = localDay % 30;
        *year         = localDay / 30;
    }
    void scriptUpdateEnergyBoundaries()
    {
        auto raise = getRaiseData(PARTNER_ENTITY.type);

        PARTNER_PARA.energyLevel = dtl::min(PARTNER_PARA.energyLevel, static_cast<int16_t>(raise->energyCap));
        if (PARTNER_PARA.condition.isHungry && PARTNER_PARA.energyLevel >= raise->energyThreshold) {
            PARTNER_PARA.condition.isHungry = false;
            setFoodTimer(PARTNER_ENTITY.type);
            PARTNER_PARA.starvationTimer = 0;
        }
    }

    void scriptSetEntityWalking(uint32_t actor, int32_t speed)
    {
        if (actor == 0xFD) {
            startAnimation(&TAMER_ENTITY, speed + 2);
            Tamer_setState(10);
        }
        else if (actor == 0xFC) {
            startAnimation(&PARTNER_ENTITY, (speed + 1) * 2);
            Partner_setState(12);
        }
        else {
            auto id = scriptIdToEntityId(actor);
            startAnimation(ENTITY_TABLE.getEntityById(id), (speed + 1) * 2);
            // vanilla calls setActiveAnim, but seems unused
        }
    }
    void scriptStartAnimation(uint32_t actor, int32_t animId)
    {
        if (actor == 0xFD) {
            startAnimation(&TAMER_ENTITY, animId);
            Tamer_setState(10);
        }
        else if (actor == 0xFC) {
            startAnimation(&PARTNER_ENTITY, animId);
            Partner_setState(12);
        }
        else {
            auto id = scriptIdToEntityId(actor);
            startAnimation(ENTITY_TABLE.getEntityById(id), animId);
            // vanilla calls setActiveAnim, but seems unused
        }
    }

    void consumeMapChangeShowName()
    {
        auto val = readPStat(6);
        switch (val) {
            case 0: SCRIPT_MAP_CHANGE_SHOW_NAME = SCRIPT_SECTION_IS_EVENT == 0; break;
            case 1: SCRIPT_MAP_CHANGE_SHOW_NAME = 1; break;
            case 2: SCRIPT_MAP_CHANGE_SHOW_NAME = 0; break;
        }
        writePStat(6, 0);
    }

    int32_t getWaitId(int32_t id)
    {
        switch (id) {
            case 200: return 10;
            case 201: return 25;
            case 202: return 11;
            case 203: return 12;
            case 204: return 13;
            case 205: return 14;
            case 206: return 15;
            case 207: return 16;
            case 208: return 17;
            case 209: return 18;
            case 210: return 19;
            case 211: return 20;
            case 212: return 21;
            case 213: return 25;
            case 214: return 26;
            default: return scriptIdToEntityId(id);
        }
    }

    int32_t removeCondition(uint32_t)
    {
        PARTNER_PARA.condition.raw &= ~readScriptUByte();
        return 1;
    }
    int32_t scriptStartBattle(uint32_t)
    {
        readScriptUByte();
        BATTLES_STARTED           = dtl::min(BATTLES_STARTED + 1, 9999);
        auto hasFixedParticipants = readPStat(0xFA);
        if (hasFixedParticipants != 0)
            for (auto i = 0xFB; i < 0xFE; i++) {
                auto id = readPStat(i);
                if (id != 0xFF) writePStat(i, scriptIdToEntityId(id));
            }

        stopBGM();
        auto result = startBattle(ACTIVE_TALKED_TO_ENTITY);
        writePStat(0xFF, static_cast<uint8_t>(result));
        if (result == BattleResult::LOST) {
            handleItemLoss();
            PARTNER_ENTITY.lives--;
            if (PARTNER_ENTITY.lives == 0) PARTNER_PARA.remainingLifetime = 0;
            ACTIVE_SCRIPT  = getScript(0);
            SCRIPT_POINTER = getScriptSection(ACTIVE_SCRIPT, 1246);
            return 1;
        }
        if (result == BattleResult::FLED) {
            BATTLES_FLED                = dtl::min(BATTLES_FLED + 1, 9999);
            SCRIPT_MAP_CHANGE_SHOW_NAME = 0;
            SCRIPT_PARAM_1              = PREVIOUS_SCREEN;
            SCRIPT_STATE_2              = PREVIOUS_EXIT;
            PREVIOUS_EXIT               = CURRENT_EXIT;
            ScriptStackEntry entry{
                .type    = 4,
                .trigger = 0xFF,
            };
            pushScriptStack(&entry);
            return 3;
        }

        if (isTriggerSet(1)) {
            setMovementEnabled(-1, 1);
            initializeTextbox();
            return 1;
        }

        SCRIPT_MOVEMENT[10].type       = ScriptMovement::MOVE_CAMERA_TO_ENTITY;
        SCRIPT_MOVEMENT[10].selfEntity = 0xFD;
        SCRIPT_MOVEMENT[10].speed      = 10;
        ACTIVE_INSTRUCTION             = 0x4A;
        WAIT_FOR_ENTITY_ID             = 10;
        initializeTextbox();
        return 2;
    }
    int32_t scriptDelay(uint32_t)
    {
        readScriptUByte();
        DELAY_FRAMES       = readScriptUShort();
        ACTIVE_INSTRUCTION = 0x67;
        return 2;
    }
    int32_t scriptSetTextboxMode(uint32_t)
    {
        readScriptUByte();
        TEXTBOX_CLOSE_MODE = readScriptUByte();
        auto frames        = readScriptUByte();
        if (TEXTBOX_CLOSE_MODE == 2) AUTOCLOSE_FRAMES = frames;
        return 1;
    }
    int32_t scriptDealDamage(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto stat   = readScriptUByte();
        auto val    = readPStat(stat);
        auto damage = dtl::min((PARTNER_ENTITY.stats.hp * val) / 100, PARTNER_ENTITY.stats.currentHP - 1);
        PARTNER_ENTITY.stats.currentHP -= damage;
        addEntityText(&PARTNER_ENTITY, 0, 0, damage, 0);
        return 1;
    }
    int32_t scriptSetAutotalk(uint32_t)
    {
        readScriptUByte();
        auto id         = scriptIdToEntityId(readScriptUByte());
        auto isAutotalk = readScriptUByte();

        if (id != 0xFF && id > 1) NPC_ENTITIES[id - 2].autotalk = isAutotalk;
        return 1;
    }
    int32_t entityMoveTo(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto entity = readScriptUByte();
        auto posX   = readScriptShort();
        auto posY   = readScriptShort();
        auto speed  = readScriptUByte();
        readScriptUByte();

        auto entityId = scriptIdToEntityId(entity);
        if (entityId != 0xFF) {
            SCRIPT_MOVEMENT[entityId].type       = ScriptMovement::MOVE_TO_POS;
            SCRIPT_MOVEMENT[entityId].selfEntity = entity;
            SCRIPT_MOVEMENT[entityId].speed      = speed;
            SCRIPT_MOVEMENT[entityId].posX       = posX;
            SCRIPT_MOVEMENT[entityId].posY       = posY;
        }
        return 1;
    }
    int32_t entityMoveToEntity(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto entity   = readScriptUByte();
        auto target   = readScriptUByte();
        auto speed    = readScriptUByte();
        auto entityId = scriptIdToEntityId(entity);

        if (entityId != 0xFF) {
            SCRIPT_MOVEMENT[entityId].type       = ScriptMovement::MOVE_TO_ENTITY;
            SCRIPT_MOVEMENT[entityId].selfEntity = entity;
            SCRIPT_MOVEMENT[entityId].speed      = speed;
            SCRIPT_MOVEMENT[entityId].targetId   = target;
        }
        return 1;
    }
    int32_t entityMoveToWithCamera(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto entity = readScriptUByte();
        auto posX   = readScriptShort();
        auto posY   = readScriptShort();
        auto speed  = readScriptUByte();
        readScriptUByte();

        auto entityId = scriptIdToEntityId(entity);
        if (entityId != 0xFF) {
            // Vanilla does MOVE_TO_ENTITY_CAMERA here, causing this instruction to bug out
            SCRIPT_MOVEMENT[entityId].type       = ScriptMovement::MOVE_TO_POS_CAMERA;
            SCRIPT_MOVEMENT[entityId].selfEntity = entity;
            SCRIPT_MOVEMENT[entityId].speed      = speed;
            SCRIPT_MOVEMENT[entityId].posX       = posX;
            SCRIPT_MOVEMENT[entityId].posY       = posY;
        }
        return 1;
    }
    int32_t entityMoveToEntityWithCamera(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto entity   = readScriptUByte();
        auto target   = readScriptUByte();
        auto speed    = readScriptUByte();
        auto entityId = scriptIdToEntityId(entity);

        if (entityId != 0xFF) {
            SCRIPT_MOVEMENT[entityId].type       = ScriptMovement::MOVE_TO_ENTITY_CAMERA;
            SCRIPT_MOVEMENT[entityId].selfEntity = entity;
            SCRIPT_MOVEMENT[entityId].speed      = speed;
            SCRIPT_MOVEMENT[entityId].targetId   = target;
        }
        return 1;
    }
    int32_t rotate3DObject(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto entity = readScriptUByte();
        readScriptUByte();
        auto angle = readScriptUByte();

        SCRIPT_MOVEMENT[11].type       = ScriptMovement::ROTATE_OBJECT;
        SCRIPT_MOVEMENT[11].selfEntity = entity;
        SCRIPT_MOVEMENT[11].targetId   = angle;
        return 1;
    }
    int32_t moveObjectTo(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto id     = readScriptUByte();
        auto entity = readScriptUByte();
        auto speed  = readScriptUByte();
        auto target = readScriptUByte();
        readScriptUByte();
        auto param3 = readScriptShort();
        auto param4 = readScriptShort();

        SCRIPT_MOVEMENT[id + 12].type       = ScriptMovement::MOVE_OBJECT_TO_POS;
        SCRIPT_MOVEMENT[id + 12].selfEntity = entity;
        SCRIPT_MOVEMENT[id + 12].speed      = speed;
        SCRIPT_MOVEMENT[id + 12].targetId   = target;
        SCRIPT_MOVEMENT[id + 12].unk1       = param3;
        SCRIPT_MOVEMENT[id + 12].unk2       = param4;
        SCRIPT_MOVEMENT[id + 12].posX       = id; // not vanilla, give the tick the ID information
        return 1;
    }
    int32_t entityMoveToAxis(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto entity   = readScriptUByte();
        auto posX     = readScriptShort();
        auto targetId = readScriptUByte();
        auto speed    = readScriptUByte();
        auto entityId = scriptIdToEntityId(entity);

        if (entityId != 0xFF) {
            SCRIPT_MOVEMENT[entityId].type       = ScriptMovement::MOVE_TO_AXIS;
            SCRIPT_MOVEMENT[entityId].selfEntity = entity;
            SCRIPT_MOVEMENT[entityId].targetId   = targetId;
            SCRIPT_MOVEMENT[entityId].posX       = posX;
            SCRIPT_MOVEMENT[entityId].speed      = speed;
        }
        return 1;
    }
    int32_t entityMoveToAxisWithCamera(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto entity   = readScriptUByte();
        auto posX     = readScriptShort();
        auto targetId = readScriptUByte();
        auto speed    = readScriptUByte();
        auto entityId = scriptIdToEntityId(entity);

        if (entityId != 0xFF) {
            SCRIPT_MOVEMENT[entityId].type       = ScriptMovement::MOVE_TO_AXIS_CAMERA;
            SCRIPT_MOVEMENT[entityId].selfEntity = entity;
            SCRIPT_MOVEMENT[entityId].targetId   = targetId;
            SCRIPT_MOVEMENT[entityId].posX       = posX;
            SCRIPT_MOVEMENT[entityId].speed      = speed;
        }
        return 1;
    }
    int32_t scriptSpawnItem(uint32_t)
    {
        auto id   = static_cast<ItemType>(readScriptUByte());
        auto posX = readScriptShort();
        auto posY = readScriptShort();
        spawnItem(id, posX, posY);
        return 1;
    }
    int32_t scriptSpawnChest(uint32_t)
    {
        auto item     = static_cast<ItemType>(readScriptUByte());
        auto posX     = readScriptShort();
        auto posY     = readScriptShort();
        auto posZ     = readScriptShort();
        auto rotation = readScriptShort();
        auto trigger  = readScriptUShort();

        spawnChest(posX, posY, posZ, rotation, item, trigger);
        return 1;
    }
    int32_t scriptSpawnBoulder(uint32_t)
    {
        readScriptUByte();
        spawnBoulder();
        return 1;
    }
    int32_t scriptMoveBoulder(uint32_t)
    {
        readScriptUByte();
        auto diffX = readScriptShort();
        auto diffZ = readScriptShort();
        moveBoulder(diffX, diffZ);
        return 2;
    }
    int32_t scriptRemoveBoulder(uint32_t)
    {
        readScriptUByte();
        removeObject(ObjectID::BOULDER, 0);
        return 1;
    }
    int32_t scriptUnloadDigimon(uint32_t)
    {
        auto model = static_cast<DigimonType>(readScriptUByte());
        scriptUnloadModel(model);
        return 1;
    }
    int32_t copyPStat(uint32_t)
    {
        readScriptUByte();
        auto src = readScriptUByte();
        auto dst = readScriptUByte();
        writePStat(dst, readPStat(src));
        return 1;
    }
    int32_t scriptSectionOnExit(uint32_t)
    {
        auto section = readScriptUByte();
        ScriptStackEntry entry{
            .type    = 4,
            .trigger = section,
        };
        pushScriptStack(&entry);
        writePStat(0, TIMESPEED);
        return 1;
    }
    int32_t scriptSetRectImpassible(uint32_t)
    {
        readScriptUByte();
        auto tileX  = readScriptShort();
        auto tileY  = readScriptShort();
        auto width  = readScriptUByte();
        auto height = readScriptUByte();
        setImpassableRect(tileX, tileY, width, height);
        return 1;
    }
    int32_t scriptSpawnSpriteAtLocation(uint32_t)
    {
        auto count  = readScriptUByte();
        auto x      = readScriptShort();
        auto y      = readScriptShort();
        auto z      = readScriptShort();
        auto sprite = readScriptShort();

        spawnSpriteAtLocation(x, y, z, sprite, count);
        return 1;
    }
    int32_t scriptSpawnSpriteAtEntity(uint32_t)
    {
        auto entity = readScriptUByte();
        auto node   = readScriptUByte();
        auto sprite = readScriptUByte();
        spawnSpriteAtEntity(entity, node, sprite);
        return 1;
    }

    int32_t loadDigimon(uint32_t)
    {
        auto type = static_cast<DigimonType>(readScriptUByte());
        scriptLoadModel(type);
        return 1;
    }
    int32_t setDigimon(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto type       = static_cast<DigimonType>(readScriptUByte());
        auto actor      = readScriptUByte();
        auto isAutotalk = readScriptUByte();
        scriptSetDigimon(type, actor, isAutotalk);
        return 1;
    }
    int32_t unloadEntity(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto id = readScriptUByte();
        scriptUnloadEntity(id);
        return 1;
    }
    int32_t scriptCallDigimonRoutine(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto id = readScriptUByte();
        callDigimonRoutine(id);
        return 1;
    }

    int32_t waitForEntity(uint32_t)
    {
        auto id            = readScriptUByte();
        ACTIVE_INSTRUCTION = 0x4A;
        WAIT_FOR_ENTITY_ID = getWaitId(id);
        if (id == 213) SOME_SCRIPT_SYNC_BIT = 0;
        return 2;
    }
    int32_t warpTo(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto mapId      = readScriptUByte();
        auto spawnPoint = readScriptUByte();
        auto trigger    = readScriptUByte();

        SCRIPT_PARAM_1 = mapId;
        SCRIPT_STATE_2 = spawnPoint;
        consumeMapChangeShowName();
        ScriptStackEntry entry{
            .type    = 4,
            .trigger = trigger,
        };
        pushScriptStack(&entry);
        return 3;
    }
    int32_t entityLookAtEntity(uint32_t)
    {
        scriptPauseGame(0xFF);
        readScriptUByte();
        auto self     = readScriptUByte();
        auto target   = readScriptUByte();
        auto entityId = scriptIdToEntityId(self);
        if (entityId != 0xFF) {
            SCRIPT_MOVEMENT[entityId].type       = ScriptMovement::LOOK_AT_ENTITY;
            SCRIPT_MOVEMENT[entityId].selfEntity = self;
            SCRIPT_MOVEMENT[entityId].targetId   = target;
        }
        return 1;
    }
    int32_t entitySetRotation(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto self     = readScriptUByte();
        auto posX     = readScriptShort();
        auto entityId = scriptIdToEntityId(self);
        if (entityId != 0xFF) {
            SCRIPT_MOVEMENT[entityId].type       = ScriptMovement::SET_ROTATION;
            SCRIPT_MOVEMENT[entityId].selfEntity = self;
            SCRIPT_MOVEMENT[entityId].posX       = posX;
        }
        return 1;
    }
    int32_t entityWalkTo(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto self      = readScriptUByte();
        auto posX      = readScriptShort();
        auto posY      = readScriptShort();
        auto isRunning = readScriptUByte();
        readScriptUByte();
        scriptSetEntityWalking(self, isRunning);
        auto entityId = scriptIdToEntityId(self);
        if (entityId != 0xFF) {
            SCRIPT_MOVEMENT[entityId].type       = ScriptMovement::WALK_TO_POS;
            SCRIPT_MOVEMENT[entityId].selfEntity = self;
            SCRIPT_MOVEMENT[entityId].posX       = posX;
            SCRIPT_MOVEMENT[entityId].posY       = posY;
        }
        return 1;
    }
    int32_t moveCameraTo(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto speed                = readScriptUByte();
        auto posX                 = readScriptShort();
        auto posY                 = readScriptShort();
        SCRIPT_MOVEMENT[10].type  = ScriptMovement::MOVE_CAMERA_TO_POS;
        SCRIPT_MOVEMENT[10].posX  = posX;
        SCRIPT_MOVEMENT[10].posY  = posY;
        SCRIPT_MOVEMENT[10].speed = speed;
        return 1;
    }
    int32_t moveCameraToEntity(uint32_t)
    {
        scriptPauseGame(0xFF);
        readScriptUByte();
        auto self                      = readScriptUByte();
        auto speed                     = readScriptUByte();
        SCRIPT_MOVEMENT[10].type       = ScriptMovement::MOVE_CAMERA_TO_ENTITY;
        SCRIPT_MOVEMENT[10].selfEntity = self;
        SCRIPT_MOVEMENT[10].speed      = speed;
        return 1;
    }
    int32_t entityWalkToEntity(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto self      = readScriptUByte();
        auto isRunning = readScriptUByte();
        auto target    = readScriptUByte();
        scriptSetEntityWalking(self, isRunning);
        auto entityId = scriptIdToEntityId(self);
        if (entityId != 0xFF) {
            SCRIPT_MOVEMENT[entityId].type       = ScriptMovement::WALK_TO_ENTITY;
            SCRIPT_MOVEMENT[entityId].selfEntity = self;
            SCRIPT_MOVEMENT[entityId].targetId   = target;
        }
        return 1;
    }
    int32_t entityWalkToWithCamera(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto self      = readScriptUByte();
        auto posX      = readScriptShort();
        auto posY      = readScriptShort();
        auto isRunning = readScriptUByte();
        readScriptUByte();
        scriptSetEntityWalking(self, isRunning);
        auto entityId = scriptIdToEntityId(self);
        if (entityId != 0xFF) {
            SCRIPT_MOVEMENT[entityId].type       = ScriptMovement::WALK_TO_POS_CAMERA;
            SCRIPT_MOVEMENT[entityId].selfEntity = self;
            SCRIPT_MOVEMENT[entityId].posX       = posX;
            SCRIPT_MOVEMENT[entityId].posY       = posY;
        }
        return 1;
    }
    int32_t entityWalkToEntityWithCamera(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto self      = readScriptUByte();
        auto isRunning = readScriptUByte();
        auto target    = readScriptUByte();
        scriptSetEntityWalking(self, isRunning);
        auto entityId = scriptIdToEntityId(self);
        if (entityId != 0xFF) {
            SCRIPT_MOVEMENT[entityId].type       = ScriptMovement::WALK_TO_ENTITY_CAMERA;
            SCRIPT_MOVEMENT[entityId].selfEntity = self;
            SCRIPT_MOVEMENT[entityId].targetId   = target;
        }
        return 1;
    }
    int32_t scriptResetEntityOrigin(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto id = readScriptUByte();
        resetEntityOrigin(id);
        return 1;
    }
    int32_t setTextboxOrigin(uint32_t)
    {
        readScriptUByte();
        TEXTBOX_ORIGIN_X = readScriptShort();
        TEXTBOX_ORIGIN_Y = readScriptShort();
        TEXTBOX_ORIGIN_Z = readScriptShort();
        return 1;
    }
    int32_t playAnimation(uint32_t)
    {
        readScriptUByte();
        auto entity = readScriptUByte();
        auto anim   = readScriptUByte();
        scriptStartAnimation(entity, anim);
        return 1;
    }
    int32_t setObjectVisibility(uint32_t)
    {
        readScriptUByte();
        auto entity = readScriptUByte();
        auto anim   = readScriptUByte();
        setMapObjectsFlag(entity, 1, anim);
        return 1;
    }
    int32_t teleport(uint32_t)
    {
        scriptPauseGame(0xFF);
        auto stat      = readScriptUByte();
        SCRIPT_PARAM_1 = readPStat(stat);
        SCRIPT_STATE_2 = readPStat((stat + 1) % 256);
        consumeMapChangeShowName();
        ScriptStackEntry entry{
            .type    = 4,
            .trigger = 0xFF,
        };
        pushScriptStack(&entry);
        return 3;
    }
    int32_t scriptPlaySound(uint32_t)
    {
        readScriptUByte();
        auto val1 = readScriptUByte();
        auto val2 = readScriptUByte();
        playSound(val1, val2);
        return 1;
    }
    int32_t scriptSetBGM(uint32_t)
    {
        auto track = readScriptUByte();
        playBGM(track);
        return 1;
    }
    int32_t scriptResetBGM(uint32_t)
    {
        readScriptUByte();
        resetBGM();
        return 1;
    }

    int32_t scriptGiveItem(uint32_t)
    {
        readScriptUByte();
        auto item   = static_cast<ItemType>(readScriptUByte());
        auto amount = readScriptUByte();
        auto result = giveItem(item, amount);
        if (result)
            unsetTrigger(0);
        else
            setTrigger(0);
        return 1;
    }
    int32_t scriptRemoveItem(uint32_t)
    {
        readScriptUByte();
        auto item   = static_cast<ItemType>(readScriptUByte());
        auto amount = readScriptUByte();
        removeItem(item, amount);
        return 1;
    }
    int32_t scriptAddMoney(uint32_t)
    {
        readScriptUByte();
        auto amount = readScriptInt();
        MONEY       = dtl::min(MONEY + amount, MAX_MONEY);
        return 1;
    }
    int32_t scriptRemoveMoney(uint32_t)
    {
        readScriptUByte();
        auto amount = readScriptInt();
        MONEY       = dtl::max(MONEY - amount, MIN_MONEY);
        return 1;
    }
    int32_t compareDate(uint32_t)
    {
        scriptCompareDate();
        return 1;
    }
    int32_t scriptLearnMove(uint32_t)
    {
        auto move = readScriptUByte();
        learnMove(move);
        return 1;
    }
    int32_t giveCard(uint32_t)
    {
        auto cardId = readScriptUByte();
        auto amount = getCardAmount(cardId);
        if (amount < 9) setCardAmount(cardId, amount + 1);
        return 1;
    }
    int32_t takeCard(uint32_t)
    {
        auto cardId = readScriptUByte();
        auto amount = getCardAmount(cardId);
        if (amount > 0) setCardAmount(cardId, amount - 1);
        return 1;
    }
    int32_t setMerit(uint32_t)
    {
        readScriptUByte();
        auto value = readScriptShort();
        MERIT      = dtl::min(static_cast<int32_t>(value), MAX_MERIT);
        return 1;
    }
    int32_t addMerit(uint32_t)
    {
        readScriptUByte();
        auto amount = readScriptShort();
        MERIT       = dtl::min(MERIT + amount, MAX_MERIT);
        return 1;
    }
    int32_t reduceMerit(uint32_t)
    {
        readScriptUByte();
        auto amount = readScriptShort();
        MERIT       = dtl::max(MERIT - amount, MIN_MERIT);
        return 1;
    }
    int32_t setStat(uint32_t)
    {
        auto stat  = static_cast<ScriptStats>(readScriptUByte());
        auto value = readScriptShort();
        auto ptr   = getStatsPointer(stat);
        *ptr       = enforceStatsLimits(stat, value);
        scriptUpdateEnergyBoundaries();
        if (stat == ScriptStats::TAMER_LEVEL)
            TAMER_ENTITY.tamerLevel = TMP_TAMER_LEVEL;
        else if (stat == ScriptStats::LIVES)
            PARTNER_ENTITY.lives = TMP_LIVES;
        return 1;
    }
    int32_t addStat(uint32_t)
    {
        auto stat  = static_cast<ScriptStats>(readScriptUByte());
        auto value = readScriptShort();
        auto ptr   = getStatsPointer(stat);
        *ptr       = enforceStatsLimits(stat, *ptr + value);
        scriptUpdateEnergyBoundaries();
        if (stat == ScriptStats::TAMER_LEVEL)
            TAMER_ENTITY.tamerLevel = TMP_TAMER_LEVEL;
        else if (stat == ScriptStats::LIVES)
            PARTNER_ENTITY.lives = TMP_LIVES;
        return 1;
    }
    int32_t reduceStat(uint32_t)
    {
        auto stat  = static_cast<ScriptStats>(readScriptUByte());
        auto value = readScriptShort();
        auto ptr   = getStatsPointer(stat);
        auto min   = stat == ScriptStats::HAPPINESS ? -100 : 0;
        *ptr       = dtl::max(*ptr - value, min);
        scriptUpdateEnergyBoundaries();
        if (stat == ScriptStats::TAMER_LEVEL)
            TAMER_ENTITY.tamerLevel = TMP_TAMER_LEVEL;
        else if (stat == ScriptStats::LIVES)
            PARTNER_ENTITY.lives = TMP_LIVES;
        return 1;
    }
    int32_t advanceToDateAt(uint32_t)
    {
        auto stat   = readScriptUByte();
        auto year   = readPStat((stat + 0) % 256);
        auto day    = readPStat((stat + 1) % 256);
        auto hour   = readPStat((stat + 2) % 256);
        auto minute = readPStat((stat + 3) % 256);

        auto input   = dateToSeconds(year, day, hour, minute);
        auto current = dateToSeconds(YEAR, DAY, HOUR, MINUTE);
        if (current < input) skipHours((input - current) / 60);

        auto isAnotherDay = day != DAY;
        YEAR              = year;
        DAY               = day;
        HOUR              = hour;
        MINUTE            = minute;
        CURRENT_FRAME     = HOUR * 1200 + MINUTE * 20;

        if (isAnotherDay) dailyPStatTrigger();
        return 1;
    }
    int32_t addMinutesToDateAt(uint32_t)
    {
        auto stat   = readScriptUByte();
        auto amount = readScriptInt();
        auto year   = readPStat((stat + 0) % 256);
        auto day    = readPStat((stat + 1) % 256);
        auto hour   = readPStat((stat + 2) % 256);
        auto minute = readPStat((stat + 3) % 256);
        auto input  = dateToSeconds(year, day, hour, minute);

        amount += input;
        secondsToDate(amount, &year, &day, &hour, &minute);
        writePStat((stat + 0) % 256, year);
        writePStat((stat + 1) % 256, day);
        writePStat((stat + 2) % 256, hour);
        writePStat((stat + 3) % 256, minute);
        return 1;
    }
    int32_t subtractMinutesToDateAt(uint32_t)
    {
        auto stat   = readScriptUByte();
        auto amount = readScriptInt();
        auto year   = readPStat((stat + 0) % 256);
        auto day    = readPStat((stat + 1) % 256);
        auto hour   = readPStat((stat + 2) % 256);
        auto minute = readPStat((stat + 3) % 256);
        auto input  = dateToSeconds(year, day, hour, minute);

        amount = dtl::max(input - amount, 0);
        secondsToDate(amount, &year, &day, &hour, &minute);
        writePStat((stat + 0) % 256, year);
        writePStat((stat + 1) % 256, day);
        writePStat((stat + 2) % 256, hour);
        writePStat((stat + 3) % 256, minute);
        return 1;
    }
    int32_t unusedRead1Byte(uint32_t)
    {
        readScriptUByte();
        return 1;
    }
    int32_t unusedRead3Byte(uint32_t)
    {
        readScriptUByte();
        readScriptUByte();
        readScriptUByte();
        return 1;
    }
    int32_t storeDigimonNature(uint32_t)
    {
        readScriptUByte();
        auto statSource = readScriptUByte();
        auto statTarget = readScriptUByte();

        auto type = getDigimonData(static_cast<DigimonType>(readPStat(statSource)))->type;
        writePStat(statTarget, static_cast<uint8_t>(type));
        return 1;
    }

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

    int32_t scriptIf(uint32_t)
    {
        readScriptUByte();
        scriptIfInstruction();
        return 1;
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

    bool tickScriptedMovement(const ScriptEntityMovement& movement)
    {
        switch (movement.type) {
            case ScriptMovement::LOOK_AT_ENTITY: return tickLookAtEntity(movement.selfEntity, movement.targetId);
            case ScriptMovement::SET_ROTATION: return tickEntitySetRotation(movement.selfEntity, movement.posX);
            case ScriptMovement::WALK_TO_POS:
                return tickEntityWalkTo(movement.selfEntity, 0xFF, movement.posX, movement.posY, false);
            case ScriptMovement::WALK_TO_ENTITY:
                return tickEntityWalkTo(movement.selfEntity, movement.targetId, 0, 0, false);
            case ScriptMovement::WALK_TO_POS_CAMERA:
                return tickEntityWalkTo(movement.selfEntity, 0xFF, movement.posX, movement.posY, true);
            case ScriptMovement::WALK_TO_ENTITY_CAMERA:
                return tickEntityWalkTo(movement.selfEntity, movement.targetId, 0, 0, true);
            case ScriptMovement::MOVE_CAMERA_TO_POS:
                return tickCameraMoveTo(movement.posX, movement.posY, movement.speed);
            case ScriptMovement::MOVE_CAMERA_TO_ENTITY:
                return tickCameraMoveToEntity(movement.selfEntity, movement.speed);
            case ScriptMovement::MOVE_TO_POS:
                return tickEntityMoveTo(movement.selfEntity, 0xFF, movement.posX, movement.posY, movement.speed, false);
            case ScriptMovement::MOVE_TO_ENTITY:
                return tickEntityMoveTo(movement.selfEntity, movement.targetId, 0, 0, movement.speed, false);
            case ScriptMovement::MOVE_TO_POS_CAMERA:
                return tickEntityMoveTo(movement.selfEntity, 0xFF, movement.posX, movement.posY, movement.speed, true);
            case ScriptMovement::MOVE_TO_ENTITY_CAMERA:
                return tickEntityMoveTo(movement.selfEntity, movement.targetId, 0, 0, movement.speed, true);
            case ScriptMovement::ROTATE_OBJECT:
                return tickRotateDoor(movement.selfEntity, static_cast<int8_t>(movement.targetId));
            case ScriptMovement::MOVE_OBJECT_TO_POS:
                return tickMoveObjectTo(movement.selfEntity,
                                        movement.posX, // not vanilla, avoids having to know the actor ID
                                        movement.targetId,
                                        movement.unk1,
                                        movement.unk2);
            case ScriptMovement::MOVE_TO_AXIS:
                return tickEntityMoveToAxis(movement.selfEntity,
                                            movement.posX,
                                            movement.targetId,
                                            movement.speed,
                                            false);
            case ScriptMovement::MOVE_TO_AXIS_CAMERA:
                return tickEntityMoveToAxis(movement.selfEntity,
                                            movement.posX,
                                            movement.targetId,
                                            movement.speed,
                                            true);
            default:
            case ScriptMovement::NONE: return false;
        }
    }

    void tickScriptedMovements()
    {
        for (auto i = 0; i < SCRIPT_MOVEMENT.size(); i++) {
            auto& movement = SCRIPT_MOVEMENT[i];
            if (tickScriptedMovement(movement)) {
                if (i < 10 && movement.type < ScriptMovement::MOVE_TO_POS)
                    startAnimation(ENTITY_TABLE.getEntityById(i), 0);

                movement.type = ScriptMovement::NONE;
            }
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
            case 22:
                // in vanilla this is leftover code that is duplicated elsewhere
                // removing this fixes the ability to get the 100 tournament won medal by winning just 100 battles
                break;
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
        printf("invalid opcode %d -> %2x \n", SCRIPT_POINTER - ACTIVE_SCRIPT, opcode);
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
        scriptIf,
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
        scriptGiveItem,
        scriptRemoveItem,
        scriptAddMoney,
        scriptRemoveMoney,
        compareDate,
        scriptLearnMove,
        unusedRead1Byte,
        giveCard,
        takeCard,
        setMerit,
        addMerit,
        reduceMerit,
        setStat,
        addStat,
        reduceStat,
        advanceToDateAt,
        addMinutesToDateAt,
        subtractMinutesToDateAt,
        unusedRead3Byte,
        unusedRead3Byte,
        unusedRead3Byte,
        unusedRead1Byte,
        unusedRead1Byte,
        storeDigimonNature,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        loadDigimon,
        setDigimon,
        unloadEntity,
        scriptCallDigimonRoutine,
        waitForEntity,
        warpTo,
        entityLookAtEntity,
        entitySetRotation,
        entityWalkTo,
        moveCameraTo,
        moveCameraToEntity,
        entityWalkToEntity,
        entityWalkToWithCamera,
        entityWalkToEntityWithCamera,
        scriptResetEntityOrigin,
        setTextboxOrigin,
        playAnimation,
        setObjectVisibility,
        teleport,
        invalidOpcode,
        scriptPlaySound,
        unusedRead1Byte,
        unusedRead1Byte,
        scriptSetBGM,
        scriptResetBGM,
        unusedRead1Byte,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        invalidOpcode,
        callRoutine,
        removeCondition,
        scriptStartBattle,
        scriptDelay,
        scriptSetTextboxMode,
        scriptDealDamage,
        scriptSetAutotalk,
        invalidOpcode,
        entityMoveTo,
        entityMoveToEntity,
        entityMoveToWithCamera,
        entityMoveToEntityWithCamera,
        rotate3DObject,
        moveObjectTo,
        entityMoveToAxis,
        entityMoveToAxisWithCamera,
        scriptSpawnItem,
        scriptSpawnChest,
        scriptSpawnBoulder,
        scriptMoveBoulder,
        scriptRemoveBoulder,
        scriptUnloadDigimon,
        copyPStat,
        scriptSectionOnExit,
        scriptSetRectImpassible,
        scriptSpawnSpriteAtLocation,
        scriptSpawnSpriteAtEntity,
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
