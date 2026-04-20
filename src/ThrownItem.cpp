
#include "GameObjects.hpp"
#include "Inventory.hpp"
#include "Map.hpp"
#include "Model.hpp"
#include "Tamer.hpp"
#include "extern/BTL.hpp"
#include "extern/dw1.hpp"

namespace
{
    void tickThrownItem(int32_t instance)
    {
        constexpr dtl::array<int16_t, 24> ITEM_THROW_HEIGHT = {
            -240, -345, -440, -525, -600, -665, -720, -765, -800, -825, -840, -845,
            -840, -825, -800, -765, -720, -665, -600, -525, -440, -345, -240, -125,
        };

        if (TAMER_ENTITY.animId == 6 && TAMER_ENTITY.animFrame < 7)
        {
            TAMER_ITEM.spriteLocation.x = TAMER_ENTITY.posData[9].posMatrix.work.t[0];
            TAMER_ITEM.spriteLocation.y = TAMER_ENTITY.posData[9].posMatrix.work.t[1];
            TAMER_ITEM.spriteLocation.z = TAMER_ENTITY.posData[9].posMatrix.work.t[2];
        }
        else
        {
            TAMER_ITEM.spriteLocation.y = ITEM_THROW_HEIGHT[TAMER_ITEM.time];
            if (TAMER_ITEM.time != 0)
            {
                TAMER_ITEM.spriteLocation.x +=
                    ((PARTNER_ENTITY.posData[0].location.x - TAMER_ITEM.spriteLocation.x) / (25 - TAMER_ITEM.time));
                TAMER_ITEM.spriteLocation.z +=
                    ((PARTNER_ENTITY.posData[0].location.z - TAMER_ITEM.spriteLocation.z) / (25 - TAMER_ITEM.time));
            }

            TAMER_ITEM.time++;
            if (TAMER_ITEM.time > 23)
            {
                if (ITEM_FUNCTIONS[static_cast<int32_t>(TAMER_ITEM.type)])
                    ITEM_FUNCTIONS[static_cast<int32_t>(TAMER_ITEM.type)](TAMER_ITEM.type);

                BTL_addItemParticles(&PARTNER_ENTITY);
                removeTamerItem();
            }
        }
    }

    void renderThrownItem(int32_t instance)
    {
        renderOverworldItem(&TAMER_ITEM);
    }

} // namespace

void addThrownItem(ItemType type)
{
    if (TAMER_ITEM.type != ItemType::NONE) return;

    TAMER_ITEM.time             = 0;
    TAMER_ITEM.type             = type;
    TAMER_ITEM.spriteLocation.x = TAMER_ENTITY.posData[9].posMatrix.work.t[0];
    TAMER_ITEM.spriteLocation.y = TAMER_ENTITY.posData[9].posMatrix.work.t[1];
    TAMER_ITEM.spriteLocation.z = TAMER_ENTITY.posData[9].posMatrix.work.t[2];
    addObject(ObjectID::THROWN_ITEM, 0, tickThrownItem, renderThrownItem);
}