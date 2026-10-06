#include "Curling.hpp"
#include "CurlingRenderTypes.hpp"
#include "CurlingTypes.hpp"

#include "../Model.hpp"
#include "../extern/dw1.hpp"
#include "../extern/libgs.hpp"
#include "../extern/libgte.hpp"
#include "../extern/psx.hpp"

extern uint16_t KAR_ANIMATION_TIMER;

namespace
{
    constexpr GsRVIEW2 STONE_VIEW = {0, 0, -3320, 0, 0, 0, 0, nullptr};

    bool isOpponentRowHidden()
    {
        return KAR_MATCH_STATE != 3 && KAR_MATCH_STATE < 16 && (KAR_HINT_STATE != 0 || KAR_MATCH_STATE < 5);
    }
} // namespace

extern "C"
{
    [[gnu::optimize("Os")]]
    void KAR_renderScene(int32_t instanceId)
    {
        libgs_GsClearOt(0, 10, &KAR_ORDERING_TABLES[ACTIVE_FRAMEBUFFER]);
        for (int32_t player = 0; player < 3; player++) {
            for (int32_t index = 0; index < 5; index++) {
                if (player == 1 && isOpponentRowHidden()) continue;
                if (KAR_MATCH_STATE < 4 && KAR_ANIMATION_TIMER < (index + 1) * 60) continue;

                auto& stone = KAR_STONE_ROWS[player].stones[index];
                if (stone.state == 0) {
                    const int8_t entity  = KAR_CURRENT_PLAYER != 0 ? static_cast<int8_t>(KAR_OPPONENT_ENTITY) : 0;
                    const auto* position = ENTITY_TABLE.getEntityById(entity)->posData;
                    if (entity == 2) {
                        stone.position = position->location;
                        stone.position.x -= 80;
                    }
                    else {
                        const auto& translation = position[entity == 0 ? 9 : 4].posMatrix.work.t;
                        stone.position.x        = translation[0];
                        stone.position.y        = translation[1] + 120;
                        stone.position.z        = translation[2];
                        stone.rotationZ         = 0;
                        stone.selectionPhase    = 0;
                        stone.rotationX         = 0;
                    }
                }
                libgte_RotMatrix(reinterpret_cast<SVector*>(&stone.rotationX), &stone.coordinate.coord);
                libgte_TransMatrix(&stone.coordinate.coord, &stone.position);
                stone.coordinate.flag = 0;

                if (stone.state < 0) {
                    libgs_GsSetProjection(512);
                    libgs_GsSetRefView2(&STONE_VIEW);
                }
                else {
                    libgs_GsSetProjection(VIEWPORT_DISTANCE);
                    libgs_GsSetRefView2(&GS_VIEWPOINT);
                }

                if (stone.state < -100 || stone.state >= 101) continue;
                if (stone.state < 0)
                    drawObject(&stone.object, &KAR_ORDERING_TABLES[ACTIVE_FRAMEBUFFER], 5);
                else
                    drawObject(&stone.object, &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER], 2);
            }
        }
        libgs_GsSortOt(&KAR_ORDERING_TABLES[ACTIVE_FRAMEBUFFER], &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER]);
        libgs_GsSetProjection(VIEWPORT_DISTANCE);
        libgs_GsSetRefView2(&GS_VIEWPOINT);
    }
}
