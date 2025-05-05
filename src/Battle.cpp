#include "GameMenu.hpp"
#include "Map.hpp"
#include "Helper.hpp"
#include "Partner.hpp"
#include "extern/BTL.hpp"
#include "extern/dw1.hpp"
#include "extern/libgs.hpp"
#include "extern/stddef.hpp"

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
}