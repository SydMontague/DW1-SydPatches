
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Map.hpp"
#include "Fishing.hpp"
#include "Math.hpp"
#include "Tamer.hpp"
#include "UIElements.hpp"
#include "constants.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/stddef.hpp"

namespace
{
    constexpr int32_t STATUS_UI_X_MIN = 75;
    constexpr int32_t STATUS_UI_X_MAX = 240;

    constexpr Sprite bellLeft = {
        .uvX          = 0xC0,
        .uvV          = 0xE0,
        .width        = 16,
        .height       = 16,
        .texture_page = getTPage(0, 0, 0x380, 0x1C0),
        .clut         = getClut(0x100, 0x1F1),
    };

    constexpr Sprite bellRight = {
        .uvX          = 0xC0,
        .uvV          = 0xF0,
        .width        = 16,
        .height       = 15,
        .texture_page = getTPage(0, 0, 0x380, 0x1C0),
        .clut         = getClut(0x100, 0x1F1),
    };

    constexpr Sprite clockBody = {
        .uvX          = 0xD0,
        .uvV          = 0xD8, // vanilla is at 0xD7, which contains one invalid pixel
        .width        = 47,
        .height       = 39,
        .texture_page = getTPage(0, 0, 0x380, 0x1C0),
        .clut         = getClut(0x100, 0x1F0),
    };

    GsSPRITE clockSprite;
    int16_t clockOffsetX;
    int16_t subframeCount;

    void tickPlaytime(int32_t instanceId)
    {
        PLAYTIME_FRAMES += 1;
        if (PLAYTIME_FRAMES % 1200 == 0)
        {
            PLAYTIME_MINUTES += 1;
            PLAYTIME_FRAMES = 0;
        }

        if (PLAYTIME_MINUTES >= 60)
        {
            PLAYTIME_HOURS += 1;
            PLAYTIME_MINUTES = 0;
        }

        if (PLAYTIME_HOURS >= 999)
        {
            PLAYTIME_HOURS   = 999;
            PLAYTIME_MINUTES = 59;
        }
    }
} // namespace

extern "C"
{
    void renderStatusBars(int32_t instanceId)
    {
        if (IS_GAMETIME_RUNNING == 1)
            STATUS_UI_OFFSET_X -= 50;
        else
            STATUS_UI_OFFSET_X += 50;

        STATUS_UI_OFFSET_X = clamp(STATUS_UI_OFFSET_X, STATUS_UI_X_MIN, STATUS_UI_X_MAX);

        renderRectPolyFT4(STATUS_UI_OFFSET_X, 85, 76, 8, 0, 236, 5, 0x7D06, 15, 0);
        renderRectPolyFT4(STATUS_UI_OFFSET_X, 97, 76, 8, 0, 236, 5, 0x7D06, 15, 0);

        auto happyX = PARTNER_PARA.happiness < HAPPINESS_MID ? 97 : 64;
        auto discX  = PARTNER_PARA.discipline < DISCIPLINE_MID ? 33 : 22;
        if (PLAYTIME_FRAMES % 10 < 5)
        {
            if (PARTNER_AREA_RESPONSE == 2) happyX += 11;
            if (PARTNER_AREA_RESPONSE == 1) happyX -= 11;
        }
        auto happyClut = getClut(0x60, 0x1f7);
        auto discClut  = getClut(0x60, 0x1f2);
        renderRectPolyFT4(STATUS_UI_OFFSET_X + 3, 83, 11, 11, happyX, 244, 5, happyClut, 14, 0);
        renderRectPolyFT4(STATUS_UI_OFFSET_X + 3, 95, 11, 11, discX, 244, 5, discClut, 14, 0);

        // clang-format off
        int32_t barValues[4];
        barValues[0] = clamp((PARTNER_PARA.happiness - HAPPINESS_MIN) * 100 / (HAPPINESS_MAX - HAPPINESS_MID), 0, 100);
        barValues[1] = clamp((PARTNER_PARA.happiness - HAPPINESS_MID) * 100 / (HAPPINESS_MAX - HAPPINESS_MID), 0, 100);
        barValues[2] = clamp((PARTNER_PARA.discipline - DISCIPLINE_MIN) * 100 / (DISCIPLINE_MAX - DISCIPLINE_MID), 0, 100);
        barValues[3] = clamp((PARTNER_PARA.discipline - DISCIPLINE_MID) * 100 / (DISCIPLINE_MAX - DISCIPLINE_MID), 0, 100);
        // clang-format on

        for (int32_t i = 0; i < 4; i++)
        {
            auto clut      = getClut(0x60, 0x1f7);
            int32_t width  = (barValues[i] * 50) / 100;
            int32_t x      = STATUS_UI_OFFSET_X + 18;
            int32_t y      = 88 + 12 * ((i & 2) / 2);
            int32_t v      = 151 + 2 * (i & 1);
            int32_t offset = 0;
            while (width > 0)
            {
                renderRectPolyFT4(x + offset, y, min(width, 20), 2, 232, v, 5, clut, 14 - (i & 1), 0);
                width -= 20;
                offset += 20;
            }
        }
    }

    void initializeClockData()
    {
        CURRENT_FRAME       = 9600;
        clockOffsetX        = -135;
        YEAR                = 0;
        DAY                 = 0;
        HOUR                = 8;
        MINUTE              = 0;
        PLAYTIME_FRAMES     = 0;
        PLAYTIME_MINUTES    = 0;
        PLAYTIME_HOURS      = 0;
        subframeCount       = 0;
        IS_GAMETIME_RUNNING = 1;

        clockSprite.attribute = 0;
        clockSprite.x         = -112;
        clockSprite.y         = -66;
        clockSprite.width     = 8;
        clockSprite.height    = 16;
        clockSprite.tpage     = getTPage(0, 0, 0x380, 0x1C0);
        clockSprite.clutX     = 256;
        clockSprite.clutY     = 499;
        clockSprite.u         = 0x78;
        clockSprite.v         = 0xc0;
        clockSprite.r         = 0x80;
        clockSprite.g         = 0x80;
        clockSprite.b         = 0x80;
        clockSprite.mx        = 3;
        clockSprite.my        = 13;
        clockSprite.scaleX    = 4096;
        clockSprite.scaleY    = 4096;
        clockSprite.rotate    = MINUTE * 0x6000;
    }

    void tickGameClock(int32_t instanceId)
    {
        if (GAME_STATE != 0) return;
        if (isFishing()) return;
        if (!MAP_LAYER_ENABLED) return;
        if (IS_GAMETIME_RUNNING == 0) return;

        if (IS_GAMETIME_RUNNING == 1 && readPStat(0) == 3) writePStat(0, 0);

        auto gamespeed = readPStat(0);
        if (gamespeed == 3) return;

        auto frameDiff = 0;

        if (gamespeed == 0)
            frameDiff = 1;
        else if (gamespeed == 1)
            frameDiff = 2 + CURRENT_FRAME % 2;
        else if (gamespeed == 2)
            frameDiff = subframeCount++ % 2 ? 1 : 0;

        CURRENT_FRAME += frameDiff;

        if (frameDiff != 0 && (CURRENT_FRAME % 20) == 0)
        {
            MINUTE += 1;

            if (MINUTE >= 60)
            {
                MINUTE = 0;
                HOUR += 1;
                PARTNER_PARA.evoTimer += 1;
                PARTNER_PARA.remainingLifetime -= 1;
                subframeCount = 0;
            }

            if (HOUR >= 24)
            {
                HOUR = 0;
                PARTNER_PARA.age += 1;
                CURRENT_FRAME = 0;
                DAY += 1;
                dailyPStatTrigger();
            }

            if (DAY >= 30)
            {
                YEAR += 1;
                DAY = 0;

                if (PARTNER_PARA.happiness == 100)
                    addTamerLevel(5, 1);
                else if (PARTNER_PARA.happiness < 0)
                    addTamerLevel(10, -1);
            }

            if (YEAR >= 100) YEAR = 0;
        }

        clockSprite.rotate = MINUTE * 0x6000;
        updateBGM();
    }

    void renderGameClock(int32_t instanceId)
    {
        constexpr dtl::array<uint8_t, 24> hourX = {
            20, 23, 26, 31, 34, 36, 36, 36, 34, 31, 26, 23, 20, 17, 14, 9, 6, 4, 4, 4, 6, 9, 14, 17,
        };
        constexpr dtl::array<uint8_t, 24> hourY = {
            5, 6, 6, 8, 12, 15, 19, 23, 26, 30, 32, 32, 33, 32, 32, 30, 26, 23, 19, 15, 12, 8, 6, 6,
        };
        constexpr dtl::array<uint8_t, 8> uOffsets = {0x80, 0xA0, 0x80, 0xA0, 0xA0, 0x80, 0xA0, 0x80};
        constexpr dtl::array<uint8_t, 8> vOffsets = {0xC0, 0xC0, 0xD0, 0xC0, 0xD0, 0xE0, 0xD0, 0xE0};

        bool isDark = (HOUR < 6 || HOUR > 16);
        auto time   = (CURRENT_FRAME % 16) >> 2;

        auto animFrame = isDark * 4 + time;

        if (IS_GAMETIME_RUNNING)
            clockOffsetX += 50;
        else
            clockOffsetX -= 50;

        clockOffsetX = clamp(clockOffsetX, -220, -135);

        bellLeft.render(clockOffsetX - 2, -90, 9, 0);
        bellRight.render(clockOffsetX + 34, -90, 9, 0);
        renderRectPolyFT4(clockOffsetX + 8,
                          -100,
                          32,
                          16,
                          uOffsets[animFrame],
                          vOffsets[animFrame],
                          getTPage(0, 0, 0x380, 0x1C0),
                          getClut(0x100, 0x1F2),
                          9,
                          0);
        clockBody.render(clockOffsetX, -87, 10, 0);

        clockSprite.x = clockOffsetX + 23;
        libgs_GsSortSprite(&clockSprite, ACTIVE_ORDERING_TABLE, 9);
        renderRectPolyFT4(clockOffsetX + hourX[HOUR],
                          hourY[HOUR] - 88,
                          6,
                          6,
                          0xcb,
                          0xd8,
                          getTPage(0, 0, 0x380, 0x1C0),
                          getClut(0x100, 0x1F1),
                          9,
                          0);
    }

    void stopGameTime()
    {
        IS_GAMETIME_RUNNING = 0;
    }

    void startGameTime()
    {
        IS_GAMETIME_RUNNING = 1;
    }

    void updateMinuteHand(int32_t hour, int32_t minute)
    {
        clockSprite.rotate = minute * 0x6000;
    }

    void advanceToTime(uint32_t hour, uint32_t minute)
    {
        if (hour < HOUR || hour == HOUR && minute < MINUTE)
        {
            DAY++;
            PARTNER_PARA.age += 1;
            dailyPStatTrigger();
            if (DAY >= 30)
            {
                DAY = 0;
                YEAR++;
            }
        }

        HOUR          = hour % 24;
        MINUTE        = minute;
        CURRENT_FRAME = HOUR * 1200 + MINUTE * 20;
        updateMinuteHand(hour, minute);
        updateTimeOfDay();
    }

    void addClock()
    {
        addObject(ObjectID::GAME_CLOCK, 0, tickGameClock, renderGameClock);
        addObject(ObjectID::STATUS_BAR, 0, nullptr, renderStatusBars);
        addObject(ObjectID::PLAYTIME, 0, tickPlaytime, nullptr);
    }
}
