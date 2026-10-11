
#include "Evolution.hpp"
#include "Helper.hpp"
#include "Inventory.hpp"
#include "Math.hpp"
#include "Partner.hpp"
#include "RecycleShop.hpp"
#include "Tournament.hpp"
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

} // namespace

extern "C"
{
    int32_t getScriptSyncBit()
    {
        return SOME_SCRIPT_SYNC_BIT;
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

    bool isTextboxBusy(int32_t id)
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
}
