#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../Model.hpp"
#include "../extern/dw1.hpp"
#include "../extern/libgs.hpp"
#include "../extern/libgte.hpp"
#include "../extern/psx.hpp"

namespace
{
    constexpr GsRVIEW2 STONE_VIEW = {.viewpointZ = -3320};

    bool isOpponentRowHidden()
    {
        return KAR_MATCH_STATE != CurlingMatchState::READY && KAR_MATCH_STATE < CurlingMatchState::RETURN_CAMERA &&
               (KAR_HINT_STATE != 0 || KAR_MATCH_STATE < CurlingMatchState::SELECT_STONE);
    }
} // namespace

[[gnu::optimize("Os")]]
void KAR_renderScene(int32_t instanceId)
{
    libgs_GsClearOt(0, 10, &KAR_ORDERING_TABLES[ACTIVE_FRAMEBUFFER]);
    for (int32_t player = 0; player < 3; player++) {
        for (int32_t index = 0; index < 5; index++) {
            if (player == 1 && isOpponentRowHidden()) continue;
            if (KAR_MATCH_STATE < CurlingMatchState::SETUP_DEMO_SHOT && KAR_ANIMATION_TIMER < (index + 1) * 60)
                continue;

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
                    stone.rotation.z        = 0;
                    stone.rotation.y        = 0;
                    stone.rotation.x        = 0;
                }
            }
            libgte_RotMatrix(&stone.rotation, &stone.coordinate.coord);
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
