#pragma once

#include "libc.hpp"
#include "libgpu.hpp"
#include "libgs.hpp"
#include "stddef.hpp"

extern "C"
{
    struct GlyphData
    {
        uint16_t pixelData[11];
        uint8_t width;
        uint8_t padding;
    };

    struct Position
    {
        int32_t x;
        int32_t y;
        int32_t z;
        int32_t pad;
    };

    enum class Type : uint8_t
    {
        UNDEFINED = 0,
        DATA      = 1,
        VACCINE   = 2,
        VIRUS     = 3,
    };

    enum class Level : uint8_t
    {
        UNDEFINED   = 0,
        FRESH       = 1,
        IN_TRAINING = 2,
        ROOKIE      = 3,
        CHAMPION    = 4,
        ULTIMATE    = 5,
    };

    enum class Special : uint8_t
    {
        FIRE    = 0,
        COMBAT  = 1,
        AIR     = 2,
        NATURE  = 3,
        ICE     = 4,
        MACHINE = 5,
        FILTH   = 6,
        NONE    = 0xFF,
    };

    enum class ItemType : uint8_t
    {
        SMALL_RECOVERY,
        MEDIUM_RECOVERY,
        LARGE_RECOVERY,
        SUPER_RECOVERY,
        MP_FLOPPY,
        MEDIUM_MP,
        LARGE_MP,
        DOUBLE_FLOPPY,
        VARIOUS,
        OMNIPOTENT,
        PROTECTION,
        RESTORE,
        SUPER_RESTORE,
        BANDAGE,
        MEDICINE,
        OFFENSE_DISK,
        DEFENSE_DISK,
        SPEED_DISK,
        OMNI_DISK,
        SUPER_OFFENSE_DISK,
        SUPER_DEFENSE_DISK,
        SUPER_SPEED_DISK,
        AUTOPILOT,
        OFFENSE_CHIP,
        DEFENSE_CHIP,
        BRAIN_SHIP,
        SPEED_CHIP,
        HP_CHIP,
        MP_CHIP,
        DV_CHIP_A,
        DV_CHIP_D,
        DV_CHIP_E,
        PORTA_POTTY,
        TRAINING_MANUAL,
        REST_PILLOW,
        ENEMY_REPEL,
        ENEMY_BELL,
        HEALTH_SHOE,
        MEAT,
        GIANT_MEAT,
        SIRLOIN,
        SUPERCARROT,
        HAWK_RADISH,
        SPINY_GREEN,
        DIGIMUSHROOM,
        ICE_MUSHROOM,
        DELUXE_MUSHROOM,
        DIGIPINE,
        BLUE_APPLE,
        RED_BERRY,
        GOLD_ACORN,
        BIG_BERRY,
        SWEET_NUT,
        SUPER_VEGGY,
        PRICKLYPEAR,
        ORANGE_BANANA,
        POWER_FRUIT,
        POWER_ICE,
        SPEED_LEAF,
        SAGE_FRUIT,
        MUSCLE_YAM,
        CALM_BERRY,
        DIGIANCHOVY,
        DIGISNAPPER,
        DIGITROUT,
        BLACK_TROUT,
        DIGICATFISH,
        DIGISEABASS,
        MOLDY_MEAT,
        HAPPYMUSHROOM,
        CHAIN_MELON,
        GREY_CLAWS,
        FIREBALL,
        FLAMEWING,
        IRON_HOOF,
        MONO_STONE,
        STEEL_DRILL,
        WHITE_FANG,
        BLACK_WING,
        SPIKE_CLUB,
        FLAMEINGMANE,
        WHITE_WING,
        TORN_TATTER,
        ELECTRO_RING,
        RAINBOWHORN,
        ROOSTER,
        UNIHORN,
        HORN_HELMET,
        SCISSOR_JAW,
        FERTILIZER,
        KOGA_LAWS,
        WATERBOTTLE,
        NORTH_STAR,
        RED_SHELL,
        HARD_SCALE,
        BLUECRYSTAL,
        ICE_CRYSTAL,
        HAIR_GROWER,
        SUNGLASSES,
        METAL_PART,
        FATAL_BONE,
        CYBER_PART,
        MEGA_HAND,
        SILVER_BALL,
        METAL_ARMOR,
        CHAINSAW,
        SMALL_SPEAR,
        X_BANDAGE,
        RAY_GUN,
        GOLD_BANANA,
        MYSTY_EGG,
        RED_RUBY,
        BETTLEPEARL,
        CORAL_CHARM,
        MOON_MIRROR,
        BLUE_FLUTE,
        OLD_ROD,
        AMAZING_ROD,
        LEOMONSTONE,
        MANSION_KEY,
        GEAR,
        RAIN_PLANT,
        STEAK,
        FRIDGE_KEY,
        AS_DECODER,
        GIGA_HAND,
        NOBLE_MANE,
        METAL_BANANA,

        NONE = 0xFF,
    };

    enum class ObjectID
    {
        ENTITY_TEXT          = 402,
        THROWN_ITEM          = 404,
        DROPPED_ITEM         = 405,
        DEATH_COUNTDOWN      = 407,
        COMMAND_BAR          = 408,
        FINISHER_BAR         = 410,
        VS_HPMP_BAR          = 412,
        VS_WIN_LOSS_WINDOW   = 413,
        PLAYER_MARKER        = 419,
        UI_BOX               = 420,
        BATTLE_TEXT          = 422,
        ARENA_MESH           = 423,
        VS_CAMERA            = 424,
        VS_INTRO_CAMERA_REEL = 426,
        VS_INTRO_NAME        = 427,
        VS_TIMER             = 428,
        MONOCHROMON_BUBBLE   = 433,
        VS_PLAYER_INPUT      = 434,
        FILE_READ_QUEUE      = 1028,
        KNOCKBACK_FX         = 1536,
        POOP_FX              = 1537,
        CONDITION_BUBBLE     = 2050,
        POISON_BUBBLE        = 2053,
        CONFUSION_EFFECT     = 2054,
        POISON_EFFECT        = 2056,
        FINISHER_AURA        = 2061,
        BUTTERFLY            = 2062,
        STUN_EFFECT          = 2063,
        STUN_SUB_FX          = 2064,
        BTL_ITEM_PARTICLES   = 2072,
        MAP                  = 4000,
        MAP_NAME             = 4001,
        GAME_CLOCK           = 4002,
        MAIN_MENU_SCREEN     = 4003,
        FADE                 = 4005,
        FEEDING_ITEM         = 4007,
        POOP                 = 4008,
        DOORS                = 4009,
        TRN_HP               = 4011,
        TRN_OFFENSE          = 4012,
        TRN_SPEED            = 4013,
        TRN_DEFENSE          = 4014,
        TRN_MP               = 4015,
        TRN_BRAINS           = 4016,
        CAMERA_MOVEMENT      = 4017,
        CHEST                = 4021,
        BOULDER              = 4022,
        MERAMON_SHAKE        = 4024,
        PLAYTIME             = 4025,
        DAYTIME_TRANSITION   = 4030,
        MAIN_MENU            = 5000,
        FISHING              = 12289,

        // Custom Objects
        DEBUG_OVERLAY = 20000,
    };

    struct DigimonData
    {
        uint8_t name[20];
        int32_t boneCount;
        int16_t radius;
        int16_t height;
        Type type;
        Level level;
        Special special[3];
        uint8_t dropItem;
        uint8_t dropChance;
        uint8_t moves[16];
        uint8_t padding;
    };

    struct RaiseData
    {
        int8_t hungerTimes[8];
        int8_t energyCap;
        int8_t energyThreshold;
        int8_t energyUsage;
        int16_t poopTimer;
        int16_t unk2;
        int8_t poopSize;
        ItemType favoriteFood;
        int8_t sleepCycle;
        int8_t favoredRegion;
        int8_t trainingType;
        int8_t defaultWeight;
        int16_t viewX;
        int16_t viewY;
        int16_t viewZ;
    };

    struct WorldItem
    {
        SVector spriteLocation;
        ItemType type;
    };

    struct TamerItem : public WorldItem
    {
        int32_t time;
    };

    struct DroppedItem : public WorldItem
    {
        int16_t tileX;
        int16_t tileY;
    };

    union Condition
    {
        struct
        {
            bool isSleepy  : 1;
            bool isTired   : 1;
            bool isHungry  : 1;
            bool isPoopy   : 1;
            bool isUnhappy : 1;
            bool isInjured : 1;
            bool isSick    : 1;
            uint32_t pad   : 1;
        };
        uint32_t raw;
    };

    static_assert(sizeof(Condition) == 4);

    struct PartnerPara
    {
        Condition condition;
        int16_t sleepyHour;
        int16_t sleepyMinute;
        int16_t wakeupHour;
        int16_t wakeupMinute;
        int16_t hoursAwakeDefault;
        int16_t hoursAsleepDefault;
        int16_t timeAwakeToday;
        int16_t sicknessCounter;
        int16_t missedSleepHours;
        int16_t tirednessSleepTimer;
        int16_t poopLevel;
        int16_t unused1;
        int16_t unused2;
        int16_t virusBar;
        int16_t poopingTimer;
        int16_t tiredness;
        int16_t subTiredness;
        int16_t tirednessHungerTimer;
        int16_t discipline;
        int16_t happiness;
        int16_t unused3;
        int16_t unused4;
        int16_t timesBeingSick;
        int16_t sicknessTries;
        int16_t areaEffectTimer;
        int16_t sicknessTimer;
        int16_t injuryTimer;
        int16_t nextHungerHour;
        int16_t energyLevel;
        int16_t foodLevel;
        int16_t starvationTimer;
        int16_t weight;
        int16_t refusedFavFood;
        int16_t emptyStomachTimer;
        int16_t remainingLifetime;
        int16_t age;
        uint16_t trainBoostFlag;
        uint16_t trainBoostValue;
        uint16_t trainBoostTimer;
        int16_t careMistakes;
        int16_t battles;
        int16_t evoTimer;
        int16_t unused5;
        int16_t unused6;
        int16_t unused7;
        int16_t fishCaught;
        int16_t unused8;
        int16_t unused9;
        int16_t unused10;
        int16_t upgradeHPcounter;
        int16_t upgradeMPcounter;
        int16_t upgradeOffCounter;
        int16_t buggedUpgradeBrainsCounter;
        int16_t upgradeDefenseCounter;
        int16_t upgradeSpeedCounter;
        int16_t upgradeBrainCounter;
        int16_t unused11;
        int16_t sukaBackupHP;
        int16_t sukaBackupMP;
        int16_t sukaBackupOff;
        int16_t sukaBackupDef;
        int16_t sukaBackupSpeed;
        int16_t sukaBackupBrain;
        int16_t unused12;
    };

    struct RGB8
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

    enum class DigimonType : int32_t
    {
        TAMER               = 0,
        BOTAMON             = 1,
        KOROMON             = 2,
        AGUMON              = 3,
        BETAMON             = 4,
        GREYMON             = 5,
        DEVIMON             = 6,
        AIRDRAMON           = 7,
        TYRANNOMON          = 8,
        MERAMON             = 9,
        SEADRAMON           = 10,
        NUMEMON             = 11,
        METALGREYMON        = 12,
        MAMEMON             = 13,
        MONZAEMON           = 14,
        PUNIMON             = 15,
        TSUNOMON            = 16,
        GABUMON             = 17,
        ELECMON             = 18,
        KABUTERIMON         = 19,
        ANGEMON             = 20,
        BIRDRAMON           = 21,
        GARURUMON           = 22,
        FRIGIMON            = 23,
        WHAMON              = 24,
        VEGIEMON            = 25,
        SKULLGREYMON        = 26,
        METALMAMEMON        = 27,
        VADEMON             = 28,
        POYOMON             = 29,
        TOKOMON             = 30,
        PATAMON             = 31,
        KUNEMON             = 32,
        UNIMON              = 33,
        OGREMON             = 34,
        SHELLMON            = 35,
        CENTARUMON          = 36,
        BAKEMON             = 37,
        DRIMOGEMON          = 38,
        SUKAMON             = 39,
        ANDROMON            = 40,
        GIROMON             = 41,
        ETEMON              = 42,
        YURAMON             = 43,
        TANEMON             = 44,
        BIYOMON             = 45,
        PALMON              = 46,
        MONOCHROMON         = 47,
        LEOMON              = 48,
        COELAMON            = 49,
        KOKATORIMON         = 50,
        KUWAGAMON           = 51,
        MOJYAMON            = 52,
        NANIMON             = 53,
        MEGADRAMON          = 54,
        PIXIMON             = 55,
        DIGITAMAMON         = 56,
        PENGUINMON          = 57,
        NINJAMON            = 58,
        PHOENIXMON          = 59,
        HERCULESKABUTERIMON = 60,
        MEGASEADRAMON       = 61,
        WEREGARURUMON       = 62,
        PANJYAMON           = 63,
        GIGADRAMON          = 64,
        METALETEMON         = 65,
        MYOTISMON           = 66,
        YANMAMON            = 67,
        GOTSUMON            = 68,
        FLAREIZAMON         = 69,
        WARUMONZAEMON       = 70,
        SNOWAGUMON          = 71,
        HYOGAMON            = 72,
        PLATINUMNUMEMON     = 73,
        DOKUNEMON           = 74,
        SHIMAUNIMON         = 75,
        TANKMON             = 76,
        REDVEGIMON          = 77,
        JMOJYAMON           = 78,
        NISEDRIMOGEMON      = 79,
        GOBURIMON           = 80,
        MUDFRIGIMON         = 81,
        PSYCHEMON           = 82,
        MODOKIBETAMON       = 83,
        TOYAGUMON           = 84,
        PIDDOMON            = 85,
        ARURAUMON           = 86,
        GEREMON             = 87,
        VERMILIMON          = 88,
        FUGAMON             = 89,
        TEKKAMON            = 90,
        MORISHELLMON        = 91,
        GUARDROMON          = 92,
        MUCHOMON            = 93,
        ICEMON              = 94,
        AKATORIMON          = 95,
        TSUKAIMON           = 96,
        SHARMAMON           = 97,
        CLEARAGUMON         = 98,
        WEEDMON             = 99,
        ICEDEVIMON          = 100,
        DARKRIZAMON         = 101,
        SANDYANMAMON        = 102,
        SNOWGOBURIMON       = 103,
        BLUEMERAMON         = 104,
        GURURUMON           = 105,
        SABERDRAMON         = 106,
        SOULDMON            = 107,
        ROCKMON             = 108,
        OTAMAMON            = 109,
        GEKOMON             = 110,
        TENTOMON            = 111,
        WARUSEADRAMON       = 112,
        METEORMON           = 113,
        UNKNOWN             = 114,
        MACHINEDRAMON       = 115,
        ANALOGMAN           = 116,
        JIJIMON             = 117,
        MARKET_MANAGER      = 118,
        SHOGUNGEKOMON       = 119,
        KING_SUKAMON        = 120,
        CHERRYMON           = 121,
        HAGURUMON           = 122,
        TINMON              = 123,
        MASTER_TYRANNOMON   = 124,
        NPC_GOBIROMON       = 125,
        BRACHIOMON          = 126,
        DEMIMERAMON         = 127,
        NPC_BETAMON         = 128,
        NPC_GREYMON         = 129,
        NPC_DEVIMON         = 130,
        NPC_AIRDRAMON       = 131,
        NPC_TYRANNOMON      = 132,
        NPC_MERAMON         = 133,
        NPC_SEADRAMON       = 134,
        NPC_NUMEMON         = 135,
        NPC_METALGREYMON    = 136,
        NPC_MAMEMON         = 137,
        NPC_MONZAEMON       = 138,
        NPC_GABUMON         = 139,
        NPC_ELECMON         = 140,
        NPC_kABUTERIMON     = 141,
        NPC_ANGEMON         = 142,
        NPC_BIDRAMON        = 143,
        NPC_GARURUMON       = 144,
        NPC_FRIGIMON        = 145,
        NPC_WHAMON          = 146,
        NPC_VEGIMON         = 147,
        NPC_SKULLGREYMON    = 148,
        NPC_METALMAMEMON    = 149,
        NPC_VADEMON         = 150,
        NPC_PATAMON         = 151,
        NPC_KUNEMON         = 152,
        NPC_UNIMON          = 153,
        NPC_OGREMON         = 154,
        NPC_SHELLMON        = 155,
        NPC_CENTARUMON      = 156,
        NPC_BAKEMON         = 157,
        NPC_DRIMOGEMON      = 158,
        NPC_SUKAMON         = 159,
        NPC_ANDROMON        = 160,
        NPC_GIROMON         = 161,
        NPC_ETEMON          = 162,
        NPC_BIYOMON         = 163,
        NPC_PALMON          = 164,
        NPC_MONOCHROMON     = 165,
        NPC_LEOMON          = 166,
        NPC_COELAMON        = 167,
        NPC_KOKATORIMON     = 168,
        NPC_KUWAGAMON       = 169,
        NPC_MOJYAMON        = 170,
        NPC_NANIMON         = 171,
        NPC_MEGADRAMON      = 172,
        NPC_PIXIMON         = 173,
        NPC_DIGITAMAMON     = 174,
        NPC_NINJAMON        = 175,
        NPC_PENGUINMON      = 176,
        NPC_MYOTISMON       = 177,
        NPC2_GREYMON        = 178,
        NPC2_METALGREYMON   = 179,

        INVALID = -1,
    };

    struct PositionData
    {
        GsDOBJ2 obj;
        GsCOORDINATE2 posMatrix;
        Vector scale;
        SVector rotation;
        Vector location;
    };

    struct MomentumData
    {
        int16_t scale1[9];
        int16_t subDelta[9];
        int16_t delta[9];
        int16_t subScale[9];
        int8_t subValue[9];
        uint8_t field5_0x51;
    };

    struct Entity
    {
        DigimonType type;
        PositionData* posData;
        int32_t* animPtr;
        MomentumData* momentum;
        int32_t locX;
        int32_t locY;
        int32_t locZ;
        int16_t animFrame;
        int16_t frameCount;
        int16_t* animInstrPtr;
        void* loopStart;
        int16_t loopEndFrame;
        int16_t textureX;
        int16_t textureY;
        uint8_t animId;
        uint8_t loopCount;
        uint8_t animFlag;
        uint8_t field17_0x31;
        int16_t field18_0x32;
        uint8_t isOnMap;
        uint8_t isOnScreen;
        uint8_t flatSprite;
        uint8_t flatTimer;
    };
    struct Stats
    {
        int16_t off;
        int16_t def;
        int16_t speed;
        int16_t brain;
        uint8_t movesPrio[4];
        uint8_t moves[4];
        int16_t hp;
        int16_t mp;
        int16_t currentHP;
        int16_t currentMP;
    };

    struct DigimonEntity : Entity
    {
        Stats stats;
        int16_t unk1;
        uint8_t unk2_1;
        uint8_t unk2_2;
        int16_t vabId;
        uint8_t chargeMode;
        uint8_t field7_0x57;
    };

    struct PartnerEntity : DigimonEntity
    {
        uint32_t learnedMoves[2];
        uint32_t unk1;
        uint32_t unk2;
        uint8_t name[20];
        int32_t lives;
    };

    struct TamerEntity : Entity
    {
        uint8_t tamerLevel;
        uint8_t raisedCount;
        uint16_t padding;
    };

    struct EvolutionPath
    {
        uint8_t from[5];
        uint8_t to[6];
    };

    struct EvoRequirements
    {
        int16_t digimon;
        int16_t hp;
        int16_t mp;
        int16_t offense;
        int16_t defense;
        int16_t speed;
        int16_t brain;
        int16_t care;
        int16_t weight;
        int16_t discipline;
        int16_t happiness;
        int16_t battles;
        int16_t techs;
        uint8_t flags;
        uint8_t padding;
    };

    struct Line
    {
        int16_t x1;
        int16_t y1;
        int16_t x2;
        int16_t y2;
        uint8_t clut;
        uint8_t pad;
    };

    struct SleepPattern
    {
        uint8_t sleepyHour;
        uint8_t sleepyMinute;
        uint8_t wakeupHour;
        uint8_t wakeupMinute;
        uint8_t wakeupDefault;
        uint8_t sleepyDefault;
    };

    struct PoopPile
    {
        uint8_t map;
        uint8_t x;
        uint8_t y;
        uint8_t size;
    };

    enum FadeMode
    {
        NONE       = 0,
        WHITE_FADE = 1,
        BLACK_FADE = 2,
    };

    struct FadeData
    {
        int16_t fadeOutTarget;
        int16_t fadeInTarget;
        int16_t fadeOutCurrent;
        int16_t fadeInCurrent;
        int8_t fadeProgress;
        FadeMode fadeMode;
        uint16_t padding;
        int32_t fadeProtection;
    };

    struct EvoModelData
    {
        int32_t modelPtr;
        int32_t unk1;
        int32_t unk2;
        int32_t modelSize;
        int32_t imagePtr;
        int32_t imageSize;
    };

    struct EvoSequenceData
    {
        int32_t unk1;
        PartnerEntity* partner;
        int16_t unk2;
        DigimonType digimonId : 16;
        EvoModelData modelData;
        int32_t unk3;
        PartnerPara* para;
        int16_t evoTarget;
        int16_t unk4;
        int32_t heightFactor;
    };

    enum class EntityType
    {
        NPC     = 0,
        UNKNOWN = 1,
        PLAYER  = 2,
        PARTNER = 3,

        NONE = -1,
    };

    class NPCEntity : DigimonEntity
    {
        SVector flee;
        int16_t bits;
        bool unk1;
        int8_t unk1_2;
        uint8_t unk2;
        uint8_t scriptId;
        uint8_t autotalk;
        uint8_t unk5;
    };

    struct EntityTable
    {
        union
        {
            struct
            {
                TamerEntity* tamer;
                PartnerEntity* partner;
                NPCEntity* npc1;
                NPCEntity* npc2;
                NPCEntity* npc3;
                NPCEntity* npc4;
                NPCEntity* npc5;
                NPCEntity* npc6;
                NPCEntity* npc7;
                NPCEntity* npc8;
            };
            Entity* table[10];
        };

        /*
         * Gets the entity pointer by their instance/entity ID or null if the ID is out of bound.
         */
        constexpr Entity* getEntityById(int32_t instanceId)
        {
            if (instanceId < 0 || instanceId > 9) return nullptr;
            return table[instanceId];
        }

        /*
         * Sets the entity pointer for a given instance. If the ID is out of bounds, nothing happens.
         */
        constexpr void setEntity(int32_t instanceId, Entity* ptr)
        {
            if (instanceId < 0 || instanceId > 9) return;
            table[instanceId] = ptr;
        }
    };

    struct TMDModel
    {
        int32_t id;
        uint32_t flags;
        int32_t objectCount;
        TMD_STRUCT objects[];
    };

    struct ModelComponent
    {
        int32_t unk0;
        TMDModel* modelPtr;
        int32_t* animTablePtr;
        void* mmdPtr;
        int16_t pixelPage;
        int16_t clutPage;
        uint8_t pixelOffsetX;
        uint8_t pixelOffsetY;
        int16_t unk2_3;
        uint16_t digiType;
        int16_t unk3;
    };

    struct SkeletonNode
    {
        int8_t object;
        int8_t parent;
    };

    using TickFunction   = void (*)(int32_t instanceId);
    using RenderFunction = void (*)(int32_t instanceId);
    using ItemFunction   = void (*)(ItemType itemId);

    extern PartnerPara PARTNER_PARA;
    // dummy size, used for unbound memory access
    extern DigimonData DIGIMON_DATA[1];
    extern RaiseData RAISE_DATA[1];
    extern EvolutionPath EVO_PATHS_DATA[1];
    extern EvoRequirements EVO_REQ_DATA[1];

    extern uint16_t CHAR_TO_GLYPH_TABLE[80];
    extern GlyphData GLYPH_DATA[79];
    extern uint8_t COLORCODE_HIGHBITS;
    extern uint8_t COLORCODE_LOWBITS;
    extern RGB8 TEXT_COLORS[17];
    extern GsOT* ACTIVE_ORDERING_TABLE;
    extern PartnerEntity PARTNER_ENTITY;
    extern ItemFunction ITEM_FUNCTIONS[128];
    extern uint32_t IS_SCRIPT_PAUSED;
    extern int32_t NANIMON_TRIGGER;
    extern int16_t EVOLUTION_TARGET;
    extern uint8_t CURRENT_SCREEN;
    extern SleepPattern SLEEP_PATTERN[8];
    extern uint32_t MENU_STATE;
    extern uint16_t PLAYTIME_FRAMES;
    extern uint16_t LAST_HANDLED_FRAME;
    extern uint16_t CURRENT_FRAME;
    extern uint16_t MINUTE;
    extern uint16_t HOUR;
    extern uint16_t DAY;
    extern uint16_t YEAR;
    extern int32_t HAS_BUTTERFLY;
    extern uint32_t BUTTERFLY_ID;
    extern GsRVIEW2 DIGIVICE_ENTITY_VIEW;
    extern uint8_t ITEM_SCOLD_FLAG;
    extern uint32_t HAS_IMMORTAL_HOUR;
    extern uint8_t IMMORTAL_HOUR;
    extern PoopPile WORLD_POOP[100];
    extern uint8_t CURRENT_POOP_ID;
    extern uint8_t PARTNER_AREA_RESPONSE;
    extern TamerItem TAMER_ITEM;
    extern DigimonType EVOLUTION_ITEM_TARGET[44];
    extern uint8_t PARTNER_STATE;
    extern uint8_t PARTNER_SUB_STATE;
    extern uint8_t TAMER_STATE;
    extern uint8_t TAMER_SUB_STATE;
    extern uint8_t PARTNER_ANIMATION;
    extern uint8_t POOP_TO_EAT;
    extern uint32_t IS_NATURAL_DEATH;
    extern FadeData FADE_DATA;
    extern uint8_t GAME_STATE;
    extern uint8_t IS_GAMETIME_RUNNING;
    extern TamerEntity TAMER_ENTITY;
    extern Stats DEATH_STATS;
    extern EvoModelData REINCARNATION_MODEL_DATA;
    extern EntityTable ENTITY_TABLE;
    extern uint8_t STOP_DISTANCE_TIMER;
    extern EvoSequenceData EVO_SEQUENCE_DATA;
    extern SVector POOP_ROTATION_MATRIX;
    extern bool MAP_LAYER_ENABLED;
    extern bool HAS_USED_EVOITEM;
    extern int16_t STATUS_UI_OFFSET_X;
    extern SkeletonNode* DIGIMON_SKELETONS[180];
    extern uint32_t PARTNER_WIREFRAME_TOTAL;
    extern uint16_t PARTNER_WIREFRAME_SUB[40];
    extern uint32_t ENTITY1_WIREFRAME_TOTAL;
    extern uint32_t PLAYER_SHADOW_ENABLED;
    // TODO: can be non-extern, but large
    extern PositionData PARTNER_POSITION_DATA[34];
    extern MomentumData PARTNER_MOMENTUM_DATA[34];
    extern PositionData TAMER_POSITION_DATA[22];
    extern MomentumData TAMER_MOMENTUM_DATA[22];

    extern void renderWireframed(GsDOBJ2* obj, int32_t wireFrameShare);
    extern void renderDropShadow(Entity* entity);
    extern void setupModelMatrix(PositionData* data);
    extern bool removeObject(ObjectID id, int32_t instance);
    extern void loadTMDandMTN(DigimonType digimonType, EntityType entityType, EvoModelData* modelData);
    extern void Partner_tick(int32_t);
    extern void projectPosition(GsCOORDINATE2* position, Vector* translation, SVector* rotation, Vector* scale);
    extern void renderObject(GsDOBJ2* obj, GsOT* ot, int32_t shift);
    extern void loadMMD(DigimonType digimonType, EntityType entityType);
    extern void loadPartnerSounds(DigimonType type);
    extern void learnMove(uint8_t move);
    extern void initializeConditionBubbles();
    extern void initializeButterfly();
    extern void initializePoop();
    extern void readFile(const char* path, void* buffer);
    extern bool isInDaytimeTransition();
    extern void updateTimeOfDay();
    extern void writePStat(int32_t address, uint8_t value);
    extern void startAnimation(Entity*, uint32_t animId);
    extern void unsetBubble(uint32_t id);
    extern uint8_t addConditionBubble(uint32_t type, Entity* entity);
    extern uint32_t setButterfly(Entity* entity);
    extern uint8_t getItemCount(ItemType item);
    extern void createPoopFX(SVector* pos);
    extern uint16_t convertAsciiToJis(uint8_t input);
    extern void clearTextSubArea(RECT* rect);
    extern bool addObject(ObjectID id, uint32_t instanceId, TickFunction tickFunc, RenderFunction renderFunc);
    extern void renderString(int32_t colorId,
                             int16_t posX,
                             int16_t posY,
                             int16_t uvWidth,
                             int16_t uvHeight,
                             int16_t uvX,
                             int16_t uvY,
                             int32_t offset,
                             int32_t hasShadow);
    extern void setUVDataPolyFT4(POLY_FT4* prim, int16_t uvX, int16_t uvY, int16_t uvWidth, int16_t uvHeight);
    extern void setPosDataPolyFT4(POLY_FT4* prim, int16_t posX, int16_t posY, int16_t uvWidth, int16_t uvHeight);
    extern int32_t random(int32_t limit);
    extern void renderBoxBar(int16_t posX,
                             int16_t posY,
                             uint16_t width,
                             uint16_t height,
                             uint8_t red,
                             uint8_t green,
                             uint8_t blue,
                             uint8_t flag,
                             int32_t layer);

    extern void setTrigger(uint32_t triggerId);
    extern void clearTextArea();
    extern void setTextColor(int32_t color);
    extern void addWithLimit(int16_t* valuePtr, int16_t value, int32_t limit);
    extern void addHealingParticleEffect(Entity* entity, int32_t param);
    extern void removeTamerItem();
    extern void callScriptSection(int32_t scriptId, uint32_t scriptSection, uint32_t param);
    extern void addTamerLevel(int32_t chance, int32_t amount);
    extern bool hasDigimonRaised(DigimonType type);
    extern void renderInsetBox(int32_t posX, int32_t posY, int32_t width, int32_t height, int32_t order);
    extern void renderDigimonStatsBar(int32_t value, int32_t maxValue, int32_t width, int32_t posX, int32_t posY);
    extern void renderSeperatorLines(Line* linePtr, int32_t count, int32_t layer);
    extern void renderRectPolyFT4(int16_t posX,
                                  int16_t posY,
                                  uint32_t width,
                                  uint32_t height,
                                  uint8_t texX,
                                  uint8_t texY,
                                  uint16_t texturePage,
                                  uint16_t clut,
                                  int zIndex,
                                  char flag);
    extern void renderDigiviceEntity(Entity* entity, int32_t entityId);

    extern void updateConditionAnimation();
    extern void setSleepDisabled(bool isDisabled);
    extern void unsetButterfly(uint32_t id);
    extern void advanceToTime(uint32_t hour, uint32_t minute);
    extern void getModelTile(Vector* location, int16_t* tileX, int16_t* tileY);

    extern EntityType getEntityType(Entity* entity);
    extern ModelComponent* getEntityModelComponent(DigimonType digimonType, EntityType entityType);
    extern void add3DSpritePrim(POLY_FT4* prim, SVector* vec1, SVector* vec2, SVector* vec3, SVector* vec4);
}

static_assert(sizeof(PositionData) == 0x88);
static_assert(sizeof(MomentumData) == 0x52);
static_assert(sizeof(Entity) == 0x38);
static_assert(sizeof(Stats) == 0x18);
static_assert(sizeof(TamerEntity) == 0x3C);
static_assert(sizeof(DigimonEntity) == 0x58);
static_assert(sizeof(PartnerEntity) == 0x80);
static_assert(sizeof(RaiseData) == 0x1C);
static_assert(sizeof(DigimonData) == 0x34);
static_assert(sizeof(PartnerPara) == 0x84);
static_assert(sizeof(Position) == 16);
static_assert(sizeof(GlyphData) == 24);
static_assert(sizeof(RGB8) == 3);
static_assert(sizeof(EvolutionPath) == 11);
static_assert(sizeof(NPCEntity) == 0x68);
static_assert(sizeof(EvoSequenceData) == 0x34);