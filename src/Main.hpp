#include "extern/libgte.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    struct AttackObject
    {
        int32_t victimId = -1;
        int32_t unk1     = -1;
        SVector position;
        uint32_t unk2;
        uint32_t unk3;
        uint32_t casterId;
    };

    extern AttackObject ATTACK_OBJECTS[17];
}