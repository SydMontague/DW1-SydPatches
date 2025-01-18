#include "Partner.hpp"

#include "Helper.hpp"
#include "extern/dw1.hpp"

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

        PARTNER_PARA.refusedFavFood      = 0;
        PARTNER_PARA.condition.isUnhappy = false;
        NANIMON_TRIGGER                  = 1;
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

    extern uint8_t getItemCount(ItemType item);
    extern uint8_t PARTNER_AREA_RESPONSE;

    void sleepRegen() {
        auto hoursSlept = getTimeDiff(HOUR, PARTNER_PARA.wakeupHour);
        if(MINUTE > 0 && HOUR != PARTNER_PARA.sleepyHour)
            hoursSlept++;

        uint32_t sleepFactor = (hoursSlept * 100) / PARTNER_PARA.hoursAsleepDefault;
        if(getItemCount(ItemType::REST_PILLOW) > 0)
            sleepFactor = (sleepFactor * 12) / 10;
        if(PARTNER_AREA_RESPONSE == 1)
            sleepFactor = (sleepFactor * 12) / 10;
        if(PARTNER_AREA_RESPONSE == 2)
            sleepFactor = (sleepFactor * 8) / 10;

        auto randomHealFactor = random(10) + 70;
        auto healHP = (PARTNER_ENTITY.stats.hp * randomHealFactor * sleepFactor) / 10000;
        auto healMP = (PARTNER_ENTITY.stats.mp * randomHealFactor * sleepFactor) / 10000;
        PARTNER_ENTITY.stats.currentHP += healHP;
        PARTNER_ENTITY.stats.currentMP += healMP;
        if(PARTNER_ENTITY.stats.currentHP > PARTNER_ENTITY.stats.hp)
            PARTNER_ENTITY.stats.currentHP = PARTNER_ENTITY.stats.hp;
        if(PARTNER_ENTITY.stats.currentMP > PARTNER_ENTITY.stats.mp)
            PARTNER_ENTITY.stats.currentMP = PARTNER_ENTITY.stats.mp;

        auto randomTirednessFactor = random(20) + 80;
        auto reduceTiredness = (sleepFactor * PARTNER_PARA.tiredness * randomTirednessFactor) / 10000;
        PARTNER_PARA.tiredness -= reduceTiredness;
        if(PARTNER_PARA.tiredness < 0)
            PARTNER_PARA.tiredness = 0;

        PARTNER_PARA.weight -= getRaiseData(PARTNER_ENTITY.type)->defaultWeight / 10;
        if(PARTNER_PARA.weight < 1)
            PARTNER_PARA.weight = 1;

    }
}
