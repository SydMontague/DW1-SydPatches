#include "AtlasFont.hpp"
#include "Battle.h"
#include "Font.hpp"
#include "Helper.hpp"
#include "Input.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "VanillaText.hpp"
#include "extern/BTL.hpp"
#include "extern/dtl/unique_ptr.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/stddef.hpp"

namespace
{
    struct Data
    {
        dtl::array<AtlasString, 6> statLabels{};
        AtlasString bitString1;
        AtlasString bitString2;
    };

    dtl::unique_ptr<Data> data;

    constexpr dtl::array<const char*, 6> statLabels{"HP", "MP", "Off", "Def", "Speed", "Brain"};
    constexpr auto bitsString = "Bits";

    dtl::array<bool, 6> STAT_BOX_HAS_GAIN;

    constexpr int32_t getLimit(Stat stat)
    {
        switch (stat)
        {
            case Stat::HP:
            case Stat::MP: return 9999;
            default: return 999;
        }
    }

    void tickBitBox(int32_t)
    {
        if (UI_BOX_DATA[2].state != 1) return;

        if (BITS_TO_GAIN == 0)
        {
            BTL_tickBattleEndText();
            return;
        }

        if (isKeyDownPolled(InputButtons::BUTTON_CROSS) || isKeyDownPolled(InputButtons::BUTTON_TRIANGLE))
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
        getAtlasVanilla().renderSlow(string.data(), layer, {.x = -18, .y = 28});
        data->bitString1.render(layer);

        if (BTL_END_BOX_TEXTBUFFER[0] != 0)
        {
            drawLine2P(0x8e8e8e, -86, 50, 84, 50, layer, 0);
            drawLine2P(0x121212, -85, 51, 85, 51, layer, 0);
            BTL_renderBattleEndText(1);
        }
    }

    void renderFinalBalance(int32_t)
    {
        const RenderSettings settings{
            .x = static_cast<int16_t>(UI_BOX_DATA[2].finalPos.x + 58),
            .y = static_cast<int16_t>(UI_BOX_DATA[2].finalPos.y + 10),
        };
        dtl::array<uint8_t, 16> string;
        sprintf(string.data(), "%d", MONEY);
        getAtlasVanilla().renderSlow(string.data(), 4, settings);
        data->bitString2.render(4);
    }

    void renderPostBattleStatsBox(int32_t instance)
    {
        const auto& box  = UI_BOX_DATA[0];
        const auto layer = 6 - instance;

        auto previous_color = COLORCODE_LOWBITS;
        setTextColor(1);
        bool first = true;
        for (int32_t i = 0; i < 6; i++)
        {
            auto stat      = static_cast<Stat>(i);
            auto remaining = STATS_GAINS.get(stat);

            if (remaining == 0) continue;

            if (POST_BATTLE_STATS_TIMER == 0 && first)
            {
                playSound(0, 22);
                INITIAL_COMBAT_STATS[0].set(stat, min(INITIAL_COMBAT_STATS[0].get(stat) + 1, getLimit(stat)));
                STATS_GAINS.set(stat, STATS_GAINS.get(stat) - 1);
                first = false;
            }

            if (STATS_GAINS.get(stat) != 0)
            {
                auto* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
                setEntityTextDigit(prim, 256, 491);
                prim->r0 = 0x80;
                prim->g0 = 0x80;
                prim->b0 = 0x80;
                setUVDataPolyFT4(prim, 0x60, 0xb4, 12, 12);
                setPosDataPolyFT4(prim, box.finalPos.x + 0x82, box.finalPos.y + 9 + i * 13, 12, 12);
                libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + layer, prim);
                libgs_GsSetWorkBase(prim + 1);
            }

            const RenderSettings settings{
                .x = static_cast<int16_t>(box.finalPos.x + 0x8e),
                .y = static_cast<int16_t>(box.finalPos.y + 9 + (i * 13)),
            };
            dtl::array<uint8_t, 8> string;
            sprintf(string.data(), "%d", STATS_GAINS.get(stat));
            getAtlasVanilla().renderSlow(string.data(), layer, settings);
        }

        for (int32_t i = 0; i < 6; i++)
        {
            const RenderSettings settings{
                .x = static_cast<int16_t>(box.finalPos.x + 0x44),
                .y = static_cast<int16_t>(box.finalPos.y + 9 + (i * 13)),
            };
            auto stat = static_cast<Stat>(i);
            dtl::array<uint8_t, 8> string;

            sprintf(string.data(), "%d", INITIAL_COMBAT_STATS[0].get(stat));
            getAtlasVanilla().renderSlow(string.data(), layer, settings);
        }

        setTextColor(previous_color);
        drawLine2P(0xfad990,
                   box.finalPos.x + 0x7a,
                   box.finalPos.y + 2,
                   box.finalPos.x + 0x7a,
                   box.finalPos.y + box.finalPos.height - 3,
                   layer,
                   0);
        drawLine2P(0x20202,
                   box.finalPos.x + 0x7b,
                   box.finalPos.y + 2,
                   box.finalPos.x + 0x7b,
                   box.finalPos.y + box.finalPos.height - 3,
                   layer,
                   0);

        for (const auto& entry : data->statLabels)
            entry.render(layer);

        GsBOXF rect{
            .attribute = 0,
            .x         = -22,
            .height    = 2,
        };

        for (int32_t i = 0; i < 6; i++)
        {
            auto yOffset = box.finalPos.y + 20 + i * 13;
            auto stat    = static_cast<Stat>(i);
            rect.width   = INITIAL_COMBAT_STATS[0].get(stat) * 50 / getLimit(stat);
            rect.y       = yOffset - 2;

            if (STAT_BOX_HAS_GAIN[i])
            {
                rect.r = 0x69;
                rect.g = 0xc2;
                rect.b = 0xff;
                libgs_GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE, layer);
                rect.r = 0x00;
                rect.g = 0x5a;
                rect.b = 0x96;
            }
            else
            {
                rect.r = 0x78;
                rect.g = 0x78;
                rect.b = 0x78;
                libgs_GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE, layer);
                rect.r = 0x28;
                rect.g = 0x28;
                rect.b = 0x28;
            }
            rect.width = 50;

            drawLine3P(0x20202,
                       box.finalPos.x + 0x40,
                       yOffset,
                       box.finalPos.x + 0x40,
                       yOffset - 3,
                       box.finalPos.x + 0x75,
                       yOffset - 3,
                       layer,
                       0);
            drawLine3P(0x666666,
                       box.finalPos.x + 0x75,
                       yOffset - 2,
                       box.finalPos.x + 0x75,
                       yOffset,
                       box.finalPos.x + 0x41,
                       yOffset,
                       layer,
                       0);
            libgs_GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE, layer);
        }
    }

    void tickPostBattleStatsBox(int32_t)
    {
        if (POST_BATTLE_STATS_TIMER > 0) POST_BATTLE_STATS_TIMER--;

        if (isKeyDownPolled(InputButtons::BUTTON_CROSS) || isKeyDownPolled(InputButtons::BUTTON_TRIANGLE))
            SHOULD_SKIP_BIT_COUNTING = true;

        if (!SHOULD_SKIP_BIT_COUNTING) return;

        INITIAL_COMBAT_STATS[0].hp      = min(INITIAL_COMBAT_STATS[0].hp + STATS_GAINS.hp, 9999);
        INITIAL_COMBAT_STATS[0].mp      = min(INITIAL_COMBAT_STATS[0].mp + STATS_GAINS.mp, 9999);
        INITIAL_COMBAT_STATS[0].offense = min(INITIAL_COMBAT_STATS[0].offense + STATS_GAINS.offense, 999);
        INITIAL_COMBAT_STATS[0].defense = min(INITIAL_COMBAT_STATS[0].defense + STATS_GAINS.defense, 999);
        INITIAL_COMBAT_STATS[0].speed   = min(INITIAL_COMBAT_STATS[0].speed + STATS_GAINS.speed, 999);
        INITIAL_COMBAT_STATS[0].brains  = min(INITIAL_COMBAT_STATS[0].brains + STATS_GAINS.brains, 999);
        STATS_GAINS                     = {};

        POST_BATTLE_STATS_TIMER = 0;
    }

    void createBitsBox()
    {
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
        const RenderSettings settings{
            .x     = static_cast<int16_t>(finalPos.x + 10),
            .y     = static_cast<int16_t>(finalPos.y + 10),
            .color = TEXT_COLOR_YELLOW,
        };
        data->bitString1 = getAtlasVanilla().render(bitsString, settings);

        createAnimatedUIBox(1, 0, 2, &finalPos, &startPos, tickBitBox, renderBitBox);
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
        const RenderSettings settings{
            .x = static_cast<int16_t>(finalPos.x + 10),
            .y = static_cast<int16_t>(finalPos.y + 10),
        };
        data->bitString2 = getAtlasVanilla().render(bitsString, settings);
        createAnimatedUIBox(2, 1, 0, &finalPos, &startPos, nullptr, renderFinalBalance);
    }

    void removeBattleEndBox(int32_t id)
    {
        if (UI_BOX_DATA[id].state != 0) removeAnimatedUIBox(id, nullptr);

        data.release();
    }

    void createPostBattleStatsBox()
    {
        SHOULD_SKIP_BIT_COUNTING = false;

        for (int32_t i = 0; i < 6; i++)
        {
            auto stat            = static_cast<Stat>(i);
            STAT_BOX_HAS_GAIN[i] = STATS_GAINS.get(stat) != 0;
        }

        POST_BATTLE_STATS_TIMER = 100;

        auto& work     = PARTNER_ENTITY.posData[1].posMatrix.work.t;
        auto entityPos = getScreenPosition(work[0], work[1], work[2]);
        const RECT finalPos{
            .x      = -88,
            .y      = -78,
            .width  = 176,
            .height = 96,
        };
        const RECT startPos{
            .x      = static_cast<int16_t>(entityPos.screenX - 5),
            .y      = static_cast<int16_t>(entityPos.screenY - 5),
            .width  = 10,
            .height = 10,
        };

        data = dtl::make_unique<Data>();
        for (int32_t i = 0; i < statLabels.size(); i++)
        {
            const RenderSettings settings{
                .x     = static_cast<int16_t>(finalPos.x + 10),
                .y     = static_cast<int16_t>(finalPos.y + 9 + (i * 13)),
                .color = TEXT_COLOR_YELLOW,
            };
            data->statLabels[i] = getAtlasVanilla().render(statLabels[i], settings);
        }

        createAnimatedUIBox(0, 0, 2, &finalPos, &startPos, tickPostBattleStatsBox, renderPostBattleStatsBox);
    }
} // namespace

extern "C"
{
    void resetStatsAfterCombat()
    {
        PARTNER_ENTITY.stats.hp    = INITIAL_COMBAT_STATS[0].hp;
        PARTNER_ENTITY.stats.mp    = INITIAL_COMBAT_STATS[0].mp;
        PARTNER_ENTITY.stats.off   = INITIAL_COMBAT_STATS[0].offense;
        PARTNER_ENTITY.stats.def   = INITIAL_COMBAT_STATS[0].defense;
        PARTNER_ENTITY.stats.speed = INITIAL_COMBAT_STATS[0].speed;
        PARTNER_ENTITY.stats.brain = INITIAL_COMBAT_STATS[0].brains;
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