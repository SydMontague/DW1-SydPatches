#include "Font.hpp"
#include "Helper.hpp"
#include "StatsViewData.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "constants.hpp"
#include "extern/dw1.hpp"
#include "extern/libc.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"

extern "C"
{
    IconSprite sprites[] = {
        // type
        {PROFILE_OFFSET_X + 263, PROFILE_OFFSET_Y + 16, 12, 12, 0, 128, 0, 5, getClut(0x60, 0x1e8 + 0)},
        // special 0
        {PROFILE_OFFSET_X + 224, PROFILE_OFFSET_Y + 31, 12, 12, 36, 128, 1, 5, getClut(0x60, 0x1e8 + 0)},
        // special 1
        {PROFILE_OFFSET_X + 240, PROFILE_OFFSET_Y + 31, 12, 12, 36, 128, 1, 5, getClut(0x60, 0x1e8 + 0)},
        // special 2
        {PROFILE_OFFSET_X + 256, PROFILE_OFFSET_Y + 31, 12, 12, 36, 128, 1, 5, getClut(0x60, 0x1e8 + 0)},
        // happiness
        {CONDITION_OFFSET_X, CONDITION_OFFSET_Y + 50, 11, 12, 0, 244, 0, 5, getClut(0x60, 0x1e8 + 10)},
        // discipline
        {CONDITION_OFFSET_X, CONDITION_OFFSET_Y + 64, 11, 12, 22, 244, 0, 5, getClut(0x60, 0x1e8 + 10)},
        // heart 1
        {CONDITION_OFFSET_X + 63, CONDITION_OFFSET_Y + 35, 12, 11, 232, 140, 0, 5, getClut(0x60, 0x1e8 + 6)},
        // heart 2
        {CONDITION_OFFSET_X + 80, CONDITION_OFFSET_Y + 35, 12, 11, 232, 140, 0, 5, getClut(0x60, 0x1e8 + 6)},
        // heart 3
        {CONDITION_OFFSET_X + 97, CONDITION_OFFSET_Y + 35, 12, 11, 232, 140, 0, 5, getClut(0x60, 0x1e8 + 6)},
        // sick
        {CONDITION_OFFSET_X + 14, CONDITION_OFFSET_Y + 12, 12, 12, 48, 140, 0, 5, getClut(0x60, 0x1e8 + 2)},
        // injury
        {CONDITION_OFFSET_X + 28, CONDITION_OFFSET_Y + 12, 12, 12, 60, 140, 0, 5, getClut(0x60, 0x1e8 + 0)},
        // hunger
        {CONDITION_OFFSET_X + 42, CONDITION_OFFSET_Y + 12, 12, 12, 72, 140, 0, 5, getClut(0x60, 0x1e8 + 0)},
        // sleep
        {CONDITION_OFFSET_X + 56, CONDITION_OFFSET_Y + 12, 12, 12, 84, 140, 0, 5, getClut(0x60, 0x1e8 + 0)},
        // unhappy
        {CONDITION_OFFSET_X + 70, CONDITION_OFFSET_Y + 12, 12, 12, 96, 140, 0, 5, getClut(0x60, 0x1e8 + 2)},
        // tired
        {CONDITION_OFFSET_X + 84, CONDITION_OFFSET_Y + 12, 12, 12, 108, 140, 0, 5, getClut(0x60, 0x1e8 + 1)},
        // poop
        {CONDITION_OFFSET_X + 98, CONDITION_OFFSET_Y + 12, 12, 12, 180, 128, 0, 5, getClut(0x60, 0x1e8 + 0)},
        // happiness 1
        {CONDITION_OFFSET_X + 62, CONDITION_OFFSET_Y + 53, 48, 5, 24, 240, 0, 24, getClut(0x70, 0x1f6)},
        // happiness 2
        {CONDITION_OFFSET_X + 62, CONDITION_OFFSET_Y + 53, 48, 5, 74, 240, 0, 24, getClut(0x70, 0x1f5)},
        // discipline 1
        {CONDITION_OFFSET_X + 62, CONDITION_OFFSET_Y + 67, 48, 5, 24, 240, 0, 24, getClut(0x70, 0x1f6)},
        // discipline 2
        {CONDITION_OFFSET_X + 62, CONDITION_OFFSET_Y + 67, 48, 5, 74, 240, 0, 24, getClut(0x70, 0x1f5)},

        {PROFILE_OFFSET_X, PROFILE_OFFSET_Y, 53, 11, 0, 203, 0, 5, getClut(0x60, 0x1e8 + 8)},
        {CONDITION_OFFSET_X, CONDITION_OFFSET_Y, 53, 11, 0, 203, 0, 5, getClut(0x60, 0x1e8 + 8)},
        {STATS_OFFSET_X, STATS_OFFSET_Y, 53, 11, 0, 203, 0, 5, getClut(0x60, 0x1e8 + 8)},
        {STATS_OFFSET_X, STATS_OFFSET_Y + 18, 11, 11, 147, 186, 0, 5, getClut(0x60, 0x1e8 + 11)},
        {STATS_OFFSET_X, STATS_OFFSET_Y + 33, 11, 11, 158, 186, 0, 5, getClut(0x60, 0x1e8 + 11)},
        {STATS_OFFSET_X, STATS_OFFSET_Y + 48, 11, 11, 169, 186, 0, 5, getClut(0x60, 0x1e8 + 11)},
        {STATS_OFFSET_X, STATS_OFFSET_Y + 63, 11, 11, 180, 186, 0, 5, getClut(0x60, 0x1e8 + 11)},
        {STATS_OFFSET_X, STATS_OFFSET_Y + 78, 11, 11, 191, 186, 0, 5, getClut(0x60, 0x1e8 + 11)},
        {STATS_OFFSET_X, STATS_OFFSET_Y + 93, 10, 11, 202, 186, 0, 5, getClut(0x60, 0x1e8 + 11)},
        {CONDITION_OFFSET_X + 2, CONDITION_OFFSET_Y + 78, 8, 11, 54, 203, 0, 5, getClut(0x60, 0x1e8 + 1)},
        {CONDITION_OFFSET_X + 1, CONDITION_OFFSET_Y + 92, 9, 12, 44, 244, 0, 5, getClut(0x60, 0x1e8 + 10)},
    };

    Line lines[] = {
        {.x1 = 18, .y1 = -15, .x2 = 18, .y2 = 98, .clut = 0},
        {.x1 = 19, .y1 = -16, .x2 = 19, .y2 = 98, .clut = 1},
        {.x1 = 20, .y1 = -15, .x2 = 20, .y2 = 98, .clut = 0},
        {.x1 = -146, .y1 = -18, .x2 = 146, .y2 = -18, .clut = 0},
        {.x1 = -147, .y1 = -17, .x2 = 147, .y2 = -17, .clut = 1},
        {.x1 = -146, .y1 = -16, .x2 = 146, .y2 = -16, .clut = 0},
    };

    TextSprite badges[] = {
        {
            .font       = &myFont7px,
            .string     = "PROFILE",
            .uvX        = DRAW_OFFSET_X + 16,
            .uvY        = DRAW_OFFSET_Y + 68,
            .uvWidth    = 0,
            .uvHeight   = 0,
            .posX       = PROFILE_OFFSET_X + 1,
            .posY       = PROFILE_OFFSET_Y + 1,
            .boxWidth   = 51,
            .boxHeight  = 10,
            .alignmentX = AlignmentX::CENTER,
            .alignmentY = AlignmentY::CENTER,
            .color      = -1,
            .layer      = 5,
            .hasShadow  = 2,
        },
        {
            .font       = &myFont7px,
            .string     = "CONDITION",
            .uvX        = DRAW_OFFSET_X + 32,
            .uvY        = DRAW_OFFSET_Y + 68,
            .uvWidth    = 0,
            .uvHeight   = 0,
            .posX       = CONDITION_OFFSET_X + 1,
            .posY       = CONDITION_OFFSET_Y + 1,
            .boxWidth   = 51,
            .boxHeight  = 10,
            .alignmentX = AlignmentX::CENTER,
            .alignmentY = AlignmentY::CENTER,
            .color      = -1,
            .layer      = 5,
            .hasShadow  = 2,
        },
        {
            .font       = &myFont7px,
            .string     = "PARAMETER",
            .uvX        = DRAW_OFFSET_X + 48,
            .uvY        = DRAW_OFFSET_Y + 68,
            .uvWidth    = 0,
            .uvHeight   = 0,
            .posX       = STATS_OFFSET_X + 1,
            .posY       = STATS_OFFSET_Y + 1,
            .boxWidth   = 51,
            .boxHeight  = 10,
            .alignmentX = AlignmentX::CENTER,
            .alignmentY = AlignmentY::CENTER,
            .color      = -1,
            .layer      = 5,
            .hasShadow  = 2,
        },
    };

    Inset insets[] = {
        //{PROFILE_OFFSET_X + 190, PROFILE_OFFSET_Y + 14, 86, 14}, // Active/Type
        //{PROFILE_OFFSET_X + 190, PROFILE_OFFSET_Y + 30, 86, 14}, // Special

        {PROFILE_OFFSET_X + 32, PROFILE_OFFSET_Y + 14, 155, 14}, // Name
        {PROFILE_OFFSET_X + 32, PROFILE_OFFSET_Y + 30, 155, 14}, // Digimon
        {PROFILE_OFFSET_X + 32, PROFILE_OFFSET_Y + 46, 28, 14},  // Age
        {PROFILE_OFFSET_X + 104, PROFILE_OFFSET_Y + 46, 28, 14}, // Weight
        {PROFILE_OFFSET_X + 176, PROFILE_OFFSET_Y + 46, 28, 14}, // Battle
        {PROFILE_OFFSET_X + 248, PROFILE_OFFSET_Y + 46, 28, 14}, // Care

        {STATS_OFFSET_X + 32, STATS_OFFSET_Y + 22, 80, 5}, // HP
        {STATS_OFFSET_X + 32, STATS_OFFSET_Y + 37, 80, 5}, // MP
        {STATS_OFFSET_X + 32, STATS_OFFSET_Y + 52, 45, 5}, // Off
        {STATS_OFFSET_X + 32, STATS_OFFSET_Y + 67, 45, 5}, // Def
        {STATS_OFFSET_X + 32, STATS_OFFSET_Y + 82, 45, 5}, // Speed
        {STATS_OFFSET_X + 32, STATS_OFFSET_Y + 97, 45, 5}, // Brains

        {CONDITION_OFFSET_X + 60, CONDITION_OFFSET_Y + 93, 51, 8},
        {CONDITION_OFFSET_X + 60, CONDITION_OFFSET_Y + 79, 51, 8},
        {CONDITION_OFFSET_X + 60, CONDITION_OFFSET_Y + 65, 51, 8},
        {CONDITION_OFFSET_X + 60, CONDITION_OFFSET_Y + 51, 51, 8},
    };

    void drawDigimonStatsViewStrings()
    {
        constexpr auto HAPPINESS_MID  = HAPPINESS_MIN + ((HAPPINESS_MAX - HAPPINESS_MIN) / 2);
        constexpr auto DISCIPLINE_MID = DISCIPLINE_MIN + ((DISCIPLINE_MAX - DISCIPLINE_MIN) / 2);

        DIGIVICE_ENTITY_VIEW.refpointX = -350; // TODO activate once the Tamer view has been re-implemented
        auto* para                     = getDigimonData(PARTNER_ENTITY.type);
        auto* raisePara                = getRaiseData(PARTNER_ENTITY.type);

        sprites[SpriteIndex::TYPE].uvX = static_cast<uint32_t>(para->type) * 12 - 12;
        initSpecialSprite(sprites[SpriteIndex::SPECIAL1], para->special[0]);
        initSpecialSprite(sprites[SpriteIndex::SPECIAL2], para->special[1]);
        initSpecialSprite(sprites[SpriteIndex::SPECIAL3], para->special[2]);
        sprites[SpriteIndex::HAPPINESS].uvX  = PARTNER_PARA.happiness < HAPPINESS_MID ? 11 : 0;
        sprites[SpriteIndex::DISCIPLINE].uvX = PARTNER_PARA.discipline < DISCIPLINE_MID ? 33 : 22;

        sprites[SpriteIndex::LIFE1].uvX = PARTNER_ENTITY.lives > 0 ? 232 : 244;
        sprites[SpriteIndex::LIFE2].uvX = PARTNER_ENTITY.lives > 1 ? 232 : 244;
        sprites[SpriteIndex::LIFE3].uvX = PARTNER_ENTITY.lives > 2 ? 232 : 244;

        sprites[SpriteIndex::SICK].height    = PARTNER_PARA.condition.isSick ? 12 : 0;
        sprites[SpriteIndex::INJURY].height  = PARTNER_PARA.condition.isInjured ? 12 : 0;
        sprites[SpriteIndex::HUNGER].height  = PARTNER_PARA.condition.isHungry ? 12 : 0;
        sprites[SpriteIndex::SLEEP].height   = PARTNER_PARA.condition.isSleepy ? 12 : 0;
        sprites[SpriteIndex::UNHAPPY].height = PARTNER_PARA.condition.isUnhappy ? 12 : 0;
        sprites[SpriteIndex::TIRED].height   = PARTNER_PARA.condition.isTired ? 12 : 0;
        sprites[SpriteIndex::POOP].height    = PARTNER_PARA.condition.isPoopy ? 12 : 0;

        auto happiness1  = PARTNER_PARA.happiness > HAPPINESS_MID ? 100 : (PARTNER_PARA.happiness - HAPPINESS_MIN) * 100 / (HAPPINESS_MAX - HAPPINESS_MID);
        auto happiness2  = PARTNER_PARA.happiness < HAPPINESS_MID ? 0 : (PARTNER_PARA.happiness - HAPPINESS_MID) * 100 / (HAPPINESS_MAX - HAPPINESS_MID);
        auto discipline1 = PARTNER_PARA.discipline > DISCIPLINE_MID ? 100 : (PARTNER_PARA.discipline - DISCIPLINE_MIN) * 100 / (DISCIPLINE_MAX - DISCIPLINE_MID);
        auto discipline2 = PARTNER_PARA.discipline < DISCIPLINE_MID ? 0 : (PARTNER_PARA.discipline - DISCIPLINE_MID) * 100 / (DISCIPLINE_MAX - DISCIPLINE_MID);

        sprites[SpriteIndex::HAPPINESS_BAR1].width  = happiness2 * 48 / 100;
        sprites[SpriteIndex::HAPPINESS_BAR2].width  = happiness1 * 48 / 100;
        sprites[SpriteIndex::DISCIPLINE_BAR1].width = discipline2 * 48 / 100;
        sprites[SpriteIndex::DISCIPLINE_BAR2].width = discipline1 * 48 / 100;

        clearTextSubArea2(0, 16, 256, 192);
        setTextColor(9);
        for (auto& entry : badges)
            drawTextSprite(entry);

        setTextColor(1);
        for (auto& entry : labels)
            drawTextSprite(entry);

        values[ValueIndex::NAME].string    = reinterpret_cast<const char*>(PARTNER_ENTITY.name);
        values[ValueIndex::DIGIMON].string = reinterpret_cast<const char*>(para->name);
        uint8_t buffer1[8];
        sprintf(buffer1, "%d", PARTNER_PARA.age);
        values[ValueIndex::AGE].string = reinterpret_cast<const char*>(buffer1);
        uint8_t buffer2[8];
        sprintf(buffer2, "%d", PARTNER_PARA.weight);
        values[ValueIndex::WEIGHT].string = reinterpret_cast<const char*>(buffer2);
        uint8_t buffer3[8];
        sprintf(buffer3, "%d", PARTNER_PARA.battles);
        values[ValueIndex::BATTLE].string = reinterpret_cast<const char*>(buffer3);
        uint8_t buffer4[8];
        sprintf(buffer4, "%d", PARTNER_PARA.careMistakes);
        values[ValueIndex::CARE].string = reinterpret_cast<const char*>(buffer4);

        SleepPattern& pattern = SLEEP_PATTERN[raisePara->sleepCycle];
        uint8_t activeTimes[32];
        sprintf(activeTimes,
                "%02d:%02d - %02d:%02d",
                pattern.wakeupHour,
                pattern.wakeupMinute,
                pattern.sleepyHour,
                pattern.sleepyMinute);
        values[ValueIndex::ACTIVE_TIME].string = reinterpret_cast<const char*>(activeTimes);

        uint8_t hp[32];
        sprintf(hp, "%4d  /  %4d", PARTNER_ENTITY.stats.currentHP, PARTNER_ENTITY.stats.hp);
        values[ValueIndex::HP].string = reinterpret_cast<const char*>(hp);
        uint8_t mp[32];
        sprintf(mp, "%4d  /  %4d", PARTNER_ENTITY.stats.currentMP, PARTNER_ENTITY.stats.mp);
        values[ValueIndex::MP].string = reinterpret_cast<const char*>(mp);
        uint8_t off[8];
        sprintf(off, "%d", PARTNER_ENTITY.stats.off);
        values[ValueIndex::OFFENSE].string = reinterpret_cast<const char*>(off);
        uint8_t def[8];
        sprintf(def, "%d", PARTNER_ENTITY.stats.def);
        values[ValueIndex::DEFENSE].string = reinterpret_cast<const char*>(def);
        uint8_t speed[8];
        sprintf(speed, "%d", PARTNER_ENTITY.stats.speed);
        values[ValueIndex::SPEED].string = reinterpret_cast<const char*>(speed);
        uint8_t brain[8];
        sprintf(brain, "%d", PARTNER_ENTITY.stats.brain);
        values[ValueIndex::BRAINS].string = reinterpret_cast<const char*>(brain);

        uint8_t happ[8];
        sprintf(happ, "%d", PARTNER_PARA.happiness);
        values[ValueIndex::HAPPINESS].string = reinterpret_cast<const char*>(happ);
        uint8_t disc[8];
        sprintf(disc, "%d", PARTNER_PARA.discipline);
        values[ValueIndex::DISCIPLINE].string = reinterpret_cast<const char*>(disc);
        uint8_t tired[8];
        sprintf(tired, "%d", PARTNER_PARA.tiredness);
        values[ValueIndex::TIREDNESS].string = reinterpret_cast<const char*>(tired);
        uint8_t virus[8];
        sprintf(virus, "%d", PARTNER_PARA.virusBar);
        values[ValueIndex::VIRUS].string = reinterpret_cast<const char*>(virus);

        for (auto& entry : values)
            drawTextSprite(entry);
    }

    void renderDigimonStatsView()
    {
        constexpr uint16_t CONDITION_OFFSETS[16] = {14, 15, 16, 17, 18, 19, 20, 21, 21, 20, 19, 18, 17, 16, 15, 14};

        if (MENU_STATE == 0)
        {
            drawDigimonStatsViewStrings();
            MENU_STATE = 1;
            return;
        }

        sprites[SpriteIndex::SICK].posY    = CONDITION_OFFSET_Y + CONDITION_OFFSETS[PLAYTIME_FRAMES % 16];
        sprites[SpriteIndex::INJURY].posY  = CONDITION_OFFSET_Y + CONDITION_OFFSETS[PLAYTIME_FRAMES % 16];
        sprites[SpriteIndex::HUNGER].posY  = CONDITION_OFFSET_Y + CONDITION_OFFSETS[PLAYTIME_FRAMES % 16];
        sprites[SpriteIndex::SLEEP].posY   = CONDITION_OFFSET_Y + CONDITION_OFFSETS[PLAYTIME_FRAMES % 16];
        sprites[SpriteIndex::UNHAPPY].posY = CONDITION_OFFSET_Y + CONDITION_OFFSETS[PLAYTIME_FRAMES % 16];
        sprites[SpriteIndex::TIRED].posY   = CONDITION_OFFSET_Y + CONDITION_OFFSETS[PLAYTIME_FRAMES % 16];
        sprites[SpriteIndex::POOP].posY    = CONDITION_OFFSET_Y + CONDITION_OFFSETS[PLAYTIME_FRAMES % 16];

        for (auto& entry : badges)
            renderTextSprite(entry);
        for (auto& entry : labels)
            renderTextSprite(entry);
        for (auto& entry : values)
            renderTextSprite(entry);

        for (auto& entry : sprites)
        {
            int16_t offsetX = 0;
            if ((entry.flag & 1) == 1 && PLAYTIME_FRAMES % 10 < 5) offsetX = entry.width;

            // clang-format off
            renderRectPolyFT4(entry.posX, entry.posY, entry.width, entry.height, entry.uvX + offsetX, entry.uvY, entry.texture_page, entry.clut, 5, 0);
        }

        renderDigimonStatsBar(PARTNER_ENTITY.stats.hp, HP_MAX, 75, STATS_OFFSET_X + 34, STATS_OFFSET_Y + 24);
        renderDigimonStatsBar(PARTNER_ENTITY.stats.mp, MP_MAX, 75, STATS_OFFSET_X + 34, STATS_OFFSET_Y + 39);
        renderDigimonStatsBar(PARTNER_ENTITY.stats.off, OFF_MAX, 40, STATS_OFFSET_X + 34, STATS_OFFSET_Y + 54);
        renderDigimonStatsBar(PARTNER_ENTITY.stats.def, DEF_MAX, 40, STATS_OFFSET_X + 34, STATS_OFFSET_Y + 69);
        renderDigimonStatsBar(PARTNER_ENTITY.stats.speed, SPEED_MAX, 40, STATS_OFFSET_X + 34, STATS_OFFSET_Y + 84);
        renderDigimonStatsBar(PARTNER_ENTITY.stats.brain, BRAIN_MAX, 40, STATS_OFFSET_X + 34, STATS_OFFSET_Y + 99);
        renderBoxBar(CONDITION_OFFSET_X + 62, CONDITION_OFFSET_Y + 81, (PARTNER_PARA.tiredness * 48 / (TIREDNESS_MAX - TIREDNESS_MIN)), 5, 50, 150, 255, 0, 5);
        renderBoxBar(CONDITION_OFFSET_X + 62, CONDITION_OFFSET_Y + 95, PARTNER_PARA.virusBar * 48 / (VIRUS_MAX - VIRUS_MIN), 5, 200, 200, 60, 0, 5);
        // clang-format on

        // profile insets
        for (auto& inset : insets)
            renderInsetBox(inset.posX + 160, inset.posY + 120, inset.width, inset.height, 5);

        renderSeperatorLines(lines, 6, 5);

        renderDigiviceEntity(&PARTNER_ENTITY, 1);
    }
}