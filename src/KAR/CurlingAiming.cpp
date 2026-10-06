#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../Model.hpp"
#include "../extern/dw1.hpp"
#include "../extern/libgs.hpp"
#include "../extern/libgte.hpp"

// Fixed resident storage shared with vanilla KAR initialization and match logic.
extern uint8_t KAR_OPPONENT_ENTITY;
extern int32_t KAR_AIM_ANGLE;

extern "C"
{
    [[gnu::optimize("Os")]]
    void KAR_beginAiming()
    {
        const int8_t entityId = KAR_CURRENT_PLAYER != 0 ? static_cast<int8_t>(KAR_OPPONENT_ENTITY) : 0;

        KAR_MATCH_STATE = 6;
        if (entityId == 0)
            startAnimation(ENTITY_TABLE.getEntityById(0), 0x23);
        else if (KAR_OPPONENT_ENTITY == 2)
            startAnimation(ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY), 0x1C);

        auto& stone = KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[KAR_SELECTED_STONE];
        stone.state = 0;
        if (stone.type == 3) libgs_GsLinkObject4(KAR_MODEL_DATA + 3, &stone.object, KAR_CURRENT_PLAYER != 0 ? 1 : 2);

        KAR_AIM_ANGLE                                             = -1024;
        ENTITY_TABLE.getEntityById(entityId)->posData->rotation.y = 0;
    }

    [[gnu::optimize("Os")]]
    void KAR_selectPreviousStone()
    {
        for (int32_t index = KAR_SELECTED_STONE - 1; index >= 0; index--) {
            if (KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[index].state == -1) {
                KAR_SELECTED_STONE = static_cast<int16_t>(index);
                return;
            }
        }
    }

    [[gnu::optimize("Os")]]
    void KAR_selectNextStone()
    {
        for (int32_t index = KAR_SELECTED_STONE + 1; index < 5; index++) {
            if (KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[index].state == -1) {
                KAR_SELECTED_STONE = static_cast<int16_t>(index);
                return;
            }
        }
    }

    // Both vanilla turn entries have the same body; their callers supply the direction.
    void KAR_turnAim(int32_t delta)
    {
        const int8_t entityId = KAR_CURRENT_PLAYER != 0 ? static_cast<int8_t>(KAR_OPPONENT_ENTITY) : 0;
        auto& rotation        = ENTITY_TABLE.getEntityById(entityId)->posData->rotation.y;
        rotation              = static_cast<int16_t>(rotation - delta);
        KAR_AIM_ANGLE += delta;
    }

    void KAR_beginThrow()
    {
        KAR_MATCH_STATE = 7;
        if (KAR_CURRENT_PLAYER == 0)
            startAnimation(ENTITY_TABLE.getEntityById(0), 0x24);
        else if (KAR_OPPONENT_ENTITY == 2)
            startAnimation(ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY), 0x1D);
        else
            startAnimation(ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY), 0x1F);
    }
}
