#include "Main.hpp"

#include "extern/dw1.hpp"
#include "extern/libgte.hpp"

extern "C"
{
    AttackObject ATTACK_OBJECTS[17];

    void initializeGsTMDMap(void)
    {
        GS_TMD_MAP[0]  = libgte_GsTMDfastF3L;
        GS_TMD_MAP[16] = libgte_GsTMDfastTF3L;
        GS_TMD_MAP[18] = libgte_GsTMDfastTF3NL;
        GS_TMD_MAP[22] = libgte_GsTMDfastTNF3;
        GS_TMD_MAP[8]  = libgte_GsTMDfastG3L;
        GS_TMD_MAP[24] = libgte_GsTMDfastTG3L;
        GS_TMD_MAP[26] = libgte_GsTMDfastTG3NL;
        GS_TMD_MAP[30] = libgte_GsTMDfastTNG3;
        GS_TMD_MAP[32] = libgte_GsTMDfastF4L;
        GS_TMD_MAP[48] = libgte_GsTMDfastTF4L;
        GS_TMD_MAP[50] = libgte_GsTMDfastTF4NL;
        GS_TMD_MAP[54] = libgte_GsTMDfastTNF4;
        GS_TMD_MAP[40] = libgte_GsTMDfastG4L;
        GS_TMD_MAP[56] = libgte_GsTMDfastTG4L;
        GS_TMD_MAP[58] = libgte_GsTMDfastTG4NL;
        GS_TMD_MAP[62] = libgte_GsTMDfastTNG4;
        GS_TMD_MAP[51] = libgte_GsTMDdivTF4L;
        GS_TMD_MAP[53] = libgte_GsTMDdivTF4NL;
        GS_TMD_MAP[21] = libgte_GsTMDdivTF3NL;
        GS_TMD_MAP[61] = libgte_GsTMDdivTG4NL;
        GS_TMD_MAP[29] = libgte_GsTMDdivTG3NL;
        GS_TMD_MAP[63] = libgte_GsTMDdivTNG4;
        GS_TMD_MAP[31] = libgte_GsTMDdivTNG3;
        GS_TMD_MAP[55] = libgte_GsTMDdivTNF4;
        GS_TMD_MAP[23] = libgte_GsTMDdivTNF3;
        GS_TMD_MAP[34] = libgte_GsTMDfastF4NL;
        GS_TMD_MAP[38] = libgte_GsTMDfastNF4;
        return;
    }

    void initializeAttackObjects()
    {
        // vanilla uses RESET_ATTACK_OBJECT here to overwrite the objects
        for (auto& obj : ATTACK_OBJECTS)
            obj = {};
    }

    bool addAttackObject(int32_t victimId, int32_t unk1, SVector* pos, uint32_t unk2, uint32_t unk3, uint32_t casterId)
    {
        // vanilla fills object 17 as a terminator, but we can do without
        for (auto& obj : ATTACK_OBJECTS)
        {
            if (obj.unk1 == -1)
            {
                obj.victimId = victimId;
                obj.unk1     = unk1;
                obj.position = *pos;
                obj.unk2     = unk2;
                obj.unk3     = unk3;
                obj.casterId = casterId;
                return true;
            }
        }
        return false;
    }

    bool popAttackObject(int32_t entityId, AttackObject* out)
    {
        for (auto& obj : ATTACK_OBJECTS)
        {
            if (obj.victimId == entityId)
            {
                *out = obj;
                obj  = {};
                // vanilla copies all trailing non-empty objects, but why bother?
                return true;
            }
        }
        return false;
    }
}