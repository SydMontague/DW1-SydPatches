#include "Helper.hpp"
#include "Partner.hpp"
#include "extern/BTL.hpp"
#include "extern/dw1.hpp"
#include "extern/libgs.hpp"
#include "extern/stddef.hpp"

extern "C"
{
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