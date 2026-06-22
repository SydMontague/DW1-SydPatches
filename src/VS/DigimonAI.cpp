#include "../Battle.h"
#include "../Entity.hpp"
#include "../Math.hpp"
#include "../extern/VS.hpp"
#include "../extern/dtl/types.hpp"
#include "InitVS.hpp"

namespace
{
    void tickSenile(int32_t playerId, DigimonEntity* entity, FighterData& fighter)
    {
        if (fighter.hpDamageBuffer >= entity->stats.currentHP) return;
        if (entity->stats.brain > 300) return;
        if (BATTLE_FRAME_COUNT % ((entity->stats.brain * 2 + 1) * 20) != 0) return;
        if (random(100) >= 70 - VS_DISCIPLINE[playerId]) return;

        fighter.flags.isSenile = true;
        fighter.senileTimer    = 100;
    }

    void tickSomeCameraState()
    {
        if (VS__CAMERA_STATE == 7 || VS__CAMERA_STATE == 8) return;

        if (VS__CAMERA_TIMER != 0 && --VS__CAMERA_TIMER == 0) VS__CAMERA_STATE = 1;

        if (BATTLE_FRAME_COUNT % 600 == 0 && random(2) == 1) {
            VS__CAMERA_STATE = 6;
            VS__trySetVictimCamera(&PARTNER_ENTITY, 4);
            VS__CAMERA_TIMER = 60 + random(41);
        }

        auto& fighter1 = COMBAT_DATA_PTR->fighter[0];
        auto& fighter2 = COMBAT_DATA_PTR->fighter[1];

        if ((fighter1.flags.isAttacking || fighter1.flags.isBlocking) &&
            (fighter2.flags.isAttacking || fighter2.flags.isBlocking))
            VS__CAMERA_STATE = 1;
    }

} // namespace

extern "C"
{
    void VS__tickDigimonAI(int32_t playerId)
    {
        auto* entity =
            reinterpret_cast<DigimonEntity*>(ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[playerId]));
        auto& fighter = COMBAT_DATA_PTR->fighter[playerId];

        if (BATTLE_FRAME_COUNT % 20 == 0 && fighter.buffPrioTimer != 0) fighter.buffPrioTimer--;

        if (COMBAT_DATA_PTR->player.commandDelay[playerId] == 0)
            COMBAT_DATA_PTR->player.currentCommand[playerId] = COMBAT_DATA_PTR->player.buffereCommand[playerId];
        else if ((fighter.flags.raw & 0x800e) == 0 && fighter.flatTimer == 0)
            COMBAT_DATA_PTR->player.commandDelay[playerId]--;

        if (NO_AI_FLAG == 0) {
            tickSenile(playerId, entity, fighter);

            if (fighter.cooldown > 1) fighter.cooldown--;
            if (!fighter.flags.isSenile) VS__increaseSpeedBuffer(&fighter, &entity->stats);
        }

        if ((fighter.flags.raw & 0x80B0) != 0) return;

        auto targetId = (playerId + 1) % 2;
        auto targetEntity =
            reinterpret_cast<DigimonEntity*>(ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[targetId]));
        auto& targetFighter = COMBAT_DATA_PTR->fighter[targetId];

        if (entity->stats.currentHP == 0) {
            VS__faintDigimon(entity, &fighter, playerId);
            if (targetFighter.hpDamageBuffer < targetEntity->stats.currentHP) VS__trySetAttackerCamera(entity, 5, 0);

            VS__DISABLE_HITTING = 1;
            return;
        }

        if (targetFighter.hpDamageBuffer >= targetEntity->stats.currentHP ||
            fighter.hpDamageBuffer >= entity->stats.currentHP) {
            handleBattleIdle(entity, &entity->stats, fighter.flags);
            fighter.moveRange = 0xFF;
            VS__resetFlatten(playerId);
            VS__removeAllStatusFX(entity, &fighter);
            fighter.flags = {.isTransforming = true};
            return;
        }

        if (NO_AI_FLAG == 1) return;

        if ((fighter.flags.raw & 0x800e) == 0 && fighter.flatTimer == 0) {
            auto currentCommand = COMBAT_DATA_PTR->player.currentCommand[playerId];
            if (currentCommand == BattleCommand::FINISHER) {
                fighter.targetId = targetId;
                if (entity->stats.moves[3] != fighter.queuedAnim || fighter.moveRange < 1)
                    VS__setMoveAnim(entity, &fighter, playerId, 3);
                VS__setChargeupFlag(entity, &fighter, getDigimonData(entity->type)->moves[fighter.queuedAnim]);
                return;
            }

            auto slotId = static_cast<int32_t>(currentCommand) - 8;
            if ((currentCommand == BattleCommand::ATTACK1 || currentCommand == BattleCommand::ATTACK2 ||
                 currentCommand == BattleCommand::ATTACK3) &&
                VS__canAffordMove(entity, &fighter, slotId)) {
                fighter.targetId = targetId;
                if (entity->stats.moves[slotId] != fighter.queuedAnim || fighter.moveRange < 1)
                    VS__setMoveAnim(entity, &fighter, playerId, slotId);
                VS__setChargeupFlag(entity, &fighter, getDigimonData(entity->type)->moves[fighter.queuedAnim]);
                return;
            }

            if (currentCommand != BattleCommand::ATTACK && currentCommand != BattleCommand::MODERATOR)
                entity->stats.chargeMode = VS__CHARGE_MODES[playerId];
        }

        if (fighter.flags.isTransforming) return;

        if (fighter.flags.isFlattened) {
            fighter.queuedAnim           = 0;
            fighter.targetId             = targetId;
            fighter.moveRange            = 2;
            entity->flatSprite           = 0;
            fighter.flags.isTransforming = true;
            return;
        }

        if (fighter.flags.isStunned) return;
        if (fighter.flags.isConfused) {
            VS__handleConfusion(entity, &fighter, playerId);
            return;
        }
        if (fighter.flags.isOnChargeup || fighter.flags.isOnCooldown || fighter.flags.isSenile) return;

        fighter.targetId     = targetId;
        auto currentCommand  = COMBAT_DATA_PTR->player.currentCommand[playerId];
        int32_t selectedMove = -1;
        if (currentCommand == BattleCommand::MODERATOR) {
            dtl::array<uint16_t, 4> dummy;
            if (!VS__hasAffordableMoves2(dummy.data(), playerId)) {
                fighter.cooldown           = 80;
                fighter.flags.isOnChargeup = true;
                return;
            }
            selectedMove             = VS__getMoveCommandModerate(playerId, dummy.data());
            entity->stats.chargeMode = 2;
        }
        else if (currentCommand == BattleCommand::ATTACK) {
            dtl::array<uint16_t, 4> dummy;
            if (!VS__hasAffordableMoves2(dummy.data(), playerId)) {
                fighter.cooldown           = 80;
                fighter.flags.isOnChargeup = true;
                return;
            }
            selectedMove             = VS__getMoveCommandAttack(playerId, dummy.data());
            entity->stats.chargeMode = 0;
        }
        if (selectedMove == -1)
            VS__getMoveCommandYourCall(entity, &fighter, playerId);
        else
            VS__setMoveAnim(entity, &fighter, playerId, selectedMove & 0xFF);
    }

    void VS__tickBattle()
    {
        if (VS__AOE_TIMER[0] > 0) VS__AOE_TIMER[0]--;
        if (VS__AOE_TIMER[1] > 0) VS__AOE_TIMER[1]--;

        for (int32_t i = 0; i <= ENEMY_COUNT; i++) {
            auto* fighter = &COMBAT_DATA_PTR->fighter[i];
            auto* entity =
                reinterpret_cast<DigimonEntity*>(ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[i]));

            DigimonEntity* targetEntity = nullptr;
            if (fighter->targetId != 0xFF)
                targetEntity = reinterpret_cast<DigimonEntity*>(
                    ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[fighter->targetId]));

            if (fighter->flags.isAttacking)
                VS__tickDigimonAttacking(entity, targetEntity, i);
            else if (fighter->flags.isKnockedBack || fighter->flags.isBlocking)
                VS__tickDigimonHitByAttack(entity, fighter, i);
            else if (fighter->moveRange == 0xFF)
                continue;
            else if (fighter->flags.isFlattened)
                VS__tickDigimonFlat(entity, targetEntity, fighter);
            else if (fighter->flags.isStunned)
                VS__tickDigimonStun(entity);
            else if (fighter->flags.isConfused)
                VS__tickDigimonConfusion(entity, targetEntity, fighter, i);
            else if (fighter->flags.isSenile)
                VS__tickDigimonSenile(entity, fighter);
            else if (fighter->flags.isOnChargeup)
                VS__tickDigimonOnChargeup(entity, targetEntity, fighter);
            else if (fighter->flags.isOnCooldown)
                VS__tickDigimonOnCooldown(entity, targetEntity, fighter);
            else
                VS__tickDigimonOther(entity, targetEntity, fighter, i);
        }

        VS__tickGettingHit();
        VS__tickStatusEffects();
        tickSomeCameraState();
    }
}
