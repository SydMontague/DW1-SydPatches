#include "../Files.hpp"
#include "../GameObjects.hpp"
#include "../extern/KAR.hpp"
#include "../extern/dw1.hpp"
#include "../extern/libc.hpp"
#include "../extern/libgs.hpp"
#include "Curling.hpp"
#include "CurlingTypes.hpp"

// The model file and its sector-sized buffer remain in the existing overlay asset allocation.
extern const char KAR_MODEL_PATH[];
extern uint32_t KAR_MODEL_BUFFER[];
extern int16_t KAR_THROW_POWER;

namespace
{
    constexpr dtl::array<int32_t, 3> stoneModelIds    = {{2, 1, 0}};
    constexpr dtl::array<int16_t, 4> stoneWeights     = {{30, 25, 35, 30}};
    constexpr dtl::array<int8_t, 10> randomStoneTypes = {{3, 2, 2, 1, 1, 1, 0, 0, 0, 0}};
    constexpr dtl::array<int32_t, 3> obstacleSpawnX   = {{705, 235, -235}};
} // namespace

[[gnu::optimize("Os")]]
void KAR_setupMatch(int32_t mode)
{
    const int32_t obstacles = rand() % 3;
    KAR_SELECTED_STONE      = 0;
    KAR_CURRENT_PLAYER      = 0;
    KAR_THROW_POWER         = 0;
    KAR_resetScoring();

    for (int32_t player = 0; player < 3; player++) {
        auto& row = KAR_STONE_ROWS[player];
        // Vanilla consumes a draw per row, even though its result is unused.
        rand();
        row.score        = 0;
        row.thrown       = 0;
        row.shotPower    = 0;
        row.plannedStone = 0;

        for (int32_t index = 0; index < 5; index++) {
            auto& stone              = row.stones[index];
            const int32_t randomType = rand() % 10;
            stone.speed              = 0;
            stone.angle              = 0;
            stone.ring               = 0;
            stone.previousRing       = 0;
            if (mode == 0)
                stone.type = randomStoneTypes[randomType];
            else if (index < 4)
                stone.type = static_cast<int8_t>(index);
            else
                stone.type = 3;
            libgs_GsLinkObject4(KAR_MODEL_DATA + 3, &stone.object, stone.type * 3 + stoneModelIds[player]);
            libgs_GsInitCoordinate2(nullptr, &stone.coordinate);
            stone.object.attribute = 0;
            stone.object.coord2    = &stone.coordinate;

            if (player < 2) {
                stone.position.y     = index * 170 - 80;
                stone.position.x     = player != 0 ? 700 : -700;
                stone.position.z     = 0;
                stone.rotationX      = 150;
                stone.selectionPhase = 0;
                stone.rotationZ      = 0;
                stone.state          = -1;
            }
            else if (index < obstacles) {
                stone.position.y     = 0;
                stone.position.x     = obstacleSpawnX[index] - rand() % 319;
                stone.position.z     = -(rand() % 1000 + 1000);
                stone.rotationX      = 0;
                stone.selectionPhase = 0;
                stone.rotationZ      = 0;
                stone.state          = 1;
                stone.speed          = 0;
            }
            else {
                stone.position.y     = 0;
                stone.position.x     = 0;
                stone.position.z     = 0;
                stone.rotationX      = 0;
                stone.selectionPhase = 0;
                stone.rotationZ      = 0;
                stone.state          = -101;
                stone.speed          = 0;
            }
            stone.weight = stoneWeights[stone.type];
        }
    }
}

extern "C" void KAR_start()
{
    KAR_MODEL_DATA       = KAR_MODEL_BUFFER;
    SOME_SCRIPT_SYNC_BIT = 0;
    readFile(KAR_MODEL_PATH, KAR_MODEL_DATA);
    libgs_GsMapModelingData(KAR_MODEL_DATA + 1);
    KAR_initializeOrderingTables();
    ENTITY_TABLE.getEntityById(0)->isOnMap = 1;
    KAR_MATCH_STATE                        = 0;
    KAR_setupMatch(0);
    // Vanilla shares object ID 5000 with the main menu, using instance 15 for curling.
    addObject(static_cast<ObjectID>(5000), 15, KAR_tickStones, KAR_renderScene);
    KAR_OPPONENT_ENTITY = readPStat(0x7A);
    KAR_OPPONENT_ENTITY = static_cast<uint8_t>(KAR_OPPONENT_ENTITY + 2);
}
