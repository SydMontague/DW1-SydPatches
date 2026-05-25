#include "DebugMenu.hpp"

#include "AtlasFont.hpp"
#include "Helper.hpp"
#include "Input.hpp"
#include "Math.hpp"
#include "MenuTab.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dtl/unique_ptr.hpp"
#include "extern/dw1.hpp"

namespace
{
    constexpr auto WINDOW_X      = -150;
    constexpr auto WINDOW_Y      = -89;
    constexpr auto WINDOW_WIDTH  = 300;
    constexpr auto WINDOW_HEIGHT = 190;

    constexpr int32_t VISIBLE_ROWS  = 11;
    constexpr int32_t ROW_HEIGHT    = 14;
    constexpr int32_t PSTAT_COUNT   = 256;
    constexpr int32_t TRIGGER_COUNT = 800;

    constexpr int32_t TAB_PSTAT_X   = -120;
    constexpr int32_t TAB_TRIGGER_X = -40;
    constexpr int32_t TAB_STATS_X   = 40;
    constexpr int32_t TAB_BOX_W     = 80;
    constexpr int32_t TAB_NAME_W    = TAB_BOX_W - 14;

    constexpr int32_t LIST_X = -130;
    constexpr int32_t LIST_Y = -75;
    constexpr int32_t LIST_W = 260;

    constexpr int32_t DATA_Y = LIST_Y + ROW_HEIGHT + 2;

    constexpr uint8_t PAGE_PSTAT   = 0;
    constexpr uint8_t PAGE_TRIGGER = 1;
    constexpr uint8_t PAGE_STATS   = 2;
    constexpr uint8_t NUM_PAGES    = 3;

    struct Named
    {
        int16_t index;
        const char* name;
    };

    struct StatRow
    {
        const char* name;
        int16_t* valuePtr;
        int16_t min;
        int16_t max;
    };

    constexpr dtl::array<Named, 11> PSTAT_NAMES = {{
        {0, "gameSpeed"},
        {1, "prosperity"},
        {2, "tournDay"},
        {3, "tournId"},
        {4, "tournReg"},
        {243, "bootFlag"},
        {250, "isPreBattle"},
        {251, "savedHP"},
        {252, "savedMP"},
        {254, "starter"},
        {255, "tournWins"},
    }};

    constexpr dtl::array<Named, 10> TRIGGER_NAMES = {{
        {45, "oldRod"},
        {46, "amazingRod"},
        {47, "keychain1"},
        {48, "keychain2"},
        {203, "Agumon"},
        {214, "Monzaemon"},
        {220, "Angemon"},
        {221, "Birdramon"},
        {225, "Vegimon"},
        {246, "Palmon"},
    }};

    constexpr dtl::array<StatRow, 15> STATS_ROWS = {{
        {"HP max", &PARTNER_ENTITY.stats.hp, 1, 9999},
        {"MP max", &PARTNER_ENTITY.stats.mp, 1, 9999},
        {"HP cur", &PARTNER_ENTITY.stats.currentHP, 1, 9999},
        {"MP cur", &PARTNER_ENTITY.stats.currentMP, 1, 9999},
        {"Off", &PARTNER_ENTITY.stats.off, 1, 999},
        {"Def", &PARTNER_ENTITY.stats.def, 1, 999},
        {"Speed", &PARTNER_ENTITY.stats.speed, 1, 999},
        {"Brain", &PARTNER_ENTITY.stats.brain, 1, 999},
        {"Happiness", &PARTNER_PARA.happiness, -100, 100},
        {"Discipline", &PARTNER_PARA.discipline, 0, 100},
        {"Tiredness", &PARTNER_PARA.tiredness, 0, 100},
        {"Weight", &PARTNER_PARA.weight, 1, 99},
        {"Age", &PARTNER_PARA.age, 0, 9999},
        {"Battles", &PARTNER_PARA.battles, 0, 9999},
        {"CareMis", &PARTNER_PARA.careMistakes, 0, 9999},
    }};

    constexpr const char* lookupName(const Named* begin, const Named* end, int32_t idx)
    {
        auto current = begin;
        while (current != end)
        {
            if (current->index == idx) return current->name;
            current++;
        }
        return "";
    }

    constexpr const char* getRowName(int8_t page, int16_t index)
    {
        switch (page)
        {
            case PAGE_PSTAT: return lookupName(PSTAT_NAMES.begin(), PSTAT_NAMES.end(), index);
            case PAGE_TRIGGER: return lookupName(TRIGGER_NAMES.begin(), TRIGGER_NAMES.end(), index);
            case PAGE_STATS: return index < STATS_ROWS.size() ? STATS_ROWS[index].name : "";
            default: return "";
        }
    }

    constexpr dtl::array<uint8_t, 256> getRowValue(int8_t page, int16_t index)
    {
        switch (page)
        {
            case PAGE_PSTAT: return format("%d", readPStat(index));
            case PAGE_TRIGGER: return format("%s", isTriggerSet(index) ? "On" : "Off");
            case PAGE_STATS:
                return index < STATS_ROWS.size() ? format("%d", *STATS_ROWS[index].valuePtr)
                                                 : dtl::array<uint8_t, 256>{};
            default: return {};
        }
    }

    constexpr int16_t getEntryCount(int8_t page)
    {
        switch (page)
        {
            case PAGE_PSTAT: return PSTAT_COUNT;
            case PAGE_TRIGGER: return TRIGGER_COUNT;
            case PAGE_STATS: return STATS_ROWS.size();
            default: return 0;
        }
    }

    void changeValue(int8_t page, int16_t index, int32_t value)
    {
        switch (page)
        {
            case PAGE_TRIGGER: isTriggerSet(index) ? unsetTrigger(index) : setTrigger(index); return;
            case PAGE_PSTAT: writePStat(index, clamp(readPStat(index) + value, 0, 255)); return;
            case PAGE_STATS:
            {
                if (index > STATS_ROWS.size()) return;

                auto& stat     = STATS_ROWS[index];
                *stat.valuePtr = clamp(*stat.valuePtr + value, stat.min, stat.max);
                return;
            }
        }
    }

    struct DebugMenu
    {
    public:
        DebugMenu();
        void render(int32_t depth);
        void tick();

    private:
        void updateRows();
        void clampScroll();

        MenuTab pstatTab{TAB_PSTAT_X, WINDOW_Y - 15, TAB_BOX_W, false, "PStat"};
        MenuTab triggerTab{TAB_TRIGGER_X, WINDOW_Y - 15, TAB_BOX_W, true, "Trigger"};
        MenuTab statsTab{TAB_STATS_X, WINDOW_Y - 15, TAB_BOX_W, true, "Stats"};

        dtl::array<AtlasString, VISIBLE_ROWS * 3> rows;
        dtl::array<int16_t, NUM_PAGES> scrolls{};
        dtl::array<int16_t, NUM_PAGES> cursors{};
        int8_t page{PAGE_PSTAT};
    };

    DebugMenu::DebugMenu()
    {
        updateRows();
    }

    void DebugMenu::updateRows()
    {
        auto scroll = scrolls[page];
        auto cursor = cursors[page];
        auto count  = getEntryCount(page);

        const auto& font = getAtlasVanilla();
        for (int32_t i = 0; i < VISIBLE_ROWS; i++)
        {
            auto index = i + scroll;
            const RenderSettings numberSetting{
                .x        = LIST_X,
                .y        = static_cast<int16_t>(DATA_Y + i * ROW_HEIGHT),
                .baseClut = 1,
                .color    = cursor == index ? TEXT_COLOR_WHITE : TEXT_COLOR_BLUE,
                .width    = 28,
                .height   = ROW_HEIGHT,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            };
            const RenderSettings nameSetting{
                .x        = LIST_X + 32,
                .y        = static_cast<int16_t>(DATA_Y + i * ROW_HEIGHT),
                .baseClut = 1,
                .color    = cursor == index ? TEXT_COLOR_WHITE : TEXT_COLOR_BLUE,
                .width    = 120,
                .height   = ROW_HEIGHT,
                .alignX   = AlignmentX::LEFT,
                .alignY   = AlignmentY::CENTER,
            };
            const RenderSettings valueSetting{
                .x        = LIST_X + 152,
                .y        = static_cast<int16_t>(DATA_Y + i * ROW_HEIGHT),
                .baseClut = 1,
                .color    = cursor == index ? TEXT_COLOR_WHITE : TEXT_COLOR_BLUE,
                .width    = 28,
                .height   = ROW_HEIGHT,
                .alignX   = AlignmentX::RIGHT,
                .alignY   = AlignmentY::CENTER,
            };

            if (index > count)
            {
                rows[i]                    = {};
                rows[i + VISIBLE_ROWS]     = {};
                rows[i + VISIBLE_ROWS * 2] = {};
            }
            else
            {
                rows[i]                    = font.render(format("%d", index).data(), numberSetting);
                rows[i + VISIBLE_ROWS]     = font.render(format("%s", getRowName(page, index)).data(), nameSetting);
                rows[i + VISIBLE_ROWS * 2] = font.render(getRowValue(page, index).data(), valueSetting);
            }
        }
    }
    void DebugMenu::clampScroll()
    {
        int32_t cursor = cursors[page];
        int32_t scroll = scrolls[page];
        int32_t count  = getEntryCount(page);
        if (cursor < scroll)
            scroll = cursor;
        else if (cursor >= scroll + VISIBLE_ROWS)
            scroll = cursor - VISIBLE_ROWS + 1;
        int32_t maxScroll = count - VISIBLE_ROWS;
        if (maxScroll < 0) maxScroll = 0;
        if (scroll > maxScroll) scroll = maxScroll;
        if (scroll < 0) scroll = 0;
        scrolls[page] = scroll;
    }

    void DebugMenu::tick()
    {
        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            TRIANGLE_MENU_STATE = 8;
            playSound(0, 4);
            return;
        }

        bool changed = false;

        if (isKeyDownRepeat(InputButtons::BUTTON_L1) && page > 0)
        {
            page--;
            changed = true;
        }
        if (isKeyDownRepeat(InputButtons::BUTTON_R1) && page < (NUM_PAGES - 1))
        {
            page++;
            changed = true;
        }
        if (isKeyDownRepeat(InputButtons::BUTTON_L2))
        {
            cursors[page] = clamp(cursors[page] - VISIBLE_ROWS, 0, getEntryCount(page) - 1);
            changed       = true;
        }
        if (isKeyDownRepeat(InputButtons::BUTTON_R2))
        {
            cursors[page] = clamp(cursors[page] + VISIBLE_ROWS, 0, getEntryCount(page) - 1);
            changed       = true;
        }
        if (isKeyDownRepeat(InputButtons::BUTTON_UP) && cursors[page] > 0)
        {
            cursors[page]--;
            changed = true;
        }
        if (isKeyDownRepeat(InputButtons::BUTTON_DOWN) && cursors[page] < getEntryCount(page) - 1)
        {
            cursors[page]++;
            changed = true;
        }
        if (isKeyDownRepeat(InputButtons::BUTTON_CROSS) && page == PAGE_TRIGGER)
        {
            changeValue(page, cursors[page], +1);
            changed = true;
        }
        if (isKeyDownRepeat(InputButtons::BUTTON_SQUARE))
        {
            changeValue(page, cursors[page], -10);
            changed = true;
        }
        if (isKeyDownRepeat(InputButtons::BUTTON_CIRCLE))
        {
            changeValue(page, cursors[page], +10);
            changed = true;
        }
        if (isKeyDownRepeat(InputButtons::BUTTON_LEFT))
        {
            changeValue(page, cursors[page], -1);
            changed = true;
        }
        if (isKeyDownRepeat(InputButtons::BUTTON_RIGHT))
        {
            changeValue(page, cursors[page], +1);
            changed = true;
        }

        if (changed)
        {
            playSound(0, 2);
            clampScroll();
            updateRows();
            pstatTab.setState(page != 0);
            triggerTab.setState(page != 1);
            statsTab.setState(page != 2);
        }
    }

    void DebugMenu::render(int32_t depth)
    {
        pstatTab.render(depth);
        triggerTab.render(depth);
        statsTab.render(depth);

        for (const auto& row : rows)
            row.render(depth);

        drawLine2P(0xc59f4a, LIST_X, DATA_Y - 2, LIST_X + LIST_W, DATA_Y - 2, 4, 0);

        auto cursorRow = cursors[page] - scrolls[page];
        if (cursorRow >= 0 && cursorRow < VISIBLE_ROWS)
            renderSelectionCursor(LIST_X + 2, DATA_Y - 2 + cursorRow * ROW_HEIGHT, LIST_W - 4, ROW_HEIGHT, 4);
    }

    dtl::unique_ptr<DebugMenu> data;

    void tickPlayerMenu(int32_t instance)
    {
        data->tick();
    }

    void renderPlayerMenu(int32_t instance)
    {
        data->render(5);
    }
} // namespace

void addDebugMenu()
{
    data = dtl::make_unique<DebugMenu>();
    createMenuBox(1, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, 0, tickPlayerMenu, renderPlayerMenu);
}

void removeDebugMenu()
{
    closeUIBoxIfOpen(1);
    data.reset();
}
