#include "GameObjects.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "extern/dtl/types.hpp"
#include "extern/dw1.hpp"

namespace
{
    constexpr auto NINJAMON_EFFECT_COUNT = 41;
    dtl::array<int16_t, NINJAMON_EFFECT_COUNT> effectX;
    dtl::array<int16_t, NINJAMON_EFFECT_COUNT> effectY;
    dtl::array<int8_t, NINJAMON_EFFECT_COUNT> effectXOffset;
    dtl::array<int8_t, NINJAMON_EFFECT_COUNT> effectYOffset;
    int16_t counter;

    void renderNinjamonEffect(int32_t instanceId)
    {
        for (int32_t i = 0; i < NINJAMON_EFFECT_COUNT; i++)
        {
            auto& data = LOCAL_MAP_OBJECT_INSTANCE[i];
            data.x += effectXOffset[i];
            data.y += effectYOffset[i];
            data.orderValue = 10;

            if (data.x > 160)
            {
                data.x           = effectX[i];
                data.y           = effectY[i];
                effectXOffset[i] = random(10) + 12;
                effectYOffset[i] = random(10) + 3;
            }
        }

        counter++;
        if (counter >= 60)
        {
            for (int32_t i = 0; i < NINJAMON_EFFECT_COUNT; i++)
            {
                LOCAL_MAP_OBJECT_INSTANCE[i].flag = 1;
            }
            removeObject(ObjectID::NINJAMON_EFFECT, 0);
        }
    }
} // namespace

extern "C"
{
    void createNinjamonEffect()
    {
        counter = 0;
        storeMapObjectPosition(effectX.data(), effectY.data(), 0, NINJAMON_EFFECT_COUNT);

        for (int32_t i = 0; i < NINJAMON_EFFECT_COUNT; i++)
        {
            effectXOffset[i] = random(10) + 12;
            effectYOffset[i] = random(10) + 3;
        }

        addObject(ObjectID::NINJAMON_EFFECT, 0, nullptr, renderNinjamonEffect);
    }
}