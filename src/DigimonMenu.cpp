#include "Entity.hpp"
#include "Font.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "StatsView.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"

constexpr auto PLAYER_MENU_TAB_DRAW_X = 704;
constexpr auto PLAYER_MENU_TAB_DRAW_Y = 256;

static TextSprite statusLabel = {
    .font       = &vanillaFont,
    .string     = "Status",
    .uvX        = PLAYER_MENU_TAB_DRAW_X + 0,
    .uvY        = PLAYER_MENU_TAB_DRAW_Y + 0,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = -138,
    .posY       = -101,
    .boxWidth   = 60,
    .boxHeight  = 12,
    .alignmentX = AlignmentX::CENTER,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
};

static TextSprite techLabel = {
    .font       = &vanillaFont,
    .string     = "Tech",
    .uvX        = PLAYER_MENU_TAB_DRAW_X + 16,
    .uvY        = PLAYER_MENU_TAB_DRAW_Y + 0,
    .uvWidth    = 0,
    .uvHeight   = 0,
    .posX       = -63,
    .posY       = -101,
    .boxWidth   = 48,
    .boxHeight  = 12,
    .alignmentX = AlignmentX::CENTER,
    .alignmentY = AlignmentY::CENTER,
    .color      = 0,
    .layer      = 5,
    .hasShadow  = 1,
};

constexpr int32_t MOVE_ROW_COUNT  = 7;
constexpr int32_t MOVE_COL_COUNT  = 8;
constexpr uint32_t TYPE_MAPPING[] = {0, 5, 1, 4, 2, 3, 6, 7, 8};

static int8_t moveSelectedRow;
static int8_t moveSelectedColumn;
static uint8_t digimonMenuState;

static int32_t getMoveIndex(int32_t selectedMove)
{
    auto* para = getDigimonData(PARTNER_ENTITY.type);
    for (int32_t i = 0; i < 16; i++)
    {
        auto move = para->moves[i];
        if (selectedMove == move) return i;
        if (selectedMove == 44 && move == 48) return i;
        if (selectedMove == 55 && move == 57) return i;
    }
    return -1;
}

static int32_t getFirstFreeSlot()
{
    for (int32_t i = 0; i < 3; i++)
    {
        if (EQUIPPED_MOVES[i] == 0xFF) return i;
    }

    return -1;
}

static void equipMove()
{
    auto row          = TYPE_MAPPING[moveSelectedRow];
    auto selectedMove = row * 8 + moveSelectedColumn;
    if (selectedMove > 47) selectedMove += 1;

    if (!hasMove(selectedMove))
    {
        playSound(0, 4);
        return;
    }

    auto moveIndex = getMoveIndex(selectedMove);
    if (moveIndex == -1)
    {
        playSound(0, 4);
        return;
    }

    auto slot = getFirstFreeSlot();
    if (slot == -1)
    {
        playSound(0, 4);
        return;
    }

    EQUIPPED_MOVES[slot]             = selectedMove;
    PARTNER_ENTITY.stats.moves[slot] = moveIndex + 0x2E;
    RECT rect                        = {.x = 0, .y = static_cast<int16_t>(slot * 12 + 24), .width = 0x84, .height = 12};
    clearTextSubArea(&rect);
    drawStringNew(&vanillaFont, MOVE_NAMES[selectedMove], 704, 256 + slot * 12 + 24);
    playSound(0, 3);
}

static int32_t getEquippedSlot()
{
    auto row  = TYPE_MAPPING[moveSelectedRow];
    auto move = row * 8 + moveSelectedColumn;
    if (move > 47) move += 1;

    for (int32_t i = 0; i < 3; i++)
    {
        auto equipped = EQUIPPED_MOVES[i];
        if (move == equipped) return i;
        if (move == 44 && equipped == 48) return i;
        if (move == 55 && equipped == 57) return i;
    }

    return -1;
}

static void tickDigimonMenuTechs()
{
    if (MENU_STATE == 1 && isKeyDown(InputButtons::BUTTON_CROSS))
    {
        MENU_STATE = 2;
        playSound(0, 3);
    }

    if (MENU_STATE == 6)
    {
        if (isKeyDown(InputButtons::BUTTON_CROSS))
        {
            auto slot = getEquippedSlot();
            if (slot == -1)
                equipMove();
            else
            {
                EQUIPPED_MOVES[slot]             = 0xFF;
                PARTNER_ENTITY.stats.moves[slot] = 0xFF;
                playSound(0, 3);
            }
        }
        if (isKeyDown(InputButtons::BUTTON_SQUARE))
        {
            MENU_STATE     = 7;
            MENU_SUB_STATE = 0;
            playSound(0, 3);
        }
        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            if (hasAttackEquipped(&PARTNER_ENTITY)) MENU_STATE = 4;
            playSound(0, 4);
        }

        auto oldRow = moveSelectedRow;
        auto oldCol = moveSelectedColumn;
        if (isKeyDownRepeat(InputButtons::BUTTON_RIGHT)) moveSelectedColumn += 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_LEFT)) moveSelectedColumn -= 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_DOWN)) moveSelectedRow += 1;
        if (isKeyDownRepeat(InputButtons::BUTTON_UP)) moveSelectedRow -= 1;

        moveSelectedRow    = clamp(moveSelectedRow, 0, MOVE_ROW_COUNT - 1);
        moveSelectedColumn = clamp(moveSelectedColumn, 0, MOVE_COL_COUNT - 1);

        // TODO this belongs into the render
        MOVE_SELECT_BOX_Y = 0x6f + moveSelectedRow * 15;
        MOVE_SELECT_BOX_X = 0x73 + moveSelectedColumn * 18;

        if (moveSelectedRow != oldRow || moveSelectedColumn != oldCol) playSound(0, 2);
    }

    if (MENU_STATE == 8 && isKeyDown(InputButtons::BUTTON_TRIANGLE))
    {
        MENU_STATE     = 9;
        MENU_SUB_STATE = 0;
        playSound(0, 4);
    }
}

void renderDigimonMenu()
{
    if (MENU_STATE == 0)
    {
        clearTextSubArea2(0, 0, 256, 12);
        drawTextSprite(statusLabel);
        drawTextSprite(techLabel);
    }

    if (digimonMenuState == 0)
        renderDigimonStatsView();
    else if (digimonMenuState == 1)
        renderDigimonMovesView();

    statusLabel.color = digimonMenuState != 0 ? 1 : 0;
    techLabel.color   = digimonMenuState != 1 ? 1 : 0;
    renderTextSprite(statusLabel, 0, 0);
    renderTextSprite(techLabel, 0, 0);
    renderMenuTab(-145, 76, digimonMenuState != 0);
    renderMenuTab(-70, 64, digimonMenuState != 1);
}

void tickDigimonMenu()
{
    // stats menu doesn't need special interaction
    if (digimonMenuState == 1) tickDigimonMenuTechs();

    if (MENU_STATE == 1)
    {
        if (isKeyDown(InputButtons::BUTTON_LEFT) && digimonMenuState >= 1)
        {
            digimonMenuState -= 1;
            MENU_STATE     = 0;
            MENU_SUB_STATE = 0;
            playSound(0, 2);
        }
        if (isKeyDown(InputButtons::BUTTON_RIGHT) && digimonMenuState < 1)
        {
            digimonMenuState += 1;
            MENU_STATE     = 0;
            MENU_SUB_STATE = 0;
            playSound(0, 2);
        }

        if (isKeyDown(InputButtons::BUTTON_TRIANGLE))
        {
            TRIANGLE_MENU_STATE = 4;
            playSound(0, 4);
        }
    }

    TAMER_ENTITY.isOnScreen   = false;
    PARTNER_ENTITY.isOnScreen = false;
}

void setDigimonMenuState(int32_t state)
{
    digimonMenuState = state;
}