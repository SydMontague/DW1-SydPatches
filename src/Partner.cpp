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

    void tickSleepMechanics()
    {
        bool isSleepy = PARTNER_PARA.condition.isSleepy;
        auto level    = getDigimonPara(PARTNER_ENTITY.type)->level;
        if (CURRENT_FRAME != LAST_HANDLED_FRAME)
        {
            if (CURRENT_FRAME % 20 == 0) PARTNER_PARA.tirednessSleepTimer += PARTNER_PARA.tiredness * 3 / 10;

            if (!isSleepy && PARTNER_PARA.tirednessSleepTimer >= 180 && CURRENT_FRAME % 200 == 0)
            {
                PARTNER_PARA.timeAwakeToday -= 1;
                PARTNER_PARA.tirednessSleepTimer = 0;

                PARTNER_PARA.sleepyHour   = PARTNER_PARA.wakeupHour + PARTNER_PARA.timeAwakeToday / 6;
                PARTNER_PARA.sleepyMinute = PARTNER_PARA.wakeupMinute + (PARTNER_PARA.timeAwakeToday % 6) * 10;
                while (PARTNER_PARA.sleepyMinute > 60)
                {
                    PARTNER_PARA.sleepyMinute -= 60;
                    PARTNER_PARA.sleepyHour += 1;
                }
                PARTNER_PARA.sleepyHour %= 24;
            }

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
        if (isSleepy && shouldBeAwake)
        {
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
            PARTNER_PARA.timeAwakeToday      = PARTNER_PARA.hoursAwakeDefault * 6;
            PARTNER_PARA.tirednessSleepTimer = 0;
            PARTNER_PARA.condition.isSleepy  = false;
            PARTNER_PARA.careMistakes++;
            updateConditionAnimation();
            addTamerLevel(1, -1);
        }

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
}