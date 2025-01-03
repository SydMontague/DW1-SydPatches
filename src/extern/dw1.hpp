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

    struct DigimonPara
    {
        char name[20];
        int32_t boneCount;
        int16_t radius;
        int16_t height;
        Type type;
        Level level;
        Special special[3];
        uint8_t dropItem;
        uint8_t dropChance;
        int8_t moves[16];
        uint8_t padding;
    };

    struct PartnerPara
    {
        int32_t condition;
        int16_t sleepyHour;
        int16_t sleepyMinute;
        int16_t wakeupHour;
        int16_t wakeupMinute;
        int16_t wakeupDefault;
        int16_t sleepyDefault;
        int16_t awakeToday;
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

    using TickFunction   = void (*)(int32_t instanceId);
    using RenderFunction = void (*)(int32_t instanceId);

    extern PartnerPara PARTNER_PARA;
    // dummy size, used for unbound memory access
    extern DigimonPara DIGIMON_PARA[1];

    extern uint16_t CHAR_TO_GLYPH_TABLE[80];
    extern GlyphData GLYPH_DATA[79];
    extern uint8_t COLORCODE_HIGHBITS;
    extern uint8_t COLORCODE_LOWBITS;
    extern RGB8 TEXT_COLORS[17];
    extern GsOT* ACTIVE_ORDERING_TABLE;

    extern uint8_t* jis_at_index(uint8_t* string, uint32_t index);
    extern uint16_t convertAsciiToJis(uint8_t input);
    extern void clearTextSubArea(RECT* rect);
    extern void initializeStatusObjects();
    extern bool addObject(ObjectID id, uint16_t instanceId, TickFunction tickFunc, RenderFunction renderFunc);
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
}

static_assert(sizeof(DigimonPara) == 0x34);
static_assert(sizeof(PartnerPara) == 0x84);
static_assert(sizeof(Position) == 16);
static_assert(sizeof(GlyphData) == 24);
static_assert(sizeof(RGB8) == 3);