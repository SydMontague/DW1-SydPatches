#include "PlayerCardView.hpp"

#include "AtlasFont.hpp"
#include "DigimonData.hpp"
#include "Files.hpp"
#include "Helper.hpp"
#include "Input.hpp"
#include "Math.hpp"
#include "Sound.hpp"
#include "UIBox.hpp"
#include "UIElements.hpp"
#include "extern/dtl/unique_ptr.hpp"
#include "extern/dw1.hpp"

namespace
{
    constexpr auto CARD_ROW_COUNT = 6;
    constexpr auto CARD_COL_COUNT = 11;
    constexpr auto CARD_BASE_X    = toRelativeX(30);
    constexpr auto CARD_BASE_Y    = toRelativeY(58);
    constexpr auto CARD_OFFSET_X  = 24;
    constexpr auto CARD_OFFSET_Y  = 24;
    constexpr RECT CARD_BOX       = {.x = -75, .y = -83, .width = 150, .height = 180};
    constexpr RECT COUNT_BOX      = {.x = 74, .y = 69, .width = 54, .height = 24};

    constexpr dtl::array<Pair<const char*, RenderSettings>, 5> LABELS = {{
        "Card List",
        {
            .x        = toRelativeX(14),
            .y        = toRelativeY(34),
            .baseClut = 1,
            .color    = TEXT_COLOR_YELLOW,
            .width    = 292,
            .height   = 19,
            .alignX   = AlignmentX::CENTER,
            .alignY   = AlignmentY::CENTER,
        },

        "\x81\xa2",
        {
            .x        = toRelativeX(120),
            .y        = toRelativeY(202),
            .baseClut = 1,
            .color    = TEXT_COLOR_X_BLUE,
            .width    = 20,
            .height   = 16,
            .alignX   = AlignmentX::RIGHT,
            .alignY   = AlignmentY::CENTER,
        },
        "Select",
        {
            .x        = toRelativeX(140),
            .y        = toRelativeY(202),
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 64,
            .height   = 16,
            .alignX   = AlignmentX::LEFT,
            .alignY   = AlignmentY::CENTER,
        },

        "\x81\x7e",
        {
            .x        = toRelativeX(204),
            .y        = toRelativeY(202),
            .baseClut = 1,
            .color    = TEXT_COLOR_GREEN,
            .width    = 20,
            .height   = 16,
            .alignX   = AlignmentX::RIGHT,
            .alignY   = AlignmentY::CENTER,
        },
        "Cancel",
        {
            .x        = toRelativeX(224),
            .y        = toRelativeY(202),
            .baseClut = 1,
            .color    = TEXT_COLOR_WHITE,
            .width    = 64,
            .height   = 16,
            .alignX   = AlignmentX::LEFT,
            .alignY   = AlignmentY::CENTER,
        },

    }};

    constexpr RenderSettings CARD_COUNT_SETTINGS{
        .x        = COUNT_BOX.x,
        .y        = COUNT_BOX.y,
        .baseClut = 1,
        .color    = TEXT_COLOR_WHITE,
        .width    = COUNT_BOX.width,
        .height   = COUNT_BOX.height,
        .alignX   = AlignmentX::CENTER,
        .alignY   = AlignmentY::CENTER,
    };

    constexpr Sprite selector = {
        .uvX          = 0,
        .uvV          = 0xE8,
        .width        = 24,
        .height       = 20,
        .texture_page = 24,
        .clut         = 0x7DC7,
    };

    constexpr void loadCardImage(int32_t card)
    {
        loadStackedTIMEntry("\\CARD\\CARD.ALL", TEXTURE_BUFFER, card * 14, 14);
    }

    constexpr dtl::array<AtlasString, 5> getLabels()
    {
        dtl::array<AtlasString, 5> array;
        int32_t i = 0;
        for(const auto& label : LABELS)
            array[i++] = getAtlasVanilla().render(LABELS[i].first, LABELS[i].second);
        return array;
    }
} // namespace

struct CardView::Private
{
public:
    void tick();
    // NOLINTNEXTLINE
    __attribute__((optimize("Os"))) void render(int32_t depth);
    bool canBeClosed();

private:
    inline void renderCardCount(int32_t depth);
    inline void renderCard(int32_t depth);

    dtl::array<AtlasString, 5> labels{getLabels()};
    AtlasString cardCount;
    UIBox cardImageBox;
    UIBox cardCountBox;
    int8_t state{0};
    int8_t selectedCardCol{0};
    int8_t selectedCardRow{0};
};

bool CardView::Private::canBeClosed()
{
    return state == 0;
}

void CardView::Private::tick()
{
    cardCountBox.tick();
    cardImageBox.tick();

    if (state == 2)
    {
        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            playSound(0, 4);
            playSound(0, 1);
            state = 1;
            cardCountBox.close();
            cardImageBox.close();
        }
    }
    else if (state == 1)
    {
        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            playSound(0, 4);
            state = 0;
        }

        auto oldCard = selectedCardRow * CARD_COL_COUNT + selectedCardCol;
        if (isKeyDownRepeat(InputButtons::BUTTON_LEFT)) selectedCardCol -= 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_RIGHT)) selectedCardCol += 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_UP)) selectedCardRow -= 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_DOWN)) selectedCardRow += 1;
        selectedCardCol = clamp(selectedCardCol, 0, CARD_COL_COUNT - 1);
        selectedCardRow = clamp(selectedCardRow, 0, CARD_ROW_COUNT - 1);

        auto newCard = selectedCardRow * CARD_COL_COUNT + selectedCardCol;
        if (oldCard != newCard) playSound(0, 2);

        // vanilla doesn't check for the boxes to be available, causing the card glitch
        // but since we don't use the vanilla boxes anymore we avoid this bug
        if (isKeyDown(InputButtons::BUTTON_CROSS) && getCardAmount(newCard) != 0)
        {
            playSound(0, 3);
            state = 2;
            loadCardImage(newCard);
            cardCount = getAtlasVanilla().render(format("%d", getCardAmount(newCard)).data(), CARD_COUNT_SETTINGS);
            RECT start = {
                .x      = static_cast<int16_t>(selectedCardCol * 24 + CARD_BASE_X + 9),
                .y      = static_cast<int16_t>(selectedCardRow * 24 + CARD_BASE_Y + 8),
                .width  = 1,
                .height = 1,
            };

            playSound(0, 0);
            cardImageBox = UIBox(CARD_BOX, {}, false, start);
            cardCountBox = UIBox(COUNT_BOX, UIBox::DEFAULT_COLOR, false, start);
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

void CardView::Private::renderCard(int32_t depth)
{
    constexpr auto clut  = getClut(0x1C0, 0x1FF);
    constexpr auto tpage = getTPage(1, 2, 0x240, 0x100);
    renderRectPolyFT4(-75, -84, 150, 180, 0, 0, tpage, clut, depth, 0);
}

void CardView::Private::renderCardCount(int32_t depth)
{
    cardCount.render(depth);
}

void CardView::Private::render(int32_t depth)
{
    if (cardCountBox.getState() == UIBox::State::OPENED) renderCardCount(depth - 1);
    if (cardImageBox.getState() == UIBox::State::OPENED) renderCard(depth - 1);
    cardCountBox.render(depth - 1);
    cardImageBox.render(depth - 1);

    if (state != 0)
        selector.render(CARD_BASE_X + selectedCardCol * CARD_OFFSET_X - 4,
                        CARD_BASE_Y + selectedCardRow * CARD_OFFSET_Y - 2,
                        depth,
                        0);

    for (int row = 0; row < CARD_ROW_COUNT; row++)
        for (int col = 0; col < CARD_COL_COUNT; col++)
        {
            auto cardId = row * CARD_COL_COUNT + col;
            auto posX   = CARD_BASE_X + col * CARD_OFFSET_X;
            auto posY   = CARD_BASE_Y + row * CARD_OFFSET_Y;

            if (getCardAmount(cardId) > 0)
            {
                auto type = CARD_DATA[cardId].type;
                if (type != 0xFF) getDigimonSprite(static_cast<DigimonType>(type)).render(posX, posY, depth, 0, 0);
            }

            renderBorderBox(posX - 1, posY - 1, 17, 17, 0xBEBEBE, 0x3C3C3C, 0x67, 0x67, 0x67, depth);
        }

    for (const auto& label : labels)
        label.render(depth);
}

CardView::CardView()
    : impl(dtl::make_unique<Private>())
{
}

CardView::~CardView() = default;

bool CardView::canBeClosed()
{
    return impl->canBeClosed();
}

void CardView::render(int32_t depth)
{
    impl->render(depth);
}

void CardView::tick()
{
    impl->tick();
}