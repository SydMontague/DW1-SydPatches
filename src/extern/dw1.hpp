#pragma once

#include "libc.hpp"
#include "libgpu.hpp"
#include "libgs.hpp"
#include "libgte.hpp"
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
        int16_t x;
        int16_t y;
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
        [[gnu::aligned(4)]]
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

        constexpr uint32_t asUint32() const
        {
            uint32_t val = red;
            val |= green << 8;
            val |= blue << 16;

            return val;
        }
    };

    struct RGB5551
    {
        uint16_t red   : 5;
        uint16_t green : 5;
        uint16_t blue  : 5;
        uint16_t alpha : 1;
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
        GABUMON             = 17, // the best
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
        PLATINUMSUKAMON     = 73,
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
        SOULMON             = 107,
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
        NPC_KABUTERIMON     = 141,
        NPC_ANGEMON         = 142,
        NPC_BIRDRAMON       = 143,
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

    struct PoopPile
    {
        uint8_t map;
        uint8_t x;
        uint8_t y;
        uint8_t size;
    };

    enum class FadeMode : uint8_t
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
        uint8_t fadeProgress;
        FadeMode fadeMode;
        uint16_t padding;
        int32_t fadeProtection;
    };

    struct EvoModelData
    {
        uint8_t* modelPtr;
        int32_t unk1;
        int32_t unk2;
        int32_t modelSize;
        uint8_t* imagePtr;
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

    struct NPCEntity : DigimonEntity
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
            dtl::array<Entity*, 10> table;
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
        int32_t useCount;
        TMDModel* modelPtr;
        int32_t* animTablePtr;
        void* mmdPtr;
        int16_t pixelPage;
        int16_t clutPage;
        uint8_t pixelOffsetX;
        uint8_t pixelOffsetY;
        int16_t modelId;
        DigimonType digiType : 16;
        int16_t unk3;
    };

    struct SectionData
    {
        uint32_t unk1;
        uint32_t* globalPointer;
        uint32_t bss1Size;
        uint32_t bss2Size;
        uint32_t unk2;
        uint32_t data1Size;
        uint32_t codeDataSize;
        uint32_t* bss1Offset;
        uint32_t* bss2Offset;
        uint32_t* data2Offset;
        uint32_t* data1Offset;
        uint32_t* codeDataOffset;
        uint32_t* unk6;
        uint32_t* unk7;
        uint32_t* unk8;
        uint32_t* stackTop;
        uint32_t unk9;
        uint32_t stackFrames;
        uint32_t unk10;
        uint32_t unk11;
        uint32_t unk12;
        uint32_t heapPtr;
        uint32_t unk13;
        uint32_t unk14;
        uint32_t unk15;
        uint32_t unk16;
        uint32_t unk17;
        void* codeOffset;
        void* btlCodeOffset;
        void* stdCodeOffset;
        void* fishCodeOffset;
        void* evlCodeOffset;
        void* karCodeOffset;
        void* vsCodeOffset;
        void* movCodeOffset;
        void* doo2CodeOffset;
        void* dooaCodeOffset;
        void* trnCodeOffset;
        void* dgetCodeOffset;
        void* trn2CodeOffset;
        void* murdCodeOffset;
        void* endiCodeOffset;
        void* eabCodeOffset;
    };

    struct SkeletonNode
    {
        int8_t object;
        int8_t parent;
    };

    struct MapWarps
    {
        int16_t spawnX[10];
        int16_t spawnY[10];
        int16_t spawnZ[10];
        int16_t rotation[10];
        uint16_t targetMap[10];
        uint16_t targetExit[10];
    };

    struct Item
    {
        uint8_t name[20];
        int32_t value;
        int16_t meritValue;
        int16_t sortingValue;
        uint8_t itemColor;
        bool dropable;
        uint16_t unk;
    };

    struct Chest
    {
        Vector location;
        Vector trayLocation;
        SVector rotation;
        SVector trayRotation;
        uint16_t trigger;
        ItemType item;
        bool isTaken;
        uint8_t tileX;
        uint8_t tileY;
        uint16_t padding;
    };

    using TickFunction     = void (*)(int32_t instanceId);
    using RenderFunction   = void (*)(int32_t instanceId);
    using ItemFunction     = void (*)(ItemType itemId);
    using FileReadCallback = void (*)(int32_t);

    using BoxTickFunction   = void (*)();
    using BoxRenderFunction = void (*)();

    union ScreenCoord
    {
        struct
        {
            int16_t x;
            int16_t y;
        };
        int32_t raw;
    };

    struct UIBoxData
    {
        RECT startPos;
        RECT finalPos;
        uint16_t frame;
        uint16_t state;
        uint16_t rowOffset;
        uint16_t visibleRows;
        uint16_t totalRows;
        uint8_t features; // flag, 1 -> header separator, 2 -> transparent bg, 3 -> scroll bar
        uint8_t color;
        BoxTickFunction tick;
        BoxRenderFunction render;
    };

    struct MapEntry
    {
        uint8_t filename[10];
        int8_t num8bppImages;
        int8_t num4bppImages;
        uint8_t flags; // 0x40 no day/night | 0x80 has Digimon
        uint8_t doorsId;
        uint8_t toiletId;
        uint8_t loadingName;
    };

    struct ParticleFXData
    {
        int16_t tickCount;
        int16_t tickTarget;
        int16_t color;
        int16_t mode;
        int16_t cloudDelay;
        uint16_t r;
        uint16_t g;
        uint16_t b;
        SVector pos;
        Entity* entity;
        int16_t flashX[3];
        int16_t flashY[3];
        int16_t flashZ[3];
        int16_t flashScale[3];
    };

    struct SPosition3D
    {
        int16_t x;
        int16_t y;
        int16_t z;
    };

    struct Position3D
    {
        int32_t x;
        int32_t y;
        int32_t z;
    };

    struct MapLight
    {
        Position3D pos;
        int32_t red;
        int32_t green;
        int32_t blue;
    };

    struct MapSetup
    {
        Position3D viewpoint;
        Position3D referencePoint;
        dtl::array<MapLight, 3> lights;
        int32_t unk1;
        int32_t unk2;
        int32_t unk3;
        int32_t viewDistance;
        dtl::array<int32_t, 4> likedArea;
        dtl::array<int32_t, 4> dislikedArea;
        int32_t width;
        int32_t height;
        uint32_t tiles[0];
    };

    enum InputButtons : uint16_t
    {
        BUTTON_L2       = 0x0001,
        BUTTON_R2       = 0x0002,
        BUTTON_L1       = 0x0004,
        BUTTON_R1       = 0x0008,
        BUTTON_TRIANGLE = 0x0010,
        BUTTON_CIRCLE   = 0x0020,
        BUTTON_CROSS    = 0x0040,
        BUTTON_SQUARE   = 0x0080,
        BUTTON_SELECT   = 0x0100,
        BUTTON_START    = 0x0800,
        BUTTON_UP       = 0x1000,
        BUTTON_RIGHT    = 0x2000,
        BUTTON_DOWN     = 0x4000,
        BUTTON_LEFT     = 0x8000,
    };

    enum class Overlay
    {
        NONE     = 0,
        BTL_REL  = 1,
        STD_REL  = 2,
        FISH_REL = 3,
        EVL_REL  = 4,
        KAR_REL  = 5,
        VS_REL   = 6,
        MOV_REL  = 7,
        DOO2_REL = 8,
        DOOA_REL = 9,
        TRN_REL  = 10,
        SHOP_REL = 11,
        DGET_REL = 12,
        TRN2_REL = 13,
        MURD_REL = 14,
        ENDI_REL = 15,
        EAB_REL  = 16,
        OFFSET   = -1,
    };

    enum class CollisionCode : int16_t
    {
        TAMER,
        PARTNER,
        NPC1,
        NPC2,
        NPC3,
        NPC4,
        NPC5,
        NPC6,
        NPC7,
        NPC8,
        MAP,
        SCREEN,
        NONE = -1,
    };

    enum class Medal
    {
        GRADE_CUP,
        VERSION_CUP,
        TYPE_CUP,
        SPECIAL_CUP,
        WIN_100_CUPS,
        ALL_TECHS,
        ALL_DIGIMON,
        MAX_STATS,
        PERFECT_CURLING,
        CATCH_100_FISH,
        BEAT_THE_GAME,
        RECRUIT_EVERYONE,
        ALL_CARDS,
        MAX_MONEY,
        PLAYTIME,
        UNUSED1,
        UNUSED2,
        UNUSED3,
        UNUSED4,
        UNUSED5,
    };

    enum class BattleResult : int32_t
    {
        FLED = 0,
        WON  = 1,
        LOST = -1,
    };

    struct ParticleFlashData
    {
        Position screenPos;
        int16_t depth;
        uint16_t unk1;
        int32_t scale;
        uint16_t sizeX;
        uint16_t sizeY;
        uint16_t tpage;
        uint8_t uBase;
        uint8_t vBase;
        uint16_t clut;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t colorScale;
    };

    struct EFEFlashData
    {
        SVector worldPos;
        int16_t progress;
        int16_t tMax;
        int16_t mode;
        int16_t fixedDepth;
        int16_t redMin;
        int16_t greenMin;
        int16_t blueMin;
        int16_t redMax;
        int16_t greenMax;
        int16_t blueMax;
        int32_t scaleMin;
        int32_t scaleMax;
        int16_t offsetX;
        int16_t offsetY;
    };

    struct MapObject
    {
        uint16_t texX;
        uint16_t texY;
        uint16_t width;
        uint16_t height;
        uint16_t someX;
        uint16_t someY;
        uint16_t someZ;
        uint16_t clut;
        uint16_t transparency;
    };

    struct LocalMapObjectInstance
    {
        int16_t orderValue;
        int16_t x;
        int16_t y;
        int16_t animSprites[8];
        uint8_t animTimes[8];
        uint8_t timer;
        uint8_t pad;
        int8_t currentFrame;
        uint8_t flag;
    };

    struct LocalMapObject
    {
        uint16_t texX;
        uint16_t texY;
        uint16_t someX;
        uint16_t someY;
        uint16_t someZ;
        uint8_t width;
        uint8_t height;
        uint8_t clut;
        uint8_t transparency;

        LocalMapObject(const MapObject& obj)
        {
            texX         = obj.texX;
            texY         = obj.texY;
            someX        = obj.someX;
            someY        = obj.someY;
            someZ        = obj.someZ;
            width        = obj.width;
            height       = obj.height;
            clut         = obj.clut;
            transparency = obj.transparency;
        }
    };

    struct MapObjectInstance
    {
        uint16_t animState[8];
        uint16_t animDuration[8];
        uint16_t posX;
        uint16_t posY;
        uint16_t flag;
    };

    struct MapDigimon
    {
        int16_t typeId;
        int16_t followMode; // follow behavior
        int16_t posX;
        int16_t posY;
        int16_t posZ;
        int16_t rotX;
        int16_t rotY;
        int16_t rotZ;
        int16_t aiTrackRange;
        int16_t unk2;
        int16_t scriptId;
        int16_t maxHP;
        int16_t maxMP;
        int16_t currentHP;
        int16_t currentMP;
        int16_t offense;
        int16_t defense;
        int16_t speed;
        int16_t brains;
        int16_t bits;
        int16_t chargeMode;
        int16_t unk5; // smart AI?
        int16_t moves[4];
        int16_t movePrio[4];
        int16_t fleeX;
        int16_t fleeY;
        int16_t fleeZ;
        int16_t waypointCount;
        int16_t aiSections[8];
        SPosition3D waypoints[];
    };

    struct MapDigimonEntity
    {
        DigimonType typeId;
        Vector waypoints[8];
        int16_t aiSections[8];
        int16_t activeSecton;
        int16_t padding2;
        Vector targetLocation;
        int16_t posX;
        int16_t posY;
        int16_t posZ;
        int16_t rotX;
        int16_t rotY;
        int16_t rotZ;
        int16_t trackingRange;
        int16_t targetAngle;
        int16_t ccDiff;
        int16_t cwDiff;
        uint8_t followMode;
        uint8_t waypointWaitTimer;
        int8_t animation;
        bool hasWaypointTarget;
        int8_t lookAtTamerState;
        bool stopAnim;
    };

    enum class Range : uint8_t
    {
        UNDEFINED = 0,
        SHORT     = 1,
        LARGE     = 2,
        WIDE      = 3,
        SELF      = 4,
    };

    enum class Status : uint8_t
    {
        NONE    = 0,
        POISON  = 1,
        CONFUSE = 2,
        STUN    = 3,
        FLATTEN = 4,
    };

    struct Move
    {
        int32_t distance;
        int16_t power;
        uint8_t mpCost;
        uint8_t iframes;
        Range range;
        uint8_t special;
        Status status;
        uint8_t accuracy;
        uint8_t statusChance;
        uint8_t unk3;
        uint8_t unk4;
        uint8_t unk5;
    };

    struct EvoChartBoxData
    {
        uint16_t posX;
        uint16_t posY;
        uint8_t u;
        uint8_t v;
        uint8_t clut;
    };

    struct CardEntry
    {
        uint8_t type;
        uint8_t rarity;
        uint16_t meritValue;
    };

    struct BattleFlags
    {
        bool isPoisoned     : 1;
        bool isConfused     : 1;
        bool isStunned      : 1;
        bool isFlattened    : 1;
        bool isKnockedBack  : 1;
        bool isAttacking    : 1;
        bool isTransforming : 1;
        bool isBlocking     : 1;
        bool isProtected    : 1;
        bool unkFlag1       : 1;
        bool unkFlag2       : 1;
        bool isOnChargeup   : 1;
        bool isOnCooldown   : 1;
        bool isSenile       : 1;
        bool unkFlag3       : 1;
        bool isDead         : 1;
    };

    struct FighterData
    {
        int32_t statusFxId;
        int32_t effectSlot[4];
        int32_t unk11;
        int16_t finisherGoal;
        int16_t finisherProgress;
        int16_t poisonTimer;
        int16_t confusionTimer;
        int16_t stunTimer;
        int16_t flatTimer;
        int16_t flatAttackTimer;
        int16_t invulnerableTimer;
        int16_t cooldown;
        int16_t senileTimer;
        int16_t unk15;
        int16_t hpDamageBuffer;
        int16_t mpDamageBuffer;
        int16_t speedBuffer;
        BattleFlags flags;
        uint8_t moveRange;
        uint8_t targetId;
        uint8_t queuedAnim;
        uint8_t buffsRemaining;
        uint8_t buffPrioTimer;
        uint8_t unk16;
        uint8_t table1[150];
        uint8_t table2[150];
    };

    struct PlayerDataSub
    {
        int32_t unk1;
        Position pos;
        int16_t unk2;
        int16_t unk3;
        int16_t unk4;
        uint8_t unk5;
        uint8_t unk6;
        int16_t unk7;
        int16_t unk8;
        uint8_t unk9;
        uint8_t unk10;
        uint8_t unk11;
        uint8_t unk12;
        uint8_t unk13;
        uint8_t unk14;
        uint8_t unk15;
        uint8_t unk16;
        uint8_t unk17;
        uint8_t unk18;
        uint8_t unk19;
        uint8_t unk20;
        uint8_t unk21;
        uint8_t unk22;
        uint8_t unk23;
        uint8_t unk24;
        uint8_t unk25;
        uint8_t unk26;
        uint8_t unk27;
        uint8_t unk28;
    };

    enum class BattleCommand : uint8_t
    {
        RUN       = 1,
        ATTACK    = 2,
        YOUR_CALL = 3,
        MODERATOR = 4,
        DISTANCE  = 5,
        DEFEND    = 6,
        CHANGE    = 7,
        ATTACK1   = 8,
        ATTACK2   = 9,
        ATTACK3   = 10,
        FINISHER  = 11,
    };

    struct PlayerData
    {
        PlayerDataSub unk1[4];

        int16_t hitCount;
        int16_t blockedCount;
        int16_t statusedCount;
        int16_t unk2;
        int16_t startingHP;
        int16_t unk3;
        BattleCommand currentCommand[2];
        BattleCommand buffereCommand[2];
        BattleCommand hoveredCommand[2];
        BattleCommand availableCommands[2][9];
        uint8_t numCommands[2];
        uint8_t finisherChargeup[2];
        uint8_t remainingChargeupTime[2];
        uint8_t entityIds[4];
        uint8_t unk4;
        uint8_t unk5;
        uint8_t unk6;
        uint8_t unk7;
        uint8_t changeTarget;
        uint8_t usedMoves[12];
        uint8_t unk8;
        uint8_t unk9;
        uint8_t unk10;
    };

    struct CombatData
    {
        FighterData fighter[4];
        PlayerData player;
    };

    struct MapSoundPara
    {
        int16_t sectorId;
        int16_t sectorCount;
    };

    struct MapTileData
    {
        uint8_t* imagePtr;
        int16_t tileId;
        int16_t posX;
        int16_t posY;
        int16_t texU;
        int16_t texV;
        int16_t tpage;
        int16_t clut;
    };

    extern PartnerPara PARTNER_PARA;
    // dummy size, used for unbound memory access
    extern DigimonData DIGIMON_DATA[];
    extern RaiseData RAISE_DATA[];
    extern EvolutionPath EVO_PATHS_DATA[];
    extern EvoRequirements EVO_REQ_DATA[];

    extern Vector CAMERA_TARGET;
    extern int16_t DRAW_OFFSET_LIMIT_X_MAX;
    extern int16_t DRAW_OFFSET_LIMIT_Y_MAX;
    extern int16_t DRAW_OFFSET_LIMIT_X_MIN;
    extern int16_t DRAW_OFFSET_LIMIT_Y_MIN;
    extern int16_t PLAYER_OFFSET_X;
    extern int16_t PLAYER_OFFSET_Y;
    extern uint8_t PREV_TILE_X;
    extern uint8_t PREV_TILE_Y;
    extern int8_t TAMER_START_TILE_X;
    extern int8_t TAMER_START_TILE_Y;
    extern int8_t TAMER_WAYPOINT_CURRENT;
    extern int8_t TAMER_WAYPOINT_ACTIVE;
    extern uint8_t TAMER_PREVIOUS_TILE_X;
    extern uint8_t TAMER_PREVIOUS_TILE_Y;
    extern uint8_t PARTNER_TAMER_PREVIOUS_TILE_X;
    extern uint8_t PARTNER_TAMER_PREVIOUS_TILE_Y;
    extern dtl::array<int8_t, 30> PARTNER_WAYPOINT_X;
    extern dtl::array<int8_t, 30> PARTNER_WAYPOINT_Y;
    extern dtl::array<int8_t, 32> TAMER_WAYPOINT_X;
    extern dtl::array<int8_t, 32> TAMER_WAYPOINT_Y;
    extern int8_t TAMER_WAYPOINT_COUNT;
    extern int8_t PARTNER_WAYPOINT_COUNT;
    extern int8_t PARTNER_WAYPOINT_CURRENT;
    extern uint8_t GENERAL_BUFFER;
    extern dtl::array<uint8_t, 66> DIGIMON_VLALL_SOUND_ID;
    extern dtl::array<uint8_t, 180> DIGIMON_VBALL_SOUND_ID;
    extern dtl::array<MapSoundPara, 22> MAP_SOUND_PARA;
    extern dtl::array<uint32_t, 10> VHB_SOUNDBUFFER_START;
    extern dtl::array<uint8_t*, 10> VHB_HEADER_ADDR;
    extern int32_t ACTIVE_MAP_SOUND_ID;
    extern CombatData* COMBAT_DATA_PTR;
    extern CardEntry CARD_DATA[66];
    extern int16_t PLAYTIME_HOURS;
    extern int16_t PLAYTIME_MINUTES;
    extern int16_t MERIT;
    extern dtl::array<ItemType, 30> INVENTORY_ITEM_TYPES;
    extern dtl::array<uint8_t, 30> INVENTORY_ITEM_AMOUNTS;
    extern dtl::array<uint8_t, 30> INVENTORY_ITEM_NAMES;
    extern uint8_t INVENTORY_POINTER;
    extern uint16_t CHANGED_INPUT;
    extern Move MOVE_DATA[122];
    extern uint32_t TRIANGLE_MENU_STATE;
    extern uint8_t MENU_SUB_STATE;
    extern MapDigimonEntity MAP_DIGIMON_TABLE[8];
    extern GsRVIEW2 GS_VIEWPOINT;
    extern GsRVIEW2 GS_VIEWPOINT_COPY;
    extern Chest CHEST_ARRAY[8];
    extern Item ITEM_PARA[128];
    extern DroppedItem DROPPED_ITEMS[10];
    extern UIBoxData UI_BOX_DATA[6];
    extern uint32_t POLLED_INPUT;
    extern uint32_t POLLED_INPUT_PREVIOUS;
    extern MapWarps MAP_WARPS;
    extern uint16_t CHAR_TO_GLYPH_TABLE[80];
    extern GlyphData GLYPH_DATA[79];
    extern uint8_t COLORCODE_HIGHBITS;
    extern uint8_t COLORCODE_LOWBITS;
    extern RGB8 TEXT_COLORS[17];
    extern GsOT* ACTIVE_ORDERING_TABLE;
    extern PartnerEntity PARTNER_ENTITY;
    extern NPCEntity NPC_ENTITIES[8];
    extern ItemFunction ITEM_FUNCTIONS[128];
    extern uint32_t IS_SCRIPT_PAUSED;
    extern int32_t NANIMON_TRIGGER;
    extern int16_t EVOLUTION_TARGET;
    extern uint8_t CURRENT_SCREEN;
    extern uint8_t MENU_STATE;
    extern uint16_t PLAYTIME_FRAMES;
    extern uint16_t LAST_HANDLED_FRAME;
    extern uint16_t CURRENT_FRAME;
    extern uint16_t MINUTE;
    extern uint16_t HOUR;
    extern uint16_t DAY;
    extern uint16_t YEAR;
    extern int32_t HAS_BUTTERFLY;
    extern uint32_t BUTTERFLY_ID;
    extern uint8_t ITEM_SCOLD_FLAG;
    extern uint32_t HAS_IMMORTAL_HOUR;
    extern uint8_t IMMORTAL_HOUR;
    extern PoopPile WORLD_POOP[100];
    extern uint8_t CURRENT_POOP_ID;
    extern uint8_t PARTNER_AREA_RESPONSE;
    extern TamerItem TAMER_ITEM;
    extern uint8_t PARTNER_STATE;
    extern uint8_t PARTNER_SUB_STATE;
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
    extern int16_t WIREFRAME_COLOR_MIN;
    extern int16_t WIREFRAME_COLOR_MAX;
    extern uint8_t WIREFRAME_RNG_TABLE[16];
    extern ModelComponent TAMER_MODEL;
    extern ModelComponent PARTNER_MODEL;
    extern uint8_t TAMER_MODEL_BUFFER[56 * 1024];
    extern uint8_t PARTNER_MODEL_BUFFER[98 * 1024];
    extern ModelComponent NPC_MODEL[5];
    extern ModelComponent UNKNOWN_MODEL[16];
    extern int32_t NPC_MODEL_TAKEN[5];
    extern int32_t UNKNOWN_MODEL_TAKEN[16];
    extern uint8_t TEXTURE_BUFFER[];
    extern GsTMDFunction GS_TMD_MAP[64];
    extern Vector STORED_TAMER_POS;
    extern uint32_t IS_IN_MENU;
    extern uint8_t TARGET_MAP;
    extern uint8_t CURRENT_EXIT;
    extern void* DOOA_DATA_PTR;
    extern uint32_t SOME_SCRIPT_SYNC_BIT;
    extern uint8_t TALKED_TO_ENTITY;
    extern uint16_t CURRENT_SCRIPT_ID;
    extern uint8_t PREVIOUS_EXIT;
    extern uint32_t MONEY;
    extern uint8_t TEXTBOX_OPEN_TIMER;
    extern uint8_t SKIP_DAYTIME_TRANSITION;
    extern uint8_t ACTIVE_BGM_FONT;
    extern int32_t ACTIVE_FRAMEBUFFER;
    extern uint8_t GS_WORK_BASES[2][81920];
    extern GsOT GS_ORDERING_TABLE[2];
    extern MapEntry MAP_ENTRIES[255];
    extern int8_t NPC_IS_WALKING_TOWARDS[8];
    extern int32_t DRAWING_OFFSET_X;
    extern int32_t DRAWING_OFFSET_Y;
    extern int32_t DRAWING_OFFSET_X_COPY;
    extern int32_t DRAWING_OFFSET_Y_COPY;
    extern uint32_t TRAINING_COMPLETE;
    extern uint32_t VIEWPORT_DISTANCE;
    extern uint32_t VIEWPORT_DISTANCE_COPY;
    extern EFEFlashData* EFE_FLASH_DATA;
    extern int32_t FLASH_INSTANCE;
    extern uint8_t* EFE_DATA_STACK;
    extern uint8_t SOME_IMAGE_DATA[3584];
    extern int16_t EFE_LOADED_MOVE_DATA[16];
    extern int16_t EFE_LOAD_STATE;
    extern uint8_t EFE_SCRIPT_MEM1_DATA[128];
    extern LocalMapObject LOCAL_MAP_OBJECTS[180];
    extern uint16_t MAP_OBJECT_INSTANCE_COUNT;
    extern uint32_t MAP_OBJECT_MOVE_TO_DATA[10];
    extern int16_t MIST_CLUT_Y[2];
    extern int16_t CAMERA_X;
    extern int16_t CAMERA_Y;
    extern int16_t CAMERA_X_PREVIOUS;
    extern int16_t CAMERA_Y_PREVIOUS;
    extern uint8_t NPC_ACTIVE_ANIM[10];
    extern LocalMapObjectInstance LOCAL_MAP_OBJECT_INSTANCE[188];
    extern int32_t LOADED_DIGIMON_MODELS[8];
    extern CollisionCode NPC_COLLISION_STATE[8];
    extern uint8_t SELECTED_MEDAL;
    extern uint8_t MEDAL_SELECTOR_INDEX;
    extern uint8_t SELECTED_CARD;
    extern uint8_t* MOVE_NAMES[122];
    extern GsOT* FRAMEBUFFER_OT[2];
    extern uint32_t* FRAMEBUFFER0_ORIGIN;
    extern uint32_t* FRAMEBUFFER1_ORIGIN;
    extern GsF_LIGHT LIGHT_DATA[3];
    extern GsDOBJ2 MEDAL_OBJECT;
    extern GsCOORDINATE2 MEDAL_COORDINATES;
    extern uint8_t MAP_COLLISION_DATA[10000];
    extern uint8_t INVENTORY_SIZE;
    extern dtl::array<int8_t, 35> MAP_TILES;
    extern dtl::array<MapTileData, 35> MAP_TILE_DATA;
    extern uint8_t PREVIOUS_SCREEN;
    extern int8_t CAMERA_REACHED_TARGET;
    extern bool CAMERA_HAS_TARGET;
    extern uint32_t CAMERA_UPDATE_TILES;
    extern uint32_t SKIP_MAP_FILE_READ;
    extern uint8_t MAP_WIDTH;
    extern uint8_t MAP_HEIGHT;
    extern uint8_t MAP_TILE_X;
    extern uint8_t MAP_TILE_Y;
    extern uint16_t SCRIPT_MAP_CHANGE_STATE;
    extern dtl::array<const char*, 70> MAP_NAME_PTR;
    extern uint8_t TRN_LOADING_COMPLETE;
    extern dtl::array<const char*, 128> ITEM_DESC_PTR;

    // TODO can be relocated
    extern uint8_t* PTR_DIGIMON_FILE_NAMES[180];
    // TODO: can be non-extern, but large
    extern dtl::array<uint8_t, 22136> SEQ_BUFFER;
    extern PositionData PARTNER_POSITION_DATA[34];
    extern MomentumData PARTNER_MOMENTUM_DATA[34];
    extern PositionData TAMER_POSITION_DATA[22];
    extern MomentumData TAMER_MOMENTUM_DATA[22];
    extern SectionData SECTION_DATA;
    extern dtl::array<SVector, 177> CONDITION_FX_OFFSETS;

    bool isFishing();
    void updateBGM();
    void startThrowingItem();
    void renderItemSprite(ItemType type, int16_t posX, int16_t posY, int32_t depth);
    void renderSelectionCursor(int16_t x, int16_t y, int16_t width, int16_t height, int32_t depth);
    void calculatePosition(GsCOORDINATE2* coord, Matrix* matrix);
    void runMapHeadScript(uint8_t mapId);
    void unloadMapParts();
    void loadDoors(int32_t doorEntryId);
    void initializeWarpCrystals(int32_t mapId);
    void initializeTrainingPoop();
    void checkFishingMap(int32_t mapId);
    void checkCurlingMap(int32_t mapId);
    void checkArenaMap(int32_t mapId);
    bool isInvisible(Entity* entity);
    void handleBattleIdle(DigimonEntity* entity, Stats* stats, BattleFlags flags);
    void startBattleIdleAnimation(Entity* entity, Stats* stats, BattleFlags flags);
    void setItemTexture(POLY_FT4* prim, ItemType item);
    void addEntityText(DigimonEntity* entity, int32_t entityId, int8_t color, int32_t amount, int8_t icon);
    void dailyPStatTrigger();
    bool hasMove(int32_t move);
    void activateMedalTexture(Medal medal);
    uint8_t entityGetTechFromAnim(DigimonEntity* entity, uint8_t move);
    void initializeFishing();
    void
    renderLinePrimitive(uint32_t color, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int32_t layer, uint32_t flag);
    void closeInventoryBoxes2();
    uint32_t hasFishingRod();
    void initializeLoadedNPCModels();
    void NPCEntity_tickBattle(int32_t instanceId);
    bool entityIsOffScreen(Entity* entity, int16_t width, int16_t height);
    bool isTriggerSet(int32_t trigger);
    bool isTamerOnScreen();
    uint32_t handleBattleStart(uint32_t talkedToEntity);
    void loadBattleData(uint32_t talkedToEntity, uint32_t enemyCount);
    void playBGM(uint8_t soundFont);
    void renderUIBox(int32_t instanceId);
    bool hasMedal(Medal medal);
    void unlockMedal(Medal medal);
    uint8_t getCardAmount(uint8_t cardId);
    void loadDynamicLibrary(Overlay lib, uint8_t* isComplete, bool isAsync, FileReadCallback callback, int32_t param);
    int32_t isSoundLoaded(bool isAsync, int32_t soundId);
    void setMapLayerEnabled(int32_t val);
    void entityLookAtTile(Entity* entity, int8_t tileX, int8_t tileY);
    bool tickOpenChestTray(uint32_t chestId);
    bool tickCloseChestTray(uint32_t chestId);
    void getEntityScreenPos(Entity* entity, int32_t objId, Position* outPos);
    void startGameTime();
    /*
     * Checks if a button has been pressed and consumes it (i.e. subsequent checks for the same button within the same
     * tick return false).
     */
    bool isKeyDown(uint16_t keyMask);
    void stopGameTime();
    void Tamer_tickFishing();
    extern void Tamer_tickBattle(int32_t instanceId);
    extern int32_t main();
    extern void addFileReadRequest2(const uint8_t* path,
                                    uint8_t* buffer,
                                    int32_t* readComplete,
                                    FileReadCallback callback,
                                    int32_t param);
    extern void renderDropShadow(Entity* entity);
    extern void Partner_tick(int32_t);
    extern void projectPosition(GsCOORDINATE2* position, Vector* translation, SVector* rotation, Vector* scale);
    extern void renderObject(GsDOBJ2* obj, GsOT* ot, int32_t shift);
    extern void learnMove(uint8_t move);
    extern void initializeConditionBubbles();
    extern void initializeButterfly();
    extern void initializePoop();
    extern uint8_t readPStat(int32_t address);
    extern void writePStat(int32_t address, uint8_t value);
    extern void unsetBubble(uint32_t id);
    extern uint8_t addConditionBubble(uint32_t type, Entity* entity);
    extern uint32_t setButterfly(Entity* entity);
    extern uint16_t convertAsciiToJis(uint8_t input);
    extern void clearTextSubArea(RECT* rect);
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
    extern void setPosDataPolyFT4(POLY_FT4* prim, int16_t posX, int16_t posY, int16_t width, int16_t height);
    void renderTrianglePrimitive(uint32_t color,
                                 int16_t x1,
                                 int16_t y1,
                                 int16_t x2,
                                 int16_t y2,
                                 int16_t x3,
                                 int16_t y3,
                                 int32_t order,
                                 uint32_t flag);

    extern void setTrigger(uint32_t triggerId);
    extern void clearTextArea();
    extern void setTextColor(int32_t color);
    extern void callScriptSection(int32_t scriptId, uint32_t scriptSection, uint32_t param);
    extern bool hasDigimonRaised(DigimonType type);

    extern void updateConditionAnimation();
    extern void unsetButterfly(uint32_t id);
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
static_assert(sizeof(Position) == 4);
static_assert(sizeof(GlyphData) == 24);
static_assert(sizeof(RGB8) == 3);
static_assert(sizeof(RGB5551) == 2);
static_assert(sizeof(EvolutionPath) == 11);
static_assert(sizeof(NPCEntity) == 0x68);
static_assert(sizeof(EvoSequenceData) == 0x34);
static_assert(sizeof(ModelComponent) == 0x1C);
static_assert(sizeof(SectionData) == 0xac);
static_assert(sizeof(MapWarps) == 0x78);
static_assert(sizeof(FadeData) == 0x10);
static_assert(sizeof(Item) == 0x20);
static_assert(sizeof(Chest) == 0x38);
static_assert(sizeof(DroppedItem) == 0x10);
static_assert(sizeof(TamerItem) == 0x10);
static_assert(sizeof(ParticleFXData) == 0x34);
static_assert(sizeof(ParticleFlashData) == 0x1C);
static_assert(sizeof(EFEFlashData) == 0x28);
static_assert(sizeof(LocalMapObject) == 0x0E);
static_assert(sizeof(MapObject) == 0x12);
static_assert(sizeof(MapObjectInstance) == 0x26);
static_assert(sizeof(MapDigimon) == 0x54);
static_assert(sizeof(MapDigimonEntity) == 0xC4);
static_assert(sizeof(Move) == 16);
static_assert(sizeof(EvoChartBoxData) == 8);
static_assert(sizeof(CardEntry) == 4);
static_assert(sizeof(BattleFlags) == 2);
static_assert(sizeof(FighterData) == 0x168);
static_assert(sizeof(PlayerDataSub) == 0x28);
static_assert(sizeof(PlayerData) == 0xe4);
static_assert(sizeof(CombatData) == 0x684);
static_assert(sizeof(MapTileData) == 0x14);
static_assert(sizeof(MapLight) == 24);
static_assert(sizeof(MapSetup) == 0x98);