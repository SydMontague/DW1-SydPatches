#include "AtlasFont.hpp"
#include "Battle.h"
#include "Font.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Input.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Sound.hpp"
#include "UIBox.hpp"
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
    constexpr dtl::array<const char*, 6> STATS_LABELS{"HP", "MP", "Off", "Def", "Speed", "Brain"};
    constexpr auto bitsString                = "Bits";
    constexpr auto BOX_X                     = -88;
    constexpr auto BITS_BOX_Y                = 18;
    constexpr auto STATS_BOX_Y               = -78;
    constexpr auto BALANCE_BOX_Y             = -13;
    constexpr auto BOX_WIDTH                 = 176;
    constexpr auto STATS_BOX_HEIGHT          = 96;
    constexpr int16_t BOTTOM_BOX_HEIGHT_BASE = 31;
    constexpr int16_t BOTTOM_BOX_HEIGHT_FULL = 66;

    constexpr RECT finalPos{
        .x      = BOX_X,
        .y      = STATS_BOX_Y,
        .width  = BOX_WIDTH,
        .height = STATS_BOX_HEIGHT,
    };

    constexpr RECT balanceFinalPos{
        .x      = BOX_X,
        .y      = BALANCE_BOX_Y,
        .width  = BOX_WIDTH,
        .height = BOTTOM_BOX_HEIGHT_BASE,
    };

    constexpr RECT getStartPos()
    {
        auto& work     = TAMER_ENTITY.posData[1].posMatrix.work.t;
        auto entityPos = getScreenPosition(work[0], work[1], work[2]);
        return {
            .x      = static_cast<int16_t>(entityPos.screenX - 5),
            .y      = static_cast<int16_t>(entityPos.screenY - 5),
            .width  = 10,
            .height = 10,
        };
    }

    constexpr RECT getBitBoxRect()
    {
        return {
            .x      = BOX_X,
            .y      = BITS_BOX_Y,
            .width  = BOX_WIDTH,
            .height = BTL_END_BOX_TEXTBUFFER[0] == 0 ? BOTTOM_BOX_HEIGHT_BASE : BOTTOM_BOX_HEIGHT_FULL,
        };
    }

    constexpr int32_t getLimit(Stat stat)
    {
        switch (stat)
        {
            case Stat::HP:
            case Stat::MP: return 9999;
            default: return 999;
        }
    }

    constexpr RenderSettings BITS_SETTING1{
        .x     = static_cast<int16_t>(BOX_X + 10),
        .y     = static_cast<int16_t>(BITS_BOX_Y + 10),
        .color = TEXT_COLOR_YELLOW,
    };
    constexpr RenderSettings BITS_SETTING2{
        .x = static_cast<int16_t>(BOX_X + 10),
        .y = static_cast<int16_t>(BALANCE_BOX_Y + 10),
    };

    struct Data
    {
        UIBox statsBox{finalPos,
                       {.color = {}, .fill = UIBox::Fill::SEMI, .playOpenSound = false, .playCloseSound = false},
                       getStartPos()};
        UIBox bitsBox{getBitBoxRect(),
                      {.color = {}, .fill = UIBox::Fill::SEMI, .playOpenSound = false, .playCloseSound = false},
                      getStartPos()};
        UIBox finalBitsBox{balanceFinalPos,
                           {.fill = UIBox::Fill::OPAQUE, .playOpenSound = false, .playCloseSound = false},
                           getStartPos()};
        int32_t bitsToGain{};
        int16_t statsTimer{};

        Data();
        void tick();
        void render(int32_t depth);
        void close();
        bool isClosed();

    private:
        void tickBitBox();
        void tickPostBattleStatsBox();

        void renderBitBox(int32_t depth);
        void renderFinalBalance(int32_t depth);
        // NOLINTNEXTLINE
        __attribute__((optimize("Os"))) void renderPostBattleStatsBox(int32_t depth);

        dtl::array<AtlasString, 6> statLabels{};
        AtlasString bitString1{getAtlasVanilla().render(bitsString, BITS_SETTING1)};
        AtlasString bitString2{getAtlasVanilla().render(bitsString, BITS_SETTING2)};
        dtl::array<bool, 6> hasStatGain{};
        bool skipBitCounting{false};
    };

    dtl::unique_ptr<Data> data;

    void Data::tickBitBox()
    {
        if (finalBitsBox.getState() != UIBox::State::OPENED) return;

        if (bitsToGain == 0)
        {
            BTL_tickBattleEndText();
            return;
        }

        if (isKeyDownPolled(InputButtons::BUTTON_CROSS) || isKeyDownPolled(InputButtons::BUTTON_TRIANGLE))
            skipBitCounting = true;

        if (skipBitCounting)
        {
            MONEY += bitsToGain;
            bitsToGain = 0;
        }
        else
        {
            playSound(0, 22);
            bitsToGain -= 1;
            MONEY += 1;
        }

        MONEY = min(MONEY, 999999);
        if (bitsToGain == 0) playSound(0, 23);
    }

    void Data::renderBitBox(int32_t depth)
    {
        if (bitsBox.getState() != UIBox::State::OPENED) return;

        getAtlasVanilla().renderSlow(format("%d", bitsToGain).data(), depth, {.x = -18, .y = 28});
        data->bitString1.render(depth);

        if (BTL_END_BOX_TEXTBUFFER[0] != 0)
        {
            drawLine2P(0x8e8e8e, -86, 50, 84, 50, depth, 0);
            drawLine2P(0x121212, -85, 51, 85, 51, depth, 0);
            BTL_renderBattleEndText(1);
        }

        bitsBox.render(depth);
    }

    void Data::renderFinalBalance(int32_t depth)
    {
        if (finalBitsBox.getState() != UIBox::State::OPENED) return;

        const RenderSettings settings{
            .x = static_cast<int16_t>(BOX_X + 58),
            .y = static_cast<int16_t>(BALANCE_BOX_Y + 10),
        };
        getAtlasVanilla().renderSlow(format("%d", MONEY).data(), depth, settings);
        data->bitString2.render(depth);

        finalBitsBox.render(depth);
    }

    void Data::renderPostBattleStatsBox(int32_t depth)
    {
        if (statsBox.getState() != UIBox::State::OPENED) return;

        auto previous_color = COLORCODE_LOWBITS;
        setTextColor(1);
        bool first = true;
        for (int32_t i = 0; i < 6; i++)
        {
            auto stat      = static_cast<Stat>(i);
            auto remaining = STATS_GAINS.get(stat);

            if (remaining == 0) continue;

            if (statsTimer == 0 && first)
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
                setPosDataPolyFT4(prim, BOX_X + 0x82, STATS_BOX_Y + 9 + i * 13, 12, 12);
                libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + depth, prim);
                libgs_GsSetWorkBase(prim + 1);
            }

            const RenderSettings settings{
                .x = static_cast<int16_t>(BOX_X + 0x8e),
                .y = static_cast<int16_t>(STATS_BOX_Y + 9 + (i * 13)),
            };
            getAtlasVanilla().renderSlow(format("%d", STATS_GAINS.get(stat)).data(), depth, settings);
        }

        for (int32_t i = 0; i < 6; i++)
        {
            const RenderSettings settings{
                .x = static_cast<int16_t>(BOX_X + 0x44),
                .y = static_cast<int16_t>(STATS_BOX_Y + 9 + (i * 13)),
            };
            auto stat = static_cast<Stat>(i);
            getAtlasVanilla().renderSlow(format("%d", INITIAL_COMBAT_STATS[0].get(stat)).data(), depth, settings);
        }

        setTextColor(previous_color);
        drawLine2P(0xfad990, BOX_X + 0x7a, STATS_BOX_Y + 2, BOX_X + 0x7a, STATS_BOX_Y + STATS_BOX_HEIGHT - 3, depth, 0);
        drawLine2P(0x20202, BOX_X + 0x7b, STATS_BOX_Y + 2, BOX_X + 0x7b, STATS_BOX_Y + STATS_BOX_HEIGHT - 3, depth, 0);

        for (const auto& entry : data->statLabels)
            entry.render(depth);

        GsBOXF rect{
            .attribute = 0,
            .x         = -22,
            .height    = 2,
        };

        for (int32_t i = 0; i < 6; i++)
        {
            auto yOffset = STATS_BOX_Y + 20 + i * 13;
            auto stat    = static_cast<Stat>(i);
            rect.width   = INITIAL_COMBAT_STATS[0].get(stat) * 50 / getLimit(stat);
            rect.y       = yOffset - 2;

            if (hasStatGain[i])
            {
                rect.r = 0x69;
                rect.g = 0xc2;
                rect.b = 0xff;
                libgs_GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE, depth);
                rect.r = 0x00;
                rect.g = 0x5a;
                rect.b = 0x96;
            }
            else
            {
                rect.r = 0x78;
                rect.g = 0x78;
                rect.b = 0x78;
                libgs_GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE, depth);
                rect.r = 0x28;
                rect.g = 0x28;
                rect.b = 0x28;
            }
            rect.width = 50;

            drawLine3P(0x20202, BOX_X + 0x40, yOffset, BOX_X + 0x40, yOffset - 3, BOX_X + 0x75, yOffset - 3, depth, 0);
            drawLine3P(0x666666, BOX_X + 0x75, yOffset - 2, BOX_X + 0x75, yOffset, BOX_X + 0x41, yOffset, depth, 0);
            libgs_GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE, depth);
        }

        statsBox.render(depth);
    }

    void Data::tickPostBattleStatsBox()
    {
        if (statsTimer > 0) statsTimer--;

        if (isKeyDownPolled(InputButtons::BUTTON_CROSS) || isKeyDownPolled(InputButtons::BUTTON_TRIANGLE))
            skipBitCounting = true;

        if (!skipBitCounting) return;

        INITIAL_COMBAT_STATS[0].hp      = min(INITIAL_COMBAT_STATS[0].hp + STATS_GAINS.hp, 9999);
        INITIAL_COMBAT_STATS[0].mp      = min(INITIAL_COMBAT_STATS[0].mp + STATS_GAINS.mp, 9999);
        INITIAL_COMBAT_STATS[0].offense = min(INITIAL_COMBAT_STATS[0].offense + STATS_GAINS.offense, 999);
        INITIAL_COMBAT_STATS[0].defense = min(INITIAL_COMBAT_STATS[0].defense + STATS_GAINS.defense, 999);
        INITIAL_COMBAT_STATS[0].speed   = min(INITIAL_COMBAT_STATS[0].speed + STATS_GAINS.speed, 999);
        INITIAL_COMBAT_STATS[0].brains  = min(INITIAL_COMBAT_STATS[0].brains + STATS_GAINS.brains, 999);
        STATS_GAINS                     = {};

        statsTimer = 0;
    }

    Data::Data()
    {
        finalBitsBox.close();

        statsTimer = 100;
        for (int32_t i = 0; i < 6; i++)
        {
            auto stat      = static_cast<Stat>(i);
            hasStatGain[i] = STATS_GAINS.get(stat) != 0;
        }

        for (int32_t i = 0; i < STATS_LABELS.size(); i++)
        {
            const RenderSettings settings{
                .x     = static_cast<int16_t>(finalPos.x + 10),
                .y     = static_cast<int16_t>(finalPos.y + 9 + (i * 13)),
                .color = TEXT_COLOR_YELLOW,
            };
            statLabels[i] = getAtlasVanilla().render(STATS_LABELS[i], settings);
        }
    }

    void Data::tick()
    {
        tickBitBox();
        tickPostBattleStatsBox();

        bitsBox.tick();
        finalBitsBox.tick();
        statsBox.tick();
    }

    void Data::render(int32_t depth)
    {
        this->renderPostBattleStatsBox(depth);
        this->renderBitBox(depth - 1);
        this->renderFinalBalance(depth - 2);
    }

    void Data::close()
    {
        bitsBox.close();
        finalBitsBox.close();
        statsBox.close();
    }

    bool Data::isClosed()
    {
        return bitsBox.getState() == UIBox::State::CLOSED && finalBitsBox.getState() == UIBox::State::CLOSED &&
               statsBox.getState() == UIBox::State::CLOSED;
    }

    void removeBattleEndBox()
    {
        data->close();
    }

    void tick(int32_t)
    {
        data->tick();
        if (data->isClosed())
        {
            removeObject(ObjectID::BATTLE_END_BOX, 0);
            data.reset();
        }
    }

    void render(int32_t)
    {
        data->render(6);
    }

    void createBattleEndBox()
    {
        data = dtl::make_unique<Data>();
        addObject(ObjectID::BATTLE_END_BOX, 0, tick, render);
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
        createBattleEndBox();
        // vanilla initializes the unused bit text, we don't need to
        data->bitsToGain = 0;
        for (int32_t i = 1; i <= ENEMY_COUNT; i++)
            data->bitsToGain += NPC_ENTITIES[COMBAT_DATA_PTR->player.entityIds[i] - 2].bits;
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

        while (data->statsBox.getState() != UIBox::State::OPENED || data->bitsBox.getState() != UIBox::State::OPENED)
            BTL_battleTickFrame();

        if (BTL_END_BOX_TEXTBUFFER[0] != 0)
        {
            for (int32_t i = 0; i < 2; i++)
            {
                BTL_drawBattleEndText(1);
                BTL_battleTickFrame();
            }
        }

        while (!STATS_GAINS.isAllZero() || data->statsTimer != 0)
            BTL_battleTickFrame();

        data->finalBitsBox.open();
        while (data->finalBitsBox.getState() != UIBox::State::OPENED)
            BTL_battleTickFrame();

        while (data->bitsToGain != 0)
            BTL_battleTickFrame();

        while (!BTL_isEndBoxTextFinished())
            BTL_battleTickFrame();

        for (int32_t i = 0; i < droppedItems.size(); i++)
        {
            if (droppedItems[i] == ItemType::NONE) continue;

            spawnDroppedItem(ENTITY_TABLE.getEntityById(COMBAT_DATA_PTR->player.entityIds[i + 1]), droppedItems[i]);
        }

        resetStatsAfterCombat();
        removeBattleEndBox();
    }
}