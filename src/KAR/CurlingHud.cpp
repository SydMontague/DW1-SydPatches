#include "Curling.hpp"
#include "CurlingRenderTypes.hpp"
#include "CurlingTypes.hpp"

#include "../Helper.hpp"
#include "../Math.hpp"
#include "../UIElements.hpp"
#include "../extern/dtl/array.hpp"
#include "../extern/dw1.hpp"
#include "../extern/libgpu.hpp"
#include "../extern/libgs.hpp"
#include "../extern/libgte.hpp"

extern uint16_t KAR_ANIMATION_TIMER;
extern uint16_t KAR_ARROW_GAP;
extern uint8_t KAR_READY_COUNTER;
extern int16_t KAR_THROW_POWER;

namespace
{
    struct ScoreGlyph
    {
        int16_t u;
        int16_t v;
    };

    alignas(4) constexpr CurlingSprite CURSOR_SPRITE = {
        .x        = -150,
        .y        = 0,
        .clutX    = 128,
        .clutY    = 486,
        .u        = 0,
        .v        = 40,
        .width    = 24,
        .height   = 24,
        .red      = 128,
        .green    = 128,
        .blue     = 128,
        .reserved = 0,
    };

    alignas(4) constexpr CurlingSprite SCORE_SPRITE = {
        .x        = -150,
        .y        = 0,
        .clutX    = 16,
        .clutY    = 486,
        .u        = 0,
        .v        = 40,
        .width    = 16,
        .height   = 16,
        .red      = 128,
        .green    = 128,
        .blue     = 128,
        .reserved = 0,
    };

    constexpr dtl::array<ScoreGlyph, 11> SCORE_GLYPHS = {
        {{64, 24}, {80, 24}, {96, 24}, {64, 40}, {80, 40}, {96, 40}, {72, 56}, {88, 56}, {72, 72}, {88, 72}, {0, 64}}};

    alignas(4) constexpr dtl::array<CurlingSprite, 2> POWER_SPRITES = {{
        {
            .x        = -50,
            .y        = -50,
            .clutX    = 128,
            .clutY    = 486,
            .u        = 48,
            .v        = 24,
            .width    = 16,
            .height   = 16,
            .red      = 128,
            .green    = 128,
            .blue     = 128,
            .reserved = 0,
        },
        {
            .x        = -50,
            .y        = -50,
            .clutX    = 112,
            .clutY    = 486,
            .u        = 0,
            .v        = 24,
            .width    = 48,
            .height   = 16,
            .red      = 128,
            .green    = 128,
            .blue     = 128,
            .reserved = 0,
        },
    }};

    alignas(4) constexpr dtl::array<CurlingSprite, 4> NAMEPLATE_SPRITES = {{
        {
            .x        = -140,
            .y        = -95,
            .clutX    = 144,
            .clutY    = 486,
            .u        = 112,
            .v        = 24,
            .width    = 32,
            .height   = 32,
            .red      = 128,
            .green    = 128,
            .blue     = 128,
            .reserved = 0,
        },
        {
            .x        = -130,
            .y        = -70,
            .clutX    = 112,
            .clutY    = 486,
            .u        = 24,
            .v        = 56,
            .width    = 48,
            .height   = 32,
            .red      = 128,
            .green    = 128,
            .blue     = 128,
            .reserved = 0,
        },
        {
            .x        = 100,
            .y        = -95,
            .clutX    = 160,
            .clutY    = 486,
            .u        = 144,
            .v        = 24,
            .width    = 32,
            .height   = 32,
            .red      = 128,
            .green    = 128,
            .blue     = 128,
            .reserved = 0,
        },
        {
            .x        = 80,
            .y        = -70,
            .clutX    = 128,
            .clutY    = 486,
            .u        = 24,
            .v        = 56,
            .width    = 48,
            .height   = 32,
            .red      = 128,
            .green    = 128,
            .blue     = 128,
            .reserved = 0,
        },
    }};

    alignas(4) constexpr CurlingSprite ALTERNATE_NAMEPLATE = {
        .x        = 100,
        .y        = -95,
        .clutX    = 192,
        .clutY    = 486,
        .u        = 176,
        .v        = 24,
        .width    = 32,
        .height   = 32,
        .red      = 128,
        .green    = 128,
        .blue     = 128,
        .reserved = 0,
    };

    alignas(4) constexpr CurlingSprite READY_SPRITE = {
        .x        = -52,
        .y        = 0,
        .clutX    = 176,
        .clutY    = 486,
        .u        = 104,
        .v        = 62,
        .width    = 151,
        .height   = 10,
        .red      = 128,
        .green    = 128,
        .blue     = 128,
        .reserved = 0,
    };

} // namespace

extern "C"
{
    [[gnu::optimize("Os")]]
    void KAR_renderAimArrow()
    {
        const int8_t entity = KAR_CURRENT_PLAYER != 0 ? static_cast<int8_t>(KAR_OPPONENT_ENTITY) : 0;
        if (KAR_MATCH_STATE < 6 || KAR_MATCH_STATE >= 8) return;

        auto* orderingTable = ACTIVE_ORDERING_TABLE->origin;
        if (KAR_ANIMATION_TIMER-- != 0) KAR_ANIMATION_TIMER += 191;
        if (KAR_ARROW_GAP++ >= 40) KAR_ARROW_GAP = 0;

        SVector previous{};
        for (int32_t index = 0; index < 15; index++) {
            Matrix rotation;
            SVector vector{0, 0, static_cast<int16_t>((index + 1) * -200), 0};
            SVector position{};
            libgte_RotMatrix(&ENTITY_TABLE.getEntityById(entity)->posData->rotation, &rotation);
            libgte_ApplyMatrixSV(&rotation, &vector, &position);
            const auto& stone = KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[KAR_SELECTED_STONE];
            position.x        = static_cast<int16_t>(position.x + stone.position.x);
            position.z        = static_cast<int16_t>(position.z + stone.position.z);

            uint32_t shade = (KAR_ANIMATION_TIMER + 191 * index / 15) & 0xFF;
            if (shade >= 191) shade = (shade - 191) & 0xFF;
            while (position.x < -750 || position.x > 750) {
                if (position.x < -750) {
                    const int16_t distance = static_cast<int16_t>(position.x + 750);
                    position.x             = static_cast<int16_t>(position.x - static_cast<int16_t>(distance * 2));
                }
                if (position.x > 750) {
                    const int16_t distance = static_cast<int16_t>(position.x - 750);
                    position.x             = static_cast<int16_t>(position.x - static_cast<int16_t>(distance * 2));
                }
            }

            auto* primitive   = static_cast<POLY_FT4*>(libgs_GsGetWorkBase());
            primitive->tag[3] = 9;
            primitive->code   = 0x2C;
            primitive->r0     = static_cast<uint8_t>(shade + 64);
            primitive->g0     = static_cast<uint8_t>(shade + 64);
            primitive->b0     = static_cast<uint8_t>(shade + 64);

            const auto projected = getMapPosition(position);
            // getMapPosition sign-extends depth; vanilla uses unsigned GTE depth divided by four.
            const int32_t depth = static_cast<uint16_t>(projected.depth) / 4;

            if (index < 14) {
                setUVDataPolyFT4(primitive, 24, 56, 16, -16);
                setPosDataPolyFT4(primitive, projected.screenX - 8, projected.screenY + 8, 16, -16);
            }
            else {
                const int32_t angle =
                    -((libgte_ratan2(previous.z - position.z, previous.x - position.x) - 3072) % 4096);
                dtl::array<SVector, 4> points{{{-8, 0, 8, 0}, {8, 0, 8, 0}, {-8, 0, -8, 0}, {8, 0, -8, 0}}};
                for (auto& point : points)
                    KAR_rotatePoint(&point, angle);
                for (auto& point : points) {
                    point.x = static_cast<int16_t>(point.x + projected.screenX);
                    point.z = static_cast<int16_t>(point.z + projected.screenY);
                }
                const int16_t width = angle == 2048 ? 16 : 15;
                setUVDataPolyFT4(primitive, 40, 40, width, width);
                primitive->x0 = points[0].x;
                primitive->y0 = points[0].z;
                primitive->x1 = points[1].x;
                primitive->y1 = points[1].z;
                primitive->x2 = points[2].x;
                primitive->y2 = points[2].z;
                primitive->x3 = points[3].x;
                primitive->y3 = points[3].z;
            }
            primitive->clut  = getClut(128, 486);
            primitive->tpage = 10;
            if (index != KAR_ARROW_GAP / 3) libgpu_AddPrim(orderingTable + depth, primitive);
            libgs_GsSetWorkBase(primitive + 1);
            previous = position;
        }
    }

    [[gnu::optimize("Os")]]
    void KAR_renderStoneCursor()
    {
        auto sprite = CURSOR_SPRITE;
        if (KAR_MATCH_STATE == 5) {
            sprite.x = KAR_CURRENT_PLAYER != 0 ? 100 : -120;
            sprite.y = static_cast<int16_t>(KAR_SELECTED_STONE * 30 - 45);
            KAR_renderSprite(&sprite);
        }
    }

    void KAR_renderScores()
    {
        auto sprite = SCORE_SPRITE;
        for (int32_t player = 0; player < 2; player++) {
            const int8_t score = KAR_STONE_ROWS[player].score;
            int16_t units      = score % 10;
            const int16_t tens = score < 0 ? 10 : score / 10;
            const int16_t x    = player != 0 ? 104 : -105;
            if (units < 0) units = -units;
            sprite.u = SCORE_GLYPHS[units].u;
            sprite.v = SCORE_GLYPHS[units].v;
            sprite.x = x;
            sprite.y = -62;
            KAR_renderSprite(&sprite);

            sprite.u = SCORE_GLYPHS[tens].u;
            sprite.v = SCORE_GLYPHS[tens].v;
            sprite.x = x - 16;
            sprite.y = -62;
            KAR_renderSprite(&sprite);
        }
    }

    [[gnu::optimize("Os")]]
    void KAR_renderPowerMeter()
    {
        auto sprites = POWER_SPRITES;
        if (KAR_MATCH_STATE >= 8 && KAR_MATCH_STATE < 10) {
            sprites[1].x     = KAR_CURRENT_PLAYER != 0 ? 50 : -50;
            sprites[1].clutX = KAR_CURRENT_PLAYER != 0 ? 128 : 112;
            for (int32_t bar = 10; bar > 0; bar--) {
                if (KAR_THROW_POWER >= bar * 250) {
                    sprites[0].x     = static_cast<int16_t>(sprites[1].x + bar * 3 - 3);
                    sprites[0].clutX = bar == 10 ? 48 : 112;
                    KAR_renderSprite(&sprites[0]);
                }
            }
            KAR_renderSprite(&sprites[1]);
        }
    }

    [[gnu::optimize("Os")]]
    void KAR_renderReadyPrompt()
    {
        if (KAR_MATCH_STATE == 3) {
            if (KAR_READY_COUNTER++ >= 49) KAR_READY_COUNTER = 48;
            if (KAR_READY_COUNTER & 8) KAR_renderSprite(&READY_SPRITE);
        }
        else
            KAR_READY_COUNTER = 0;
    }

    void KAR_renderNamePlates()
    {
        for (int32_t index = 0; index < 4; index++)
            KAR_renderSprite(index == 2 && KAR_OPPONENT_ENTITY == 3 ? &ALTERNATE_NAMEPLATE : &NAMEPLATE_SPRITES[index]);
    }
}
