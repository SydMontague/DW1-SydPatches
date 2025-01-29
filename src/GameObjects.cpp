
#include "GameObjects.hpp"

#include "extern/dw1.hpp"

extern "C"
{
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
}