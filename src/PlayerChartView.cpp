
#include "PlayerChartView.hpp"

#include "AtlasFont.hpp"
#include "DigimonData.hpp"
#include "Helper.hpp"
#include "Input.hpp"
#include "Math.hpp"
#include "Sound.hpp"
#include "TechView.hpp"
#include "UIBox.hpp"
#include "UIElements.hpp"
#include "extern/dw1.hpp"

// TODO improve detail view, ideas:
// jump to next Digimon from within detail view
// display special paths?
// display Digimon info

namespace
{
    constexpr auto CHART_COL_COUNT      = 12;
    constexpr auto CHART_ENTRY_BASE_Y   = toRelativeY(43);
    constexpr auto CHART_ENTRY_OFFSET_Y = 19;

    struct ChartBoxColumn
    {
        int16_t posX;
        DigimonType digimon[9];

        void render(DigimonType selectedDigimon) const
        {
            for (int32_t i = 0; i < 9; i++)
            {
                auto posY        = CHART_ENTRY_BASE_Y + CHART_ENTRY_OFFSET_Y * i;
                DigimonType type = digimon[i];
                if (type == DigimonType::INVALID) continue;

                if (hasValidSprite(type) && hasDigimonRaised(type))
                {
                    auto frame = 0;
                    if (selectedDigimon == type && PLAYTIME_FRAMES % 10 < 5) frame = 1;
                    getDigimonSprite(type).render(posX, posY, 5, 0, frame);
                }
                renderBorderBox(posX - 1, posY - 1, 17, 17, 0xBEBEBE, 0x3C3C3C, 0x87, 0x87, 0x87, 5);
            }
        }
    };

    struct ChartSprite
    {
        int16_t posX;
        int16_t posY;

        [[gnu::optimize("Os")]]
        void render(uint8_t texX, uint8_t texY, uint16_t clut, uint16_t tpage, int32_t layer, bool spriteVisible) const
        {
            if (spriteVisible) renderRectPolyFT4(posX, posY, 16, 16, texX, texY, tpage, clut, layer, 0);
            // vanilla is 18x18, resulting in a 19x19 box
            renderBorderBox(posX - 1, posY - 1, 17, 17, 0xBEBEBE, 0x3C3C3C, 0x87, 0x87, 0x87, layer);
        }
    };

    constexpr BorderBox BOXES[] = {
        {
            .posX   = toRelativeX(17),
            .posY   = toRelativeY(39),
            .width  = 25,
            .height = 175,
            .color1 = 0xaaa0c8,
            .color2 = 0x5a3c8c,
            .red    = 0xa5,
            .green  = 0x5a,
            .blue   = 0x73,
            .layer  = 5,
        },
        {
            .posX   = toRelativeX(45),
            .posY   = toRelativeY(39),
            .width  = 25,
            .height = 175,
            .color1 = 0xa078a5,
            .color2 = 0x46144b,
            .red    = 0x6e,
            .green  = 0x2d,
            .blue   = 0x5a,
            .layer  = 5,
        },
        {
            .posX   = toRelativeX(73),
            .posY   = toRelativeY(39),
            .width  = 25,
            .height = 175,
            .color1 = 0xc88250,
            .color2 = 0x5f370a,
            .red    = 0x0f,
            .green  = 0x50,
            .blue   = 0x82,
            .layer  = 5,
        },
        {
            .posX   = toRelativeX(101),
            .posY   = toRelativeY(39),
            .width  = 109,
            .height = 175,
            .color1 = 0x6eaf5a,
            .color2 = 0x285a00,
            .red    = 0x00,
            .green  = 0x87,
            .blue   = 0x41,
            .layer  = 5,
        },
        {
            .posX   = toRelativeX(214),
            .posY   = toRelativeY(39),
            .width  = 88,
            .height = 175,
            .color1 = 0x6996d2,
            .color2 = 0x1e4178,
            .red    = 0xaf,
            .green  = 0x64,
            .blue   = 0x2d,
            .layer  = 5,
        },
    };

    constexpr Sprite SELECTOR = {
        .uvX          = 0,
        .uvV          = 0xE8,
        .width        = 24,
        .height       = 20,
        .texture_page = 24,
        .clut         = 0x7DC7,
    };

    constexpr ChartBoxColumn COLS[] = {
        {
            .posX = toRelativeX(22),
            .digimon =
                {
                    DigimonType::BOTAMON,
                    DigimonType::INVALID,
                    DigimonType::PUNIMON,
                    DigimonType::INVALID,
                    DigimonType::POYOMON,
                    DigimonType::INVALID,
                    DigimonType::YURAMON,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                },
        },
        {
            .posX = toRelativeX(50),
            .digimon =
                {
                    DigimonType::KOROMON,
                    DigimonType::INVALID,
                    DigimonType::TSUNOMON,
                    DigimonType::INVALID,
                    DigimonType::TOKOMON,
                    DigimonType::INVALID,
                    DigimonType::TANEMON,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                },
        },
        {
            .posX = toRelativeX(78),
            .digimon =
                {
                    DigimonType::AGUMON,
                    DigimonType::GABUMON,
                    DigimonType::PATAMON,
                    DigimonType::ELECMON,
                    DigimonType::BIYOMON,
                    DigimonType::KUNEMON,
                    DigimonType::PALMON,
                    DigimonType::BETAMON,
                    DigimonType::PENGUINMON,
                },
        },
        {
            .posX = toRelativeX(106),
            .digimon =
                {
                    DigimonType::GREYMON,
                    DigimonType::MONOCHROMON,
                    DigimonType::OGREMON,
                    DigimonType::AIRDRAMON,
                    DigimonType::KUWAGAMON,
                    DigimonType::WHAMON,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                },
        },
        {
            .posX = toRelativeX(127),
            .digimon =
                {
                    DigimonType::MERAMON,
                    DigimonType::DRIMOGEMON,
                    DigimonType::LEOMON,
                    DigimonType::KOKATORIMON,
                    DigimonType::VEGIEMON,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                },
        },
        {
            .posX = toRelativeX(148),
            .digimon =
                {
                    DigimonType::BIRDRAMON,
                    DigimonType::TYRANNOMON,
                    DigimonType::ANGEMON,
                    DigimonType::UNIMON,
                    DigimonType::NINJAMON,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                    DigimonType::NUMEMON,
                },
        },
        {
            .posX = toRelativeX(169),
            .digimon =
                {
                    DigimonType::CENTARUMON,
                    DigimonType::DEVIMON,
                    DigimonType::BAKEMON,
                    DigimonType::KABUTERIMON,
                    DigimonType::SEADRAMON,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                    DigimonType::SUKAMON,
                },
        },
        {
            .posX = toRelativeX(190),
            .digimon =
                {
                    DigimonType::GARURUMON,
                    DigimonType::COELAMON,
                    DigimonType::SHELLMON,
                    DigimonType::MOJYAMON,
                    DigimonType::FRIGIMON,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                    DigimonType::NANIMON,
                },
        },
        {
            .posX = toRelativeX(219),
            .digimon =
                {
                    DigimonType::METALGREYMON,
                    DigimonType::INVALID,
                    DigimonType::GIROMON,
                    DigimonType::INVALID,
                    DigimonType::VADEMON,
                    DigimonType::INVALID,
                    DigimonType::HERCULESKABUTERIMON,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                },
        },
        {
            .posX = toRelativeX(240),
            .digimon =
                {
                    DigimonType::ANDROMON,
                    DigimonType::INVALID,
                    DigimonType::MAMEMON,
                    DigimonType::INVALID,
                    DigimonType::DIGITAMAMON,
                    DigimonType::INVALID,
                    DigimonType::PHOENIXMON,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                },
        },
        {
            .posX = toRelativeX(261),
            .digimon =
                {
                    DigimonType::SKULLGREYMON,
                    DigimonType::INVALID,
                    DigimonType::METALMAMEMON,
                    DigimonType::INVALID,
                    DigimonType::ETEMON,
                    DigimonType::INVALID,
                    DigimonType::MEGASEADRAMON,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                },
        },
        {
            .posX = toRelativeX(282),
            .digimon =
                {
                    DigimonType::MEGADRAMON,
                    DigimonType::INVALID,
                    DigimonType::PIXIMON,
                    DigimonType::INVALID,
                    DigimonType::MONZAEMON,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                    DigimonType::INVALID,
                },
        },
    };

    constexpr Inset DETAIL_INSETS[] = {
        {.posX = toRelativeX(28), .posY = toRelativeY(42), .width = 78, .height = 130},
        {.posX = toRelativeX(214), .posY = toRelativeY(42), .width = 78, .height = 130},
        {.posX = toRelativeX(136), .posY = toRelativeY(85), .width = 48, .height = 46},
        {.posX = toRelativeX(129), .posY = toRelativeY(50), .width = 60, .height = 4},
        {.posX = toRelativeX(38), .posY = toRelativeY(187), .width = 108, .height = 4},
        {.posX = toRelativeX(26), .posY = toRelativeY(206), .width = 132, .height = 4},
    };

    constexpr Line4Points FROM_LINE_EVEN[] = {
        {-84, -68, -37, -10, -48, -48, -17, -17},
        {-102, -74, -64, -10, -24, -24, -14, -14},
        {-102, -74, -64, -10, -1, -1, -11, -11},
        {-84, -68, -37, -10, 23, 23, -8, -8},
    };

    constexpr Line4Points FROM_LINE_ODD[] = {
        {-66, -52, -15, -10, -60, -60, -18, -18},
        {-84, -58, -37, -10, -36, -36, -15, -15},
        {-102, -10, -10, -10, -12, -12, -12, -12},
        {-84, -58, -37, -10, 12, 12, -9, -9},
        {-66, -52, -15, -10, 36, 36, -6, -6},
    };

    constexpr Line4Points TO_LINES_ODD[] = {
        {9, 51, 64, 64, -18, -60, -60, -60},
        {9, 36, 57, 82, -15, -15, -36, -36},
        {9, 100, 100, 100, -12, -12, -12, -12},
        {9, 36, 57, 82, -9, -9, 12, 12},
        {9, 51, 64, 64, -6, 36, 36, 36},
    };

    constexpr Line4Points TO_LINES_EVEN[] = {
        {9, 61, 64, 64, -20, -72, -72, -72},
        {9, 36, 67, 82, -17, -17, -48, -48},
        {9, 63, 73, 100, -14, -14, -24, -24},
        {9, 63, 73, 100, -11, -11, -1, -1},
        {9, 36, 67, 82, -8, -8, 23, 23},
        {9, 61, 64, 64, -5, 47, 47, 47},
    };

    constexpr RGB8 TO_COLORS[][2] = {
        {{0xFB, 0xE8, 0x02}, {0xEC, 0x9A, 0x00}},
        {{0x00, 0xB6, 0x00}, {0x00, 0x64, 0x00}},
        {{0x00, 0xAE, 0xB8}, {0x00, 0x6F, 0x8E}},
        {{0x00, 0x3C, 0xC8}, {0x00, 0x1E, 0x8C}},
        {{0xBE, 0x00, 0xA0}, {0x6E, 0x00, 0x6E}},
        {{0xBE, 0x00, 0x1E}, {0x82, 0x1E, 0x1E}},
    };

    constexpr ChartSprite CENTER_SPRITE = {.posX = -8, .posY = -20};
    constexpr ChartSprite FROM_ODD[]    = {{-83, -68}, {-101, -44}, {-119, -20}, {-101, 4}, {-83, 28}};
    constexpr ChartSprite FROM_EVEN[]   = {{-101, -56}, {-119, -32}, {-119, -8}, {-101, 16}};
    constexpr ChartSprite TO_ODD[]      = {{66, -69}, {84, -45}, {102, -21}, {84, 3}, {66, 27}};
    constexpr ChartSprite TO_EVEN[]     = {{66, -81}, {84, -57}, {102, -33}, {102, -9}, {84, 15}, {66, 39}};

    [[gnu::optimize("Os")]]
    constexpr char const* getLevelName(Level level)
    {
        switch (level)
        {
            case Level::FRESH: return "Fresh";
            case Level::IN_TRAINING: return "In-Training";
            case Level::ROOKIE: return "Rookie";
            case Level::CHAMPION: return "Champion";
            case Level::ULTIMATE: return "Ultimate";
            case Level::UNDEFINED:
            default: return "Unkown";
        }
    }

    constexpr RenderSettings TREE_LABEL{
        .x        = -20,
        .y        = -79,
        .baseClut = 1,
        .color    = TEXT_COLOR_LIGHT_YELLOW,
        .width    = 36,
        .height   = 12,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::CENTER,
    };

    constexpr RenderSettings LEVEL_LABEL{
        .x        = -100,
        .y        = 58,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 70,
        .height   = 12,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::CENTER,
    };

    constexpr RenderSettings NAME_LABEL{
        .x        = -130,
        .y        = 77,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = 120,
        .height   = 12,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::CENTER,
    };
} // namespace

struct ChartView::Private
{
public:
    [[gnu::optimize("Os")]]
    void tick();
    void render(int32_t depth);
    bool canBeClosed();

private:
    inline void renderChart(int32_t depth);
    inline void renderDetail(int32_t depth);

    UIBox detailView;
    AtlasString titleString = getAtlasVanilla().render("Tree", TREE_LABEL);
    AtlasString levelString;
    AtlasString nameString;
    int16_t selectedCol{0};
    int16_t selectedRow{0};
    DigimonType selectedDigimon{DigimonType::INVALID};

    int8_t state{0};
};

bool ChartView::Private::canBeClosed()
{
    return state == 0;
}

void ChartView::Private::render(int32_t depth)
{
    renderChart(depth);
    renderDetail(depth - 1);
    detailView.render(depth - 1);
}

void ChartView::Private::renderChart(int32_t depth)
{
    if (state != 0)
        SELECTOR.render(COLS[selectedCol].posX - 4, CHART_ENTRY_BASE_Y + CHART_ENTRY_OFFSET_Y * selectedRow - 2, 5, 0);

    for (auto& col : COLS)
        col.render(selectedDigimon);

    for (auto& box : BOXES)
        box.render();

    detailView.tick();
}

void ChartView::Private::renderDetail(int32_t depth)
{
    if (detailView.getState() != UIBox::State::OPENED) return;

    uint32_t toCount   = 0;
    uint32_t fromCount = 0;
    auto& paths        = EVO_PATHS_DATA[static_cast<int32_t>(selectedDigimon)];

    for (auto val : paths.from)
    {
        if (val != 0xFF) fromCount++;
    }
    for (auto val : paths.to)
    {
        if (val != 0xFF) toCount++;
    }

    auto& data = getDigimonSprite(selectedDigimon);

    CENTER_SPRITE.render(data.uvX, data.uvV, DIGIMON_SPRITE_CLUTS[data.clut], data.tpage, 4, true);

    auto* toLines        = (toCount & 1) == 0 ? TO_LINES_EVEN : TO_LINES_ODD;
    auto* toSprite       = (toCount & 1) == 0 ? TO_EVEN : TO_ODD;
    auto toSpriteCount   = (toCount & 1) == 0 ? 6 : 5;
    auto* fromLines      = (fromCount & 1) == 0 ? FROM_LINE_EVEN : FROM_LINE_ODD;
    auto* fromSprite     = (fromCount & 1) == 0 ? FROM_EVEN : FROM_ODD;
    auto fromSpriteCount = (fromCount & 1) == 0 ? 4 : 5;

    for (int32_t i = 0; i < toSpriteCount; i++)
    {
        auto digi = paths.to[i];
        if (digi == 0xFF) continue;
        auto type  = static_cast<DigimonType>(digi);
        auto& data = getDigimonSprite(type);

        toSprite[i]
            .render(data.uvX, data.uvV, DIGIMON_SPRITE_CLUTS[data.clut], data.tpage, depth, hasDigimonRaised(type));
        toLines[i].render(TO_COLORS[i][1].asUint32(), TO_COLORS[i][0].asUint32(), depth);
    }

    for (int32_t i = 0; i < fromSpriteCount; i++)
    {
        auto digi = paths.from[i];
        if (digi == 0xFF) continue;
        auto type  = static_cast<DigimonType>(digi);
        auto& data = getDigimonSprite(type);

        fromSprite[i]
            .render(data.uvX, data.uvV, DIGIMON_SPRITE_CLUTS[data.clut], data.tpage, depth, hasDigimonRaised(type));
        fromLines[i].render(0x65db, 0x794e3, depth);
    }

    titleString.render(depth);
    levelString.render(depth);
    nameString.render(depth);

    for (auto& val : DETAIL_INSETS)
        val.render(depth);
}

void ChartView::Private::tick()
{
    if (state == 2)
    {
        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            detailView.close();
            playSound(0, 3);
            state = 1;
        }
    }

    else if (state == 1)
    {
        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            playSound(0, 4);
            state           = 0;
            selectedDigimon = DigimonType::INVALID;
            return;
        }
        auto initialCol = selectedCol;
        auto initialRow = selectedRow;

        if (isKeyDownRepeat(InputButtons::BUTTON_RIGHT)) selectedCol = min(selectedCol + 1, CHART_COL_COUNT - 1);
        if (isKeyDownRepeat(InputButtons::BUTTON_LEFT)) selectedCol = max(selectedCol - 1, 0);

        auto rowAmount = 0;
        if (isKeyDownRepeat(InputButtons::BUTTON_UP)) rowAmount = -1;
        if (isKeyDownRepeat(InputButtons::BUTTON_DOWN)) rowAmount = 1;
        if (COLS[selectedCol].digimon[selectedRow] == DigimonType::INVALID) rowAmount = -1;

        auto tmpRow = selectedRow;
        while (rowAmount != 0 && tmpRow + rowAmount >= 0 && tmpRow + rowAmount < 9)
        {
            tmpRow += rowAmount;
            if (COLS[selectedCol].digimon[tmpRow] != DigimonType::INVALID)
            {
                selectedRow = tmpRow;
                break;
            }
        }

        if (initialCol != selectedCol || initialRow != selectedRow) playSound(0, 2);

        selectedDigimon = COLS[selectedCol].digimon[selectedRow];

        if (isKeyDown(InputButtons::BUTTON_CROSS) && hasDigimonRaised(selectedDigimon))
        {
            constexpr RECT FINAL = {.x = -150, .y = -89, .width = 300, .height = 190};
            int16_t posX         = COLS[selectedCol].posX;
            int16_t posY         = CHART_ENTRY_BASE_Y + CHART_ENTRY_OFFSET_Y * selectedRow;
            RECT start           = {
                .x      = static_cast<int16_t>(posX),
                .y      = static_cast<int16_t>(posY),
                .width  = 10,
                .height = 10,
            };

            auto* para  = getDigimonData(selectedDigimon);
            nameString  = getAtlasVanilla().render(getDigimonData(selectedDigimon)->name, NAME_LABEL);
            levelString = getAtlasVanilla().render(getLevelName(para->level), LEVEL_LABEL);
            detailView  = UIBox(FINAL, UIBox::DEFAULT_COLOR, false, start);
            state       = 2;
        }
    }
    else if (state == 0)
    {
        if (isKeyDown(InputButtons::BUTTON_CROSS))
        {
            state = 1;
            playSound(0, 3);
        }
    }
}

ChartView::ChartView()
    : impl(dtl::make_unique<Private>())
{
}

ChartView::~ChartView() = default;

bool ChartView::canBeClosed()
{
    return impl->canBeClosed();
}

void ChartView::render(int32_t depth)
{
    impl->render(depth);
}

void ChartView::tick()
{
    impl->tick();
}