#include "GameMenu.hpp"

#include "Entity.hpp"
#include "Font.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "PlayerMenu.hpp"
#include "StatsView.hpp"
#include "Tamer.hpp"
#include "UIElements.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    constexpr uint8_t PLAYER_MENU_TABS[]  = "Player   Chart    Med. Card Fish";

    constexpr auto GAME_MENU_X            = -66;
    constexpr auto GAME_MENU_Y            = -80;
    constexpr auto GAME_MENU_WIDTH        = 132;
    constexpr auto GAME_MENU_HEIGHT       = 110;
    constexpr auto GAME_MENU_EXTRA_HEIGHT = 39;
    constexpr auto GAME_MENU_DATE_X       = GAME_MENU_X + 4;
    constexpr auto GAME_MENU_DATE_Y       = GAME_MENU_Y + 5;
    constexpr auto GAME_MENU_OPTIONS_X    = GAME_MENU_X + 6;
    constexpr auto GAME_MENU_OPTIONS_Y    = GAME_MENU_Y + 27;

    constexpr auto GAME_MENU_TEXT_DRAW_X = 704;
    constexpr auto GAME_MENU_TEXT_DRAW_Y = 256;

    constexpr Line GAME_MENU_LINES[] = {
        {
            .x1   = GAME_MENU_X + 3,
            .y1   = GAME_MENU_Y + 21,
            .x2   = GAME_MENU_X + GAME_MENU_WIDTH - 4,
            .y2   = GAME_MENU_Y + 21,
            .clut = 0,
        },
        {
            .x1   = GAME_MENU_X + 3,
            .y1   = GAME_MENU_Y + 22,
            .x2   = GAME_MENU_X + GAME_MENU_WIDTH - 4,
            .y2   = GAME_MENU_Y + 22,
            .clut = 1,
        },
        {
            .x1   = GAME_MENU_X + 3,
            .y1   = GAME_MENU_Y + 23,
            .x2   = GAME_MENU_X + GAME_MENU_WIDTH - 4,
            .y2   = GAME_MENU_Y + 23,
            .clut = 0,
        },
    };

    constexpr IconSprite GAME_MENU_SPRITES[] = {
        {GAME_MENU_OPTIONS_X + 10, GAME_MENU_OPTIONS_Y + 44, 20, 20, 0, 0, 0, 30, getClut(0x100, 0x1F0 + 12)},
        {GAME_MENU_OPTIONS_X + 50, GAME_MENU_OPTIONS_Y + 44, 20, 20, 40, 0, 0, 30, getClut(0x100, 0x1F0 + 13)},
        {GAME_MENU_OPTIONS_X + 90, GAME_MENU_OPTIONS_Y + 44, 20, 20, 0, 20, 0, 30, getClut(0x100, 0x1F0 + 12)},
        // reduced width by 1 to make it align better ingame
        {GAME_MENU_OPTIONS_X + 10, GAME_MENU_OPTIONS_Y + 83, 19, 20, 41, 20, 0, 30, getClut(0x100, 0x1F0 + 13)},
        {GAME_MENU_OPTIONS_X + 50, GAME_MENU_OPTIONS_Y + 83, 20, 20, 0, 40, 0, 30, getClut(0x100, 0x1F0 + 13)},
        {GAME_MENU_OPTIONS_X + 90, GAME_MENU_OPTIONS_Y + 83, 20, 20, 40, 40, 0, 30, getClut(0x100, 0x1F0 + 13)},
        // vanilla only has 19px for us here, too
        {GAME_MENU_OPTIONS_X + 10, GAME_MENU_OPTIONS_Y + 5, 20, 19, 80, 0, 0, 30, getClut(0x100, 0x1F0 + 13)},
    };

    constexpr SelectionSprite GAME_MENU_SELECTOR[] = {
        {0, 4, 251, 255, 0, 0, 4, 4},
        {4, 0, 251, 255, 23, 0, 4, 4},
        {0, 4, 255, 251, 0, 23, 4, 4},
        {4, 0, 255, 251, 23, 23, 4, 4},
        {4, 8, 251, 255, 4, 0, 20, 4},
        {4, 8, 251, 255, 4, 23, 20, 4},
        {8, 12, 252, 255, 0, 3, 4, 22},
        {11, 7, 252, 255, 24, 3, 4, 22},
    };

    static TextSprite yearLabel = {
        .font       = &vanillaFont,
        .string     = "Year",
        .uvX        = GAME_MENU_TEXT_DRAW_X + 0,
        .uvY        = GAME_MENU_TEXT_DRAW_Y + 20,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = GAME_MENU_DATE_X,
        .posY       = GAME_MENU_DATE_Y,
        .boxWidth   = 40,
        .boxHeight  = 16,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::CENTER,
        .color      = 3,
        .layer      = 6,
        .hasShadow  = 1,
    };

    static TextSprite dayLabel = {
        .font       = &vanillaFont,
        .string     = "Day",
        .uvX        = GAME_MENU_TEXT_DRAW_X + 16,
        .uvY        = GAME_MENU_TEXT_DRAW_Y + 20,
        .uvWidth    = 0,
        .uvHeight   = 0,
        .posX       = GAME_MENU_DATE_X + 62,
        .posY       = GAME_MENU_DATE_Y,
        .boxWidth   = 36,
        .boxHeight  = 16,
        .alignmentX = AlignmentX::CENTER,
        .alignmentY = AlignmentY::CENTER,
        .color      = 3,
        .layer      = 6,
        .hasShadow  = 1,
    };

    static TextSprite statusLabel = {
        .font       = &vanillaFont,
        .string     = "Status",
        .uvX        = GAME_MENU_TEXT_DRAW_X + 0,
        .uvY        = GAME_MENU_TEXT_DRAW_Y + 0,
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
        .uvX        = GAME_MENU_TEXT_DRAW_X + 16,
        .uvY        = GAME_MENU_TEXT_DRAW_Y + 0,
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

    static TextSprite gameMenuLabels[] = {
        {
            .font       = &myFont7px,
            .string     = "ITEM",
            .uvX        = GAME_MENU_TEXT_DRAW_X + 10,
            .uvY        = GAME_MENU_TEXT_DRAW_Y + 0,
            .uvWidth    = 0,
            .uvHeight   = 0,
            .posX       = GAME_MENU_OPTIONS_X + 0,
            .posY       = GAME_MENU_OPTIONS_Y + 68,
            .boxWidth   = 40,
            .boxHeight  = 10,
            .alignmentX = AlignmentX::CENTER,
            .alignmentY = AlignmentY::CENTER,
            .color      = 0,
            .layer      = 6,
            .hasShadow  = 1,
        },
        {
            .font       = &myFont7px,
            .string     = "DIGIMON",
            .uvX        = GAME_MENU_TEXT_DRAW_X + 20,
            .uvY        = GAME_MENU_TEXT_DRAW_Y + 0,
            .uvWidth    = 0,
            .uvHeight   = 0,
            .posX       = GAME_MENU_OPTIONS_X + 40,
            .posY       = GAME_MENU_OPTIONS_Y + 68,
            .boxWidth   = 40,
            .boxHeight  = 10,
            .alignmentX = AlignmentX::CENTER,
            .alignmentY = AlignmentY::CENTER,
            .color      = 0,
            .layer      = 6,
            .hasShadow  = 1,
        },
        {
            .font       = &myFont7px,
            .string     = "PLAYER",
            .uvX        = GAME_MENU_TEXT_DRAW_X + 30,
            .uvY        = GAME_MENU_TEXT_DRAW_Y + 0,
            .uvWidth    = 0,
            .uvHeight   = 0,
            .posX       = GAME_MENU_OPTIONS_X + 80,
            .posY       = GAME_MENU_OPTIONS_Y + 68,
            .boxWidth   = 40,
            .boxHeight  = 10,
            .alignmentX = AlignmentX::CENTER,
            .alignmentY = AlignmentY::CENTER,
            .color      = 0,
            .layer      = 6,
            .hasShadow  = 1,
        },
        {
            .font       = &myFont7px,
            .string     = "PRAISE",
            .uvX        = GAME_MENU_TEXT_DRAW_X + 40,
            .uvY        = GAME_MENU_TEXT_DRAW_Y + 0,
            .uvWidth    = 0,
            .uvHeight   = 0,
            .posX       = GAME_MENU_OPTIONS_X + 0,
            .posY       = GAME_MENU_OPTIONS_Y + 107,
            .boxWidth   = 40,
            .boxHeight  = 10,
            .alignmentX = AlignmentX::CENTER,
            .alignmentY = AlignmentY::CENTER,
            .color      = 0,
            .layer      = 6,
            .hasShadow  = 1,
        },
        {
            .font       = &myFont7px,
            .string     = "SCOLD",
            .uvX        = GAME_MENU_TEXT_DRAW_X + 50,
            .uvY        = GAME_MENU_TEXT_DRAW_Y + 0,
            .uvWidth    = 0,
            .uvHeight   = 0,
            .posX       = GAME_MENU_OPTIONS_X + 40,
            .posY       = GAME_MENU_OPTIONS_Y + 107,
            .boxWidth   = 40,
            .boxHeight  = 10,
            .alignmentX = AlignmentX::CENTER,
            .alignmentY = AlignmentY::CENTER,
            .color      = 0,
            .layer      = 6,
            .hasShadow  = 1,
        },
        {
            .font       = &myFont7px,
            .string     = "SLEEP",
            .uvX        = GAME_MENU_TEXT_DRAW_X + 0,
            .uvY        = GAME_MENU_TEXT_DRAW_Y + 8,
            .uvWidth    = 0,
            .uvHeight   = 0,
            .posX       = GAME_MENU_OPTIONS_X + 80,
            .posY       = GAME_MENU_OPTIONS_Y + 107,
            .boxWidth   = 40,
            .boxHeight  = 10,
            .alignmentX = AlignmentX::CENTER,
            .alignmentY = AlignmentY::CENTER,
            .color      = 0,
            .layer      = 6,
            .hasShadow  = 1,
        },
        {
            .font       = &myFont7px,
            .string     = "FISH",
            .uvX        = GAME_MENU_TEXT_DRAW_X + 0,
            .uvY        = GAME_MENU_TEXT_DRAW_Y + 0,
            .uvWidth    = 0,
            .uvHeight   = 0,
            .posX       = GAME_MENU_OPTIONS_X + 0,
            .posY       = GAME_MENU_OPTIONS_Y + 29,
            .boxWidth   = 40,
            .boxHeight  = 10,
            .alignmentX = AlignmentX::CENTER,
            .alignmentY = AlignmentY::CENTER,
            .color      = 0,
            .layer      = 6,
            .hasShadow  = 1,
        },
    };

    constexpr int32_t MOVE_ROW_COUNT = 7;
    constexpr int32_t MOVE_COL_COUNT = 8;

    static int8_t moveSelectedRow;
    static int8_t moveSelectedColumn;
    static bool menuOptionDisabled[7]{};
    static int8_t selectedOption;
    static int8_t optionCount;
    static uint8_t fishingRodState;
    static uint8_t digimonMenuState;

    void renderTriangleCursor(int32_t selection, int16_t yOffset)
    {
        auto posX = GAME_MENU_SPRITES[selection].posX - 4;
        auto posY = GAME_MENU_SPRITES[selection].posY - 4;

        for (auto& val : GAME_MENU_SELECTOR)
        {
            POLY_FT4* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
            libgpu_SetPolyFT4(prim);
            prim->u0 = val.uMin;
            prim->v0 = val.vMin;
            prim->u1 = val.uMax;
            prim->v1 = val.vMin;
            prim->u2 = val.uMin;
            prim->v2 = val.vMax;
            prim->u3 = val.uMax;
            prim->v3 = val.vMax;

            setPosDataPolyFT4(prim, posX + val.posX, posY + val.posY + yOffset, val.width, val.height);
            prim->r0    = 128;
            prim->g0    = 128;
            prim->b0    = 128;
            prim->tpage = libgpu_GetTPage(0, 0, 896, 448);
            prim->clut  = getClut(0x100, 0x1FC);
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 5, prim);
            libgs_GsSetWorkBase(prim + 1);
        }
    }

    void drawGameMenu()
    {
        clearTextSubArea2(GAME_MENU_TEXT_DRAW_X, GAME_MENU_TEXT_DRAW_Y, 256, 32);

        for (auto& entry : gameMenuLabels)
            drawTextSprite(entry);

        drawTextSprite(dayLabel);
        drawTextSprite(yearLabel);
    }

    void renderDateNumber(int32_t value, int32_t posX, int32_t posY)
    {
        uint8_t buffer[8]{};
        sprintf(buffer, "%d", value);

        for (int32_t i = 0; i < sizeof(buffer); i++)
        {
            auto val = buffer[i];
            if (val == 0) break;
            if (val < '0' || val > '9') continue;

            auto digit     = val - '0';
            uint8_t uCoord = 121;
            uint8_t vCoord = 52;
            if (digit == 0) {}
            else if (digit < 5)
            {
                vCoord = 40;
                uCoord = (digit - 1) * 8 + 97;
            }
            else { uCoord = (digit - 5) * 8 + 81; }

            renderRectPolyFT4(posX + i * 9, -74, 7, 12, uCoord, vCoord - 64, 30, 0x7F10, 5, 0);
        }
    }

    void renderDigimonMenu()
    {
        if (MENU_STATE == 0)
        {
            clearTextSubArea2(0, 0, 256, 16);
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

    void renderGameMenu()
    {
        // not strictly vanilla equivalent, adjusted to improve layout and fix inconsistencies
        const auto yOffset = optionCount <= 6 ? -39 : 0;

        if (MENU_STATE == 0)
        {
            drawGameMenu();
            MENU_STATE = 1;
        }

        renderSeperatorLines(GAME_MENU_LINES, 3, 5);
        renderDateNumber(DAY + 1, 37, -74);
        renderDateNumber(YEAR + 1, -22, -74);
        renderTriangleCursor(selectedOption, yOffset);

        for (int32_t i = 0; i < optionCount; i++)
        {
            auto option  = menuOptionDisabled[i];
            auto& badge  = gameMenuLabels[i];
            auto& sprite = GAME_MENU_SPRITES[i];
            auto uOffset = 0;
            if (selectedOption == i && PLAYTIME_FRAMES % 10 < 5) uOffset = 20;

            badge.hasShadow = option ? 0 : 1;
            badge.color     = option ? -1 : 0;

            renderRectPolyFT4(sprite.posX,
                              sprite.posY + yOffset,
                              sprite.width,
                              sprite.height,
                              sprite.uvX + uOffset,
                              sprite.uvY - 64,
                              sprite.texture_page,
                              sprite.clut,
                              6,
                              option);
            renderTextSprite(badge, 0, yOffset);
        }

        renderTextSprite(yearLabel, 0, 0);
        renderTextSprite(dayLabel, 0, 0);
    }

    void handleGameMenuSelection(int32_t option)
    {
        switch (option)
        {
            case 0:
                TRIANGLE_MENU_STATE = 2;
                drawInventoryText();
                break;
            case 1: TRIANGLE_MENU_STATE = 3; break;
            case 2: TRIANGLE_MENU_STATE = 5; break;
            case 3:
                closeTriangleMenu();
                Partner_setState(15);
                IS_IN_MENU = 0;
                startGameTime();
                break;
            case 4:
                closeTriangleMenu();
                Partner_setState(4);
                IS_IN_MENU = 0;
                startGameTime();
                break;
            case 5:
                closeTriangleMenu();
                Partner_setState(3);
                IS_IN_MENU = 0;
                startGameTime();
                break;
            case 6:
                closeTriangleMenu();
                setCameraFollowPlayer();
                initializeFishing();
                Tamer_setState(11);
                IS_IN_MENU = 0;
                break;
        }
    }

    void tickDigimonMenuTechs()
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

    void tickGameMenu()
    {
        constexpr int16_t mapping[3][3] = {
            {0, 1, 2},
            {3, 4, 5},
            {6, 1, 2},
        };

        menuOptionDisabled[5] = !PARTNER_PARA.condition.isSleepy;

        // vanilla allows only handled at most one key per frame, we but can handle all of them
        // order it set to allow diagonal movements even with fishing enabled
        auto newSelection = selectedOption;
        if (isKeyDown(InputButtons::BUTTON_DOWN))
        {
            auto col     = newSelection % 3;
            auto row     = ring((newSelection / 3) + 1, 0, (optionCount + 2 - col) / 3);
            newSelection = mapping[row][col];
        }
        if (isKeyDown(InputButtons::BUTTON_LEFT)) newSelection = ring(newSelection - 1, 0, optionCount);
        if (isKeyDown(InputButtons::BUTTON_RIGHT)) newSelection = ring(newSelection + 1, 0, optionCount);
        if (isKeyDown(InputButtons::BUTTON_UP))
        {
            auto col     = newSelection % 3;
            auto row     = ring((newSelection / 3) - 1, 0, (optionCount + 2 - col) / 3);
            newSelection = mapping[row][col];
        }

        if (newSelection != selectedOption)
        {
            selectedOption = newSelection;
            playSound(0, 2);
        }

        if (MENU_STATE == 1)
        {
            if (isKeyDown(InputButtons::BUTTON_CROSS))
            {
                if (menuOptionDisabled[selectedOption])
                    playSound(0, 4);
                else
                {
                    playSound(0, 3);
                    // vanilla does this unconditional, but that seems stupid
                    handleGameMenuSelection(selectedOption);
                }
            }

            if (isKeyDown(InputButtons::BUTTON_TRIANGLE) && (UI_BOX_DATA[0].state == 1 || UI_BOX_DATA[0].frame == 0))
            {
                playSound(0, 4);
                closeTriangleMenu();
                Tamer_setState(0);
                setCameraFollowPlayer();
                IS_IN_MENU = 0;
                startGameTime();
            }
        }
    }

    void closeUIBoxIfOpen(int32_t instanceId)
    {
        if (UI_BOX_DATA[instanceId].frame < 5) return;
        if (UI_BOX_DATA[instanceId].state != 1) return;

        // vanilla calls getEntityScreenPos here, but it's results are unused?
        unsetUIBoxAnimated(instanceId, nullptr);
    }

    void closeTriangleMenu()
    {
        closeUIBoxIfOpen(1);
        closeInventoryBoxes2();
        closeUIBoxIfOpen(0);
        removeObject(ObjectID::GAME_MENU, 0);
    }

    void tickTriangleMenu(int32_t instanceId)
    {
        switch (TRIANGLE_MENU_STATE)
        {
            case 0:
            {
                auto height = GAME_MENU_HEIGHT;
                if (fishingRodState != 0) height += GAME_MENU_EXTRA_HEIGHT;
                createMenuBox(0, GAME_MENU_X, GAME_MENU_Y, GAME_MENU_WIDTH, height, 2, tickGameMenu, renderGameMenu);
                MENU_STATE          = 0;
                TRIANGLE_MENU_STATE = 0xFFFFFFFF;
                break;
            }
            case 1:
            {
                closeUIBoxIfOpen(0);
                removeObject(ObjectID::GAME_MENU, 0);
                break;
            }
            case 2:
            {
                closeUIBoxIfOpen(0);
                if (UI_BOX_DATA[0].frame == 0)
                {
                    initializeInventoryObject();
                    TRIANGLE_MENU_STATE = 0xFFFFFFFF;
                }
                break;
            }
            case 3:
            {
                closeUIBoxIfOpen(0);
                if (UI_BOX_DATA[0].frame == 0)
                {
                    createMenuBox(1, -150, -89, 300, 190, 0, tickDigimonMenu, renderDigimonMenu);
                    MENU_STATE          = 0;
                    MENU_SUB_STATE      = 0;
                    TRIANGLE_MENU_STATE = 0xFFFFFFFF;
                    digimonMenuState    = 0;
                }
                break;
            }
            case 4:
            {
                TAMER_ENTITY.isOnScreen   = true;
                PARTNER_ENTITY.isOnScreen = true;
                closeUIBoxIfOpen(1);
                if (UI_BOX_DATA[1].frame == 0) TRIANGLE_MENU_STATE = 0;
                break;
            }
            case 5:
            {
                closeUIBoxIfOpen(0);
                if (UI_BOX_DATA[0].frame == 0)
                {
                    createMenuBox(1, -150, -89, 300, 190, 0, tickPlayerMenu, renderPlayerMenu);
                    // TODO move this draw into the menu box tick/render
                    clearTextArea();
                    drawStringNew(&vanillaFont, PLAYER_MENU_TABS, 704, 256);
                    MENU_STATE          = 0;
                    MENU_SUB_STATE      = 0;
                    TRIANGLE_MENU_STATE = 0xFFFFFFFF;
                    PLAYER_MENU_STATE   = 0;
                }
                break;
            }
            case 6:
            {
                TAMER_ENTITY.isOnScreen   = true;
                PARTNER_ENTITY.isOnScreen = true;
                closeUIBoxIfOpen(1);
                if (UI_BOX_DATA[1].frame == 0) TRIANGLE_MENU_STATE = 0;
                break;
            }
        }
    }

    void addGameMenu()
    {
        fishingRodState = hasFishingRod();
        if (fishingRodState != 0)
        {
            optionCount           = 7;
            menuOptionDisabled[6] = fishingRodState != 2;
            selectedOption        = 6;
        }
        else
        {
            optionCount           = 6;
            menuOptionDisabled[6] = false;
            selectedOption        = 0;
        }
        menuOptionDisabled[6] = PARTNER_PARA.condition.isSleepy;
        TRIANGLE_MENU_STATE   = 0;
        addObject(ObjectID::GAME_MENU, 0, tickTriangleMenu, nullptr);
    }
}