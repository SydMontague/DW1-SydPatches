#include "../Battle.h"
#include "../Entity.hpp"
#include "../Math.hpp"
#include "../extern/VS.hpp"
#include "../extern/dtl/types.hpp"

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
            fighter.hpDamageBuffer > entity->stats.currentHP) {
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

            if (currentCommand == BattleCommand::ATTACK1 || currentCommand == BattleCommand::ATTACK2 ||
                currentCommand == BattleCommand::ATTACK3 &&
                    VS__canAffordMove(entity, &fighter, static_cast<int32_t>(currentCommand) - 8)) {
                fighter.targetId = targetId;
                if (entity->stats.moves[static_cast<int32_t>(currentCommand) - 8] != fighter.queuedAnim ||
                    fighter.moveRange < 1)
                    VS__setMoveAnim(entity, &fighter, playerId, static_cast<int32_t>(currentCommand) - 8);
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
}
