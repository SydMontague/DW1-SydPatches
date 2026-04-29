
#include "GameObjects.hpp"
#include "extern/dw1.hpp"
#include "extern/libgs.hpp"

namespace
{
    int32_t buffModelFrame;

    constexpr dtl::array<int16_t, 2> buffModelValue{31436, 31500};

    void morphBuffModel(TMDModel* model, int32_t node, int16_t value)
    {
        auto count = model->objects[node].n_primitive;
        auto* ptr  = model->objects[node].primitive_top;

        for (int32_t i = 0; i < count; i++)
        {
            auto val = *ptr >> 0x18;
            if ((val & 4) == 0) continue;

            reinterpret_cast<int16_t*>(ptr)[3] = value;
            if ((val & 8) == 0)
            {
                if ((val & 0x10) == 0)
                    ptr += 7;
                else
                    ptr += 9;
            }
            else if ((val & 0x10) == 0)
                ptr += 8;
            else
                ptr += 11;
        }
    }

    void tickBuffModelObject(int32_t instanceId)
    {
        morphBuffModel(BUFF_MODEL, 5, buffModelValue[buffModelFrame % 2]);
        buffModelFrame++;
    }
} // namespace

extern "C"
{
    void initializeBuffModel(TMDModel* model)
    {
        BUFF_MODEL = model;
        libgs_GsMapModelingData(&model->flags);
    }

    void initializeBuffModelObject()
    {
        buffModelFrame = 0;
        addObject(ObjectID::BATTLE_UNKNOWN_MODEL, 0, tickBuffModelObject, nullptr);
    }

    void removeBuffModelObject()
    {
        removeObject(ObjectID::BATTLE_UNKNOWN_MODEL, 0);
    }
}