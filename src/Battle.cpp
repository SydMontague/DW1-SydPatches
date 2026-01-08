#include "Camera.hpp"
#include "Entity.hpp"
#include "Files.hpp"
#include "GameMenu.hpp"
#include "GameTime.hpp"
#include "Helper.hpp"
#include "Map.hpp"
#include "Model.hpp"
#include "Partner.hpp"
#include "Sound.hpp"
#include "extern/BTL.hpp"
#include "extern/dw1.hpp"
#include "extern/libgs.hpp"
#include "extern/stddef.hpp"

namespace
{
    bool isScreenConcave()
    {
        static constexpr dtl::array<uint8_t, 18> maps =
            {166, 167, 210, 212, 219, 226, 227, 228, 247, 248, 249, 253, 254, 161, 132, 2, 13, 101};

        for (auto map : maps)
            if (map == CURRENT_SCREEN) return true;

        return false;
    }

    template<typename T> static void waitUntil(T fn)
    {
        while (!fn())
        {
            if (!IS_PREDEFINED_BATTLE)
            {
                for (int32_t i = 1; i <= ENEMY_COUNT; i++)
                    entityLookAtLocation(ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[i]),
                                         &PARTNER_ENTITY.posData->location);

                Partner_tickCollision();
            }

            loadCombatDataTick();
        }
    }

    int32_t playBattleMusic(uint32_t talkedToEntity)
    {
        static constexpr dtl::array<uint8_t, 45> music{
            2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 1, 1,
            1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 1, 1, 2, 1, 2, 2, 2,
        };

        if (ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[1])->type == DigimonType::MACHINEDRAMON)
            return 3;

        if (IS_PREDEFINED_BATTLE) return 0;

        if (talkedToEntity < 2 || talkedToEntity > 9) return 1;

        auto enemyType = static_cast<uint32_t>(ENTITY_TABLE.getEntityById(talkedToEntity)->type) - 67;

        if (enemyType < 0 || enemyType > music.size()) return 1;

        return music[enemyType];
    }

    void loadBattleData(uint32_t talkedToEntity, uint32_t enemyCount)
    {
        LOAD_EFE_STATE = -1;
        playMusic(33, playBattleMusic(talkedToEntity));

        uint8_t finishedLoading;
        loadDynamicLibrary(Overlay::BTL_REL, &finishedLoading, true, nullptr, nullptr);
        tickPartnerWaypoints();
        PARTNER_ENTITY.isOnScreen                 = 1;
        ENEMY_COUNT                               = enemyCount;
        COMBAT_DATA_PTR->player.currentCommand[0] = BattleCommand::YOUR_CALL;
        startAnimation(&TAMER_ENTITY, 1);

        for (int32_t i = 2; i < 10; i++)
        {
            auto* entity = ENTITY_TABLE.getEntityById(i);
            if (!isInvisible(entity)) startAnimation(entity, 33);
        }

        GAME_STATE = 3;
        if (!IS_PREDEFINED_BATTLE) startAnimation(&PARTNER_ENTITY, 36);

        // wait for the library to load
        waitUntil([&finishedLoading] { return finishedLoading == 0; });

        // start sound loading
        loadSB();
        auto lastId = 4;
        for (int32_t i = 1; i <= ENEMY_COUNT; i++)
        {
            auto* entity =
                reinterpret_cast<DigimonEntity*>(ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[i]));
            entity->vabId = 4 + i;
            lastId        = readVBALLSection(4 + i, static_cast<int32_t>(entity->type));
        }

        // wait for sounds to finish loading
        waitUntil([&] { return isSoundBufferLoadingDone(lastId) == 0; });

        BTL_initializeBattleItemParticles();
        BTL_initializeUnk3();
        BTL_initializeUnk2();
        BTL_loadEmbeddedTextures(&BTL_EMBEDDED_TEXTURE1, &BTL_EMBEDDED_TEXTURE2);

        if (!IS_PREDEFINED_BATTLE) Partner_tickCollision();
        loadCombatDataTick();

        BTL_initializePoisonBubble();
        BTL_initializeConfusionEffect(&BTL_CONFUSION_MODEL);

        if (!IS_PREDEFINED_BATTLE) Partner_tickCollision();
        loadCombatDataTick();

        BTL_initializeStunEffect(&BTL_STUN_MODEL);

        if (!IS_PREDEFINED_BATTLE) Partner_tickCollision();
        loadCombatDataTick();

        initializeUnknownModel(&BTL_UNKNOWN_MODEL);

        if (!IS_PREDEFINED_BATTLE) Partner_tickCollision();
        loadCombatDataTick();

        initializeUnknownModelObject();
        BTL_initializeEFEEngine(GENERAL_BUFFER.data());

        if (!IS_PREDEFINED_BATTLE) Partner_tickCollision();
        loadCombatDataTick();

        dtl::array<int16_t, 18> moves{};
        dtl::array<int16_t, 18> effectIds{};
        int32_t moveCount = 0;

        for (int32_t i = 0; i <= ENEMY_COUNT; i++)
        {
            auto* entity =
                reinterpret_cast<DigimonEntity*>(ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[i]));
            entity->unk1   = -1;
            entity->unk2_1 = 0xFF;

            for (auto& move : entity->stats.moves)
            {
                if (move == 0xFF) continue;
                auto tech = entityGetTechFromAnim(entity, move);
                if (tech == 0xFF)
                {
                    move = 0xFF;
                    continue;
                }
                moves[moveCount++] = tech + 256;
            }
        }
        moves[moveCount] = -1;

        BTL_loadMoveEFE(moves.data(), effectIds.data(), &LOAD_EFE_STATE);

        // vanilla calls isInvisible on all entities, but there are no side effects?
        waitUntil([] { return LOAD_EFE_STATE <= 0; });

        auto effectSlot = 0;
        for (int32_t i = 0; i <= ENEMY_COUNT; i++)
        {
            auto* entity =
                reinterpret_cast<DigimonEntity*>(ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[i]));

            for (int32_t j = 0; j < 4; j++)
            {
                if (entity->stats.moves[j] == 0xFF)
                    COMBAT_DATA_PTR->fighter[i].effectSlot[j] = -1;
                else
                    COMBAT_DATA_PTR->fighter[i].effectSlot[j] = effectIds[effectSlot++];
            }
        }

        handleBattleIdle(&PARTNER_ENTITY, &PARTNER_ENTITY.stats, {0});
    }
} // namespace

extern "C"
{
    void advanceBattleTime(BattleResult result)
    {
        CURRENT_FRAME += 400;
        MINUTE += 20;

        // TODO add more missing time mechanics from tickGameClock/unify the code
        if (MINUTE >= 60)
        {
            // TODO this is missing a lot of hourly mechanics
            HOUR += 1;
            MINUTE = MINUTE % 60;
            PARTNER_PARA.remainingLifetime -= 1;
            // vanilla updates neither evo timer
            PARTNER_PARA.evoTimer += 1;

            // vanilla doesn't perform happiness lifetime penalty
            if ((HOUR % 4) == 0 && PARTNER_PARA.happiness < 80)
                PARTNER_PARA.remainingLifetime -= getHappinessLifetimePenalty(PARTNER_PARA.happiness);

            if (PARTNER_PARA.remainingLifetime < 0) PARTNER_PARA.remainingLifetime = 0;

            if (PARTNER_PARA.condition.isSleepy)
            {
                PARTNER_PARA.sicknessCounter += 1;
                PARTNER_PARA.missedSleepHours += 1;
            }
            if (PARTNER_PARA.condition.isSick)
            {
                PARTNER_PARA.sicknessTries += 1;
                PARTNER_PARA.sicknessTimer += 1;
            }
            if (PARTNER_PARA.condition.isInjured) PARTNER_PARA.injuryTimer += 1;

            if (HOUR >= 24)
            {
                DAY += 1;
                CURRENT_FRAME = MINUTE * 20;
                HOUR          = HOUR % 24;
                // vanilla doesn't update age
                PARTNER_PARA.age += 1;
                dailyPStatTrigger();

                if (DAY >= 30)
                {
                    YEAR += 1;
                    DAY = 0;
                    if (YEAR > 99) YEAR = 0;
                }
            }
        }

        // vanilla checks if the starvation timer is negative here, leading to a duplicate care mistake
        if (PARTNER_PARA.condition.isHungry)
            PARTNER_PARA.starvationTimer -= 40;
        else
            PARTNER_PARA.foodLevel -= 20;

        if (PARTNER_PARA.condition.isPoopy)
            PARTNER_PARA.poopingTimer -= 400;
        else
            PARTNER_PARA.poopLevel -= 2;

        updateMinuteHand(HOUR, MINUTE);

        if (result != BattleResult::WON) return;
        if ((MAP_ENTRIES[CURRENT_SCREEN].flags & 0x40) != 0) return;

        if (HOUR == 16) initializeDaytimeTransition(0);
        if (HOUR == 20) initializeDaytimeTransition(1);
        if (HOUR == 6) initializeDaytimeTransition(2);
    }

    BattleResult startBattle(uint32_t talkedToEntity)
    {
        GAME_STATE       = 1;
        STORED_TAMER_POS = TAMER_ENTITY.posData->location;
        unsetCameraFollowPlayer();
        closeTriangleMenu();
        stopGameTime();

        auto enemyCount = handleBattleStart(talkedToEntity);
        loadBattleData(talkedToEntity, enemyCount);

        BattleResult result = BTL_battleMain();

        GAME_STATE = 0;
        if (result == BattleResult::LOST)
        {
            PARTNER_PARA.happiness -= 30;
            PARTNER_PARA.discipline -= 20;
            Partner_setState(0xFF);
            SKIP_DAYTIME_TRANSITION = 1;
        }
        else if (result == BattleResult::FLED)
        {
            PARTNER_PARA.happiness -= 10;
            PARTNER_PARA.discipline -= 6;
            PARTNER_PARA.tiredness += 2;
            handlePostBattleTiredness();
            SKIP_DAYTIME_TRANSITION = 1;
        }
        else if (result == BattleResult::WON)
        {
            playBGM(ACTIVE_BGM_FONT);
            readMapTFS(CURRENT_SCREEN);
            STORED_TAMER_POS = TAMER_ENTITY.posData->location;
            Partner_setState(1);
            PARTNER_PARA.happiness += 2;
            PARTNER_PARA.battles += 1; // TODO: enemy count?
            handlePostBattleTiredness();
            SKIP_DAYTIME_TRANSITION = 0;
        }

        tickConditionBoundaries();

        ACTIVE_FRAMEBUFFER = libgs_GsGetActiveBuffer();
        libgs_GsSetWorkBase(GS_WORK_BASES + ACTIVE_FRAMEBUFFER);
        libgs_GsClearOt(0, 0, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER);
        ACTIVE_ORDERING_TABLE = GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER;
        advanceBattleTime(result);
        return result;
    }

    void damageTick(FighterData* fighter, Stats* stats)
    {
        // TODO shouldn't this be if/else instead?
        if (fighter->hpDamageBuffer > 999)
        {
            stats->currentHP -= 900;
            fighter->hpDamageBuffer -= 900;
        }
        if (fighter->hpDamageBuffer > 99)
        {
            stats->currentHP -= 80;
            fighter->hpDamageBuffer -= 80;
        }
        if (fighter->hpDamageBuffer > 9)
        {
            stats->currentHP -= 6;
            fighter->hpDamageBuffer -= 6;
        }
        if (fighter->hpDamageBuffer > 0)
        {
            stats->currentHP -= 1;
            fighter->hpDamageBuffer -= 1;
        }

        if (stats->currentHP <= 0)
        {
            stats->currentHP        = 0;
            fighter->hpDamageBuffer = 0;
        }
    }

    void swapByte(uint8_t* left, uint8_t* right)
    {
        dtl::swap(*left, *right);
    }

    void swapShort(uint16_t* left, uint16_t* right)
    {
        dtl::swap(*left, *right);
    }

    void swapInt(uint32_t* left, uint32_t* right)
    {
        dtl::swap(*left, *right);
    }
}