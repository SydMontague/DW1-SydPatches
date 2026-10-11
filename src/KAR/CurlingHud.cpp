#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../Helper.hpp"
#include "../Math.hpp"
#include "../UIElements.hpp"
#include "../extern/dtl/array.hpp"
#include "../extern/dw1.hpp"
#include "../extern/libgpu.hpp"
#include "../extern/libgs.hpp"
#include "../extern/libgte.hpp"

namespace
{
    uint16_t arrowGap = 0;
    struct CurlingSprite
    {
        int16_t x;
        int16_t y;
        int16_t clutX;
        int16_t clutY;
        int16_t u;
        int16_t v;
        int16_t width;
        int16_t height;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t reserved;
    };

    void renderSprite(const CurlingSprite* sprite)
    {
        auto* primitive = static_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyFT4(primitive);
        primitive->tpage = getTPage(0, 0, 640, 0);
        primitive->r0    = sprite->red;
        primitive->g0    = sprite->green;
        primitive->b0    = sprite->blue;
        primitive->clut  = getClut(sprite->clutX, sprite->clutY);
        setUVDataPolyFT4(primitive, sprite->u, sprite->v, sprite->width, sprite->height);
        setPosDataPolyFT4(primitive, sprite->x, sprite->y, sprite->width, sprite->height);
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 10, primitive);
        libgs_GsSetWorkBase(primitive + 1);
    }

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

    void rotatePoint(SVector* point, int32_t angle)
    {
        const int32_t x = point->x;
        const int32_t z = point->z;

        // Each product truncates toward zero separately before the signed-16 narrowing.
        point->x = static_cast<int16_t>(x * libgte_rcos(angle) / 4096 - z * libgte_rsin(angle) / 4096);
        point->z = static_cast<int16_t>(x * libgte_rsin(angle) / 4096 + z * libgte_rcos(angle) / 4096);
    }

} // namespace

void KAR_resetHud()
{
    arrowGap          = 0;
    KAR_READY_COUNTER = 0;
}

[[gnu::optimize("Os")]]
void KAR_renderAimArrow()
{
    const int8_t entity = KAR_CURRENT_PLAYER != 0 ? static_cast<int8_t>(KAR_OPPONENT_ENTITY) : 0;
    if (KAR_MATCH_STATE < CurlingMatchState::AIM || KAR_MATCH_STATE >= CurlingMatchState::SELECT_POWER) return;

    auto* orderingTable = ACTIVE_ORDERING_TABLE->origin;
    if (KAR_ANIMATION_TIMER-- != 0) KAR_ANIMATION_TIMER += 191;
    if (arrowGap++ >= 40) arrowGap = 0;

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

        auto* primitive = static_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyFT4(primitive);
        primitive->r0 = static_cast<uint8_t>(shade + 64);
        primitive->g0 = static_cast<uint8_t>(shade + 64);
        primitive->b0 = static_cast<uint8_t>(shade + 64);

        const auto projected = getMapPosition(position);
        // getMapPosition sign-extends depth; vanilla uses unsigned GTE depth divided by four.
        const int32_t depth = static_cast<uint16_t>(projected.depth) / 4;

        if (index < 14) {
            setUVDataPolyFT4(primitive, 24, 56, 16, -16);
            setPosDataPolyFT4(primitive, projected.screenX - 8, projected.screenY + 8, 16, -16);
        }
        else {
            const int32_t angle = -((libgte_ratan2(previous.z - position.z, previous.x - position.x) - 3072) % 4096);
            dtl::array<SVector, 4> points{{{-8, 0, 8, 0}, {8, 0, 8, 0}, {-8, 0, -8, 0}, {8, 0, -8, 0}}};
            for (auto& point : points)
                rotatePoint(&point, angle);
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
        primitive->tpage = getTPage(0, 0, 640, 0);
        if (index != arrowGap / 3) libgpu_AddPrim(orderingTable + depth, primitive);
        libgs_GsSetWorkBase(primitive + 1);
        previous = position;
    }
}

[[gnu::optimize("Os")]]
void KAR_renderStoneCursor()
{
    if (KAR_MATCH_STATE == CurlingMatchState::SELECT_STONE) {
        auto sprite = CURSOR_SPRITE;
        sprite.x    = KAR_CURRENT_PLAYER != 0 ? 100 : -120;
        sprite.y    = static_cast<int16_t>(KAR_SELECTED_STONE * 30 - 45);
        renderSprite(&sprite);
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
        renderSprite(&sprite);

        sprite.u = SCORE_GLYPHS[tens].u;
        sprite.v = SCORE_GLYPHS[tens].v;
        sprite.x = x - 16;
        sprite.y = -62;
        renderSprite(&sprite);
    }
}

[[gnu::optimize("Os")]]
void KAR_renderPowerMeter()
{
    if (KAR_MATCH_STATE >= CurlingMatchState::SELECT_POWER && KAR_MATCH_STATE < CurlingMatchState::WAIT_FOR_RELEASE) {
        auto sprites     = POWER_SPRITES;
        sprites[1].x     = KAR_CURRENT_PLAYER != 0 ? 50 : -50;
        sprites[1].clutX = KAR_CURRENT_PLAYER != 0 ? 128 : 112;
        for (int32_t bar = 10; bar > 0; bar--) {
            if (KAR_THROW_POWER >= bar * 250) {
                sprites[0].x     = static_cast<int16_t>(sprites[1].x + bar * 3 - 3);
                sprites[0].clutX = bar == 10 ? 48 : 112;
                renderSprite(&sprites[0]);
            }
        }
        renderSprite(&sprites[1]);
    }
}

[[gnu::optimize("Os")]]
void KAR_renderReadyPrompt()
{
    if (KAR_MATCH_STATE == CurlingMatchState::READY) {
        if (KAR_READY_COUNTER++ >= 49) KAR_READY_COUNTER = 48;
        if (KAR_READY_COUNTER & 8) renderSprite(&READY_SPRITE);
    }
    else
        KAR_READY_COUNTER = 0;
}

void KAR_renderNamePlates()
{
    for (int32_t index = 0; index < 4; index++)
        renderSprite(index == 2 && KAR_OPPONENT_ENTITY == 3 ? &ALTERNATE_NAMEPLATE : &NAMEPLATE_SPRITES[index]);
}
