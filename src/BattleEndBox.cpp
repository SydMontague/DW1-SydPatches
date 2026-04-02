#include "Battle.h"
#include "Helper.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "extern/BTL.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/stddef.hpp"

namespace
{
    void tickBitBox(int32_t)
    {
        if (UI_BOX_DATA[2].state != 1) return;

        if (BITS_TO_GAIN == 0)
        {
            BTL_tickBattleEndText();
            return;
        }

        if (isKeyDown(InputButtons::BUTTON_CROSS) || isKeyDown(InputButtons::BUTTON_TRIANGLE))
            SHOULD_SKIP_BIT_COUNTING = true;

        if (SHOULD_SKIP_BIT_COUNTING)
        {
            MONEY += BITS_TO_GAIN;
            BITS_TO_GAIN = 0;
        }
        else
        {
            playSound(0, 22);
            BITS_TO_GAIN -= 1;
            MONEY += 1;
        }

        MONEY = min(MONEY, 999999);
        if (BITS_TO_GAIN == 0) playSound(0, 23);
    }

    void renderBitBox(int32_t index)
    {
        uint8_t layer = 6 - index;

        dtl::array<uint8_t, 16> string;
        sprintf(string.data(), "%d", BITS_TO_GAIN);
        TextSprite sprite{
            .font       = &vanillaFont,
            .string     = reinterpret_cast<char*>(string.data()),
            .uvX        = 704 + 0,
            .uvY        = 256 + 180,
            .uvWidth    = 15,
            .uvHeight   = 12,
            .posX       = -18,
            .posY       = 28,
            .boxWidth   = 60,
            .boxHeight  = 12,
            .alignmentX = AlignmentX::LEFT,
            .alignmentY = AlignmentY::CENTER,
            .color      = 2,
            .layer      = layer,
        };

        auto previous_color = COLORCODE_LOWBITS;
        setTextColor(1);
        drawTextSprite(sprite);
        setTextColor(previous_color);
        renderTextSprite(sprite);
        renderStringNew(4, -78, 28, 48, 12, 704, 256 + 0x48, layer, 0);

        if (BTL_END_BOX_TEXTBUFFER[0] != 0)
        {
            drawLine2P(0x8e8e8e, -86, 50, 84, 50, layer, 0);
            drawLine2P(0x121212, -85, 51, 85, 51, layer, 0);
            BTL_renderBattleEndText(1);
        }
    }

    void renderFinalBalance(int32_t)
    {
        dtl::array<uint8_t, 16> string;
        sprintf(string.data(), "%d", MONEY);
        TextSprite sprite{
            .font       = &vanillaFont,
            .string     = reinterpret_cast<char*>(string.data()),
            .uvX        = 704 + 16,
            .uvY        = 256 + 180,
            .uvWidth    = 15,
            .uvHeight   = 12,
            .posX       = static_cast<int16_t>(UI_BOX_DATA[2].finalPos.x + 60),
            .posY       = static_cast<int16_t>(UI_BOX_DATA[2].finalPos.y + 10),
            .boxWidth   = 60,
            .boxHeight  = 12,
            .alignmentX = AlignmentX::LEFT,
            .alignmentY = AlignmentY::CENTER,
            .color      = 0,
            .layer      = 4,
        };

        auto previous_color = COLORCODE_LOWBITS;
        setTextColor(1);
        drawTextSprite(sprite);
        setTextColor(previous_color);
        renderTextSprite(sprite);
        renderStringNew(0,
                        UI_BOX_DATA[2].finalPos.x + 10,
                        UI_BOX_DATA[2].finalPos.y + 10,
                        48,
                        12,
                        704,
                        256 + 0x48,
                        4,
                        0);
    }
} // namespace

extern "C"
{
    void createBitsBox()
    {
        constexpr uint8_t bitsString[5] = "Bits";

        auto& work     = TAMER_ENTITY.posData[1].posMatrix.work.t;
        auto entityPos = getScreenPosition(work[0], work[1], work[2]);
        const RECT startPos{
            .x      = static_cast<int16_t>(entityPos.screenX - 5),
            .y      = static_cast<int16_t>(entityPos.screenY - 5),
            .width  = 10,
            .height = 10,
        };
        const RECT finalPos{
            .x      = -88,
            .y      = 18,
            .width  = 176,
            .height = static_cast<int16_t>(BTL_END_BOX_TEXTBUFFER[0] == 0 ? 31 : 66),
        };

        createAnimatedUIBox(1, 0, 2, &finalPos, &startPos, tickBitBox, renderBitBox);
        drawStringNew(&vanillaFont, bitsString, 704, 256 + 0x48);
    }

    void createFinalBalanceBox()
    {
        const RECT startPos = UI_BOX_DATA[1].startPos;
        const RECT finalPos{
            .x      = -88,
            .y      = -13,
            .width  = 176,
            .height = 31,
        };
        createAnimatedUIBox(2, 1, 0, &finalPos, &startPos, nullptr, renderFinalBalance);
    }

    void handleBattleEndBox()
    {
        dtl::array<ItemType, 3> droppedItems{};
        // vanilla initializes the unused bit text, we don't need to
        BITS_TO_GAIN = 0;
        for (int32_t i = 1; i <= ENEMY_COUNT; i++)
            BITS_TO_GAIN += NPC_ENTITIES[COMBAT_DATA_PTR->player.entityIds[i] - 2].bits;
        battleStatsGainsAndDrops(droppedItems.data());
        RECT boxPosition{
            .x      = -78,
            .y      = 54,
            .width  = 156,
            .height = 24,
        };
        BTL_initializeBattleEndText(96, 2, &boxPosition);

        for (int32_t i = 0; i < droppedItems.size(); i++)
        {
            if (droppedItems[i] == ItemType::NONE) continue;
            BTL_appendItemDroppedText(ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[i + 1]));
        }

        if (!PARTNER_PARA.condition.isInjured)
        {
            if (HAS_TAKEN_DAMAGE == 1) handleBattleInjury();
            if (PARTNER_PARA.condition.isInjured) BTL_appendInjuredText(getDigimonData(PARTNER_ENTITY.type)->name);
        }

        BTL_appendCommandLearnedText();
        BTL_appendMPBonusText();
        battleMoveLearning();
        GAME_STATE = 2;
        createPostBattleStatsBox();
        createBitsBox();

        while (UI_BOX_DATA[0].state != 1 || UI_BOX_DATA[1].state != 1)
            BTL_battleTickFrame();

        if (BTL_END_BOX_TEXTBUFFER[0] != 0)
        {
            for (int32_t i = 0; i < 2; i++)
            {
                BTL_drawBattleEndText(1);
                BTL_battleTickFrame();
            }
        }

        while (!STATS_GAINS.isAllZero() || POST_BATTLE_STATS_TIMER != 0)
            BTL_battleTickFrame();

        createFinalBalanceBox();
        while (UI_BOX_DATA[2].state != 1)
            BTL_battleTickFrame();

        while (BITS_TO_GAIN != 0)
            BTL_battleTickFrame();

        SHOULD_SKIP_BIT_COUNTING = 0;
        while (!BTL_isEndBoxTextFinished())
            BTL_battleTickFrame();

        for (int32_t i = 0; i < droppedItems.size(); i++)
        {
            if (droppedItems[i] == ItemType::NONE) continue;

            spawnDroppedItem(ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[i + 1]), droppedItems[i]);
        }

        resetStatsAfterCombat();
        removeBattleEndBox(0);
        removeBattleEndBox(1);
        removeBattleEndBox(2);
    }
}