#include "../Files.hpp"
#include "../GameObjects.hpp"
#include "../extern/KAR.hpp"
#include "../extern/dw1.hpp"
#include "../extern/libc.hpp"
#include "../extern/libgs.hpp"
#include "Curling.hpp"
#include "CurlingTypes.hpp"

dtl::array<CurlingStoneRow, 3> KAR_STONE_ROWS{};
dtl::array<uint32_t, KAR_MODEL_BUFFER_WORDS> KAR_MODEL_BUFFER{};
int16_t KAR_SELECTED_STONE        = 0;
int8_t KAR_CURRENT_PLAYER         = 0;
CurlingMatchState KAR_MATCH_STATE = CurlingMatchState::INITIALIZE;
uint8_t KAR_OPPONENT_ENTITY       = 0;
int8_t KAR_HINT_STATE             = 0;
CurlingStone KAR_PREVIOUS_PLAYER_STONE{};
uint16_t KAR_ANIMATION_TIMER = 0;
uint8_t KAR_READY_COUNTER    = 0;
int16_t KAR_THROW_POWER      = 0;
int32_t KAR_AIM_ANGLE        = 0;
int16_t KAR_AIM_VELOCITY     = 0;
dtl::array<GsOT, 2> KAR_ORDERING_TABLES{};

namespace
{
    constexpr int32_t OPPONENT_PROGRESS_STAT = 0x7A;
    dtl::array<dtl::array<GsOT_TAG, 32>, 2> orderingTags{};

    constexpr dtl::array<int32_t, 3> stoneModelIds    = {{2, 1, 0}};
    constexpr dtl::array<int16_t, 4> stoneWeights     = {{30, 25, 35, 30}};
    constexpr dtl::array<int8_t, 10> randomStoneTypes = {{3, 2, 2, 1, 1, 1, 0, 0, 0, 0}};
    constexpr dtl::array<int32_t, 3> obstacleSpawnX   = {{705, 235, -235}};

    void initializeOrderingTables()
    {
        KAR_ORDERING_TABLES[0].length = 5;
        KAR_ORDERING_TABLES[0].origin = orderingTags[0].data();
        KAR_ORDERING_TABLES[1].length = 5;
        KAR_ORDERING_TABLES[1].origin = orderingTags[1].data();
    }
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
        // Consume one draw per row, even though its result is unused, to keep later RNG draws consistent.
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
            libgs_GsLinkObject4(KAR_MODEL_BUFFER.data() + 3, &stone.object, stone.type * 3 + stoneModelIds[player]);
            libgs_GsInitCoordinate2(nullptr, &stone.coordinate);
            stone.object.attribute = 0;
            stone.object.coord2    = &stone.coordinate;

            if (player < 2) {
                stone.position.y = index * 170 - 80;
                stone.position.x = player != 0 ? 700 : -700;
                stone.position.z = 0;
                stone.rotation.x = 150;
                stone.rotation.y = 0;
                stone.rotation.z = 0;
                stone.state      = -1;
            }
            else if (index < obstacles) {
                stone.position.y = 0;
                stone.position.x = obstacleSpawnX[index] - rand() % 319;
                stone.position.z = -(rand() % 1000 + 1000);
                stone.rotation.x = 0;
                stone.rotation.y = 0;
                stone.rotation.z = 0;
                stone.state      = 1;
                stone.speed      = 0;
            }
            else {
                stone.position.y = 0;
                stone.position.x = 0;
                stone.position.z = 0;
                stone.rotation.x = 0;
                stone.rotation.y = 0;
                stone.rotation.z = 0;
                stone.state      = -101;
                stone.speed      = 0;
            }
            stone.weight = stoneWeights[stone.type];
        }
    }
}

void KAR_start()
{
    KAR_resetHints();
    SOME_SCRIPT_SYNC_BIT = 0;
    readFile("\\ETCDAT\\KARRING.TMD", KAR_MODEL_BUFFER.data());
    libgs_GsMapModelingData(KAR_MODEL_BUFFER.data() + 1);
    initializeOrderingTables();
    KAR_resetHud();
    ENTITY_TABLE.getEntityById(0)->isOnMap = 1;
    KAR_MATCH_STATE                        = CurlingMatchState::INITIALIZE;
    KAR_setupMatch(0);
    addObject(ObjectID::KAR, KAR_OBJECT_INSTANCE, KAR_tickStones, KAR_renderScene);
    KAR_OPPONENT_ENTITY = readPStat(OPPONENT_PROGRESS_STAT) + 2;
}
