#include "../extern/VS.hpp"

#include "../Entity.hpp"
#include "../Files.hpp"
#include "../GameObjects.hpp"
#include "../Model.hpp"
#include "../Pause.hpp"
#include "../Sound.hpp"
#include "../extern/dtl/types.hpp"
#include "../extern/dw1.hpp"
#include "../extern/libetc.hpp"

namespace
{
    enum class DefeatedState
    {
        ALIVE,
        DYING,
        DEAD,
    };

    uint32_t inputCurrent  = 0;
    uint32_t inputPrevious = 0;
    bool pauseFrame        = false;

    DigimonEntity* getFighterEntity(int32_t fighterId)
    {
        return reinterpret_cast<DigimonEntity*>(
            ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[fighterId]));
    }

    void prepareCommandList(DigimonEntity* entity, int32_t playerId)
    {
        auto brains                                            = entity->stats.brain;
        COMBAT_DATA_PTR->player.availableCommands[playerId][0] = BattleCommand::FINISHER;

        if (brains < 100)
            COMBAT_DATA_PTR->player.availableCommands[playerId][1] = BattleCommand::YOUR_CALL;
        else if (brains < 200) {
            COMBAT_DATA_PTR->player.availableCommands[playerId][1] = BattleCommand::ATTACK;
            COMBAT_DATA_PTR->player.availableCommands[playerId][2] = BattleCommand::YOUR_CALL;
            COMBAT_DATA_PTR->player.numCommands[playerId]          = 3;
        }
        else if (brains < 300) {
            COMBAT_DATA_PTR->player.availableCommands[playerId][1] = BattleCommand::MODERATOR;
            COMBAT_DATA_PTR->player.availableCommands[playerId][2] = BattleCommand::ATTACK;
            COMBAT_DATA_PTR->player.availableCommands[playerId][3] = BattleCommand::YOUR_CALL;
            COMBAT_DATA_PTR->player.numCommands[playerId]          = 4;
        }
        else if (brains < 400) {
            COMBAT_DATA_PTR->player.availableCommands[playerId][1] = BattleCommand::DISTANCE;
            COMBAT_DATA_PTR->player.availableCommands[playerId][2] = BattleCommand::MODERATOR;
            COMBAT_DATA_PTR->player.availableCommands[playerId][3] = BattleCommand::ATTACK;
            COMBAT_DATA_PTR->player.availableCommands[playerId][4] = BattleCommand::YOUR_CALL;
            COMBAT_DATA_PTR->player.numCommands[playerId]          = 5;
        }
        else if (brains < 500) {
            COMBAT_DATA_PTR->player.availableCommands[playerId][1] = BattleCommand::DEFEND;
            COMBAT_DATA_PTR->player.availableCommands[playerId][2] = BattleCommand::DISTANCE;
            COMBAT_DATA_PTR->player.availableCommands[playerId][3] = BattleCommand::MODERATOR;
            COMBAT_DATA_PTR->player.availableCommands[playerId][4] = BattleCommand::ATTACK;
            COMBAT_DATA_PTR->player.availableCommands[playerId][5] = BattleCommand::YOUR_CALL;
            COMBAT_DATA_PTR->player.numCommands[playerId]          = 6;
        }
        else {
            COMBAT_DATA_PTR->player.availableCommands[playerId][1] = BattleCommand::DEFEND;
            COMBAT_DATA_PTR->player.availableCommands[playerId][2] = BattleCommand::DISTANCE;
            auto slotId                                            = 3;
            if (entity->stats.moves[2] != 0xFF)
                COMBAT_DATA_PTR->player.availableCommands[playerId][slotId++] = BattleCommand::ATTACK3;
            if (entity->stats.moves[1] != 0xFF)
                COMBAT_DATA_PTR->player.availableCommands[playerId][slotId++] = BattleCommand::ATTACK2;
            if (entity->stats.moves[0] != 0xFF)
                COMBAT_DATA_PTR->player.availableCommands[playerId][slotId++] = BattleCommand::ATTACK1;
            COMBAT_DATA_PTR->player.availableCommands[playerId][slotId++] = BattleCommand::YOUR_CALL;

            COMBAT_DATA_PTR->player.numCommands[playerId] = slotId;
        }

        COMBAT_DATA_PTR->player.hoveredCommand[playerId] = COMBAT_DATA_PTR->player.numCommands[playerId] - 1;
        COMBAT_DATA_PTR->player.buffereCommand[playerId] = BattleCommand::YOUR_CALL;
        COMBAT_DATA_PTR->player.currentCommand[playerId] = BattleCommand::YOUR_CALL;
    }

    void prepareFighterData(DigimonEntity* entity, int32_t playerId)
    {
        VS__STARTING_HP[playerId]              = entity->stats.currentHP;
        INITIAL_COMBAT_STATS[playerId].hp      = entity->stats.hp;
        INITIAL_COMBAT_STATS[playerId].mp      = entity->stats.mp;
        INITIAL_COMBAT_STATS[playerId].offense = entity->stats.off;
        INITIAL_COMBAT_STATS[playerId].defense = entity->stats.def;
        INITIAL_COMBAT_STATS[playerId].speed   = entity->stats.speed;
        INITIAL_COMBAT_STATS[playerId].brains  = entity->stats.brain;

        auto& fighter = COMBAT_DATA_PTR->fighter[playerId];
        auto brains   = entity->stats.brain;

        entity->stats.unk2_2                = 0;
        fighter.targetId                    = playerId ^ 1;
        fighter.hpDamageBuffer              = 0;
        fighter.mpDamageBuffer              = 0;
        fighter.flags                       = {};
        fighter.moveRange                   = 0;
        fighter.flatTimer                   = 0;
        fighter.invulnerableTimer           = 0;
        fighter.cooldown                    = 0;
        fighter.finisherProgress            = 0;
        fighter.statusFxId                  = -1;
        fighter.activeEffectSlot            = -1;
        fighter.speedBuffer                 = 100;
        fighter.unk15                       = 0;
        fighter.hasCollidedWhileDistanceCmd = 0;
        fighter.buffsRemaining              = brains < 600 ? dtl::min((brains / 100) + 1, 4) : 5;
        fighter.buffPrioTimer               = (brains / 10) + 5;
        fighter.finisherGoal                = 3000 - entity->stats.speed;
        for (auto& val : fighter.table1)
            val = 0xFF;
        for (auto& val : fighter.table2)
            val = 0xFF;
    }

    [[gnu::optimize("Os")]]
    void loadMoves()
    {
        dtl::array<int16_t, 16> moves;
        dtl::array<int16_t, 16> outMoves;
        int8_t isBusy;
        int32_t moveCount = 0;
        for (int32_t i = 0; i < 2; i++) {
            auto entity          = getFighterEntity(i);
            entity->stats.unk1   = -1;
            entity->stats.unk2_1 = 0xFF;
            for (auto& move : entity->stats.moves) {
                if (move == 0xFF) continue;

                moves[moveCount++] = getDigimonData(entity->type)->moves[move - 0x2e] + 256;
            }
        }
        moves[moveCount] = -1;
        VS__loadMoveEFE(moves.data(), outMoves.data(), &isBusy);
        while (isBusy > 0)
            tickFileReadQueue();

        int32_t slot = 0;
        for (int32_t i = 0; i < 2; i++) {
            auto entity = getFighterEntity(i);
            for (int32_t j = 0; j < 4; j++) {
                auto move                                 = entity->stats.moves[j];
                COMBAT_DATA_PTR->fighter[i].effectSlot[j] = move == 0xFF ? -1 : outMoves[slot++];
            }
        }
    }

    DefeatedState isDigimonDeafeated(int32_t fighterId)
    {
        auto entity = getFighterEntity(fighterId);

        // HP is above 0, so can't be dead
        if (entity->stats.currentHP > COMBAT_DATA_PTR->fighter[fighterId].hpDamageBuffer) return DefeatedState::ALIVE;

        // knockdown anim not done yet
        if (entity->animId != 0x2B || (entity->animFlag & 1) != 0) return DefeatedState::DYING;

        return DefeatedState::DEAD;
    }

    int8_t isPausePressed()
    {
        constexpr uint32_t MASK_P1 = 0x800;
        constexpr uint32_t MASK_P2 = MASK_P1 << 0x10;

        inputPrevious = inputCurrent;
        inputCurrent  = libetc_PadRead(1);
        if ((inputCurrent & MASK_P1) != 0 && (inputPrevious & MASK_P1) == 0) return 1;
        if ((inputCurrent & MASK_P2) != 0 && (inputPrevious & MASK_P2) == 0) return 2;
        return 0;
    }

    void VS__removePlayerMarker()
    {
        removeObject(ObjectID::PLAYER_MARKER, 0);
        removeObject(ObjectID::PLAYER_MARKER, 1);
    }

    void VS__deinitializeStatusEffects()
    {
        if (PARTNER_ENTITY.type == NPC_ENTITIES[0].type) VS__removePlayerMarker();
        VS__deinitializeStun();
        VS__deinitializeFinisherAura();
        VS__deinitializePoison();
        VS__deinitializeFlatBullets();
        for (int32_t i = 0; i <= ENEMY_COUNT; i++) {
            auto* fighter = &COMBAT_DATA_PTR->fighter[i];
            auto* entity =
                reinterpret_cast<DigimonEntity*>(ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[i]));
            VS__cancelActiveEffect(entity, fighter);
        }
        VS__unloadMoveData();
        VS__deinitializeEFEEngine();
    }
} // namespace

extern "C"
{
    void VS__combatInit()
    {
        resetFlattenGlobal();
        initializeAttackObjects();
        VS__addTimer(99);

        if (PARTNER_ENTITY.type == NPC_ENTITIES[0].type) VS__initializePlayerMarker();

        VS__IS_DRAW                 = 0;
        VS__ACTIVE_FINISHER_AURA_ID = -1;
        BATTLE_FRAME_COUNT          = 1;
        ENEMY_COUNT                 = 1;
        FLEE_DISABLED               = 1;
        NO_AI_FLAG                  = 0;
        VS__TIMER_ACTIVE            = 0;
        VS__DISABLE_HITTING         = 0;

        for (int32_t i = 0; i < 2; i++) {
            COMBAT_DATA_PTR->player.entityIds[i]             = i + 1;
            auto entity                                      = getFighterEntity(i);
            COMBAT_DATA_PTR->player.remainingChargeupTime[i] = -1;
            VS__DAMAGE[i]                                    = 0;
            VS__AOE_TIMER[i]                                 = 0;
            VS__CHARGE_MODES[i]                              = entity->stats.chargeMode;
            prepareFighterData(entity, i);
            prepareCommandList(entity, i);
            VS__addHPMPBar(i);
            VS__addCommandBar(i);
        }
    }

    void VS__combatSetup()
    {
        GAME_STATE = 5;
        startAnimation(&PARTNER_ENTITY, 33);
        entityLookAtLocation(&PARTNER_ENTITY, &NPC_ENTITIES[0].posData->location);
        startAnimation(&NPC_ENTITIES[0], 33);
        entityLookAtLocation(&NPC_ENTITIES[0], &PARTNER_ENTITY.posData->location);
        VS__initializeEFEEngine(GENERAL_BUFFER.data());

        loadMoves();

        VS__DEFAULT_CAM_MIN_DISTANCE = 200;
        VS__addBattleTextIn();
        playSound(0, 16);

        int32_t frameCount = 0;
        uint8_t result     = true;
        do {
            if (VS__DEFAULT_CAM_MIN_DISTANCE < 4200) VS__DEFAULT_CAM_MIN_DISTANCE += 400;

            result = VS__isBattleTextFinished();
            VS__tickFrame();
        } while (++frameCount < 60 || result == 0);

        VS__removeBattleTextIn();
        VS__addBattleTextOut();
        playSound(0, 17);

        while (VS__isBattleTextFinished() == 0) {
            if (VS__DEFAULT_CAM_MIN_DISTANCE > 1000) VS__DEFAULT_CAM_MIN_DISTANCE -= 400;
            VS__tickFrame();
        }
        VS__removeBattleTextOut();

        VS__DEFAULT_CAM_MIN_DISTANCE = 1000;
        VS__TIMER_ACTIVE             = 1;
        VS__CAMERA_STATE             = 1;
        GAME_STATE                   = 4;
    }

    // -1 -> P1 lost
    // 0 -> ongoing
    // 1 -> P2 lost
    // 2 -> draw
    int32_t VS__checkEndCondition()
    {
        if (COMBAT_DATA_PTR->fighter[0].hpDamageBuffer != 0) return 0;
        if (COMBAT_DATA_PTR->fighter[1].hpDamageBuffer != 0) return 0;

        auto defeatedP1 = isDigimonDeafeated(0);
        auto defeatedP2 = isDigimonDeafeated(1);

        if (defeatedP1 == DefeatedState::DYING || defeatedP2 == DefeatedState::DYING) return 0;
        if (defeatedP1 == DefeatedState::DEAD && defeatedP2 == DefeatedState::ALIVE) return -1;
        if (defeatedP1 == DefeatedState::ALIVE && defeatedP2 == DefeatedState::DEAD) return 1;
        if (defeatedP1 == DefeatedState::DEAD && defeatedP2 == DefeatedState::DEAD) return 2;

        if (VS__TIMER == 0) {
            VS__DAMAGE[0]                              = VS__STARTING_HP[0] - getFighterEntity(0)->stats.currentHP;
            VS__DAMAGE[1]                              = VS__STARTING_HP[1] - getFighterEntity(1)->stats.currentHP;
            COMBAT_DATA_PTR->fighter[0].hpDamageBuffer = 0;
            COMBAT_DATA_PTR->fighter[1].hpDamageBuffer = 0;

            if (VS__DAMAGE[0] < VS__DAMAGE[1]) {
                VS__tickBattleResultScreen(0, 1);
                return 1;
            }
            if (VS__DAMAGE[0] > VS__DAMAGE[1]) {
                VS__tickBattleResultScreen(1, 0);
                return -1;
            }
            if (VS__DAMAGE[0] == VS__DAMAGE[1]) {
                VS__tickBattleResultScreen(1, 1);
                return 2;
            }
        }

        return 0;
    }

    void VS__handlePause()
    {
        if (pauseFrame) {
            pauseFrame = false;
            return;
        }

        while (VS__PAUSING_PLAYER != 0 && VS__PAUSING_PLAYER != isPausePressed())
            ;

        removePauseBox();

        VS__PAUSING_PLAYER = isPausePressed();
        if (VS__PAUSING_PLAYER != 0) {
            createPauseBox();
            pauseFrame = true;
        }
    }

    int32_t VS__deinitializeCombat(int32_t lostP1, int32_t lostP2)
    {
        PARTNER_ENTITY.stats.chargeMode  = VS__CHARGE_MODES[0];
        NPC_ENTITIES[0].stats.chargeMode = VS__CHARGE_MODES[1];
        GAME_STATE                       = 5;
        VS__deinitializeStatusEffects();

        for (int32_t i = 0; i <= ENEMY_COUNT; i++) {
            auto* fighter = &COMBAT_DATA_PTR->fighter[i];
            auto* entity =
                reinterpret_cast<DigimonEntity*>(ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[i]));
            removeEntityText(i);
            VS__resetFlatten(i);
            VS__removeAllStatusFX(entity, fighter);
            fighter->flags                       = {};
            fighter->moveRange                   = 0;
            fighter->targetId                    = 0;
            fighter->queuedAnim                  = 0;
            fighter->buffsRemaining              = 0;
            fighter->buffPrioTimer               = 0;
            fighter->hasCollidedWhileDistanceCmd = 0;

            entity->stats.off   = INITIAL_COMBAT_STATS[i].offense;
            entity->stats.def   = INITIAL_COMBAT_STATS[i].defense;
            entity->stats.speed = INITIAL_COMBAT_STATS[i].speed;
        }

        if (lostP1 == lostP2) {
            if (VS__TIMER != 0) {
                stopBGM();
                stopSound();
                VS__initializeDrawModel();
                VS__addWinLossDrawWindow();
                while (VS__checkWinLossDrawTimer())
                    VS__tickFrame();
                VS__removeWinLossDrawWindow();
            }
        }
        else {
            auto winningEntity =
                reinterpret_cast<DigimonEntity*>(ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[lostP1]));
            startAnimation(winningEntity, 0x2A);
            winningEntity->animFlag |= 2;
            stopBGM();
            stopSound();
            playMusic(VS_MUSIC, 3);
            for (int32_t i = 0; i < 140; i++) {
                VS__tickFrame();
                if ((winningEntity->animFlag & 1) == 0) {
                    startAnimation(winningEntity, 0x2A);
                    winningEntity->animFlag |= 2;
                }
            }
        }

        for (int32_t i = 0; i < 20; i++)
            VS__tickFrame();

        VS__removeHPMPBar(0);
        VS__removeHPMPBar(1);
        VS__removeCommandBar(0);
        VS__removeCommandBar(1);
        VS__removeVSTimer();
        stopBGM();
        stopSound();

        GAME_STATE = 0;
        return lostP1 == lostP2 ? 2 : lostP1;
    }
}
