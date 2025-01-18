#include "Evolution.hpp"

#include "extern/dw1.hpp"
#include "Helper.hpp"

extern "C"
{
    DigimonType getFreshEvolutionTarget(DigimonType type)
    {
        // FIX: removed evo timer reset, as it should be unnecessary
        switch (type)
        {
            case DigimonType::BOTAMON: return DigimonType::KOROMON;
            case DigimonType::PUNIMON: return DigimonType::TSUNOMON;
            case DigimonType::POYOMON: return DigimonType::TOKOMON;
            case DigimonType::YURAMON: return DigimonType::TANEMON;
            default: return DigimonType::TAMER;
        }
    }

    bool highestStatFulfilled(EvoRequirements& reqs)
    {
        int32_t highestStatValue = 0;
        bool isRequired          = false;

        if (PARTNER_ENTITY.stats.hp / 10 > highestStatValue)
        {
            highestStatValue = PARTNER_ENTITY.stats.hp / 10;
            isRequired       = reqs.hp == 1;
        }
        if (PARTNER_ENTITY.stats.mp / 10 > highestStatValue)
        {
            highestStatValue = PARTNER_ENTITY.stats.mp / 10;
            isRequired       = reqs.mp == 1;
        }
        if (PARTNER_ENTITY.stats.off > highestStatValue)
        {
            highestStatValue = PARTNER_ENTITY.stats.off;
            isRequired       = reqs.offense == 1;
        }
        if (PARTNER_ENTITY.stats.def > highestStatValue)
        {
            highestStatValue = PARTNER_ENTITY.stats.def;
            isRequired       = reqs.defense == 1;
        }
        if (PARTNER_ENTITY.stats.speed > highestStatValue)
        {
            highestStatValue = PARTNER_ENTITY.stats.speed;
            isRequired       = reqs.speed == 1;
        }
        if (PARTNER_ENTITY.stats.brain > highestStatValue)
        {
            highestStatValue = PARTNER_ENTITY.stats.brain;
            isRequired       = reqs.brain == 1;
        }

        return isRequired;
    }

    bool allStatsFulfilled(EvoRequirements& reqs)
    {
        if (PARTNER_ENTITY.stats.hp / 10 < reqs.hp) return false;
        if (PARTNER_ENTITY.stats.mp / 10 < reqs.mp) return false;
        if (PARTNER_ENTITY.stats.off < reqs.offense) return false;
        if (PARTNER_ENTITY.stats.def < reqs.defense) return false;
        if (PARTNER_ENTITY.stats.speed < reqs.speed) return false;
        if (PARTNER_ENTITY.stats.brain < reqs.brain) return false;

        return true;
    }

    bool isBonusFulfilled(EvoRequirements& reqs)
    {
        auto type         = PARTNER_ENTITY.type;
        bool isMaxBattles = (reqs.flags & 1) != 0;

        // bonus Digimon
        if (reqs.digimon != -1 && static_cast<int16_t>(type) == reqs.digimon) return true;
        // discipline
        if (reqs.discipline != -1 && PARTNER_PARA.discipline >= reqs.discipline) return true;
        // happiness
        if (reqs.happiness != -1 && PARTNER_PARA.happiness >= reqs.happiness) return true;
        // battles
        if (reqs.battles != -1)
        {
            if (isMaxBattles)
            {
                if (reqs.battles >= PARTNER_PARA.battles) return true;
            }
            else if (reqs.battles <= PARTNER_PARA.battles)
                return true;
        }
        // techs
        if (reqs.techs != -1 && getNumMasteredMoves() >= reqs.techs) return true;

        return false;
    }

    uint32_t calculateRequirementScore(DigimonType current, DigimonType target, DigimonType currentBest)
    {
        auto& data        = EVO_REQ_DATA[static_cast<uint32_t>(target)];
        int32_t reqPoints = 0;

        bool isMaxCM = (data.flags & 0x10) != 0;

        // care mistakes
        if (isMaxCM)
        {
            if (data.care >= PARTNER_PARA.careMistakes) reqPoints++;
        }
        else if (data.care <= PARTNER_PARA.careMistakes)
            reqPoints++;

        // weight
        auto ownWeight    = PARTNER_PARA.weight;
        auto targetWeight = data.weight;
        if (targetWeight - 5 <= ownWeight && ownWeight <= targetWeight + 5) reqPoints++;

        // stats
        if (getDigimonPara(target)->level == Level::ROOKIE)
        {
            if (highestStatFulfilled(data)) reqPoints++;
        }
        else if (allStatsFulfilled(data))
            reqPoints++;

        // bonus
        if (isBonusFulfilled(data)) reqPoints++;

        // prioritize Digimon you didn't already have
        if (reqPoints >= 3 && currentBest != DigimonType::INVALID)
        {
            // FIX: don't use ID from statsgains data, since we already have it...
            bool isTargetRaised      = hasDigimonRaised(target);
            bool isCurrentBestRaised = hasDigimonRaised(currentBest);
            if (isTargetRaised && !isCurrentBestRaised) reqPoints = 0;
            if (!isTargetRaised && isCurrentBestRaised) reqPoints++;
        }

        return reqPoints;
    }

    DigimonType getInTrainingEvolutionTarget(DigimonType current)
    {
        DigimonType best  = DigimonType::INVALID;
        int32_t bestScore = 0;

        for (auto target : EVO_PATHS_DATA[static_cast<uint32_t>(current)].to)
        {
            if (target == 0xFF) continue;
            auto score = calculateRequirementScore(current, static_cast<DigimonType>(target), best);

            if (score < 3) continue;
            if (score < bestScore) continue;

            best      = static_cast<DigimonType>(target);
            bestScore = score;
        }

        return best;
    }

    DigimonType getRegularEvolutionTarget(DigimonType current)
    {
        DigimonType best  = DigimonType::INVALID;
        int32_t bestScore = 0;

        for (auto target : EVO_PATHS_DATA[static_cast<uint32_t>(current)].to)
        {
            if (target == 0xFF) continue;
            auto score = calculateRequirementScore(current, static_cast<DigimonType>(target), best);
            if (score < 3) continue;

            int32_t statsSum   = 0;
            int32_t statsCount = 0;
            auto& data         = EVO_REQ_DATA[static_cast<uint32_t>(target)];
            if (data.hp != -1)
            {
                statsSum += PARTNER_ENTITY.stats.hp / 10;
                statsCount++;
            }
            if (data.mp != -1)
            {
                statsSum += PARTNER_ENTITY.stats.mp / 10;
                statsCount++;
            }
            if (data.offense != -1)
            {
                statsSum += PARTNER_ENTITY.stats.off;
                statsCount++;
            }
            if (data.defense != -1)
            {
                statsSum += PARTNER_ENTITY.stats.def;
                statsCount++;
            }
            if (data.speed != -1)
            {
                statsSum += PARTNER_ENTITY.stats.speed;
                statsCount++;
            }
            if (data.brain != -1)
            {
                statsSum += PARTNER_ENTITY.stats.brain;
                statsCount++;
            }

            auto finalScore = statsCount != 0 ? statsSum / statsCount : 0;
            if (bestScore < finalScore)
            {
                best      = static_cast<DigimonType>(target);
                bestScore = finalScore;
            }
        }

        if (getDigimonPara(current)->level == Level::ROOKIE && best == DigimonType::INVALID &&
            PARTNER_PARA.evoTimer >= 96)
            return DigimonType::NUMEMON;

        return best;
    }

    DigimonType handleSpecialEvolutionPraise(int32_t mode, PartnerEntity* entity)
    {
        if (Tamer_getState() != 0) return DigimonType::INVALID;

        DigimonType target = DigimonType::INVALID;
        DigimonType type   = entity->type;
        auto level         = getDigimonPara(type)->level;
        auto evotimer      = PARTNER_PARA.evoTimer;
        auto rand          = random(100);

        if ((type == DigimonType::WHAMON || type == DigimonType::SHELLMON) && evotimer == 200 && rand < 30)
            target = DigimonType::COELAMON;

        if (NANIMON_TRIGGER == 1)
        {
            target          = DigimonType::NANIMON;
            NANIMON_TRIGGER = 0;
        }
        if (level == Level::CHAMPION && evotimer >= 240 && rand < 50) target = DigimonType::VADEMON;

        if (target != DigimonType::INVALID)
        {
            EVOLUTION_TARGET = static_cast<int16_t>(target);
            Tamer_setState(6);
            Partner_setState(13);
        }
        return target;
    }

    DigimonType handleSpecialEvolutionSleep(int32_t mode, PartnerEntity* entity)
    {
        auto rand          = random(100);
        DigimonType target = DigimonType::INVALID;
        DigimonType type   = entity->type;
        auto discipline    = PARTNER_PARA.discipline;
        auto happiness     = PARTNER_PARA.happiness;
        auto tiredness     = PARTNER_PARA.tiredness;
        auto battles       = PARTNER_PARA.battles;
        auto level         = getDigimonPara(type)->level;
        auto def           = entity->stats.def;

        if (rand < 30)
        {
            if ((type == DigimonType::BIRDRAMON || type == DigimonType::SEADRAMON) && discipline == 100 &&
                happiness == 100 && tiredness == 0)
            {
                target = DigimonType::AIRDRAMON;
            }

            if (type == DigimonType::VEGIEMON && battles >= 50 && discipline == 100) target = DigimonType::NINJAMON;

            if (type == DigimonType::DRIMOGEMON && def >= 500 && discipline == 100) target = DigimonType::MONOCHROMON;
        }
        if (rand < 50 && level == Level::IN_TRAINING && CURRENT_SCREEN == 1) { target = DigimonType::KUNEMON; }

        if (target != DigimonType::INVALID)
        {
            EVOLUTION_TARGET = static_cast<int16_t>(target);
            Tamer_setState(6);
            Partner_setState(13);
        }

        return target;
    }

    int32_t getNumMasteredMoves() {
      int32_t count;

      for(auto val : PARTNER_ENTITY.learnedMoves) {
        for(uint32_t i = 0; i < 32; i++) {
          count += (val >> i) & 1;
        }
      }

      return count;
    }
}