#include "Partner.hpp"

#include "Helper.hpp"
#include "ItemEffects.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"

extern "C"
{
    void tickConditionBoundaries()
    {
        if (PARTNER_PARA.happiness > 100) PARTNER_PARA.happiness = 100;
        if (PARTNER_PARA.happiness < -100) PARTNER_PARA.happiness = -100;

        if (PARTNER_PARA.discipline > 100) PARTNER_PARA.discipline = 100;
        if (PARTNER_PARA.discipline < 0) PARTNER_PARA.discipline = 0;

        if (PARTNER_PARA.tiredness > 100) PARTNER_PARA.tiredness = 100;
        if (PARTNER_PARA.tiredness < 0) PARTNER_PARA.tiredness = 0;

        auto energyCap = getRaiseData(PARTNER_ENTITY.type)->energyCap;
        if (PARTNER_PARA.energyLevel > energyCap) PARTNER_PARA.energyLevel = energyCap;
        if (PARTNER_PARA.energyLevel < 0) PARTNER_PARA.energyLevel = 0;

        if (PARTNER_PARA.weight > 99) PARTNER_PARA.weight = 99;
        if (PARTNER_PARA.weight < 1) PARTNER_PARA.weight = 1;
    }

    inline void updateSleepingTimes()
    {
        auto level = getDigimonData(PARTNER_ENTITY.type)->level;
        if (level < Level::ROOKIE)
        {
            PARTNER_PARA.sleepyHour   = (PARTNER_PARA.wakeupHour + PARTNER_PARA.hoursAwakeDefault) % 24;
            PARTNER_PARA.sleepyMinute = 0;
            PARTNER_PARA.wakeupHour   = (PARTNER_PARA.sleepyHour + PARTNER_PARA.hoursAsleepDefault) % 24;
            PARTNER_PARA.wakeupMinute = 0;
        }
        else
        {
            auto pattern              = SLEEP_PATTERN[getRaiseData(PARTNER_ENTITY.type)->sleepCycle];
            PARTNER_PARA.sleepyHour   = pattern.sleepyHour;
            PARTNER_PARA.sleepyMinute = pattern.sleepyMinute;
            PARTNER_PARA.wakeupHour   = pattern.wakeupHour;
            PARTNER_PARA.wakeupMinute = pattern.wakeupMinute;
        }
    }

    void tickSleepMechanics()
    {
        bool isSleepy = PARTNER_PARA.condition.isSleepy;
        auto level    = getDigimonData(PARTNER_ENTITY.type)->level;
        if (CURRENT_FRAME != LAST_HANDLED_FRAME)
        {
            // increase tiredness sleep timer
            if (CURRENT_FRAME % 20 == 0) PARTNER_PARA.tirednessSleepTimer += PARTNER_PARA.tiredness * 3 / 10;

            // handle tiredness sleep timer overflow
            if (!isSleepy && PARTNER_PARA.tirednessSleepTimer >= 180 && CURRENT_FRAME % 200 == 0)
            {
                PARTNER_PARA.timeAwakeToday -= 1;
                PARTNER_PARA.tirednessSleepTimer -= 180;

                PARTNER_PARA.sleepyHour   = PARTNER_PARA.wakeupHour + PARTNER_PARA.timeAwakeToday / 6;
                PARTNER_PARA.sleepyMinute = PARTNER_PARA.wakeupMinute + (PARTNER_PARA.timeAwakeToday % 6) * 10;
                while (PARTNER_PARA.sleepyMinute > 60)
                {
                    PARTNER_PARA.sleepyMinute -= 60;
                    PARTNER_PARA.sleepyHour += 1;
                }
                PARTNER_PARA.sleepyHour %= 24;
            }

            // handle sleepy status effects
            if (isSleepy)
            {
                if (level == Level::FRESH)
                {
                    if (CURRENT_FRAME % 200 == 0)
                    {
                        PARTNER_PARA.happiness -= 1;
                        PARTNER_PARA.discipline -= 1;
                    }
                }
                else if (level == Level::IN_TRAINING)
                {
                    if (CURRENT_FRAME % 300 == 0)
                    {
                        PARTNER_PARA.happiness -= 1;
                        PARTNER_PARA.discipline -= 1;
                    }
                }
                else if (CURRENT_FRAME % 1200 == 0)
                {
                    PARTNER_PARA.happiness -= 2;
                    PARTNER_PARA.discipline -= 4;
                }

                if (CURRENT_FRAME % 1200 == 0 && PARTNER_PARA.sleepyHour != HOUR)
                {
                    PARTNER_PARA.sicknessCounter++;
                    PARTNER_PARA.missedSleepHours++;
                }
            }
        }

        auto wakeupTime    = PARTNER_PARA.wakeupHour * 60 + PARTNER_PARA.wakeupMinute;
        auto sleepyTime    = PARTNER_PARA.sleepyHour * 60 + PARTNER_PARA.sleepyMinute;
        auto currentTime   = HOUR * 60 + MINUTE;
        auto shouldBeAwake = isWithinTimeframe(currentTime, wakeupTime, sleepyTime);
        // handle sleepy care mistake
        if (isSleepy && shouldBeAwake)
        {
            updateSleepingTimes();
            PARTNER_PARA.timeAwakeToday      = PARTNER_PARA.hoursAwakeDefault * 6;
            PARTNER_PARA.tirednessSleepTimer = 0;
            PARTNER_PARA.condition.isSleepy  = false;
            PARTNER_PARA.careMistakes++;
            updateConditionAnimation();
            addTamerLevel(1, -1);
        }

        // set sleepy
        if (!isSleepy && !shouldBeAwake) { PARTNER_PARA.condition.isSleepy = true; }

        setSleepDisabled(!PARTNER_PARA.condition.isSleepy);
    }

    void tickUnhappinessMechanics()
    {
        if (CURRENT_FRAME != LAST_HANDLED_FRAME)
        {
            // roll butterfly chance
            if (PARTNER_PARA.condition.full == 0 && PARTNER_PARA.happiness < 0 && CURRENT_FRAME % 200 == 0)
            {
                auto rand = random(100);
                if (rand < (-PARTNER_PARA.happiness - PARTNER_PARA.discipline)) PARTNER_PARA.condition.isUnhappy = true;
            }

            // perform butterfly effects
            if (PARTNER_PARA.condition.isUnhappy)
            {
                if (CURRENT_FRAME % 15 == 0) PARTNER_PARA.happiness += 1;

                if (CURRENT_FRAME % 300 == 0)
                {
                    PARTNER_ENTITY.stats.hp -= (PARTNER_ENTITY.stats.hp / 50);
                    PARTNER_ENTITY.stats.mp -= (PARTNER_ENTITY.stats.mp / 50);
                    PARTNER_ENTITY.stats.off -= (PARTNER_ENTITY.stats.off / 50);
                    PARTNER_ENTITY.stats.def -= (PARTNER_ENTITY.stats.def / 50);
                    PARTNER_ENTITY.stats.speed -= (PARTNER_ENTITY.stats.speed / 50);
                    PARTNER_ENTITY.stats.brain -= (PARTNER_ENTITY.stats.brain / 50);

                    if (PARTNER_ENTITY.stats.hp < 1) PARTNER_ENTITY.stats.hp = 1;
                    if (PARTNER_ENTITY.stats.mp < 1) PARTNER_ENTITY.stats.mp = 1;
                    if (PARTNER_ENTITY.stats.off < 1) PARTNER_ENTITY.stats.off = 1;
                    if (PARTNER_ENTITY.stats.def < 1) PARTNER_ENTITY.stats.def = 1;
                    if (PARTNER_ENTITY.stats.speed < 1) PARTNER_ENTITY.stats.speed = 1;
                    if (PARTNER_ENTITY.stats.brain < 1) PARTNER_ENTITY.stats.brain = 1;
                    if (PARTNER_ENTITY.stats.currentHP > PARTNER_ENTITY.stats.hp)
                        PARTNER_ENTITY.stats.currentHP = PARTNER_ENTITY.stats.hp;
                    if (PARTNER_ENTITY.stats.currentMP > PARTNER_ENTITY.stats.mp)
                        PARTNER_ENTITY.stats.currentMP = PARTNER_ENTITY.stats.mp;
                }
            }
        }

        // check if condition should be removed
        if (PARTNER_PARA.condition.isUnhappy && PARTNER_PARA.happiness >= 0)
        {
            PARTNER_PARA.condition.isUnhappy = false;
            PARTNER_ENTITY.loopCount         = 1;
            unsetButterfly(BUTTERFLY_ID);
            HAS_BUTTERFLY = -1;
        }
    }

    inline void handlePraise()
    {
        PARTNER_PARA.happiness += 2 + PARTNER_PARA.discipline / 10;
        PARTNER_PARA.discipline -= 5;
    }

    inline void handleScold()
    {
        auto deltaDisc = 0;
        if (ITEM_SCOLD_FLAG == 1)
        {
            PARTNER_PARA.discipline += 8;
            ITEM_SCOLD_FLAG = 2;
        }
        else
        {
            PARTNER_PARA.happiness -= 10;
            PARTNER_PARA.discipline += 2;
        }

        PARTNER_PARA.refusedFavFood      = 0; // TODO unused mechanic
        PARTNER_PARA.condition.isUnhappy = false;
        NANIMON_TRIGGER                  = 0;
        if (HAS_BUTTERFLY == 0)
        {
            unsetButterfly(BUTTERFLY_ID);
            HAS_BUTTERFLY = -1;
        }

        if (getDigimonData(PARTNER_ENTITY.type)->level != Level::ROOKIE) return;
        if (PARTNER_PARA.happiness != -100) return;
        if (PARTNER_PARA.discipline == 0) return;

        NANIMON_TRIGGER = 1;
    }

    void handlePraiseScold(int32_t action)
    {
        if (action == 4)
            handleScold();
        else
            handlePraise();
    }

    void handleSleeping()
    {
        auto type       = PARTNER_ENTITY.type;
        auto hoursSlept = getTimeDiff(HOUR, PARTNER_PARA.wakeupHour);

        advanceToTime(PARTNER_PARA.wakeupHour, PARTNER_PARA.wakeupMinute);

        PARTNER_PARA.evoTimer += hoursSlept;
        PARTNER_PARA.remainingLifetime -= hoursSlept;
        if (PARTNER_PARA.remainingLifetime < 0) PARTNER_PARA.remainingLifetime = 0;
        updateSleepingTimes();

        setFoodTimer(type);
        PARTNER_PARA.starvationTimer     = 0;
        PARTNER_PARA.timeAwakeToday      = PARTNER_PARA.hoursAwakeDefault * 6;
        PARTNER_PARA.tirednessSleepTimer = 0;
        PARTNER_PARA.sicknessCounter     = 0;
        PARTNER_PARA.condition.isSleepy  = false;
        HAS_IMMORTAL_HOUR                = 1;
        IMMORTAL_HOUR                    = HOUR;
        setSleepDisabled(1);

        if (PARTNER_PARA.condition.isPoopy)
        {
            int16_t tileX;
            int16_t tileY;
            getModelTile(&PARTNER_ENTITY.posData->location, &tileX, &tileY);
            createPoopPile(tileX, tileY);
            PARTNER_PARA.poopingTimer = -1;
            // fixes lack of care mistakes and doubles the poop level
            handleWildPoop();
            addTamerLevel(1, -1);
        }

        if (PARTNER_PARA.condition.isHungry)
        {
            PARTNER_PARA.weight -= 1;
            // TODO care mistakes? reset condition?
        }

        if (PARTNER_PARA.condition.isSick)
        {
            PARTNER_PARA.sicknessTimer += hoursSlept;
            PARTNER_PARA.happiness -= 20;
            PARTNER_PARA.discipline -= 10;
            PARTNER_PARA.tiredness += 10;
        }

        if (PARTNER_PARA.condition.isInjured)
        {
            PARTNER_PARA.injuryTimer += hoursSlept;
            PARTNER_PARA.happiness -= 10;
            PARTNER_PARA.discipline -= 5;
            PARTNER_PARA.tiredness += 5;
        }
    }

    inline bool hasSpotPoop(int16_t tileX, int16_t tileY, uint8_t map)
    {
        for (auto& poop : WORLD_POOP)
        {
            if (poop.x == tileX && poop.y == tileY && poop.map == map) return true;
        }

        return false;
    }

    inline uint32_t countPoopsOnMap(uint8_t map)
    {
        uint32_t count = 0;
        for (auto& poop : WORLD_POOP)
            if (poop.map == map) count++;

        return count;
    }

    inline PoopPile* getPoopSlotToFill()
    {
        auto countOnMap = countPoopsOnMap(CURRENT_SCREEN);
        if (countOnMap >= 16)
        {
            for (int32_t i = 0; i < 100; i++)
            {
                auto& poop = WORLD_POOP[(CURRENT_POOP_ID + i) % 100];
                if (poop.map == CURRENT_SCREEN) return &poop;
            }
        }

        for (auto& poop : WORLD_POOP)
            if (poop.size == 0) return &poop;

        auto* val       = &WORLD_POOP[CURRENT_POOP_ID];
        CURRENT_POOP_ID = (CURRENT_POOP_ID + 1) % 100;
        return val;
    }

    void createPoopPile(int16_t tileX, int16_t tileY)
    {
        auto rotation = PARTNER_ENTITY.posData->rotation.y;
        if (rotation <= 0x300 || rotation >= 0xD00) tileY -= 1;
        if (rotation <= 0xB00 && rotation >= 0x500) tileY += 1;
        if (rotation <= 0xF00 && rotation >= 0x900) tileX -= 1;
        if (rotation < 0x700 && rotation >= 0x100) tileX += 1;

        while (hasSpotPoop(tileX, tileY, CURRENT_SCREEN))
        {
            switch (random(4))
            {
                case 0: tileX -= 1; break;
                case 1: tileX += 1; break;
                case 2: tileY -= 1; break;
                case 3: tileY += 1; break;
            }
        }

        auto* poop = getPoopSlotToFill();
        poop->map  = CURRENT_SCREEN;
        poop->x    = tileX;
        poop->y    = tileY;
        poop->size = getRaiseData(PARTNER_ENTITY.type)->poopSize;

        if (PARTNER_ENTITY.type != DigimonType::SUKAMON) PARTNER_PARA.virusBar++;

        SVector pos = {
            .x = static_cast<int16_t>((tileX - 50) * 100 + 50),
            .y = static_cast<int16_t>(PARTNER_ENTITY.posData->location.y),
            .z = static_cast<int16_t>((50 - tileY) * 100 - 50),
        };
        createPoopFX(&pos);
    }

    void sleepRegen()
    {
        auto hoursSlept = getTimeDiff(HOUR, PARTNER_PARA.wakeupHour);
        if (MINUTE > 0 && HOUR != PARTNER_PARA.sleepyHour) hoursSlept++;

        uint32_t sleepFactor = (hoursSlept * 100) / PARTNER_PARA.hoursAsleepDefault;
        if (getItemCount(ItemType::REST_PILLOW) > 0) sleepFactor = (sleepFactor * 12) / 10;
        if (PARTNER_AREA_RESPONSE == 1) sleepFactor = (sleepFactor * 12) / 10;
        if (PARTNER_AREA_RESPONSE == 2) sleepFactor = (sleepFactor * 8) / 10;

        auto randomHealFactor = random(10) + 70;
        auto healHP           = (PARTNER_ENTITY.stats.hp * randomHealFactor * sleepFactor) / 10000;
        auto healMP           = (PARTNER_ENTITY.stats.mp * randomHealFactor * sleepFactor) / 10000;
        PARTNER_ENTITY.stats.currentHP += healHP;
        PARTNER_ENTITY.stats.currentMP += healMP;
        if (PARTNER_ENTITY.stats.currentHP > PARTNER_ENTITY.stats.hp)
            PARTNER_ENTITY.stats.currentHP = PARTNER_ENTITY.stats.hp;
        if (PARTNER_ENTITY.stats.currentMP > PARTNER_ENTITY.stats.mp)
            PARTNER_ENTITY.stats.currentMP = PARTNER_ENTITY.stats.mp;

        auto randomTirednessFactor = random(20) + 80;
        auto reduceTiredness       = (sleepFactor * PARTNER_PARA.tiredness * randomTirednessFactor) / 10000;
        PARTNER_PARA.tiredness -= reduceTiredness;
        if (PARTNER_PARA.tiredness < 0) PARTNER_PARA.tiredness = 0;

        PARTNER_PARA.weight -= getRaiseData(PARTNER_ENTITY.type)->defaultWeight / 10;
        if (PARTNER_PARA.weight < 1) PARTNER_PARA.weight = 1;
    }

    void tickTirednessMechanics()
    {
        if (PARTNER_PARA.areaEffectTimer % 1200 == 0 && PARTNER_PARA.areaEffectTimer > 0)
        {
            if (PARTNER_AREA_RESPONSE == 1)
            {
                PARTNER_PARA.happiness += 1;
                PARTNER_PARA.tiredness -= 2;
            }
            if (PARTNER_AREA_RESPONSE == 2)
            {
                PARTNER_PARA.happiness -= 1;
                PARTNER_PARA.tiredness += 1;
            }
        }

        // never used?
        if (PARTNER_PARA.subTiredness >= 60)
        {
            PARTNER_PARA.tiredness += 1;
            if (PARTNER_PARA.tiredness > 100) PARTNER_PARA.tiredness = 100;
            PARTNER_PARA.subTiredness = 0;
        }

        if (PARTNER_PARA.tiredness < 50)
            PARTNER_PARA.tirednessHungerTimer = 0;
        else
            PARTNER_PARA.tirednessHungerTimer += 1;

        PARTNER_PARA.condition.isTired = PARTNER_PARA.tiredness >= 80;

        if (CURRENT_FRAME % 100 && CURRENT_FRAME != LAST_HANDLED_FRAME && PARTNER_PARA.tiredness >= 80)
            PARTNER_PARA.happiness -= 2;
    }

    uint32_t partnerWillRefuseItem()
    {
        auto type     = PARTNER_ENTITY.type;
        auto itemType = TAMER_ITEM.type;

        // key items
        if (itemType >= ItemType::BLUE_FLUTE && itemType <= ItemType::GEAR) return true;
        if (itemType == ItemType::FRIDGE_KEY || itemType == ItemType::AS_DECODER) return true;
        // battle only items
        if (itemType >= ItemType::OFFENSE_DISK && itemType <= ItemType::SUPER_SPEED_DISK) return true;
        // tamer items
        if (itemType >= ItemType::TRAINING_MANUAL && itemType <= ItemType::HEALTH_SHOE) return true;
        // invalid item
        if (itemType > ItemType::METAL_BANANA) return true;

        // evolution items
        auto level = getDigimonData(type)->level;
        if (itemType == ItemType::METAL_BANANA && level != Level::CHAMPION) return true;
        if (itemType == ItemType::GIGA_HAND && level != Level::CHAMPION) return true;
        if (itemType == ItemType::NOBLE_MANE && level != Level::ROOKIE) return true;
        if (itemType >= ItemType::GREY_CLAWS && itemType <= ItemType::MOON_MIRROR)
        {
            auto target =
                EVOLUTION_ITEM_TARGET[static_cast<uint32_t>(itemType) - static_cast<uint32_t>(ItemType::GREY_CLAWS)];
            auto targetLevel = getDigimonData(target)->level;
            auto levelDiff   = static_cast<int32_t>(targetLevel) - static_cast<int32_t>(level);
            if (levelDiff != 1) return true;
        }

        // take any item after successful scold
        if (ITEM_SCOLD_FLAG == 2)
        {
            ITEM_SCOLD_FLAG = 0;
            return false;
        }

        if (itemType < ItemType::MEAT)
        {
            auto roll1 = random(100);
            auto roll2 = random(10);
            if (roll1 < (110 - PARTNER_PARA.discipline - roll2 - 10))
            {
                ITEM_SCOLD_FLAG = 1;
                return true;
            }
        }

        // 20% chance of refusing favorite food when not hungry
        // vanilla would grant a justified scold if the Digimon were hungry, but as it can't refuse when hungry this
        // could never happen
        if (!PARTNER_PARA.condition.isHungry && itemType == getRaiseData(type)->favoriteFood && random(10) < 2)
            return true;

        return false;
    }

    void tickHungerMechanics()
    {
        auto type   = PARTNER_ENTITY.type;
        auto* raise = getRaiseData(type);

        // timers
        if (!PARTNER_PARA.condition.isHungry && CURRENT_FRAME % 20 == 0) PARTNER_PARA.foodLevel -= 1;
        if (PARTNER_PARA.condition.isHungry && CURRENT_FRAME % 10 == 0) PARTNER_PARA.starvationTimer -= 1;

        // reduce energy level per hour
        if (CURRENT_FRAME % 1200 == 0)
        {
            PARTNER_PARA.energyLevel -= raise->energyUsage;
            if (PARTNER_PARA.energyLevel < 0) PARTNER_PARA.energyLevel = 0;
        }

        // check if hungry condition should be set
        if (!PARTNER_PARA.condition.isHungry && PARTNER_PARA.foodLevel < 1)
        {
            PARTNER_PARA.starvationTimer    = 180; // 90 ingame minutes
            PARTNER_PARA.condition.isHungry = 1;
            // handleConditionBubble(); // TODO does this even belong here?
        }

        // check if hungry condition timed out
        if (PARTNER_PARA.condition.isHungry && PARTNER_PARA.starvationTimer < 1)
        {
            setFoodTimer(PARTNER_ENTITY.type);
            PARTNER_PARA.starvationTimer    = 0;
            PARTNER_PARA.condition.isHungry = false;
            if (PARTNER_PARA.energyLevel < raise->energyThreshold) PARTNER_PARA.careMistakes++;
        }

        // perform empty stomach weight reduction
        if (PARTNER_PARA.energyLevel < 1)
        {
            if (CURRENT_FRAME % 200 == 0)
            {
                PARTNER_PARA.weight -= 1;
                if (PARTNER_PARA.weight < 1) PARTNER_PARA.weight = 1;
                if (CURRENT_SCREEN % 20) PARTNER_PARA.emptyStomachTimer += 1; // unused
            }
        }

        // do tiredness hunger reduction
        if (CURRENT_SCREEN % 1200 == 0 && PARTNER_PARA.tirednessHungerTimer >= 50) PARTNER_PARA.foodLevel -= 3;

        // set trigger 640 if energy level reaches threshold (used for Restaurant?)
        if (PARTNER_PARA.energyLevel >= raise->energyThreshold) setTrigger(640);
    }

    uint8_t conditionBubbleType   = -1;
    uint8_t conditionBubbleId     = -1;
    uint16_t conditionBubbleTimer = 0;

    void handleConditionBubble()
    {
        // TODO rework, this sucks to begin with since only two bubbles are visible at most
        if (PARTNER_STATE != 1 && PARTNER_STATE != 10) return;

        int32_t bubbleType = -1;
        if (PARTNER_PARA.condition.isSick && conditionBubbleType != 2) bubbleType = 2;
        if (PARTNER_PARA.condition.isInjured && conditionBubbleType != 6) bubbleType = 6;
        if (PARTNER_PARA.condition.isPoopy && conditionBubbleType != 1) bubbleType = 1;
        if (PARTNER_PARA.condition.isHungry && conditionBubbleType != 0) bubbleType = 0;
        if (PARTNER_PARA.condition.isTired && conditionBubbleType != 4) bubbleType = 4;
        if (PARTNER_PARA.condition.isSleepy && conditionBubbleType != 3) bubbleType = 3;

        // having this here saves like 150 bytes...
        if (++conditionBubbleTimer >= 60) conditionBubbleType = -1;

        if (bubbleType != -1)
        {
            if (HAS_BUTTERFLY == 0)
            {
                unsetButterfly(BUTTERFLY_ID);
                HAS_BUTTERFLY                    = -1;
                PARTNER_PARA.condition.isUnhappy = false;
                PARTNER_ENTITY.loopCount         = 1;
            }
            if (bubbleType != conditionBubbleType && conditionBubbleTimer >= 50)
            {
                unsetBubble(conditionBubbleId);
                conditionBubbleId    = addConditionBubble(bubbleType, &PARTNER_ENTITY);
                conditionBubbleTimer = 0;
                conditionBubbleType  = bubbleType;
            }
        }
        else if (PARTNER_PARA.condition.isUnhappy && HAS_BUTTERFLY == -1)
        {
            unsetBubble(conditionBubbleId);
            BUTTERFLY_ID  = setButterfly(&PARTNER_ENTITY);
            HAS_BUTTERFLY = 0;
        }
    }

    void partnerHandleFoodFeed(ItemType item)
    {
        if (!isFoodItem(item)) return;

        auto* raise = getRaiseData(PARTNER_ENTITY.type);

        if (!PARTNER_PARA.condition.isHungry)
        {
            PARTNER_PARA.happiness -= 3;
            PARTNER_PARA.discipline -= 2;
        }
        else
        {
            if (PARTNER_PARA.energyLevel >= raise->energyThreshold)
            {
                PARTNER_PARA.happiness += 5;
                PARTNER_PARA.discipline += 1;
                PARTNER_PARA.condition.isHungry = false;
                setFoodTimer(PARTNER_ENTITY.type);
                PARTNER_PARA.starvationTimer = 0;
            }

            if (PARTNER_PARA.energyLevel >= raise->energyThreshold || raise->favoriteFood == item)
            {
                PARTNER_ANIMATION = 11;
                startAnimation(&PARTNER_ENTITY, 11);
            }
        }
    }

    void tickPartnerPoopingMechanic()
    {
        // TODO: this stops the timer during transitions
        if (Tamer_getState() != 0) return;

        if (!PARTNER_PARA.condition.isPoopy)
        {
            if (CURRENT_FRAME % 200 == 0 && CURRENT_FRAME != LAST_HANDLED_FRAME) PARTNER_PARA.poopLevel -= 1;

            if (PARTNER_PARA.poopLevel < 1)
            {
                PARTNER_PARA.condition.isPoopy = true;
                PARTNER_PARA.poopingTimer      = ((PARTNER_PARA.discipline + 20) * 12);
            }
        }
        else
        {
            if (CURRENT_FRAME != LAST_HANDLED_FRAME) PARTNER_PARA.poopingTimer -= 1;

            if (PARTNER_PARA.poopingTimer < 1)
            {
                Partner_setState(7);
                PARTNER_PARA.poopingTimer = -1;
                ITEM_SCOLD_FLAG           = 1;
            }
        }
    }

    void detectEdiblePoop()
    {
        if (PARTNER_ENTITY.type != DigimonType::NUMEMON && PARTNER_ENTITY.type != DigimonType::SUKAMON) return;

        for (int32_t i = 0; i < 100; i++)
        {
            auto& poop = WORLD_POOP[i];
            if (poop.size == 0) continue;

            auto radius  = poop.size < 11 ? 200 : 300;
            auto centerX = tileToPos(poop.x);
            auto centerY = tileToPos(poop.y);

            if (isWithinRect(centerX, centerY, radius, &PARTNER_ENTITY.posData->location))
            {
                Partner_setState(9);
                POOP_TO_EAT = i;
                return;
            }
        }
    }

    void handlePoopWeightLoss(DigimonType type)
    {
        PARTNER_PARA.weight -= (getRaiseData(type)->poopSize + random(4)) / 4;
        if (PARTNER_PARA.weight < 1) PARTNER_PARA.weight = 1;
    }

    void handleToilet()
    {
        PARTNER_PARA.happiness += 2;
        PARTNER_PARA.discipline += 2;
        PARTNER_PARA.poopLevel         = getRaiseData(PARTNER_ENTITY.type)->poopTimer * 2;
        PARTNER_PARA.condition.isPoopy = false;
        handlePoopWeightLoss(PARTNER_ENTITY.type);
    }

    void handleWildPoop()
    {
        PARTNER_PARA.condition.isPoopy = false;
        PARTNER_PARA.careMistakes += 1;
        PARTNER_PARA.happiness -= 10;
        PARTNER_PARA.discipline -= 5;
        PARTNER_PARA.poopLevel = getRaiseData(PARTNER_ENTITY.type)->poopTimer * 2;
        handlePoopWeightLoss(PARTNER_ENTITY.type);
    }

    void handleEatingPoop() {
        auto& poop = WORLD_POOP[POOP_TO_EAT];
        auto healingChance = 0;

        if(poop.size < 11) {
            PARTNER_ENTITY.stats.currentHP += (PARTNER_ENTITY.stats.currentHP * 5) / 100;
            PARTNER_ENTITY.stats.currentMP += (PARTNER_ENTITY.stats.currentMP * 2) / 100;
            PARTNER_PARA.weight += 1;
            healingChance = 2;
        }
        else if(poop.size < 14) {
            PARTNER_ENTITY.stats.currentHP += (PARTNER_ENTITY.stats.currentHP * 10) / 100;
            PARTNER_ENTITY.stats.currentMP += (PARTNER_ENTITY.stats.currentMP * 5) / 100;
            PARTNER_PARA.weight += 3;
            healingChance = 7;
        }
        else {
            PARTNER_ENTITY.stats.currentHP += (PARTNER_ENTITY.stats.currentHP * 50) / 100;
            PARTNER_ENTITY.stats.currentMP += (PARTNER_ENTITY.stats.currentMP * 10) / 100;
            PARTNER_PARA.weight += 10;
            healingChance = 20;
        }

        if(PARTNER_ENTITY.stats.hp < PARTNER_ENTITY.stats.currentHP)
            PARTNER_ENTITY.stats.currentHP = PARTNER_ENTITY.stats.hp;
        if(PARTNER_ENTITY.stats.mp < PARTNER_ENTITY.stats.currentMP)
            PARTNER_ENTITY.stats.currentMP = PARTNER_ENTITY.stats.mp;
        if(PARTNER_PARA.weight > 99)
            PARTNER_PARA.weight = 99;

        handleMedicineHealing(healingChance, healingChance);

        poop.map = 0xFF;
        poop.x = -1;
        poop.y = -1;
        poop.size = 0;
    }
}
