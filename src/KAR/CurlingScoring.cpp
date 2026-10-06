#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../Camera.hpp"
#include "../Map.hpp"
#include "../Sound.hpp"
#include "../extern/dw1.hpp"

// These values survive leaving and re-entering KAR; overlay reload must not reset them.
extern int8_t KAR_TALLY_SKIP_REQUEST;
extern int8_t KAR_TALLY_PHASE;
extern int32_t KAR_TALLY_BLINK_TIMER;

constexpr dtl::array<CurlingRingZone, 4> KAR_RING_ZONES = {
    {{0, 1718, 145}, {0, 1718, 375}, {-400, 2318, 205}, {400, 2318, 205}}};

namespace
{
    constexpr dtl::array<int8_t, 4> tallyValues = {{2, 1, -2, 2}};
    dtl::array<CurlingStone*, 11> thrownStones{};
    int8_t previousStone   = -1;
    int8_t tallyStoneIndex = 0;
    int32_t blinkCount     = 0;

    [[gnu::optimize("Os")]]
    void registerThrownStone()
    {
        tallyStoneIndex               = static_cast<int8_t>(KAR_STONE_ROWS[0].thrown + KAR_STONE_ROWS[1].thrown);
        thrownStones[tallyStoneIndex] = &KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[KAR_SELECTED_STONE];
        previousStone                 = -1;
    }

} // namespace

const CurlingStone& KAR_getTallyStone()
{
    return *thrownStones[tallyStoneIndex];
}

extern "C"
{
    void KAR_classifyStoneRings()
    {
        for (int32_t player = 0; player < 2; player++) {
            KAR_STONE_ROWS[player].score = 0;
            for (auto& stone : KAR_STONE_ROWS[player].stones) {
                stone.previousRing = stone.ring;
                stone.ring         = 0;
                if (stone.state <= 0 || stone.position.z >= -1380) continue;

                // The first ring query supersedes vanilla's discarded distance and its GTE scratch values.
                for (int32_t ring = 0; ring < 4; ring++) {
                    const auto& zone = KAR_RING_ZONES[ring];
                    if (KAR_distance(stone.position.x + zone.dx, stone.position.z + zone.dz) <= zone.radius) {
                        stone.ring = static_cast<int8_t>((ring + 1) | (player != 0 ? 0x10 : 0));
                        break;
                    }
                }
            }
        }
        registerThrownStone();
    }

    [[gnu::optimize("Os")]]
    int32_t KAR_tickScoreTally()
    {
        if ((POLLED_INPUT & InputButtons::BUTTON_CROSS) && !(POLLED_INPUT_PREVIOUS & InputButtons::BUTTON_CROSS))
            KAR_TALLY_SKIP_REQUEST = 1;

        if (KAR_TALLY_SKIP_REQUEST == 1 && (KAR_TALLY_PHASE == 1 || KAR_TALLY_PHASE == 2)) {
            tallyStoneIndex = static_cast<int8_t>(KAR_STONE_ROWS[0].thrown + KAR_STONE_ROWS[1].thrown);
            tallyStoneIndex--;
            KAR_STONE_ROWS[0].score = 0;
            KAR_STONE_ROWS[1].score = 0;
            while (tallyStoneIndex >= 0) {
                const auto& stone    = *thrownStones[tallyStoneIndex];
                const int32_t player = (stone.ring & 0x10) != 0 ? 1 : 0;
                const int32_t ring   = stone.ring & 0x0F;
                if (stone.state > 0 && ring != 0) {
                    auto& score = KAR_STONE_ROWS[player].score;
                    score       = static_cast<int8_t>(score + tallyValues[ring - 1]);
                }
                tallyStoneIndex--;
            }
            for (auto& row : KAR_STONE_ROWS) {
                for (auto& stone : row.stones) {
                    if (stone.state >= 100) stone.state -= 100;
                }
            }
            blinkCount             = 0;
            KAR_TALLY_BLINK_TIMER  = 0;
            KAR_TALLY_PHASE        = 0;
            KAR_TALLY_SKIP_REQUEST = 0;
            return 1;
        }

        if (previousStone != tallyStoneIndex) {
            while (tallyStoneIndex >= 0) {
                const auto& stone = *thrownStones[tallyStoneIndex];
                if ((stone.ring & 0x0F) != 0) {
                    previousStone   = tallyStoneIndex;
                    KAR_TALLY_PHASE = 0;
                    break;
                }
                tallyStoneIndex--;
            }
            if (tallyStoneIndex < 0) {
                KAR_TALLY_SKIP_REQUEST = 0;
                return 1;
            }
        }

        // Independent stages preserve vanilla's camera/score/blink fallthrough within one tick.
        if (KAR_TALLY_PHASE == 0) {
            const auto& stone = *thrownStones[tallyStoneIndex];
            if (stone.ring != stone.previousRing) {
                const bool done =
                    tickCameraMoveTo(static_cast<int16_t>(stone.position.x), static_cast<int16_t>(stone.position.z), 5);
                const int32_t cameraRemainder = CAMERA_Y % 128;
                if (cameraRemainder == 0 || cameraRemainder >= 106) {
                    // Vanilla loads signed bytes here; the shared map declarations are unsigned.
                    const int32_t tileOffset = static_cast<int8_t>(MAP_TILE_X) +
                                               static_cast<int8_t>(MAP_TILE_Y) * static_cast<int8_t>(MAP_WIDTH);
                    uploadMapTileImages(MAP_TILE_DATA.data(), tileOffset);
                }
                if (!done) return 0;
            }
            KAR_TALLY_PHASE = 1;
        }

        if (KAR_TALLY_PHASE == 1) {
            const auto& stone    = *thrownStones[tallyStoneIndex];
            const int32_t player = (stone.ring & 0x10) != 0 ? 1 : 0;
            const int32_t ring   = stone.ring & 0x0F;
            auto& score          = KAR_STONE_ROWS[player].score;
            score                = static_cast<int8_t>(score + tallyValues[ring - 1]);
            KAR_TALLY_PHASE      = 2;
            if (stone.ring != stone.previousRing && ring != 0) playSound2(8, ring == 3 ? 6 : 5);
        }

        if (KAR_TALLY_PHASE == 2 && tallyStoneIndex >= 0) {
            auto& stone = *thrownStones[tallyStoneIndex];
            if (stone.ring != stone.previousRing && (stone.ring & 0x0F) != 0) {
                if (KAR_TALLY_BLINK_TIMER++ >= 11) {
                    if (stone.state < 100)
                        stone.state += 100;
                    else
                        stone.state -= 100;
                    KAR_TALLY_BLINK_TIMER -= 10;
                    if (blinkCount++ >= 7) {
                        KAR_TALLY_PHASE = 4;
                        blinkCount      = 0;
                        tallyStoneIndex--;
                    }
                }
            }
            else {
                tallyStoneIndex--;
            }
            if (tallyStoneIndex < 0) return 1;
        }
        return 0;
    }
}
