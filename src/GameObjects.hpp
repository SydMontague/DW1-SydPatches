#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    enum class ObjectID : uint16_t
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
        ENTITY_PARTICLE_FX   = 1282,
        PARTICLE_FX          = 1536,
        CLOUD_FX             = 1537,
        EFE_FLASH            = 1538,
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
        NINJAMON_EFFECT      = 4026,
        DAYTIME_TRANSITION   = 4030,
        MAIN_MENU            = 5000,
        FISHING              = 12289,

        // Custom Objects
        DEBUG_OVERLAY = 20000,

        NONE = 0xFFFF,
    };

    struct Object
    {
        ObjectID objectId;
        uint16_t instanceId;
        TickFunction tick;
        RenderFunction render;
    };

    struct AttackObject
    {
        int32_t victimId = -1;
        int32_t unk1     = -1;
        SVector position;
        uint32_t unk2;
        uint32_t unk3;
        uint32_t casterId;
    };

    // TODO can be relocated
    extern Object WORLD_OBJECTS[129];

    void initializeWorldObjects();
    bool addObject(ObjectID id, uint16_t instanceId, TickFunction tickFunc, RenderFunction renderFunc);
    bool removeObject(ObjectID id, uint16_t instance);
    void tickObjects();
    void renderObjects();

    void initializeAttackObjects();
    bool popAttackObject(int32_t entityId, AttackObject* out);
    bool addAttackObject(int32_t victimId, int32_t unk1, SVector* pos, uint32_t unk2, uint32_t unk3, uint32_t casterId);
}