#include "Battle.h"

#include "Camera.hpp"
#include "Entity.hpp"
#include "Files.hpp"
#include "GameMenu.hpp"
#include "GameObjects.hpp"
#include "GameTime.hpp"
#include "Helper.hpp"
#include "Inventory.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "Partner.hpp"
#include "Sound.hpp"
#include "extern/BTL.hpp"
#include "extern/dw1.hpp"
#include "extern/libetc.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"
#include "extern/stddef.hpp"

namespace
{
    struct FleeBubbleState
    {
        int16_t frame;
        int16_t type;
        // vanilla has 8 additional bytes, which are unused
    };

    dtl::array<FleeBubbleState, 9> FLEE_BUBBLE_DATA;

    bool isScreenConcave()
    {
        static constexpr dtl::array<uint8_t, 18> maps =
            {166, 167, 210, 212, 219, 226, 227, 228, 247, 248, 249, 253, 254, 161, 132, 2, 13, 101};

        for (auto map : maps)
            if (map == CURRENT_SCREEN) return true;

        return false;
    }

    void loadBattleDataTick()
    {
        POLLED_INPUT          = libetc_PadRead(1);
        ACTIVE_FRAMEBUFFER    = libgs_GsGetActiveBuffer();
        ACTIVE_ORDERING_TABLE = GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER;
        libgs_GsSetWorkBase(GS_WORK_BASES[ACTIVE_FRAMEBUFFER]);
        libgs_GsClearOt(0, 0, ACTIVE_ORDERING_TABLE);
        tickObjects();
        renderObjects();
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 32, &DRAW_OFFSETS[ACTIVE_FRAMEBUFFER]);
        libgpu_DrawSync(0);
        libetc_vsync(3);
        libgs_GsSetOrign(DRAWING_OFFSET_X, DRAWING_OFFSET_Y);
        libgs_GsSwapDispBuff();
        libgs_GsSortClear(0, 0, 0, ACTIVE_ORDERING_TABLE);
        libgs_GsDrawOt(ACTIVE_ORDERING_TABLE);
        POLLED_INPUT_PREVIOUS = POLLED_INPUT;
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

            loadBattleDataTick();
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
        loadBattleDataTick();

        BTL_initializePoisonBubble();
        BTL_initializeConfusionEffect(&BTL_CONFUSION_MODEL);

        if (!IS_PREDEFINED_BATTLE) Partner_tickCollision();
        loadBattleDataTick();

        BTL_initializeStunEffect(&BTL_STUN_MODEL);

        if (!IS_PREDEFINED_BATTLE) Partner_tickCollision();
        loadBattleDataTick();

        initializeUnknownModel(&BTL_UNKNOWN_MODEL);

        if (!IS_PREDEFINED_BATTLE) Partner_tickCollision();
        loadBattleDataTick();

        initializeUnknownModelObject();
        BTL_initializeEFEEngine(GENERAL_BUFFER.data());

        if (!IS_PREDEFINED_BATTLE) Partner_tickCollision();
        loadBattleDataTick();

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

    void removeFleeBubble(int32_t entityId)
    {
        removeObject(ObjectID::FLEE_BUBBLE, entityId);
    }

    void renderFleeBubble(int32_t id)
    {
        constexpr dtl::array<int8_t, 10> offsetArray{0x64, 0x68, 0x6C, 0x70, 0x74, 0x78, 0x74, 0x70, 0x6c, 0x68};

        auto* entity = ENTITY_TABLE.getEntityById(id);
        auto& bubble = FLEE_BUBBLE_DATA[id - 2];

        SVector bubblePos{
            .x = static_cast<int16_t>(entity->posData[0].location.x),
            .y = static_cast<int16_t>(-250 - getDigimonData(entity->type)->height),
            .z = static_cast<int16_t>(entity->posData[0].location.z),
        };
        if (bubble.frame < 5) bubblePos.y = -200 - getDigimonData(entity->type)->height - ((bubble.frame + 1) * 50) / 5;

        auto pos       = getScreenPosition(bubblePos);
        auto entityPos = getScreenPosition(entity->posData->location);
        auto screenX   = clamp(pos.screenX, -140, 140);
        auto screenY   = clamp(pos.screenY, -100, 100);
        auto depth     = (VIEWPORT_DISTANCE * 0x90) / pos.depth;
        if (bubble.frame < 5)
            depth = (depth * (bubble.frame + 1)) / 5;
        else
            depth = (depth * offsetArray[(bubble.frame - 5) % 10]) / 100;

        GsSPRITE sprite{
            .attribute = 0,
            .x         = screenX,
            .y         = screenY,
            .width     = static_cast<uint16_t>(depth < 0x30 ? 24 : 23),
            .height    = static_cast<uint16_t>(depth < 0x30 ? 24 : 23),
            .tpage     = 30,
            .u         = static_cast<uint8_t>(bubble.type * 24 + 184),
            .v         = 128,
            .clutX     = 256,
            .clutY     = 487,
            .r         = 128,
            .g         = 128,
            .b         = 128,
            .mx        = 11,
            .my        = 12,
            .scaleX    = static_cast<int16_t>((depth * 4096) / (depth < 0x30 ? 24 : 23)),
            .scaleY    = static_cast<int16_t>((depth * 4096) / (depth < 0x30 ? 24 : 23)),
            .rotate    = atan(-(entityPos.screenY - screenY), entityPos.screenX - screenX),
        };

        auto* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyFT4(prim);
        libgpu_SetSemiTrans(prim, 1);
        prim->r0    = 128;
        prim->g0    = 128;
        prim->b0    = 128;
        prim->tpage = 30;
        prim->clut  = getClut(256, 0x1e7);
        prim->u0    = bubble.type * 16 + 200;
        prim->v0    = 152;
        prim->u1    = (sprite.width == 0x18 ? 16 : 15) + bubble.type * 16 - 56;
        prim->v1    = 152;
        prim->u2    = bubble.type * 16 + 200;
        prim->v2    = (sprite.width == 0x18 ? 16 : 15) + 152;
        prim->u3    = (sprite.width == 0x18 ? 16 : 15) + bubble.type * 16 - 56;
        prim->v3    = (sprite.width == 0x18 ? 16 : 15) + 152;

        auto val   = ((depth * 2) / 3);
        auto baseX = screenX - val / 2;
        auto baseY = screenY - val / 2;
        prim->x0   = baseX;
        prim->y0   = baseY;
        prim->x1   = baseX + val;
        prim->y1   = baseY;
        prim->x2   = baseX;
        prim->y2   = baseY + val;
        prim->x3   = baseX + val;
        prim->y3   = baseY + val;

        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 8, prim);
        libgs_GsSetWorkBase(prim + 1);
        libgs_GsSortSprite(&sprite, ACTIVE_ORDERING_TABLE, 8);
        bubble.frame++;
        if (LOAD_EFE_STATE == 0 || bubble.frame >= 200) removeFleeBubble(id);
    }

    void setFleeBubble(int32_t entityId, int16_t type)
    {
        FLEE_BUBBLE_DATA[entityId - 2].frame = 0;
        FLEE_BUBBLE_DATA[entityId - 2].type  = type;
        addObject(ObjectID::FLEE_BUBBLE, entityId, nullptr, renderFleeBubble);
    }

    uint32_t handleBattleStartPredefined()
    {
        auto enemySlot = 1;
        dtl::array<uint8_t, 4> val;
        for (int32_t i = 0; i < 3; i++)
        {
            val[i] = readPStat(251 + i);
            if (val[i] == 0xFF) continue;

            setFleeBubble(val[i], 0);
            COMBAT_DATA_PTR->player.entityIds[enemySlot++] = val[i];
        }

        for (int32_t i = 2; i < 10; i++)
        {
            if (val.contains(i)) continue;

            auto* entity = ENTITY_TABLE.getEntityById(i);
            if (!entity->isOnScreen)
                entity->isOnMap = false;
            else
                setFleeBubble(i, 1);
        }

        return enemySlot - 1;
    }

    int8_t getFleeChance(DigimonType partner, DigimonType enemy)
    {
        constexpr dtl::array<dtl::array<int8_t, 3>, 3> lookup{{
            {60, 70, 100},
            {80, 60, 100},
            {90, 100, 30},
        }};

        auto partnerTypus = getDigimonData(partner)->type;
        auto enemyTypus   = getDigimonData(enemy)->type;

        if (partnerTypus == Type::UNDEFINED) return 0;
        if (enemyTypus == Type::UNDEFINED) return 0;
        if (static_cast<uint8_t>(partnerTypus) == 0xFF) return 70;
        if (static_cast<uint8_t>(enemyTypus) == 0xFF) return 70;

        return lookup[static_cast<uint8_t>(partnerTypus) - 1][static_cast<uint8_t>(enemyTypus) - 1];
    }

    uint32_t handleBattleStartRandom(uint32_t talkedToEntity)
    {
        COMBAT_DATA_PTR->player.entityIds[1] = talkedToEntity;
        auto isConcave                       = isScreenConcave();
        auto enemyCount                      = 2;

        for (int32_t i = 2; i < 10; i++)
        {
            auto* entity = ENTITY_TABLE.getEntityById(i);

            if (entity == nullptr || !entity->isOnMap) continue;

            if (i == talkedToEntity)
            {
                setFleeBubble(i, 0);
                continue;
            }

            if (!entity->isOnScreen)
            {
                entity->isOnMap = false;
                continue;
            }
            if (enemyCount > 3)
            {
                setFleeBubble(i, 1);
                continue;
            }

            if (isConcave)
            {
                auto tamerTileX   = getTileX(TAMER_ENTITY.posData[0].location.x);
                auto tamerTileY   = getTileZ(TAMER_ENTITY.posData[0].location.z);
                auto enemyTileX   = getTileX(entity->posData[0].location.x);
                auto enemyTileY   = getTileZ(entity->posData[0].location.z);
                auto partnerTileX = getTileX(PARTNER_ENTITY.posData[0].location.x);
                auto partnerTileY = getTileZ(PARTNER_ENTITY.posData[0].location.z);

                auto tamerEnemyBlocked   = isLinearPathBlocked(tamerTileX, tamerTileY, enemyTileX, enemyTileY);
                auto partnerEnemyBlocked = isLinearPathBlocked(partnerTileX, partnerTileY, enemyTileX, enemyTileY);

                if (tamerEnemyBlocked || partnerEnemyBlocked)
                {
                    setFleeBubble(i, 1);
                    continue;
                }
            }

            auto roll     = random(100);
            auto hasRepel = getItemCount(ItemType::ENEMY_REPEL) > 0;
            auto hasBell  = getItemCount(ItemType::ENEMY_BELL) > 0;

            if (hasBell)
                roll -= 50;
            else if (hasRepel)
                roll += 20;

            if (roll < getFleeChance(PARTNER_ENTITY.type, entity->type))
            {
                COMBAT_DATA_PTR->player.entityIds[enemyCount++] = i;
                setFleeBubble(i, 0);
            }
            else
                setFleeBubble(i, 1);
        }

        return enemyCount - 1;
    }

    uint32_t handleBattleStart(uint32_t talkedToEntity)
    {
        COMBAT_DATA_PTR->player.entityIds[0] = 1;
        COMBAT_DATA_PTR->player.unk4         = 0;
        FLEE_DISABLED                        = isTriggerSet(1);
        IS_PREDEFINED_BATTLE                 = readPStat(0xfa);

        if (IS_PREDEFINED_BATTLE) return handleBattleStartPredefined();

        return handleBattleStartRandom(talkedToEntity);
    }

} // namespace

extern "C"
{
    void handleBattleIdle(DigimonEntity* entity, Stats* stats, BattleFlags flags)
    {
        if (NO_AI_FLAG != 0 && entity != FINISHING_ENTITY) return;
        if (entity->animId == 33) return;
        if (entity->animId == 34) return;

        startBattleIdleAnimation(entity, &entity->stats, flags);
    }
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