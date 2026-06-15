#include "../AtlasFont.hpp"
#include "../DigimonData.hpp"
#include "../Fade.hpp"
#include "../Font.hpp"
#include "../GameObjects.hpp"
#include "../Helper.hpp"
#include "../Input.hpp"
#include "../Model.hpp"
#include "../Sound.hpp"
#include "../extern/VS.hpp"
#include "../extern/dtl/types.hpp"

namespace
{
    constexpr auto NAME_GLYPH_SIZE = 16;

    struct VSIntroData
    {
        int16_t frameCount;
        int16_t renderedCharacters;
        int8_t color;
        uint8_t charCount;
        int16_t posX;
        int16_t posY;
        int8_t animFrame;
    };

    constexpr dtl::array<RGB8, 7> COLORS = {{
        {.red = 0x80, .green = 0x80, .blue = 0x80},
        {.red = 0xc8, .green = 0x64, .blue = 0x32},
        {.red = 0x1e, .green = 0xff, .blue = 0x1e},
        {.red = 0xd0, .green = 0x1e, .blue = 0x50},
        {.red = 0x1e, .green = 0x80, .blue = 0x80},
        {.red = 0xc8, .green = 0xc8, .blue = 0x00},
        {.red = 0x32, .green = 0xb4, .blue = 0xc8},
    }};

    constexpr dtl::array<uint8_t, 4> ANIMATION_SIZES = {
        NAME_GLYPH_SIZE * 2,
        NAME_GLYPH_SIZE + 12,
        NAME_GLYPH_SIZE + 6,
        NAME_GLYPH_SIZE,
    };

    constexpr dtl::array<uint8_t, 4> ANIMATION_OFFSET = {16, 7, 3, 0};
    constexpr dtl::array<const char*, 6> STATS_LABELS{"HP", "MP", "Off", "Def", "Speed", "Brain"};

    constexpr uint8_t convertCharacter(uint8_t character)
    {
        if (character >= 'a' && character <= 'z') return character - 'a';
        if (character >= 'A' && character <= 'Z') return character - 'A';
        return 0xFF;
    }

    VSIntroData introData;
    StatsGains stats;
    bool statsActive{false};

    void VS__tickIntroStats(int32_t id)
    {
        auto entity = reinterpret_cast<DigimonEntity*>(ENTITY_TABLE.getEntityById(id));

        stats.hp      = dtl::min(static_cast<int16_t>(stats.hp + 200), entity->stats.hp);
        stats.mp      = dtl::min(static_cast<int16_t>(stats.mp + 200), entity->stats.mp);
        stats.offense = dtl::min(static_cast<int16_t>(stats.offense + 20), entity->stats.off);
        stats.defense = dtl::min(static_cast<int16_t>(stats.defense + 20), entity->stats.def);
        stats.speed   = dtl::min(static_cast<int16_t>(stats.speed + 20), entity->stats.speed);
        stats.brains  = dtl::min(static_cast<int16_t>(stats.brains + 20), entity->stats.brain);
    }

    void VS__renderIntroStatBar(Stat stat, int32_t value)
    {
        auto prim = reinterpret_cast<POLY_F4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyF4(prim);
        prim->r0 = 80;
        prim->g0 = 200;
        prim->b0 = 80;
        prim->x0 = -50;
        prim->x1 = (value * 100 / getStatLimit(stat)) - 50;
        prim->x2 = -50;
        prim->x3 = (value * 100 / getStatLimit(stat)) - 50;
        prim->y0 = static_cast<int16_t>(stat) * 16 - 26;
        prim->y1 = static_cast<int16_t>(stat) * 16 - 26;
        prim->y2 = static_cast<int16_t>(stat) * 16 - 18;
        prim->y3 = static_cast<int16_t>(stat) * 16 - 18;
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 1, prim);
        libgs_GsSetWorkBase(prim + 1);
    }

    void VS__renderIntroStats(int32_t id)
    {
        auto entity = reinterpret_cast<DigimonEntity*>(ENTITY_TABLE.getEntityById(id));
        for (int32_t i = 0; i < 6; i++) {
            RenderSettings settings{
                .x        = -100,
                .y        = static_cast<int16_t>(-28 + i * 16),
                .baseClut = 1,
                .color    = TEXT_COLOR_WHITE,
                .width    = 48,
                .height   = 12,
            };
            Stat stat = static_cast<Stat>(i);
            getAtlasVanilla().renderSlow(STATS_LABELS[i], 0, settings);
            VS__renderIntroStatBar(stat, stats.get(stat));
        }

        bool hasAllStats =
            stats.get(Stat::HP) == entity->stats.hp && stats.get(Stat::MP) == entity->stats.mp &&
            stats.get(Stat::OFFENSE) == entity->stats.off && stats.get(Stat::DEFENSE) == entity->stats.def &&
            stats.get(Stat::SPEED) == entity->stats.speed && stats.get(Stat::BRAINS) == entity->stats.brain;
        if (!hasAllStats)
            playSound(0, 22);
        else {
            for (int32_t i = 0; i < 6; i++) {
                RenderSettings settings{
                    .x        = 52,
                    .y        = static_cast<int16_t>(-28 + i * 16),
                    .baseClut = 1,
                    .color    = TEXT_COLOR_WHITE,
                    .width    = 48,
                    .height   = 12,
                    .alignX   = AlignmentX::RIGHT,
                };
                Stat stat = static_cast<Stat>(i);
                getAtlasVanilla().renderSlow(format("%d", stats.get(stat)).data(), 3, settings);
            }
        }
    }

    void VS__addIntroStats(int32_t id)
    {
        if (statsActive) return;

        statsActive = true;
        stats       = {
            .hp      = -100,
            .mp      = -100,
            .offense = -10,
            .defense = -10,
            .speed   = -10,
            .brains  = -10,
        };
        addObject(ObjectID::VS_INTRO_STATS, id, VS__tickIntroStats, VS__renderIntroStats);
    }

    void VS__placePlayer1(int32_t stage)
    {
        auto startX = stage == 1 ? 1800 : 2500;
        setEntityPosition(1, startX, 0, 0);
        setEntityRotation(1, 0, 0x400, 0);
        startAnimation(ENTITY_TABLE.partner, 33);
    }

    void VS__placePlayer2(int32_t stage)
    {
        auto startX = stage == 1 ? -1800 : -2500;
        setEntityPosition(2, startX, 0, 0);
        setEntityRotation(2, 0, 0xc00, 0);
        startAnimation(ENTITY_TABLE.npc1, 33);
    }

    void VS__renderIntroNameChar(int16_t x, int16_t y, int32_t size, uint8_t character)
    {
        auto prim = reinterpret_cast<POLY_GT4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyGT4(prim);
        prim->tpage = 12;
        prim->clut  = getClut(0, 0x1e0);
        prim->r0    = COLORS[introData.color].red;
        prim->g0    = COLORS[introData.color].green;
        prim->b0    = COLORS[introData.color].blue;
        prim->r1    = COLORS[introData.color].red;
        prim->g1    = COLORS[introData.color].green;
        prim->b1    = COLORS[introData.color].blue;
        prim->r2    = COLORS[introData.color].red / 10;
        prim->g2    = COLORS[introData.color].green / 10;
        prim->b2    = COLORS[introData.color].blue / 10;
        prim->r3    = COLORS[introData.color].red / 10;
        prim->g3    = COLORS[introData.color].green / 10;
        prim->b3    = COLORS[introData.color].blue / 10;

        prim->u0 = (character % 8) * 32;
        prim->u1 = dtl::min(255, (character % 8) * 32 + 32);
        prim->u2 = (character % 8) * 32;
        prim->u3 = dtl::min(255, (character % 8) * 32 + 32);
        prim->v0 = (character / 8) * 32;
        prim->v1 = (character / 8) * 32;
        prim->v2 = dtl::min(255, (character / 8) * 32 + 32);
        prim->v3 = dtl::min(255, (character / 8) * 32 + 32);

        prim->x0 = x;
        prim->x1 = x + size;
        prim->x2 = x;
        prim->x3 = x + size;
        prim->y0 = y;
        prim->y1 = y;
        prim->y2 = y + size;
        prim->y3 = y + size;

        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin, prim);
        libgs_GsSetWorkBase(prim + 1);
    }

    void VS__tickIntroName(int32_t id)
    {
        introData.frameCount++;
        if (introData.charCount == introData.renderedCharacters && introData.animFrame == 3) {
            if (VS_INTRO_CAMERA_FRAME_COUNT == 0) {
                startAnimation(ENTITY_TABLE.getEntityById(id), 0x23);
                VS_INTRO_CAMERA_FRAME_COUNT = 20;
            }

            if (introData.posY < -0x47)
                VS__addIntroStats(id);
            else
                introData.posY -= 28;
        }
    }

    void VS__renderIntroName(int32_t id)
    {
        if (introData.frameCount % 4 == 0 && introData.renderedCharacters < introData.charCount) {
            introData.renderedCharacters++;
            introData.animFrame = 0;
        }
        if (introData.animFrame != 3) introData.animFrame++;

        auto type = ENTITY_TABLE.getEntityById(id)->type;
        auto name = getDigimonData(type)->name;
        for (int32_t i = 0; i < introData.renderedCharacters; i++) {
            auto character = convertCharacter(name[i]);

            if (character == 0xFF) continue;

            auto size = NAME_GLYPH_SIZE;
            auto y    = introData.posY;
            if (i == introData.renderedCharacters - 1) {
                size = ANIMATION_SIZES[introData.animFrame];
                y -= ANIMATION_OFFSET[introData.animFrame];
            }

            VS__renderIntroNameChar(introData.posX + i * NAME_GLYPH_SIZE, y, size, character);
        }
    }

    void VS__addIntroText(Entity* entity, int32_t id)
    {
        introData.frameCount         = 4;
        introData.renderedCharacters = 0;
        switch (getDigimonData(entity->type)->special[0]) {
            case Special::FIRE: introData.color = 3; break;
            case Special::COMBAT: introData.color = 1; break;
            case Special::AIR: introData.color = 6; break;
            case Special::NATURE: introData.color = 2; break;
            case Special::ICE: introData.color = 4; break;
            default:
            case Special::MACHINE: introData.color = 0; break;
            case Special::FILTH: introData.color = 5; break;
        }

        introData.charCount = strlen(getDigimonData(entity->type)->name);
        introData.posX      = -(NAME_GLYPH_SIZE / 2) * introData.charCount;
        introData.posY      = 68;
        addObject(ObjectID::VS_INTRO_NAME, id, VS__tickIntroName, VS__renderIntroName);
    }

    void VS__setPostIntroPosition(Entity* entity)
    {
        if (VS_INTRO_CAMERA_FRAME_COUNT == -1) return;

        entity->posData->location = INTRO_TARGET_POS;
        entity->locX              = INTRO_TARGET_POS.x << 15;
        entity->locY              = INTRO_TARGET_POS.y << 15;
        entity->locZ              = INTRO_TARGET_POS.z << 15;
        startAnimation(entity, 33);
        VS_INTRO_CAMERA_FRAME_COUNT = -1;
    }

    void VS__removeIntroText(int32_t id)
    {
        removeObject(ObjectID::VS_INTRO_NAME, id);
    }

    void VS__removeIntroStats(int32_t id)
    {
        if (!statsActive) return;

        statsActive = false;
        removeObject(ObjectID::VS_INTRO_STATS, id);
    }

} // namespace

extern "C"
{
    void VS__runIntro(int32_t stage)
    {
        VS__placePlayer1(stage);
        VS__placePlayer2(stage);
        {
            VS__setupIntroCamera(&PARTNER_ENTITY, stage == 1 ? 1300 : 2000, 0);
            VS__addIntroText(&PARTNER_ENTITY, 1);
            stopBGM();
            stopSound();
            playMusic(VS_MUSIC, 0);
            fadeFromBlack(5);
            for (int32_t i = 0; i < 6; i++)
                VS__tickFrame();

            do {
                VS__tickFrame();
                if (isKeyDownPolled(InputButtons::BUTTON_CROSS)) {
                    VS__setPostIntroPosition(&PARTNER_ENTITY);
                    break;
                }
            } while ((PARTNER_ENTITY.animFlag & 1) != 0);

            VS__removeIntroText(1);
            VS__removeIntroStats(1);
            removeObject(ObjectID::VS_INTRO_CAMERA_REEL, 0);
        }
        {
            VS__setupIntroCamera(&NPC_ENTITIES[0], stage == 1 ? -1300 : -2000, 1);
            VS__addIntroText(&NPC_ENTITIES[0], 2);
            stopBGM();
            stopSound();
            playMusic(VS_MUSIC, 1);

            do {
                VS__tickFrame();
                if (isKeyDownPolledP2(InputButtons::BUTTON_CROSS)) {
                    VS__setPostIntroPosition(&NPC_ENTITIES[0]);
                    break;
                }
            } while ((NPC_ENTITIES[0].animFlag & 1) != 0);

            VS__removeIntroText(2);
            VS__removeIntroStats(2);
            removeObject(ObjectID::VS_INTRO_CAMERA_REEL, 0);
        }
        stopBGM();
        stopSound();
    }
}
