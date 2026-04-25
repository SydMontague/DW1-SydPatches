#include "DebugMenu.hpp"

#include "Helper.hpp"
#include "Math.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"

namespace
{
constexpr int32_t DRAW_X = 704;
constexpr int32_t DRAW_Y = 272;

constexpr int32_t VISIBLE_ROWS  = 11;
constexpr int32_t ROW_HEIGHT    = 12;
constexpr int32_t PSTAT_COUNT   = 256;
constexpr int32_t TRIGGER_COUNT = 800;

constexpr int32_t TAB_PSTAT_X   = -120;
constexpr int32_t TAB_TRIGGER_X = -40;
constexpr int32_t TAB_STATS_X   = 40;
constexpr int32_t TAB_BOX_W     = 80;

constexpr int32_t LIST_X = -130;
constexpr int32_t LIST_Y = -75;
constexpr int32_t LIST_W = 260;

constexpr int32_t COL_IDX_W   = 28;
constexpr int32_t COL_VALUE_W = 50;
constexpr int32_t COL_NAME_X  = LIST_X + COL_IDX_W;
constexpr int32_t COL_VALUE_X = LIST_X + LIST_W - COL_VALUE_W;

constexpr int32_t HEADER_Y = LIST_Y;
constexpr int32_t DATA_Y   = LIST_Y + ROW_HEIGHT + 2;

constexpr uint8_t PAGE_PSTAT   = 0;
constexpr uint8_t PAGE_TRIGGER = 1;
constexpr uint8_t PAGE_STATS   = 2;
constexpr uint8_t NUM_PAGES    = 3;

struct Named
{
    int16_t index;
    const char* name;
};

constexpr Named PSTAT_NAMES[] = {
    {0,   "gameSpeed"  },
    {1,   "prosperity" },
    {2,   "tournDay"   },
    {3,   "tournId"    },
    {4,   "tournReg"   },
    {243, "bootFlag"   },
    {250, "isPreBattle"},
    {251, "savedHP"    },
    {252, "savedMP"    },
    {254, "starter"    },
    {255, "tournWins"  },
};

constexpr Named TRIGGER_NAMES[] = {
    {45,  "oldRod"    },
    {46,  "amazingRod"},
    {47,  "keychain1" },
    {48,  "keychain2" },
    {203, "Agumon"    },
    {214, "Monzaemon" },
    {220, "Angemon"   },
    {221, "Birdramon" },
    {225, "Vegimon"   },
    {246, "Palmon"    },
};

struct StatRow
{
    const char* name;
    int16_t* valuePtr;
};

extern const StatRow STATS_ROWS[];
extern const int32_t STATS_COUNT;

const char* lookupName(const Named* arr, int32_t count, int32_t idx)
{
    for (int32_t i = 0; i < count; i++)
        if (arr[i].index == idx) return arr[i].name;
    return "";
}

uint8_t debugPage;
int32_t cursors[NUM_PAGES];
int32_t scrolls[NUM_PAGES];
char rowStrings[VISIBLE_ROWS][24];

TextSprite headerTabs[3] = {
    {
        .font       = &vanillaFont,
        .string     = "PStat",
        .uvX        = DRAW_X + 0,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = TAB_PSTAT_X,
        .posY       = -101,
        .boxWidth   = TAB_BOX_W,
        .boxHeight  = 12,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &vanillaFont,
        .string     = "Trigger",
        .uvX        = DRAW_X + 16,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = TAB_TRIGGER_X,
        .posY       = -101,
        .boxWidth   = TAB_BOX_W,
        .boxHeight  = 12,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
    {
        .font       = &vanillaFont,
        .string     = "Stats",
        .uvX        = DRAW_X + 32,
        .uvY        = DRAW_Y + 0,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = TAB_STATS_X,
        .posY       = -101,
        .boxWidth   = TAB_BOX_W,
        .boxHeight  = 12,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    },
};

uint16_t rowUvWidths[VISIBLE_ROWS];

TextSprite makeRowSprite(int32_t i)
{
    return TextSprite{
        .font       = &vanillaFont,
        .string     = rowStrings[i],
        .uvX        = DRAW_X + 0,
        .uvY        = static_cast<uint16_t>(DRAW_Y + 28 + i * ROW_HEIGHT),
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = LIST_X + 4,
        .posY       = static_cast<int16_t>(DATA_Y + i * ROW_HEIGHT),
        .boxWidth   = LIST_W - 8,
        .boxHeight  = ROW_HEIGHT,
        .alignmentX = AlignmentX::LEFT,
        .alignmentY = AlignmentY::CENTER,
        .color      = 0,
        .layer      = 5,
        .hasShadow  = 1,
    };
}

int32_t entryCount()
{
    if (debugPage == PAGE_PSTAT) return PSTAT_COUNT;
    if (debugPage == PAGE_TRIGGER) return TRIGGER_COUNT;
    return STATS_COUNT;
}

void formatRow(int32_t rowIndex, int32_t entryIndex)
{
    char* dst = rowStrings[rowIndex];
    if (debugPage == PAGE_PSTAT)
    {
        uint8_t v        = readPStat(entryIndex);
        const char* name = lookupName(PSTAT_NAMES, sizeof(PSTAT_NAMES) / sizeof(PSTAT_NAMES[0]), entryIndex);
        sprintf(reinterpret_cast<uint8_t*>(dst), "%3d  %-13s %5d", entryIndex, name, v);
    }
    else if (debugPage == PAGE_TRIGGER)
    {
        bool on          = isTriggerSet(entryIndex);
        const char* name = lookupName(TRIGGER_NAMES, sizeof(TRIGGER_NAMES) / sizeof(TRIGGER_NAMES[0]), entryIndex);
        sprintf(reinterpret_cast<uint8_t*>(dst), "%3d  %-13s %5s", entryIndex, name, on ? "ON" : "OFF");
    }
    else
    {
        const StatRow& row = STATS_ROWS[entryIndex];
        sprintf(reinterpret_cast<uint8_t*>(dst), "%3d  %-13s %5d", entryIndex, row.name, *row.valuePtr);
    }
}

void redrawAllRows()
{
    clearTextSubArea2(0, 16 + 28, 256, VISIBLE_ROWS * ROW_HEIGHT);
    int32_t scroll = scrolls[debugPage];
    int32_t count  = entryCount();
    for (int32_t i = 0; i < VISIBLE_ROWS; i++)
    {
        int32_t entry = scroll + i;
        if (entry >= count) { rowStrings[i][0] = 0; }
        else { formatRow(i, entry); }
        TextSprite s = makeRowSprite(i);
        drawTextSprite(s);
        rowUvWidths[i] = s.uvWidth;
    }
}

void clampScroll()
{
    int32_t cursor = cursors[debugPage];
    int32_t scroll = scrolls[debugPage];
    int32_t count  = entryCount();
    if (cursor < scroll) scroll = cursor;
    else if (cursor >= scroll + VISIBLE_ROWS) scroll = cursor - VISIBLE_ROWS + 1;
    int32_t maxScroll = count - VISIBLE_ROWS;
    if (maxScroll < 0) maxScroll = 0;
    if (scroll > maxScroll) scroll = maxScroll;
    if (scroll < 0) scroll = 0;
    scrolls[debugPage] = scroll;
}

void changeValue(int32_t delta)
{
    int32_t entry = cursors[debugPage];
    if (debugPage == PAGE_TRIGGER)
    {
        if (isTriggerSet(entry)) unsetTrigger(entry);
        else setTrigger(entry);
        return;
    }
    int32_t cur, max;
    int16_t* statPtr = nullptr;
    if (debugPage == PAGE_PSTAT) { cur = readPStat(entry); max = 255; }
    else { statPtr = STATS_ROWS[entry].valuePtr; cur = *statPtr; max = 9999; }
    int32_t v = cur + delta;
    if (v < 0) v = 0;
    if (v > max) v = max;
    if (debugPage == PAGE_PSTAT) writePStat(entry, static_cast<uint8_t>(v));
    else *statPtr = static_cast<int16_t>(v);
}

const StatRow STATS_ROWS[] = {
    {"HP max",     &PARTNER_ENTITY.stats.hp},
    {"MP max",     &PARTNER_ENTITY.stats.mp},
    {"HP cur",     &PARTNER_ENTITY.stats.currentHP},
    {"MP cur",     &PARTNER_ENTITY.stats.currentMP},
    {"Off",        &PARTNER_ENTITY.stats.off},
    {"Def",        &PARTNER_ENTITY.stats.def},
    {"Speed",      &PARTNER_ENTITY.stats.speed},
    {"Brain",      &PARTNER_ENTITY.stats.brain},
    {"Happiness",  &PARTNER_PARA.happiness},
    {"Discipline", &PARTNER_PARA.discipline},
    {"Tiredness",  &PARTNER_PARA.tiredness},
    {"Weight",     &PARTNER_PARA.weight},
    {"Age",        &PARTNER_PARA.age},
    {"Battles",    &PARTNER_PARA.battles},
    {"CareMis",    &PARTNER_PARA.careMistakes},
};

const int32_t STATS_COUNT = sizeof(STATS_ROWS) / sizeof(STATS_ROWS[0]);
} // namespace

void resetDebugMenu()
{
    debugPage = PAGE_PSTAT;
    for (int32_t i = 0; i < NUM_PAGES; i++)
    {
        cursors[i] = 0;
        scrolls[i] = 0;
    }
}

void renderDebugIcon(int16_t posX, int16_t posY, int16_t width, int16_t height)
{
    uint8_t pulse = (PLAYTIME_FRAMES % 16 < 8) ? 220 : 150;
    renderBox(posX + 1, posY + 1, width - 2, height - 2, 60, pulse, 220, 0, 6);
}

void tickDebugMenu(int32_t)
{
    if (MENU_STATE == 0)
    {
        clearTextSubArea2(0, 16, 256, VISIBLE_ROWS * ROW_HEIGHT + 28);
        for (int32_t i = 0; i < 3; i++) drawTextSprite(headerTabs[i]);
        redrawAllRows();
        MENU_STATE = 1;
        return;
    }

    if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
    {
        TRIANGLE_MENU_STATE = 8;
        playSound(0, 4);
        return;
    }

    bool changed  = false;
    int32_t count = entryCount();

    if (isKeyDownRepeat(InputButtons::BUTTON_L1))
    {
        if (debugPage > 0) { debugPage--; }
        else
        {
            cursors[0] -= VISIBLE_ROWS;
            if (cursors[0] < 0) cursors[0] = 0;
        }
        changed = true;
        playSound(0, 2);
    }
    if (isKeyDownRepeat(InputButtons::BUTTON_R1))
    {
        if (debugPage + 1 < NUM_PAGES) { debugPage++; }
        else
        {
            cursors[debugPage] += VISIBLE_ROWS;
            int32_t c = entryCount();
            if (cursors[debugPage] >= c) cursors[debugPage] = c - 1;
        }
        changed = true;
        playSound(0, 2);
    }
    if (isKeyDownRepeat(InputButtons::BUTTON_UP))
    {
        if (cursors[debugPage] > 0) { cursors[debugPage]--; changed = true; playSound(0, 2); }
    }
    if (isKeyDownRepeat(InputButtons::BUTTON_DOWN))
    {
        if (cursors[debugPage] + 1 < count) { cursors[debugPage]++; changed = true; playSound(0, 2); }
    }
    static constexpr struct
    {
        uint16_t button;
        int8_t delta;
    } EDIT_INPUTS[] = {
        {InputButtons::BUTTON_LEFT,   -1 },
        {InputButtons::BUTTON_RIGHT,  +1 },
        {InputButtons::BUTTON_SQUARE, -10},
        {InputButtons::BUTTON_CIRCLE, +10},
        {InputButtons::BUTTON_CROSS,  +1 }, // triggers only
    };
    for (auto& e : EDIT_INPUTS)
    {
        if (!isKeyDownRepeat(e.button)) continue;
        if (e.button == InputButtons::BUTTON_CROSS && debugPage != PAGE_TRIGGER) continue;
        changeValue(e.delta);
        changed = true;
        playSound(0, 3);
    }

    if (changed)
    {
        clampScroll();
        redrawAllRows();
    }
}

void renderDebugMenu(int32_t)
{
    for (int32_t i = 0; i < 3; i++)
    {
        headerTabs[i].color = (debugPage == i) ? 0 : 1;
        renderTextSprite(headerTabs[i]);
    }
    renderMenuTab(TAB_PSTAT_X - 5,   TAB_BOX_W, debugPage != PAGE_PSTAT);
    renderMenuTab(TAB_TRIGGER_X - 5, TAB_BOX_W, debugPage != PAGE_TRIGGER);
    renderMenuTab(TAB_STATS_X - 5,   TAB_BOX_W, debugPage != PAGE_STATS);

    int32_t cursor    = cursors[debugPage];
    int32_t scroll    = scrolls[debugPage];
    int32_t cursorRow = cursor - scroll;
    int32_t count     = entryCount();
    for (int32_t i = 0; i < VISIBLE_ROWS; i++)
    {
        int32_t entry = scroll + i;
        if (entry >= count) continue;
        TextSprite s = makeRowSprite(i);
        s.uvWidth    = rowUvWidths[i];
        s.uvHeight   = vanillaFont.height;
        s.color      = (i == cursorRow) ? 0 : 1;
        renderTextSprite(s);
    }

    int16_t headLineY = static_cast<int16_t>(DATA_Y - 2);
    drawLine2P(0xc59f4a, LIST_X, headLineY, LIST_X + LIST_W, headLineY, 4, 0);

    if (cursorRow >= 0 && cursorRow < VISIBLE_ROWS)
    {
        renderSelectionCursor(LIST_X + 2,
                              DATA_Y - 2 + cursorRow * ROW_HEIGHT,
                              LIST_W - 4,
                              ROW_HEIGHT,
                              4);
    }
}
