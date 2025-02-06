
#include "GameObjects.hpp"

#include "extern/dw1.hpp"

extern "C"
{
    static AttackObject ATTACK_OBJECTS[17];

    void initializeWorldObjects()
    {
        for (auto& obj : WORLD_OBJECTS)
        {
            obj.objectId   = ObjectID::NONE;
            obj.instanceId = 0xFFFF;
            obj.tick       = nullptr;
            obj.render     = nullptr;
        }
    }

    bool addObject(ObjectID id, uint16_t instanceId, TickFunction tickFunc, RenderFunction renderFunc)
    {
        // vanilla fills object 129 as a terminator, but we can do without
        for (auto& obj : WORLD_OBJECTS)
        {
            if (obj.objectId != ObjectID::NONE) continue;

            obj.objectId   = id;
            obj.instanceId = instanceId;
            obj.tick       = tickFunc;
            obj.render     = renderFunc;
            return true;
        }

        return false;
    }

    bool removeObject(ObjectID id, uint16_t instance)
    {
        // vanilla fills object 129 as a terminator, but we can do without
        for (auto& obj : WORLD_OBJECTS)
        {
            if (obj.objectId != id || obj.instanceId != instance) continue;

            obj.objectId   = ObjectID::NONE;
            obj.instanceId = 0xFFFF;
            obj.tick       = nullptr;
            obj.render     = nullptr;
            return true;
        }

        return false;
    }

    void tickObjects()
    {
        for (auto& obj : WORLD_OBJECTS)
            if (obj.objectId != ObjectID::NONE && obj.tick) obj.tick(obj.instanceId);
    }

    void renderObjects()
    {
        for (auto& obj : WORLD_OBJECTS)
            if (obj.objectId != ObjectID::NONE && obj.render) obj.render(obj.instanceId);
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