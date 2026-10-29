
#include "Script.hpp"
#include "Evolution.hpp"
#include "Files.hpp"
#include "Helper.hpp"
#include "Inventory.hpp"
#include "Math.hpp"
#include "Partner.hpp"
#include "RecycleShop.hpp"
#include "Tournament.hpp"
#include "VanillaText.hpp"
#include "VanillaTextbox.hpp"
#include "extern/dtl/algorithm.hpp"
#include "extern/dtl/array.hpp"
#include "extern/dtl/vector.hpp"
#include "extern/dw1.hpp"

namespace
{
    int32_t getShopkeeperScriptID()
    {
        constexpr dtl::array<DigimonType, 12> SHOPKEEPER_TYPES{
            DigimonType::NPC_PATAMON,
            DigimonType::NPC_UNIMON,
            DigimonType::NPC_BIYOMON,
            DigimonType::NPC_MONOCHROMON,
            DigimonType::NPC_DEVIMON,
            DigimonType::NPC_MAMEMON,
            DigimonType::NPC_NUMEMON,
            DigimonType::NPC_MOJYAMON,
            DigimonType::MARKET_MANAGER,
            DigimonType::BETAMON,
            DigimonType::INVALID,
            DigimonType::TAMER,
        };

        const auto speaker = readPStat(0xFE);
        if (speaker == 0xFF) return 0x4CE;

        const auto entityId = scriptIdToEntityId(speaker);
        if (entityId == 0xFF) return 0x4C4;

        const auto type = ENTITY_TABLE.getEntityById(entityId)->type;
        for (auto i = 0; i < SHOPKEEPER_TYPES.size(); i++)
            if (type == SHOPKEEPER_TYPES[i]) return 0x4C4 + i;

        return 0x4C4;
    }

    uint8_t* resolveMapHeadEntry(uint32_t sectionId, int32_t line)
    {
        auto script = getScript(0);
        auto offset = getScriptSection(script, sectionId);
        return script + *reinterpret_cast<uint16_t*>(offset + line * 4 + 2) + 2;
    }

    uint8_t* getShopkeeperLine(int32_t line)
    {
        auto scriptId = getShopkeeperScriptID();
        return resolveMapHeadEntry(scriptId, line);
    }

    constexpr uint8_t getColorFromSpeaker(uint32_t speaker)
    {
        if (speaker == 0xFD) return 6;
        if (speaker == 0xFC) return 10;
        if (speaker < 200) return 7;

        return 5;
    }

    int32_t getSpeakerName(uint32_t speaker, uint8_t* str)
    {
        constexpr dtl::array<const char*, 6> SPECIAL_SPEAKERS{
            "Sign",
            "Box",
            "Betamon",
            "Coelamon",
            "Tanemon",
            "Palmon",
        };

        const uint8_t* name = nullptr;

        if (speaker == 0xFF)
            return 0;
        else if (speaker == 0xFC)
            name = PARTNER_ENTITY.name;
        else if (speaker == 0xFD)
            name = getDigimonData(DigimonType::TAMER)->name;
        else if (speaker >= 200)
            name = reinterpret_cast<const uint8_t*>(SPECIAL_SPEAKERS[speaker - 200]);
        else
            name = getDigimonData(ENTITY_TABLE.getEntityById(scriptIdToEntityId(speaker))->type)->name;

        strcpy(str, name);
        return strlen(name);
    }

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
} // namespace

extern "C"
{
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

    int16_t enforceStatsLimits(ScriptStats type, int16_t value)
    {
        dtl::array<int16_t, 22> LIMITS{999, 999, 999, 999,  9999, 9999, 9999, 9999, 100, 100, 999,
                                       999, 999, 999, 9999, 9999, 9999, 999,  999,  999, 99,  10};

        auto limit = LIMITS[static_cast<int32_t>(type)];
        if (type == ScriptStats::CURRENT_HP) limit = PARTNER_ENTITY.stats.hp;
        if (type == ScriptStats::CURRENT_MP) limit = PARTNER_ENTITY.stats.mp;

        return dtl::min(value, limit);
    }

    int16_t* getStatsPointer(ScriptStats stat)
    {
        switch (stat) {
            case ScriptStats::OFFSENSE: return &PARTNER_ENTITY.stats.off;
            case ScriptStats::DEFENSE: return &PARTNER_ENTITY.stats.def;
            case ScriptStats::SPEED: return &PARTNER_ENTITY.stats.speed;
            case ScriptStats::BRAINS: return &PARTNER_ENTITY.stats.brain;
            case ScriptStats::MAX_HP: return &PARTNER_ENTITY.stats.hp;
            case ScriptStats::MAX_MP: return &PARTNER_ENTITY.stats.mp;
            case ScriptStats::CURRENT_HP: return &PARTNER_ENTITY.stats.currentHP;
            case ScriptStats::CURRENT_MP: return &PARTNER_ENTITY.stats.currentMP;
            case ScriptStats::TIREDNESS: return &PARTNER_PARA.tiredness;
            case ScriptStats::HAPPINESS: return &PARTNER_PARA.happiness;
            case ScriptStats::DISCIPLINE: return &PARTNER_PARA.discipline;
            case ScriptStats::ENERGY: return &PARTNER_PARA.energyLevel;
            case ScriptStats::VIRUS: return &PARTNER_PARA.virusBar;
            case ScriptStats::LIFETIME: return &PARTNER_PARA.remainingLifetime;
            case ScriptStats::MERIT: return &MERIT;
            case ScriptStats::STARTED_BATTLES: return &BATTLES_STARTED;
            case ScriptStats::FLED_BATTLES: return &BATTLES_FLED;
            case ScriptStats::TOURNAMENTS_WON: return &TOURNAMENTS_WON;
            case ScriptStats::TOURNAMENT_WINS: return &TOURNAMENT_WINS;
            case ScriptStats::TOURNAMENTS_LOST: return &TOURNAMENTS_LOST;
            case ScriptStats::WEIGHT: return &PARTNER_PARA.weight;
            case ScriptStats::TAMER_LEVEL: TMP_TAMER_LEVEL = TAMER_ENTITY.tamerLevel; return &TMP_TAMER_LEVEL;
            case ScriptStats::LIVES: TMP_LIVES = PARTNER_ENTITY.lives; return &TMP_LIVES;
        }

        return nullptr;
    }

    uint32_t scriptCompareStat()
    {
        auto stat         = static_cast<ScriptStats>(readScriptUByte());
        auto mode         = readScriptUByte();
        int32_t statValue = *getStatsPointer(stat);
        auto scriptValue  = readScriptShort();
        return compareValue(mode, statValue, scriptValue);
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

        auto storedYear   = readPStat(stat);
        auto storedDay    = readPStat(stat);
        auto storedHour   = readPStat(stat);
        auto storedMinute = readPStat(stat);

        auto storedDate = dateToSeconds(storedYear, storedDay, storedHour, storedMinute);
        auto inputDate  = dateToSeconds(inputYear, inputDay, inputHour, inputMinute);

        if (compareValue(compareMode, storedDate, inputDate))
            setTrigger(trigger);
        else
            unsetTrigger(trigger);
    }

    uint8_t scriptIdToEntityId(uint8_t scriptId)
    {
        if (scriptId == 253) return 0;
        if (scriptId == 252) return 1;

        for (auto i = 2; i < 10; i++) {
            const auto* entity = reinterpret_cast<NPCEntity*>(ENTITY_TABLE.getEntityById(i));
            if (entity && entity->scriptId == scriptId) return i;
        }

        return 0xFF;
    }

    uint32_t showTextbox(int32_t id, uint32_t speaker)
    {
        auto& textbox = TEXTBOX_DATA[id];
        auto* linePtr = TEXTBOX_LINES_PTR + textbox.lineOffset * 0x40;
        if (textbox.vramMode == VRAMMode::END_HALF) linePtr += 0x20;
        if (textbox.isDoubleBuffered) linePtr += ((textbox.activeBufferId ^ 1) * textbox.lineCount * 0x40);

        auto lineOffset = 0;
        if (speaker != 0xFF) {
            auto str = linePtr;
            str[0]   = 1;
            str[1]   = getColorFromSpeaker(speaker);
            str += 2;
            auto length = getSpeakerName(speaker, str);
            str += length;
            str[0] = 1;
            str[1] = 1;
            str[2] = 0xd;
            str[3] = 0;
            lineOffset += 0x40;
        }

        auto maxWidth     = 0;
        auto currentWidth = 0;
        auto str          = linePtr + lineOffset;
        while (true) {
            auto entry = SCRIPT_POINTER[0];
            switch (entry) {
                default:
                    str[0] = SCRIPT_POINTER[0];
                    str[1] = SCRIPT_POINTER[1];
                    str += 2;
                    SCRIPT_POINTER += 2;
                    currentWidth++;
                    break;
                case 0x01:
                case 0x02:
                case 0x0C:
                case 0x0E:
                    str[0] = SCRIPT_POINTER[0];
                    str[1] = SCRIPT_POINTER[1];
                    str += 2;
                    SCRIPT_POINTER += 2;
                    break;
                case 0x03:
                    str[0] = SCRIPT_POINTER[0];
                    str[1] = SCRIPT_POINTER[1];
                    str[2] = SCRIPT_POINTER[2];
                    str[3] = SCRIPT_POINTER[3];
                    str += 4;
                    SCRIPT_POINTER += 4;
                    break;
                case 0x04:
                {
                    auto value = readPStat(SCRIPT_POINTER[1]);
                    str        = intToStringSJIS(str, value, 3, 1);
                    SCRIPT_POINTER += 2;
                    break;
                }
                case 0x05:
                {
                    SCRIPT_POINTER += 2;
                    auto width = getSpeakerName(0xFD, str);
                    str += width;
                    currentWidth += (width / 2);
                    break;
                }
                case 0x06:
                {
                    SCRIPT_POINTER += 2;
                    auto width = getSpeakerName(0xFC, str);
                    str += width;
                    currentWidth += (width / 2);
                    break;
                }
                case 0x07:
                {
                    auto name = getDigimonData(static_cast<DigimonType>(readPStat(SCRIPT_POINTER[1])))->name;
                    strcpy(str, name);
                    auto length = strlen(name);
                    str += length;
                    currentWidth += (length / 2);
                    SCRIPT_POINTER += 2;
                    break;
                }
                case 0x08:
                {
                    auto name = MOVE_NAMES[readPStat(SCRIPT_POINTER[1])];
                    strcpy(str, name);
                    auto length = strlen(name);
                    str += length;
                    currentWidth += (length / 2);
                    SCRIPT_POINTER += 2;
                    break;
                }
                case 0x09:
                {
                    auto name = getItem(static_cast<ItemType>(readPStat(SCRIPT_POINTER[1])))->name;
                    strcpy(str, name);
                    auto length = strlen(name);
                    str += length;
                    currentWidth += (length / 2);
                    SCRIPT_POINTER += 2;
                    break;
                }
                case 0x0A:
                {
                    SCRIPT_POINTER += 2;
                    str = intToStringSJIS(str, MONEY, 6, 0);
                    break;
                }
                case 0x0B:
                {
                    SCRIPT_POINTER += 2;
                    str = intToStringSJIS(str, MERIT, 4, 0);
                    break;
                }
                case 0x0D:
                {
                    str[0] = SCRIPT_POINTER[0];
                    str[1] = SCRIPT_POINTER[1];
                    SCRIPT_POINTER += 2;
                    maxWidth     = dtl::max(maxWidth, currentWidth);
                    currentWidth = 0;

                    if (*SCRIPT_POINTER == 0) {
                        str[0] = 0;
                        str[1] = 0;
                        SCRIPT_POINTER += 2;
                        if (SCRIPT_POINTER[0] != 0x1A) textbox.pageReady = 1;
                        textbox.writeCount++;
                        ACTIVE_INSTRUCTION = 0x1A;
                        return maxWidth;
                    }

                    lineOffset += 0x40;
                    str = linePtr + lineOffset;
                    break;
                }
                case 0x10:
                {
                    SCRIPT_POINTER += 2;
                    str = intToStringSJIS(str, SHOP_VARIABLE, 5, 0);
                    break;
                }
                case 0x11:
                {
                    auto name = MUSIC_NAME_LIST[readPStat(SCRIPT_POINTER[1])];
                    strcpy(str, name);
                    auto length = strlen(name);
                    str += length;
                    currentWidth += (length / 2);
                    SCRIPT_POINTER += 2;
                    break;
                }
                case 0x12:
                {
                    auto name = TOURNAMENT_NAME_LIST[readPStat(SCRIPT_POINTER[1])];
                    strcpy(str, name);
                    auto length = strlen(name);
                    str += length;
                    currentWidth += (length / 2);
                    SCRIPT_POINTER += 2;
                    break;
                }
                case 0x13:
                {
                    SCRIPT_POINTER += 2;
                    str = intToStringSJIS(str, TOURNAMENTS_WON, 3, 0);
                    break;
                }
                case 0x14:
                {
                    SCRIPT_POINTER += 2;
                    str = intToStringSJIS(str, TOURNAMENT_WINS, 3, 0);
                    break;
                }
                case 0x15:
                {
                    SCRIPT_POINTER += 2;
                    str = intToStringSJIS(str, TOURNAMENTS_LOST, 3, 0);
                    break;
                }
            }
        }
    }

    uint32_t showTextboxReady(int32_t textboxId, int32_t speaker)
    {
        TEXTBOX_DATA[textboxId].pageReady = 1;
        return showTextbox(textboxId, speaker);
    }

    void scriptStartTournament()
    {
        if (SCRIPT_STATE_2 == 3) ACTIVE_INSTRUCTION = 0;
        if (SCRIPT_STATE_2 == 2) {
            closeBox(0);
            startTournament();
            ACTIVE_INSTRUCTION = 0;
        }
        if (SCRIPT_STATE_2 == 1) return;
        if (SCRIPT_STATE_2 == 0) SCRIPT_STATE_2 = 2;
    }

    void checkTournamentMedalConditions()
    {
        // in vanilla this is leftover code that is duplicated elsewhere
        // removing this fixes the ability to get the 100 tournament won medal by winning just 100 battles
        return;
    }

    void scriptCheckTournamentMedal()
    {
        ACTIVE_INSTRUCTION = 0;
        // in vanilla this is leftover code that is duplicated elsewhere
        // removing this fixes the ability to get the 100 tournament won medal by winning just 100 battles
        return;
    }

    void handleItemLoss()
    {
        dtl::vector<int32_t> validSlots;
        validSlots.reserve(INVENTORY_SIZE);
        for (auto i = 0; i < INVENTORY_SIZE; i++) {
            if (INVENTORY_ITEM_TYPES[i] != ItemType::NONE) {
                validSlots.push_back(i);
            }
        }

        auto itemCount = validSlots.size() * 30 / 100;
        writePStat(200, itemCount); // TODO: is this ever actually used?

        for (auto i = 0; i < itemCount; i++) {
            // vanilla counts the number of taken item slots here to determine the upper limit, which means that after
            // the first iteration the last items in the bag can't be lost anymore
            // instead we're always using the full range of viable items
            // TODO: if a slot is rolled twice, one item fewer will be lost, do we want to keep it that way?
            auto roll      = random(validSlots.size());
            auto type      = INVENTORY_ITEM_TYPES[roll];
            auto recycleId = getRecycleId(type);
            if (recycleId != 0xFF) {
                auto& amount = GAME_STATE_PTR->recycleItems[recycleId];
                amount += INVENTORY_ITEM_AMOUNTS[roll];
                if (amount > 99) amount = 99;
            }
            removeItem(type, 99);
        }
    }

    void dailyPStatTrigger()
    {
        for (auto i = 0; i < 6; i++) {
            auto card = random(0x40) + 1;
            while (GAME_STATE_PTR->dailySingleCards.contains(card) || card == 4)
                card = random(0x40) + 1;

            GAME_STATE_PTR->dailySingleCards[i] = card;
        }

        for (auto i = 0x1C; i < 0x20; i++) {
            auto val = readPStat(i);
            if (val != 0xFF) writePStat(i, val + 1);
        }

        auto val = readPStat(2);
        if (val != 0xFF) writePStat(2, val & 0x7F);
    }

    bool isPartnerBaby()
    {
        // vanilla compares against a list of Digimon IDs, but why do that when we can just check the data?
        auto level = getDigimonData(PARTNER_ENTITY.type)->level;
        return level == Level::FRESH || level == Level::IN_TRAINING;
    }

    void setDialogueOwner(int32_t speaker)
    {
        if (CURRENT_DIALOGUE_OWNER == speaker) return;

        CURRENT_DIALOGUE_OWNER = speaker;
        scriptPauseGame(speaker);
        setupDialogueBox(speaker);
    }

    void showMapheadSelection(int32_t line,
                              int32_t speaker,
                              int32_t selectionCount,
                              uint32_t* usePreviousSelection,
                              uint16_t sectionId)
    {
        if (speaker == 0xFE) {
            CURRENT_DIALOGUE_OWNER = 0xFF;
            speaker                = 0xFD;
        }
        else
            setDialogueOwner(speaker);

        auto backup = SCRIPT_POINTER;
        if (sectionId == 0xFF)
            SCRIPT_POINTER = getShopkeeperLine(line);
        else
            SCRIPT_POINTER = resolveMapHeadEntry(sectionId, line);
        SELECTION_COUNT = selectionCount;
        if (*usePreviousSelection == 0) {
            *usePreviousSelection = 1;
            SELECTION_CURRENT     = 0;
        }
        auto glypthCount          = showTextboxReady(0, CURRENT_DIALOGUE_OWNER);
        SELECTION_CURSOR_WIDTH    = glypthCount * 12 + 2;
        SELECTION_CURSOR_OFFSET_Y = speaker == 0xFF ? 0 : 13;
        ACTIVE_INSTRUCTION        = 100;
        SCRIPT_POINTER            = backup;
    }

    void scriptShowSelection()
    {
        SELECTION_COUNT   = readScriptUByte();
        SELECTION_CURRENT = 0;
        SELECTION_POINTER = reinterpret_cast<uint16_t*>(SCRIPT_POINTER);
        SCRIPT_POINTER += ((SELECTION_COUNT + 1) * 2);

        auto width             = showTextbox(0, CURRENT_DIALOGUE_OWNER);
        SELECTION_CURSOR_WIDTH = dtl::min(width * 12 + 2, 240U);
        SCRIPT_POINTER += 2;
        SELECTION_END_POINTER     = SCRIPT_POINTER;
        SELECTION_CURSOR_OFFSET_Y = CURRENT_DIALOGUE_OWNER == 0xFF ? 0 : 13;
        ACTIVE_INSTRUCTION        = 0x10;
    }

    void showMapheadTextbox(int32_t line, int32_t speaker, int32_t textboxId, int32_t sectionId)
    {
        if (textboxId == 0 && speaker != 0xFE) setDialogueOwner(speaker);

        auto backup    = SCRIPT_POINTER;
        SCRIPT_POINTER = sectionId == 0xFF ? getShopkeeperLine(line) : resolveMapHeadEntry(sectionId, line);
        if (speaker == 0xFE) speaker = 0xFF;

        showTextboxReady(textboxId, speaker);
        ACTIVE_INSTRUCTION = 100;
        SCRIPT_POINTER     = backup;
    }

    void lostAllLives()
    {
        switch (SCRIPT_STATE_2) {
            case 0:
            {
                auto masteredMoves = getNumMasteredMoves();
                // lose (20 - TamerLevel * 2) percent of moves, rounded up
                auto loseCount = ((10 - TAMER_ENTITY.tamerLevel) * 2 * masteredMoves + 50) / 100;
                if (loseCount == 0)
                    ACTIVE_INSTRUCTION = 0;
                else {
                    writePStat(0xF4, loseCount);
                    SCRIPT_STATE_2 = 2;
                }
                break;
            }
            case 1: break;
            case 2:
            {
                dtl::array<uint8_t, 58> moveArray;
                auto moveCount = 0;

                for (auto i = 0; i < moveArray.size(); i++) {
                    if (!hasMove(i)) continue;
                    moveArray[moveCount++] = i;
                }

                auto move = moveArray[random(moveCount)];
                unlearnMove(move);
                writePStat(0xF3, move);
                showMapheadTextbox(7, 0xFF, 0, 1240);
                SCRIPT_STATE_2      = 1;
                SCRIPT_NEXT_STATE_2 = 3;
                SCRIPT_TEXTBOX_MODE = ScriptTextboxMode::CONFIRM;
                break;
            }
            case 3:
            {
                auto remaining = readPStat(0xF4) - 1;
                if (remaining <= 0) {
                    ACTIVE_INSTRUCTION = 0;
                }
                else {
                    writePStat(0xF4, remaining);
                    SCRIPT_STATE_2 = 2;
                }
                break;
            }
        }
    }

    bool flipTextboxPage(int32_t id)
    {
        auto& box = TEXTBOX_DATA[id];

        if (!box.isDoubleBuffered) return false;
        if (!box.registered) return true;
        if (box.writeCount != box.renderCount) return true;
        if (!box.pageReady) return false;

        box.activeBufferId ^= 1;
        box.flipCount++;
        box.pageReady = false;
        return true; // vanilla lacks a return, causing UB
    }

    void loadShopLibrary()
    {
        // SHOP_REL is never actually used
    }
}

void triggerSeadramonCutscene()
{
    callScriptSection(0, 1252, 0);
}

void checkArenaMap(int32_t mapId)
{
    constexpr dtl::array<uint16_t, 2> arenaMaps{0xD0, 0xDF};

    if (!arenaMaps.contains(mapId)) return;

    loadDynamicLibrary(Overlay::DGET_REL, nullptr, false, nullptr, nullptr);
}

uint8_t readScriptUByte()
{
    return *SCRIPT_POINTER++;
}

uint16_t readScriptUShort()
{
    auto value = *reinterpret_cast<uint16_t*>(SCRIPT_POINTER);
    SCRIPT_POINTER += 2;
    return value;
}

int16_t readScriptShort()
{
    auto value = *reinterpret_cast<int16_t*>(SCRIPT_POINTER);
    SCRIPT_POINTER += 2;
    return value;
}

int32_t readScriptInt()
{
    return readScriptUShort() + (readScriptShort() << 16);
}
