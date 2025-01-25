#include "Partner.hpp"

#include "Evolution.hpp"
#include "Font.hpp"
#include "Helper.hpp"
#include "ItemEffects.hpp"
#include "constants.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"

constexpr auto FRESH_EVOLUTION_TIME       = 6;
constexpr auto IN_TRAINING_EVOLUTION_TIME = 24;
constexpr auto ROOKIE_EVOLUTION_TIME      = 72;
constexpr auto CHAMPION_EVOLUTION_TIME    = 96;
constexpr auto VADEMON_EVOLUTION_TIME     = 360;

constexpr auto TAMER_RAISED_MAX = 99;
constexpr auto START_LIFESPAN   = 360;
constexpr auto START_HAPPINESS  = 50;
constexpr auto START_DISCIPLINE = 50;

constexpr auto FRESH_AWAKE_TIME       = 3;
constexpr auto IN_TRAINING_AWAKE_TIME = 7;

extern "C"
{
    void setSleepTimes(PartnerPara* para, DigimonType type)
    {
        auto level   = getDigimonData(type)->level;
        auto raise   = getRaiseData(type);
        auto pattern = SLEEP_PATTERN[raise->sleepCycle];

        if (level == Level::FRESH || level == Level::IN_TRAINING)
        {
            auto awakeTime = random(1) + (level == Level::FRESH ? FRESH_AWAKE_TIME : IN_TRAINING_AWAKE_TIME);

            para->sleepyHour         = (HOUR + awakeTime) % 24;
            para->sleepyMinute       = 0;
            para->wakeupHour         = (para->sleepyHour + pattern.sleepyDefault) % 24;
            para->wakeupMinute       = para->sleepyMinute;
            para->hoursAwakeDefault  = awakeTime;
            para->hoursAsleepDefault = pattern.sleepyDefault;
        }
        else
        {
            para->sleepyHour         = pattern.sleepyHour;
            para->sleepyMinute       = pattern.sleepyMinute;
            para->wakeupHour         = pattern.wakeupHour;
            para->wakeupMinute       = pattern.wakeupMinute;
            para->hoursAwakeDefault  = pattern.wakeupMinute;
            para->hoursAsleepDefault = pattern.sleepyDefault;
        }

        para->timeAwakeToday      = para->hoursAwakeDefault * 6;
        para->sicknessCounter     = 0;
        para->tirednessSleepTimer = 0;
    }

    void initializePartner(DigimonType type,
                           int32_t posX,
                           int32_t posY,
                           int32_t posZ,
                           int32_t rotX,
                           int32_t rotY,
                           int32_t rotZ)
    {
        loadMMD(type, EntityType::PARTNER);
        ENTITY_TABLE.partner = &PARTNER_ENTITY;
        initializeDigimonObject(type, 1, Partner_tick);
        setEntityPosition(1, posX, posY, posZ);
        setEntityRotation(1, rotX, rotY, rotZ);
        setupEntityMatrix(1);
        startAnimation(&PARTNER_ENTITY, 0);
        STOP_DISTANCE_TIMER  = 0;
        PARTNER_ENTITY.vabId = 4;
        loadPartnerSounds(type);
        PARTNER_ENTITY.isOnMap     = 1;
        PARTNER_ENTITY.isOnScreen  = 1;
        PARTNER_PARA.condition.raw = 0;
        setSleepTimes(&PARTNER_PARA, type);
        PARTNER_PARA.missedSleepHours = 0;
        PARTNER_PARA.poopLevel        = getRaiseData(type)->poopTimer;
        PARTNER_PARA.poopingTimer     = -1;
        PARTNER_PARA.virusBar         = 0;
        PARTNER_PARA.tiredness        = 0;
        PARTNER_PARA.discipline       = START_DISCIPLINE;
        PARTNER_PARA.happiness        = START_HAPPINESS;
        PARTNER_PARA.unused3          = 50;
        setFoodTimer(type);
        PARTNER_PARA.energyLevel       = getRaiseData(type)->energyThreshold;
        PARTNER_PARA.remainingLifetime = START_LIFESPAN;
        PARTNER_PARA.weight            = getRaiseData(type)->defaultWeight;
        PARTNER_PARA.trainBoostFlag    = 0;
        PARTNER_PARA.trainBoostTimer   = 0;
        PARTNER_PARA.trainBoostValue   = 0;
        PARTNER_PARA.evoTimer          = 0;
        PARTNER_PARA.battles           = 0;
        PARTNER_PARA.careMistakes      = 0;
        PARTNER_PARA.injuryTimer       = 0;
        PARTNER_PARA.sicknessTimer     = 0;
        PARTNER_PARA.areaEffectTimer   = 0;
        PARTNER_PARA.timesBeingSick    = 0;
        PARTNER_ENTITY.lives           = 3;

        if (type == DigimonType::AGUMON)
        {
            PARTNER_ENTITY.stats.moves[0] = 0x2E;
            learnMove(2);
        }
        if (type == DigimonType::GABUMON)
        {
            PARTNER_ENTITY.stats.moves[0] = 0x30;
            learnMove(0x2B);
        }

        PARTNER_ENTITY.stats.moves[3] = 0xFF;
        for (int32_t i = 0; i < 16; i++)
        {
            auto move = getDigimonData(type)->moves[i];
            if (move == 0xFF) continue;
            if (move < 58 || move > 112) continue;
            PARTNER_ENTITY.stats.moves[3] = i + 46;
        }

        addObject(ObjectID::POOP, 0, 0, renderPoop);
        HAS_IMMORTAL_HOUR = 0;
        IMMORTAL_HOUR     = 0xFF;
    }

    void setFoodTimer(DigimonType type)
    {
        auto level = getDigimonData(type)->level;

        if (level == Level::FRESH)
        {
            // every even hour
            PARTNER_PARA.nextHungerHour = ((HOUR & ~1) + 2) % 24;
        }
        else if (level == Level::IN_TRAINING)
        {
            // every 3 hours
            PARTNER_PARA.nextHungerHour = (((HOUR / 3) * 3) + 3) % 24;
        }
        else
        {
            auto raise   = getRaiseData(type);
            auto hour    = HOUR;
            auto minDiff = 24;
            for (auto time : raise->hungerTimes)
            {
                // the vanilla game does this differently, but the vanilla game also has a bug here
                if (time == -1) continue;
                auto diff = getTimeDiff(hour, time);
                if (diff < minDiff)
                {
                    minDiff                     = diff;
                    PARTNER_PARA.nextHungerHour = time;
                }
            }
        }

        PARTNER_PARA.foodLevel = getTimeDiff(HOUR, PARTNER_PARA.nextHungerHour) * 60 - MINUTE;
    }

    void renderPoop(int32_t instanceId)
    {
        if (!MAP_LAYER_ENABLED) return;

        libgte_SetRotMatrix(&libgs_REFERENCE_MATRIX);
        libgte_SetTransMatrix(&libgs_REFERENCE_MATRIX);

        for (auto& poop : WORLD_POOP)
        {
            if (poop.size == 0) continue;
            if (poop.map != CURRENT_SCREEN) continue;

            SVector rotation = POOP_ROTATION_MATRIX;
            Vector translation;
            Vector scale;

            translation.x = (poop.x - 50) * 100 + 50;
            translation.y = PARTNER_ENTITY.posData->location.y;
            translation.z = (50 - poop.y) * 100 - 50;

            scale.x = (poop.size * 4096) / 10;
            scale.y = scale.x;
            scale.z = scale.x;

            projectPosition(&POOP_POSITION, &translation, &rotation, &scale);
            renderObject(&POOP_OBJECT, ACTIVE_ORDERING_TABLE, 2);
        }
    }

    void resetPartnerPara(DigimonType type)
    {
        PARTNER_PARA.poopLevel            = getRaiseData(type)->poopTimer;
        PARTNER_PARA.unused2              = 0;
        PARTNER_PARA.unused1              = 0;
        PARTNER_PARA.poopingTimer         = -1;
        PARTNER_PARA.tiredness            = 0;
        PARTNER_PARA.subTiredness         = 0;
        PARTNER_PARA.tirednessHungerTimer = 0;
        PARTNER_PARA.timesBeingSick       = 0;
        PARTNER_PARA.areaEffectTimer      = 0;
        PARTNER_PARA.sicknessTimer        = 0;
        PARTNER_PARA.injuryTimer          = 0;
        PARTNER_PARA.sicknessTries        = 0;
        PARTNER_PARA.unused4              = 0;
        PARTNER_PARA.starvationTimer      = -1;
        PARTNER_PARA.emptyStomachTimer    = 0;
        PARTNER_PARA.weight               = getRaiseData(type)->defaultWeight;
        PARTNER_PARA.careMistakes         = 0;
        PARTNER_PARA.battles              = 0;
        setFoodTimer(type);
    }

    void initializeEvolvedPartner(DigimonType type,
                                  int32_t posX,
                                  int32_t posY,
                                  int32_t posZ,
                                  int32_t rotX,
                                  int32_t rotY,
                                  int32_t rotZ)
    {
        removeObject(static_cast<ObjectID>(type), 1);
        loadTMDandMTN(type, EntityType::PARTNER, &EVO_SEQUENCE_DATA.modelData);
        ENTITY_TABLE.partner = &PARTNER_ENTITY;
        initializeDigimonObject(type, 1, Partner_tick);
        setEntityPosition(1, posX, posY, posZ);
        setEntityRotation(1, rotX, rotY, rotZ);
        setupEntityMatrix(1);
        startAnimation(&PARTNER_ENTITY, 0);
        PARTNER_ENTITY.vabId       = 4;
        STOP_DISTANCE_TIMER        = 0;
        PARTNER_PARA.condition.raw = 0;
        setSleepTimes(&PARTNER_PARA, type);
        resetPartnerPara(type);
        HAS_IMMORTAL_HOUR = 0;
        IMMORTAL_HOUR     = 0xFF;
    }

    void setReincarnateStats(int16_t hp, int16_t mp, int16_t offense, int16_t defense, int16_t speed, int16_t brains)
    {
        int32_t tamerLevel = IS_NATURAL_DEATH ? TAMER_ENTITY.tamerLevel : 0;

        PARTNER_ENTITY.stats.hp        = hp + (DEATH_STATS.hp * tamerLevel) / 100;
        PARTNER_ENTITY.stats.mp        = mp + (DEATH_STATS.mp * tamerLevel) / 100;
        PARTNER_ENTITY.stats.off       = offense + (DEATH_STATS.off * tamerLevel) / 100;
        PARTNER_ENTITY.stats.def       = defense + (DEATH_STATS.def * tamerLevel) / 100;
        PARTNER_ENTITY.stats.speed     = speed + (DEATH_STATS.speed * tamerLevel) / 100;
        PARTNER_ENTITY.stats.brain     = brains + (DEATH_STATS.brain * tamerLevel) / 100;
        PARTNER_ENTITY.stats.currentHP = PARTNER_ENTITY.stats.hp;
        PARTNER_ENTITY.stats.currentMP = PARTNER_ENTITY.stats.mp;
    }

    void initializeReincarnatedPartner(DigimonType type,
                                       int32_t posX,
                                       int32_t posY,
                                       int32_t posZ,
                                       int32_t rotX,
                                       int32_t rotY,
                                       int32_t rotZ)
    {
        removeObject(static_cast<ObjectID>(type), 1);
        loadTMDandMTN(type, EntityType::PARTNER, &REINCARNATION_MODEL_DATA);
        ENTITY_TABLE.partner = &PARTNER_ENTITY;
        initializeDigimonObject(type, 1, Partner_tick);
        setEntityPosition(1, posX, posY, posZ);
        setEntityRotation(1, rotX, rotY, rotZ);
        setupEntityMatrix(1);
        startAnimation(&PARTNER_ENTITY, 0);
        PARTNER_ENTITY.vabId = 4;
        STOP_DISTANCE_TIMER  = 0;

        if (type == DigimonType::BOTAMON) setReincarnateStats(90, 110, 10, 11, 9, 10);
        if (type == DigimonType::PUNIMON) setReincarnateStats(120, 100, 16, 8, 8, 6);
        if (type == DigimonType::POYOMON) setReincarnateStats(100, 140, 12, 6, 6, 8);
        if (type == DigimonType::YURAMON) setReincarnateStats(100, 120, 8, 8, 8, 8);

        PARTNER_PARA.condition.raw = 0;
        setSleepTimes(&PARTNER_PARA, type);
        PARTNER_PARA.missedSleepHours    = 0;
        PARTNER_PARA.tirednessSleepTimer = 0;
        resetPartnerPara(type);
        PARTNER_PARA.discipline        = START_HAPPINESS;
        PARTNER_PARA.happiness         = START_DISCIPLINE;
        PARTNER_PARA.unused3           = 50;
        PARTNER_PARA.timesBeingSick    = 0;
        PARTNER_PARA.remainingLifetime = START_LIFESPAN;
        PARTNER_PARA.age               = 0;
        PARTNER_PARA.evoTimer          = 0;
        PARTNER_PARA.careMistakes      = 0;
        PARTNER_PARA.battles           = 0;
        PARTNER_PARA.weight            = getRaiseData(type)->defaultWeight;

        // fix evolutions not giving stats/lifetime after having used an evo item
        HAS_USED_EVOITEM = false;

        TAMER_ENTITY.raisedCount++;
        if (TAMER_ENTITY.raisedCount > TAMER_RAISED_MAX) TAMER_ENTITY.raisedCount = TAMER_RAISED_MAX;

        EVOLUTION_TARGET  = -1;
        HAS_IMMORTAL_HOUR = 0;
        IMMORTAL_HOUR     = 0xFF;
    }

    void setupPartnerOnWarp(int32_t posX, int32_t posY, int32_t posZ, int32_t rotation)
    {
        setEntityPosition(1, posX, posY, posZ);
        setEntityRotation(1, PARTNER_ENTITY.posData->rotation.x, rotation, PARTNER_ENTITY.posData->rotation.z);
        setupEntityMatrix(1);
        startAnimation(&PARTNER_ENTITY, 0);
        Partner_setState(1);
        PARTNER_PARA.areaEffectTimer = 0;
    }

    void tickConditionBoundaries()
    {
        if (PARTNER_PARA.happiness > HAPPINESS_MAX) PARTNER_PARA.happiness = HAPPINESS_MAX;
        if (PARTNER_PARA.happiness < HAPPINESS_MIN) PARTNER_PARA.happiness = HAPPINESS_MIN;

        if (PARTNER_PARA.discipline > DISCIPLINE_MAX) PARTNER_PARA.discipline = DISCIPLINE_MAX;
        if (PARTNER_PARA.discipline < DISCIPLINE_MIN) PARTNER_PARA.discipline = DISCIPLINE_MIN;

        if (PARTNER_PARA.tiredness > TIREDNESS_MAX) PARTNER_PARA.tiredness = TIREDNESS_MAX;
        if (PARTNER_PARA.tiredness < TIREDNESS_MIN) PARTNER_PARA.tiredness = TIREDNESS_MIN;

        auto energyCap = getRaiseData(PARTNER_ENTITY.type)->energyCap;
        if (PARTNER_PARA.energyLevel > energyCap) PARTNER_PARA.energyLevel = energyCap;
        if (PARTNER_PARA.energyLevel < ENERGY_MIN) PARTNER_PARA.energyLevel = ENERGY_MIN;

        if (PARTNER_PARA.weight > WEIGHT_MAX) PARTNER_PARA.weight = WEIGHT_MAX;
        if (PARTNER_PARA.weight < WEIGHT_MIN) PARTNER_PARA.weight = WEIGHT_MIN;
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
            if (PARTNER_PARA.condition.raw == 0 && PARTNER_PARA.happiness < 0 && CURRENT_FRAME % 200 == 0)
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

                    if (PARTNER_ENTITY.stats.hp < HP_MIN) PARTNER_ENTITY.stats.hp = HP_MIN;
                    if (PARTNER_ENTITY.stats.mp < MP_MIN) PARTNER_ENTITY.stats.mp = MP_MIN;
                    if (PARTNER_ENTITY.stats.off < OFF_MIN) PARTNER_ENTITY.stats.off = OFF_MIN;
                    if (PARTNER_ENTITY.stats.def < DEF_MIN) PARTNER_ENTITY.stats.def = DEF_MIN;
                    if (PARTNER_ENTITY.stats.speed < SPEED_MIN) PARTNER_ENTITY.stats.speed = SPEED_MIN;
                    if (PARTNER_ENTITY.stats.brain < BRAIN_MIN) PARTNER_ENTITY.stats.brain = BRAIN_MIN;
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
        if (PARTNER_PARA.happiness != HAPPINESS_MIN) return;
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
        if (PARTNER_PARA.tiredness < TIREDNESS_MIN) PARTNER_PARA.tiredness = TIREDNESS_MIN;

        PARTNER_PARA.weight -= getRaiseData(PARTNER_ENTITY.type)->defaultWeight / 10;
        if (PARTNER_PARA.weight < WEIGHT_MIN) PARTNER_PARA.weight = WEIGHT_MIN;
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
            if (PARTNER_PARA.tiredness > TIREDNESS_MAX) PARTNER_PARA.tiredness = TIREDNESS_MAX;
            PARTNER_PARA.subTiredness = 0;
        }

        if (PARTNER_PARA.tiredness < 50)
            PARTNER_PARA.tirednessHungerTimer = 0;
        else
            PARTNER_PARA.tirednessHungerTimer += 1;

        PARTNER_PARA.condition.isTired = PARTNER_PARA.tiredness >= 80;

        if (CURRENT_FRAME % 100 == 0 && CURRENT_FRAME != LAST_HANDLED_FRAME && PARTNER_PARA.tiredness >= 80)
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
            if (PARTNER_PARA.energyLevel < ENERGY_MIN) PARTNER_PARA.energyLevel = ENERGY_MIN;
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
                if (PARTNER_PARA.weight < WEIGHT_MIN) PARTNER_PARA.weight = WEIGHT_MIN;
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
        if (Partner_getState() != 1 && Partner_getState() != 10) return;

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
        if (PARTNER_PARA.weight < WEIGHT_MIN) PARTNER_PARA.weight = WEIGHT_MIN;
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

    void handleEatingPoop()
    {
        auto& poop         = WORLD_POOP[POOP_TO_EAT];
        auto healingChance = 0;

        if (poop.size < 11)
        {
            PARTNER_ENTITY.stats.currentHP += (PARTNER_ENTITY.stats.currentHP * 5) / 100;
            PARTNER_ENTITY.stats.currentMP += (PARTNER_ENTITY.stats.currentMP * 2) / 100;
            PARTNER_PARA.weight += 1;
            healingChance = 2;
        }
        else if (poop.size < 14)
        {
            PARTNER_ENTITY.stats.currentHP += (PARTNER_ENTITY.stats.currentHP * 10) / 100;
            PARTNER_ENTITY.stats.currentMP += (PARTNER_ENTITY.stats.currentMP * 5) / 100;
            PARTNER_PARA.weight += 3;
            healingChance = 7;
        }
        else
        {
            PARTNER_ENTITY.stats.currentHP += (PARTNER_ENTITY.stats.currentHP * 50) / 100;
            PARTNER_ENTITY.stats.currentMP += (PARTNER_ENTITY.stats.currentMP * 10) / 100;
            PARTNER_PARA.weight += 10;
            healingChance = 20;
        }

        if (PARTNER_ENTITY.stats.hp < PARTNER_ENTITY.stats.currentHP)
            PARTNER_ENTITY.stats.currentHP = PARTNER_ENTITY.stats.hp;
        if (PARTNER_ENTITY.stats.mp < PARTNER_ENTITY.stats.currentMP)
            PARTNER_ENTITY.stats.currentMP = PARTNER_ENTITY.stats.mp;
        if (PARTNER_PARA.weight > WEIGHT_MAX) PARTNER_PARA.weight = WEIGHT_MAX;

        handleMedicineHealing(healingChance, healingChance);

        poop.map  = 0xFF;
        poop.x    = -1;
        poop.y    = -1;
        poop.size = 0;
    }

    void tickSicknessMechanics()
    {
        // already guaranteed by caller, actually
        if (CURRENT_FRAME != LAST_HANDLED_FRAME) return;

        bool gotSick;

        if (!PARTNER_PARA.condition.isSick && PARTNER_PARA.sicknessCounter >= 10 && CURRENT_FRAME % 1200 == 0)
        {
            PARTNER_PARA.sicknessTries += 1;
            auto roll = random(100);
            if (roll < PARTNER_PARA.tiredness - 50 + (PARTNER_PARA.sicknessTries - 10) * 5)
            {
                PARTNER_PARA.condition.isSick = true;
                PARTNER_PARA.timesBeingSick += 1;
                PARTNER_PARA.sicknessTimer = 1;
                PARTNER_PARA.happiness -= 20;
                PARTNER_PARA.sicknessCounter = 0;

                gotSick = true;
            }
        }

        if (!PARTNER_PARA.condition.isSick && PARTNER_PARA.areaEffectTimer >= 12000 && CURRENT_SCREEN % 1200 == 0 &&
            PARTNER_AREA_RESPONSE == 2)
        {
            PARTNER_PARA.condition.isSick = true;
            PARTNER_PARA.timesBeingSick += 1;
            PARTNER_PARA.sicknessTimer   = 1;
            PARTNER_PARA.sicknessCounter = 0;

            gotSick = true;
        }

        if (CURRENT_FRAME % 1200 == 0 && PARTNER_PARA.condition.isSick)
        {
            PARTNER_PARA.happiness -= 10;
            PARTNER_PARA.discipline -= 5;
            PARTNER_PARA.tiredness += 5;
        }

        if (!PARTNER_PARA.condition.isSick && PARTNER_PARA.sicknessTimer > 0) PARTNER_PARA.sicknessTimer = 0;

        if (CURRENT_FRAME % 1200 == 0)
        {
            if (PARTNER_PARA.condition.isSick) PARTNER_PARA.sicknessTimer += 1;
            if (PARTNER_PARA.condition.isInjured) PARTNER_PARA.injuryTimer += 1;
        }

        if (!HAS_IMMORTAL_HOUR || IMMORTAL_HOUR != HOUR)
        {
            if (Tamer_getState() == 0 && PARTNER_PARA.injuryTimer >= 12)
            {
                PARTNER_PARA.timesBeingSick += 1;
                PARTNER_PARA.sicknessTimer       = 1;
                PARTNER_PARA.condition.isSick    = true;
                PARTNER_PARA.condition.isInjured = false;
                PARTNER_PARA.injuryTimer         = 0;

                gotSick = true;
            }

            if (gotSick)
            {
                Tamer_setState(20);
                clearTextArea();
                setTextColor(10);
                auto width = drawStringNew(&vanillaFont, PARTNER_ENTITY.name, 704, 256 + 0x78);
                setTextColor(1);
                drawStringNew(&vanillaFont, reinterpret_cast<const uint8_t*>(" is sick!"), 704 + width, 256 + 0x78);
            }

            if (PARTNER_PARA.sicknessCounter >= 12 && Partner_getState() != 8 && Tamer_getState() == 0 &&
                PARTNER_PARA.remainingLifetime != 0 && IS_SCRIPT_PAUSED == 1)
            {
                writePStat(255, 0);
                PARTNER_ENTITY.lives -= 1;
                callScriptSection(0, 1246, 0);
                PARTNER_PARA.sicknessTimer = 0;
            }
        }
    }

    void tickDeathCondition()
    {
        if (HAS_IMMORTAL_HOUR != 1 && IMMORTAL_HOUR == HOUR) return;
        if (PARTNER_PARA.remainingLifetime > 0) return;
        if (Partner_getState() == 8) return;
        if (Tamer_getState() != 0) return;
        if (IS_SCRIPT_PAUSED != 1) return;

        IS_NATURAL_DEATH = 1;
        writePStat(0xFF, 0);
        PARTNER_PARA.remainingLifetime = 0;
        callScriptSection(0, 1246, 0);
    }

    void skipHours(uint32_t amount)
    {
        PARTNER_PARA.evoTimer += amount;
        PARTNER_PARA.remainingLifetime -= amount;
        PARTNER_PARA.age += (HOUR + amount) / 24;

        updateTimeOfDay();

        if ((HOUR + amount % 4) == 0 && PARTNER_PARA.happiness < 80)
        {
            PARTNER_PARA.remainingLifetime -= getHappinessLifetimePenalty(PARTNER_PARA.happiness);
        }

        if (PARTNER_PARA.remainingLifetime < 0) PARTNER_PARA.remainingLifetime = 0;

        if (PARTNER_PARA.condition.isSleepy)
        {
            PARTNER_PARA.sicknessCounter += amount;
            PARTNER_PARA.missedSleepHours += amount;
        }
        if (!PARTNER_PARA.condition.isHungry) { PARTNER_PARA.foodLevel -= amount * 60; }
        else
        {
            PARTNER_PARA.starvationTimer -= amount * 120;
            // vanilla adds a care mistake here, but this causes the doubling of them
        }

        // vanilla doesn't multiply with hours
        PARTNER_PARA.energyLevel -= getRaiseData(PARTNER_ENTITY.type)->energyUsage * amount;
        if (PARTNER_PARA.energyLevel < ENERGY_MIN) PARTNER_PARA.energyLevel = ENERGY_MIN;

        if (!PARTNER_PARA.condition.isPoopy)
            PARTNER_PARA.poopLevel -= amount * 6;
        else
            PARTNER_PARA.poopingTimer -= amount * 1200; // vanilla doesn't multiply with hours

        if (PARTNER_PARA.condition.isSick) PARTNER_PARA.sicknessTimer += amount;

        if (PARTNER_PARA.sicknessCounter >= 10) // vanilla does this only when sick
            PARTNER_PARA.sicknessTries += amount;

        if (PARTNER_PARA.condition.isInjured) PARTNER_PARA.injuryTimer += amount;
    }

    void handlePostBattleTiredness()
    {
        auto maxMP     = PARTNER_ENTITY.stats.mp;
        auto currentMP = PARTNER_ENTITY.stats.currentMP;

        // in vanilla the math is broken, so the MP factor can only ever be 0 or 10
        PARTNER_PARA.tiredness += 5 + ((maxMP - currentMP) * 10) / maxMP;
        // TODO shouldn't this be 20? Or is this additional
        PARTNER_PARA.foodLevel -= 15;
    }

    void tickPartner()
    {
        if (GAME_STATE != 0) return;
        if (Partner_getState() != 1) return;
        if (IS_GAMETIME_RUNNING == 0) return;
        if (CURRENT_FRAME == LAST_HANDLED_FRAME) return;
        if (FADE_DATA.fadeProtection == 1) return;

        tickDeathCondition();
        tickSleepMechanics();
        tickSicknessMechanics();
        tickTirednessMechanics();
        tickHungerMechanics();
        tickUnhappinessMechanics();
        tickConditionBoundaries();

        PARTNER_PARA.areaEffectTimer += 1;
        if (PARTNER_PARA.areaEffectTimer > 28800) PARTNER_PARA.areaEffectTimer = 0;

        PARTNER_PARA.trainBoostTimer -= 1;
        if (PARTNER_PARA.trainBoostTimer < 1)
        {
            PARTNER_PARA.trainBoostFlag  = 0;
            PARTNER_PARA.trainBoostTimer = 0;
            PARTNER_PARA.trainBoostValue = 10; // reset to 10 -> 100%, not done in vanilla
        }

        if (CURRENT_FRAME % 4800 == 0)
        {
            PARTNER_PARA.remainingLifetime -= getHappinessLifetimePenalty(PARTNER_PARA.happiness);
            if (PARTNER_PARA.remainingLifetime < 0) PARTNER_PARA.remainingLifetime = 0;
        }

        if (!isInDaytimeTransition()) return;
        if (HAS_IMMORTAL_HOUR)
        {
            if (IMMORTAL_HOUR == HOUR) return;

            HAS_IMMORTAL_HOUR = 0;
            IMMORTAL_HOUR     = -1;
        }

        if (Tamer_getState() == 0 && Partner_getState() == 1)
        {
            auto type     = PARTNER_ENTITY.type;
            auto level    = getDigimonData(type)->level;
            auto evoTimer = PARTNER_PARA.evoTimer;
            if (level == Level::FRESH && evoTimer >= FRESH_EVOLUTION_TIME)
                EVOLUTION_TARGET = static_cast<int16_t>(getFreshEvolutionTarget(type));
            if (level == Level::IN_TRAINING && evoTimer >= IN_TRAINING_EVOLUTION_TIME)
                EVOLUTION_TARGET = static_cast<int16_t>(getInTrainingEvolutionTarget(type));
            if (level == Level::ROOKIE && evoTimer >= ROOKIE_EVOLUTION_TIME)
                EVOLUTION_TARGET = static_cast<int16_t>(getRegularEvolutionTarget(type));
            if (level == Level::CHAMPION && evoTimer >= CHAMPION_EVOLUTION_TIME)
                EVOLUTION_TARGET = static_cast<int16_t>(getRegularEvolutionTarget(type));

            // forced Vademon, shouldn't need to call the function
            if (level == Level::CHAMPION && evoTimer == VADEMON_EVOLUTION_TIME)
                EVOLUTION_TARGET = static_cast<int16_t>(handleSpecialEvolutionPraise(3, &PARTNER_ENTITY));
        }

        if (PARTNER_PARA.virusBar >= VIRUS_MAX && PARTNER_ENTITY.type != DigimonType::SUKAMON)
        {
            EVOLUTION_TARGET = static_cast<int16_t>(DigimonType::SUKAMON);
            writePStat(5, static_cast<uint8_t>(PARTNER_ENTITY.type));
            PARTNER_PARA.sukaBackupHP    = PARTNER_ENTITY.stats.hp;
            PARTNER_PARA.sukaBackupMP    = PARTNER_ENTITY.stats.mp;
            PARTNER_PARA.sukaBackupOff   = PARTNER_ENTITY.stats.off;
            PARTNER_PARA.sukaBackupDef   = PARTNER_ENTITY.stats.def;
            PARTNER_PARA.sukaBackupSpeed = PARTNER_ENTITY.stats.speed;
            PARTNER_PARA.sukaBackupBrain = PARTNER_ENTITY.stats.brain;
            PARTNER_PARA.virusBar        = VIRUS_MIN;
        }

        if (EVOLUTION_TARGET != -1 && Partner_getState() != 13)
        {
            Tamer_setState(6);
            Partner_setState(13);
        }
    }
}
