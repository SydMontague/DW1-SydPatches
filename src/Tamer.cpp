#include "Entity.hpp"
#include "Evolution.hpp"
#include "Fade.hpp"
#include "Font.hpp"
#include "GameMenu.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Inventory.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "Partner.hpp"
#include "Sound.hpp"
#include "UIElements.hpp"
#include "constants.hpp"
#include "extern/DOOA.hpp"
#include "extern/EAB.hpp"
#include "extern/ENDI.hpp"
#include "extern/KAR.hpp"
#include "extern/MURD.hpp"
#include "extern/STD.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgte.hpp"

extern "C"
{
    constexpr uint8_t findItemStr[]       = "Woah!";
    constexpr uint8_t emptyChestStr[]     = "Hey! It's empty!";
    constexpr uint8_t inventoryFullStr[]  = "But I can't hold any more!";
    constexpr uint8_t tamerLevelUpStr[]   = "Tamer level went up!!!";
    constexpr uint8_t tamerLevelDownStr[] = "Tamer level went down!!!";
    constexpr uint8_t medalLine1[]        = "Congratulations!";
    constexpr uint8_t medalLine2[]        = "To recognize your great";
    constexpr uint8_t medalLine3[]        = "records, we sent you a medal!";

    static uint8_t takeItemFrameCount;
    static bool isStandingOnDrop;
    static uint8_t pickedDropId;
    static int8_t levelsAwarded;
    static bool hasLevelsAwardPending;
    static bool hasMedalAwardPending;
    static uint8_t interactedChest;

    static uint8_t state;
    static uint8_t subState;

    void setTamerDirection(int16_t direction)
    {
        TAMER_ENTITY.posData->rotation.y = direction % 4096;
    }

    void Tamer_setState(uint8_t newState)
    {
        state    = newState;
        subState = 0;
    }

    void Tamer_setSubState(uint8_t newState)
    {
        subState = newState;
    }

    void Tamer_setFullState(uint8_t newState, uint8_t newSubState)
    {
        state    = newState;
        subState = newSubState;
    }

    uint8_t Tamer_getState()
    {
        return state;
    }

    uint8_t Tamer_getSubState()
    {
        return subState;
    }

    bool isTrainingComplete()
    {
        if (TRAINING_COMPLETE == 1)
        {
            TRAINING_COMPLETE = 0;
            return true;
        }
        return false;
    }

    void addTamerLevel(int32_t chance, int32_t amount)
    {
        if (random(100) >= chance) return;

        TAMER_ENTITY.tamerLevel += amount;
        if (TAMER_ENTITY.tamerLevel <= 10 && TAMER_ENTITY.tamerLevel >= 0)
        {
            levelsAwarded         = amount;
            hasLevelsAwardPending = true;
        }

        if (TAMER_ENTITY.tamerLevel < 0) TAMER_ENTITY.tamerLevel = 0;
        if (TAMER_ENTITY.tamerLevel > 10) TAMER_ENTITY.tamerLevel = 10;
    }

    // this function blows up in size otherwise
    // NOLINTNEXTLINE: dunno why it doesn't know optimize...
    __attribute__((optimize("Os"))) uint8_t checkChestCollision()
    {
        int16_t tileX;
        int16_t tileY;
        getModelTile(&TAMER_ENTITY.posData->location, &tileX, &tileY);

        for (int32_t i = 0; i < sizeof(CHEST_ARRAY) / sizeof(CHEST_ARRAY[0]); i++)
        {
            auto& chest = CHEST_ARRAY[i];

            if (chest.item == ItemType::NONE) continue;
            if (abs(chest.tileX - tileX) > 1) continue;
            if (abs(chest.tileY - tileY) > 1) continue;

            return i;
        }

        return 0xFF;
    }

    void renderAwardSomethingTextbox()
    {
        // vanilla doesn't use shadows
        renderStringNew(0, -125, 45, 255, 36, 704, 120 + 256, 5, 1);
        renderUIBox(1);
        TEXTBOX_OPEN_TIMER++;
    }

    void renderItemPickupTextbox()
    {
        // vanilla doesn't use shadows
        // vanilla uses the take state to determine how many lines to render, but that's just redundant
        renderStringNew(15, -125, 45, 255, 12, 704, 12 + 256, 5, 1);
        renderStringNew(0, -125, 57, 255, 12, 704, 24 + 256, 5, 1);
        renderStringNew(0, -125, 69, 255, 12, 704, 36 + 256, 5, 1);
        renderUIBox(1);
        TEXTBOX_OPEN_TIMER++;
    }

    // this function blows up in size otherwise
    // NOLINTNEXTLINE: dunno why it doesn't know optimize...
    __attribute__((optimize("Os"))) void checkItemPickup()
    {
        int16_t tileX;
        int16_t tileY;

        getModelTile(&TAMER_ENTITY.posData->location, &tileX, &tileY);
        pickedDropId = -1;

        for (int32_t i = 0; i < sizeof(DROPPED_ITEMS) / sizeof(DROPPED_ITEMS[0]); i++)
        {
            auto& item = DROPPED_ITEMS[i];

            if (item.type == ItemType::NONE) continue;
            if (abs(item.tileX - tileX) > 1) continue;
            if (abs(item.tileY - tileY) > 1) continue;

            pickedDropId = i;
            if (!isStandingOnDrop)
            {
                Tamer_setState(7);
                isStandingOnDrop = true;
            }
            return;
        }

        isStandingOnDrop = false;
    }

    void checkMapInteraction()
    {
        // vanilla checks for CHANGED_INPUT instead of isKeyDown, but this allows buffering input
        auto collision = entityCheckCollision(&PARTNER_ENTITY, &TAMER_ENTITY, 0, 0);

        if (collision == CollisionCode::MAP) { collisionGrace(nullptr, &TAMER_ENTITY, 0, 0); }
        else if (collision >= CollisionCode::NPC1 && collision <= CollisionCode::NPC8)
        {
            TAMER_ENTITY.animFlag |= 2;
            auto& npc = NPC_ENTITIES[static_cast<int32_t>(collision) - 2];
            if (IS_SCRIPT_PAUSED && (npc.autotalk || isKeyDown(BUTTON_CROSS)))
            {
                removeTriangleMenu();
                closeInventoryBoxes();
                removeUIBox1();
                TALKED_TO_ENTITY = static_cast<uint8_t>(collision);
                callScriptSection(CURRENT_SCRIPT_ID, npc.scriptId, 1);
            }
        }

        if (Tamer_getState() != 0) return;

        auto trigger = getTileTrigger(&TAMER_ENTITY.posData->location);

        if (IS_SCRIPT_PAUSED == 1)
        {
            if (trigger == 120 && PARTNER_PARA.condition.isPoopy) callScriptSection(0, 1250, 0);

            if (trigger >= 80 && trigger < 110 && isKeyDown(BUTTON_CROSS))
                callScriptSection(CURRENT_SCRIPT_ID, trigger, 0);

            if (trigger >= 50 && trigger < 80) callScriptSection(CURRENT_SCRIPT_ID, trigger, 0);
        }

        if (trigger >= 110 && trigger < 120)
        {
            PREVIOUS_EXIT = trigger - 110;
            TARGET_MAP    = MAP_WARPS.targetMap[trigger - 110];
            CURRENT_EXIT  = MAP_WARPS.targetExit[trigger - 110];
            // vanilla writes the previous exit 0x80134df9, but it seems unused
            Tamer_setState(5);
            unsetCameraFollowPlayer();
            stopGameTime();
        }

        auto chest = checkChestCollision();
        if (chest != 0xFF && isKeyDown(BUTTON_CROSS))
        {
            interactedChest = chest;
            Tamer_setState(14);
        }
    }

    void Tamer_tickIdle()
    {
        if (Tamer_getSubState() != 0) return;

        startAnimation(&TAMER_ENTITY, 0);
        Tamer_setSubState(1);
    }

    void Tamer_tickTraining()
    {
        if (Tamer_getSubState() == 0)
        {
            startAnimation(&TAMER_ENTITY, 10);
            Tamer_setSubState(1);
        }
        else if (Tamer_getSubState() == 1) { entityLookAtLocation(&TAMER_ENTITY, &PARTNER_ENTITY.posData->location); }
    }

    void Tamer_startAnimation(int32_t animId)
    {
        startAnimation(&TAMER_ENTITY, animId);
    }

    void Tamer_tickPraise()
    {
        if (Tamer_getSubState() == 0)
        {
            auto animId = 39;
            auto type   = PARTNER_ENTITY.type;
            if (getDigimonData(type)->level < Level::CHAMPION) animId = 38;
            if (type == DigimonType::MAMEMON) animId = 38;
            if (type == DigimonType::METALMAMEMON) animId = 38;
            if (type == DigimonType::DIGITAMAMON) animId = 38;

            startAnimation(&TAMER_ENTITY, animId);
            startAnimation(&PARTNER_ENTITY, 11); // TODO this shouldn't be in tamer code?
            Tamer_setSubState(1);
        }
        else if (Tamer_getSubState() == 1)
        {
            if (TAMER_ENTITY.frameCount > TAMER_ENTITY.animFrame) return;

            Tamer_setState(6);
        }
    }

    void Tamer_tickScold()
    {
        if (Tamer_getSubState() == 0)
        {
            playSound(0, 13);
            startAnimation(&TAMER_ENTITY, 7);
            startAnimation(&PARTNER_ENTITY, ITEM_SCOLD_FLAG == 1 ? 25 : 12);
            Tamer_setSubState(1);
        }
        else if (Tamer_getSubState() == 1)
        {
            if (TAMER_ENTITY.frameCount > TAMER_ENTITY.animFrame) return;

            Tamer_setState(6);
        }
    }

    void Tamer_tickChangeMap()
    {
        if (Tamer_getSubState() == 0)
        {
            fadeToBlack(20);
            startAnimation(&TAMER_ENTITY, 2);
            Tamer_setSubState(1);
        }
        else if (Tamer_getSubState() == 1)
        {
            if (FADE_DATA.fadeOutCurrent == 10) { addMapNameObject(TARGET_MAP); }

            if (FADE_DATA.fadeOutCurrent >= 20)
            {
                changeMap(TARGET_MAP, CURRENT_EXIT);
                STORED_TAMER_POS = TAMER_ENTITY.posData->location;
                fadeFromBlack(20);
                removeObject(ObjectID::MAP_NAME, TARGET_MAP);
                Tamer_setSubState(2);
            }
        }
        else if (Tamer_getSubState() == 2)
        {
            if (FADE_DATA.fadeInCurrent >= 20)
            {
                Tamer_setState(0);
                Partner_setState(1);
                checkMapInteraction();
                STORED_TAMER_POS = TAMER_ENTITY.posData->location;
                startGameTime();
            }
        }
    }

    // TODO: refactor takeChest, pickupItem and awardSomething, as they duplicate a lot of code
    void Tamer_tickTakeChest()
    {
        RECT textArea = {.x = 0, .y = 12, .width = 256, .height = 200};
        Chest& chest  = CHEST_ARRAY[interactedChest];
        auto itemType = chest.item;

        if (Tamer_getSubState() == 0)
        {
            startAnimation(&TAMER_ENTITY, 0);
            Partner_setState(11);
            unsetCameraFollowPlayer();
            entityLookAtLocation(&TAMER_ENTITY, &chest.location);
            clearTextSubArea(&textArea);

            drawStringNew(&vanillaFont, getDigimonData(DigimonType::TAMER)->name, 704 + 0, 256 + 12);
            if (chest.isTaken) { drawStringNew(&vanillaFont, emptyChestStr, 704 + 0, 256 + 24); }
            else
            {
                auto offset = drawStringNew(&vanillaFont, findItemStr, 704 + 0, 256 + 24) / 4;
                setTextColor(5);
                drawStringNew(&vanillaFont, getItem(itemType)->name, 704 + offset + 1, 256 + 24);
                setTextColor(1);
            }
            Tamer_setSubState(1);
        }
        else if (Tamer_getSubState() == 1)
        {
            if (!tickOpenChestTray(interactedChest)) return;
            if (!isUIBoxAvailable(1)) return;

            Position pos;
            getEntityScreenPos(&TAMER_ENTITY, 1, &pos);

            RECT target = {.x = -130, .y = 42, .width = 262, .height = 59};
            RECT source = {
                .x      = static_cast<int16_t>(pos.x - 5),
                .y      = static_cast<int16_t>(pos.y - 5),
                .width  = 10,
                .height = 10,
            };

            createAnimatedUIBox(1, 0, 2, &target, &source, nullptr, renderItemPickupTextbox);
            Tamer_setSubState(chest.isTaken != 0 ? 2 : 4);
            takeItemFrameCount = 0;
        }
        else if (Tamer_getSubState() == 2)
        {
            takeItemFrameCount++;
            // vanilla checks for polled inputs instead of consuming them, but for the sake of code unification this is
            // the same as the item pickup code. It also gives 5 frames delay instead of 4.
            if (takeItemFrameCount < 5) return;
            if (!isKeyDown(BUTTON_CROSS)) return;

            takeItemFrameCount = 0;
            if (!giveItem(itemType, 1))
            {
                drawStringNew(&vanillaFont, inventoryFullStr, 704 + 0, 256 + 24);
                Tamer_setSubState(3);
            }
            else
            {
                setTrigger(chest.trigger);
                Tamer_setSubState(5);
            }
        }
        else if (Tamer_getSubState() == 3)
        {
            if (!tickCloseChestTray(interactedChest)) return;
            Tamer_setSubState(4);
        }
        else if (Tamer_getSubState() == 4)
        {
            // added extra state to fix some paths requiring more confirmation inputs than necessary
            takeItemFrameCount++;
            if (takeItemFrameCount < 5) return;
            if (!isKeyDown(BUTTON_CROSS)) return;
            Tamer_setSubState(5);
        }
        else if (Tamer_getSubState() == 5)
        {
            Position pos;
            getEntityScreenPos(&TAMER_ENTITY, 1, &pos);
            RECT target = {
                .x      = static_cast<int16_t>(pos.x - 5),
                .y      = static_cast<int16_t>(pos.y - 5),
                .width  = 10,
                .height = 10,
            };

            removeAnimatedUIBox(1, &target);

            Tamer_setState(0);
            Partner_setState(1);
            setCameraFollowPlayer();
        }

        // vanilla limits the takeItemFrameCount to 10 here, but doesn't even use it within this state...
    }

    void Tamer_tickPickupItem()
    {
        // vanilla has a delay of 4 frames between boxes, but it takes 5 for text to be come visble
        // TODO bug: the lack of Partner_setState and stopGameTime allows for text stacking
        RECT textArea = {.x = 0, .y = 12, .width = 256, .height = 200};
        auto itemType = DROPPED_ITEMS[pickedDropId].type;

        if (Tamer_getSubState() == 0)
        {
            startAnimation(&TAMER_ENTITY, 12);
            unsetCameraFollowPlayer();
            clearTextSubArea(&textArea);
            drawStringNew(&vanillaFont, getDigimonData(DigimonType::TAMER)->name, 704 + 0, 256 + 12);
            auto offset = drawStringNew(&vanillaFont, findItemStr, 704 + 0, 256 + 24) / 4;
            setTextColor(5);
            drawStringNew(&vanillaFont, getItem(itemType)->name, 704 + offset + 1, 256 + 24);
            setTextColor(1);

            takeItemFrameCount = 0;
            Tamer_setSubState(1);
        }
        else if (Tamer_getSubState() == 1)
        {
            if (!isUIBoxAvailable(1)) return;

            Position pos;
            getEntityScreenPos(&TAMER_ENTITY, 1, &pos);

            RECT target = {.x = -130, .y = 42, .width = 262, .height = 59};
            RECT source = {
                .x      = static_cast<int16_t>(pos.x - 5),
                .y      = static_cast<int16_t>(pos.y - 5),
                .width  = 10,
                .height = 10,
            };
            // vanilla writes TAKE_CHEST_ITEM here, but it's never used so skip that

            createAnimatedUIBox(1, 0, 2, &target, &source, nullptr, renderItemPickupTextbox);
            Tamer_setSubState(2);
        }
        else if (Tamer_getSubState() == 2)
        {
            takeItemFrameCount++;
            // vanilla does the isKeyDown check before the frame count check,
            // but changing the order allows to buffer inputs
            if (takeItemFrameCount < 5) return;
            if (!isKeyDown(BUTTON_CROSS)) return;

            playSound(0, 3);

            if (!giveItem(itemType, 0))
            {
                clearTextSubArea(&textArea);
                drawStringNew(&vanillaFont, getDigimonData(DigimonType::TAMER)->name, 704 + 0, 256 + 12);
                drawStringNew(&vanillaFont, inventoryFullStr, 704 + 0, 256 + 24);
                takeItemFrameCount = 0;
                Tamer_setSubState(3);
            }
            else
            {
                playSound(0, 7);
                Tamer_setSubState(4);
            }
        }
        else if (Tamer_getSubState() == 3)
        {
            takeItemFrameCount++;
            // vanilla does the isKeyDown check before the frame count check,
            // but changing the order allows to buffer inputs
            if (takeItemFrameCount < 5) return;
            if (!isKeyDown(BUTTON_CROSS)) return;

            playSound(0, 3);
            Tamer_setSubState(4);
        }
        else if (Tamer_getSubState() == 4)
        {
            Position pos;
            getEntityScreenPos(&TAMER_ENTITY, 1, &pos);
            RECT target = {
                .x      = static_cast<int16_t>(pos.x - 5),
                .y      = static_cast<int16_t>(pos.y - 5),
                .width  = 10,
                .height = 10,
            };

            removeAnimatedUIBox(1, &target);

            pickupItem(pickedDropId);

            Tamer_setState(0);
            setCameraFollowPlayer();
        }

        // Vanilla check for takeItemFrameCount to be below 60 before calling playSound(0, 3),
        // but it also limits the value to 10. So that code is dead and got removed.
    }

    void Tamer_tickAwardSomething()
    {
        if (Tamer_getSubState() == 0)
        {
            stopGameTime();
            startAnimation(&TAMER_ENTITY, 0);
            Partner_setState(11);
            unsetCameraFollowPlayer();

            if (hasMedalAwardPending)
            {
                clearTextArea();
                setTextColor(7);
                // vanilla draws these strings in multiple frames/states
                drawStringNew(&vanillaFont, medalLine1, 704, 256 + 120);
                drawStringNew(&vanillaFont, medalLine2, 704, 256 + 132);
                drawStringNew(&vanillaFont, medalLine3, 704, 256 + 144);
            }
            else if (hasLevelsAwardPending)
            {
                clearTextArea();
                if (levelsAwarded > 0)
                {
                    setTextColor(7);
                    drawStringNew(&vanillaFont, tamerLevelUpStr, 704, 256 + 120);
                }
                else
                {
                    setTextColor(3);
                    drawStringNew(&vanillaFont, tamerLevelDownStr, 704, 256 + 120);
                }
            }

            setTextColor(1);
            Tamer_setSubState(1);
        }
        else if (Tamer_getSubState() == 1)
        {
            if (!isUIBoxAvailable(1)) return;

            Position pos;
            getEntityScreenPos(&TAMER_ENTITY, 1, &pos);

            RECT target = {.x = -130, .y = 42, .width = 262, .height = 59};
            RECT source = {
                .x      = static_cast<int16_t>(pos.x - 5),
                .y      = static_cast<int16_t>(pos.y - 5),
                .width  = 10,
                .height = 10,
            };
            // vanilla writes TAKE_CHEST_ITEM here, but it's never used so skip that

            createAnimatedUIBox(1, 0, 2, &target, &source, nullptr, renderAwardSomethingTextbox);
            Tamer_setSubState(2);
        }
        else if (Tamer_getSubState() == 2)
        {
            if (!isKeyDown(BUTTON_CROSS)) return;

            Position pos;
            getEntityScreenPos(&TAMER_ENTITY, 1, &pos);
            RECT target = {
                .x      = static_cast<int16_t>(pos.x - 5),
                .y      = static_cast<int16_t>(pos.y - 5),
                .width  = 10,
                .height = 10,
            };

            removeAnimatedUIBox(1, &target);
            Tamer_setState(0);
            Partner_setState(1);
            setCameraFollowPlayer();
            startGameTime();
            hasMedalAwardPending  = false;
            hasLevelsAwardPending = false;
        }
    }

    constexpr bool hasMaxStats(const Stats& stats)
    {
        if (stats.hp != HP_MAX) return false;
        if (stats.mp != MP_MAX) return false;
        if (stats.off != OFF_MAX) return false;
        if (stats.def != DEF_MAX) return false;
        if (stats.speed != SPEED_MAX) return false;
        if (stats.brain != BRAIN_MAX) return false;

        return true;
    }

    inline bool hasAllDigimonRaised()
    {
        constexpr auto begin = static_cast<uint32_t>(DigimonType::BOTAMON);
        constexpr auto end   = static_cast<uint32_t>(DigimonType::WEREGARURUMON);

        for (auto i = begin; i < end; i++)
            if (!hasDigimonRaised(static_cast<DigimonType>(i))) return false;

        return true;
    }

    inline bool hasAllCards()
    {
        for (int32_t i = 0; i < 66; i++)
            if (getCardAmount(i) == 0) return false;

        return true;
    }

    void checkMedalConditions()
    {
        // vanilla doesn't use getNumMasteredMoves, but it should boil down to the same
        if (!hasMedal(Medal::ALL_TECHS) && getNumMasteredMoves() >= 57)
        {
            unlockMedal(Medal::ALL_TECHS);
            hasMedalAwardPending = true;
        }

        if (!hasMedal(Medal::MAX_STATS) && hasMaxStats(PARTNER_ENTITY.stats))
        {
            unlockMedal(Medal::MAX_STATS);
            hasMedalAwardPending = true;
        }

        if (!hasMedal(Medal::MAX_MONEY) && MONEY == MAX_MONEY)
        {
            unlockMedal(Medal::MAX_MONEY);
            hasMedalAwardPending = true;
        }

        if (!hasMedal(Medal::PLAYTIME) && YEAR >= 10)
        {
            unlockMedal(Medal::PLAYTIME);
            hasMedalAwardPending = true;
        }

        if (!hasMedal(Medal::ALL_DIGIMON) && hasAllDigimonRaised())
        {
            unlockMedal(Medal::ALL_DIGIMON);
            hasMedalAwardPending    = true;
            TAMER_ENTITY.tamerLevel = min(TAMER_ENTITY.tamerLevel + 1, MAX_TAMER_LEVEL);
        }

        if (!hasMedal(Medal::CATCH_100_FISH) && PARTNER_PARA.fishCaught >= 100)
        {
            unlockMedal(Medal::CATCH_100_FISH);
            hasMedalAwardPending = true;
        }

        if (!hasMedal(Medal::ALL_CARDS) && hasAllCards())
        {
            unlockMedal(Medal::ALL_CARDS);
            hasMedalAwardPending = true;
        }
    }

    void checkPendingAwards()
    {
        if (hasLevelsAwardPending || hasMedalAwardPending)
        {
            Tamer_setState(20);
            stopGameTime(); // is this necessary? The state already does that
        }
    }

    inline void Tamer_tickWalking()
    {
        // TODO is triangle really intended here?
        bool isRunning          = (POLLED_INPUT & (BUTTON_TRIANGLE | BUTTON_R1)) == 0;
        bool isDirectionPressed = (POLLED_INPUT & (BUTTON_UP | BUTTON_RIGHT | BUTTON_DOWN | BUTTON_LEFT));
        int32_t animId          = 0;

        if (isDirectionPressed)
        {
            animId          = isRunning ? 3 : 2;
            ITEM_SCOLD_FLAG = 0;
        }
        if (TAMER_ENTITY.animId != animId) startAnimation(&TAMER_ENTITY, animId);

        auto mapRotation = getMapRotation();
        // align rotation to axis
        mapRotation += mapRotation % 0x200 > 0xFF ? 0x200 : 0;
        mapRotation &= ~0x1FF;
        auto playerRotation = 0;

        if ((POLLED_INPUT & BUTTON_DOWN) != 0) playerRotation = 0x000;
        if ((POLLED_INPUT & BUTTON_UP) != 0) playerRotation = 0x800;
        if ((POLLED_INPUT & BUTTON_RIGHT) != 0) playerRotation = 0xC00;
        if ((POLLED_INPUT & BUTTON_LEFT) != 0) playerRotation = 0x400;

        if ((POLLED_INPUT & (BUTTON_DOWN | BUTTON_RIGHT)) == (BUTTON_DOWN | BUTTON_RIGHT)) playerRotation = 0xE00;
        if ((POLLED_INPUT & (BUTTON_DOWN | BUTTON_LEFT)) == (BUTTON_DOWN | BUTTON_LEFT)) playerRotation = 0x200;
        if ((POLLED_INPUT & (BUTTON_UP | BUTTON_RIGHT)) == (BUTTON_UP | BUTTON_RIGHT)) playerRotation = 0xA00;
        if ((POLLED_INPUT & (BUTTON_UP | BUTTON_LEFT)) == (BUTTON_UP | BUTTON_LEFT)) playerRotation = 0x600;

        if (isDirectionPressed) setTamerDirection(mapRotation + playerRotation);

        checkItemPickup();
        checkMapInteraction();
        checkMedalConditions();
        checkPendingAwards();
    }

    inline void Tamer_tickOpenMenu()
    {
        addGameMenu();
        Tamer_setState(1);
        startAnimation(&TAMER_ENTITY, 0);
        unsetCameraFollowPlayer();
        IS_IN_MENU = 1;
        stopGameTime();
        setPartnerIdling();
    }

    void Tamer_tickWalkingState()
    {
        tickTamerWaypoints();

        auto isTrianglePressed = isKeyDown(BUTTON_TRIANGLE); // menu open button

        if (!isTrianglePressed || IS_SCRIPT_PAUSED != 1 || FADE_DATA.fadeProtection != 0 || UI_BOX_DATA[0].state == 1 ||
            UI_BOX_DATA[0].frame != 0)
            Tamer_tickWalking();
        else
            Tamer_tickOpenMenu();

        STORED_TAMER_POS = TAMER_ENTITY.posData->location;
    }

    void Tamer_tickOpening()
    {
        // potential useless/unused state, log to see if it gets used
        printf("Debug: tickOpening called.");

        if (Tamer_getSubState() == 0) { Tamer_setSubState(1); }
        else if (Tamer_getSubState() == 1)
        {
            setMapLayerEnabled(1);
            SOME_SCRIPT_SYNC_BIT = 1;
        }
    }

    void Tamer_tickEnding()
    {
        if (Tamer_getSubState() == 0)
        {
            isSoundLoaded(false, 8);
            ENDI_tickEnding(&TAMER_ENTITY, false);
            Tamer_setSubState(1);
        }
        else if (Tamer_getSubState() == 1)
        {
            if (ENDI_tickEnding(&TAMER_ENTITY, true) < 0) SOME_SCRIPT_SYNC_BIT = 1;
        }
    }

    void Tamer_tickSicknessLostLife()
    {
        if (Tamer_getSubState() == 0)
        {
            loadDynamicLibrary(Overlay::MURD_REL, nullptr, false, nullptr, 0);
            MURD_tick(&PARTNER_ENTITY, false);
            Tamer_setSubState(1);
        }
        else if (Tamer_getSubState() == 1)
        {
            if (MURD_tick(&PARTNER_ENTITY, true) >= 0) return;
            if (DoOA_tick(&PARTNER_ENTITY, DOOA_DATA_PTR, true) >= 0) return;
            SOME_SCRIPT_SYNC_BIT = 1;
        }
    }

    void Tamer_tickMachinedramonSpawn()
    {
        if (Tamer_getSubState() == 0)
        {
            EaB_tick(ENTITY_TABLE.npc1, false);
            Tamer_setSubState(1);
        }
        else if (Tamer_getSubState() == 1)
        {
            if (EaB_tick(ENTITY_TABLE.npc1, true) < 0) SOME_SCRIPT_SYNC_BIT = 1;
        }
    }

    void Tamer_tickBattleLostLife()
    {
        if (Tamer_getSubState() == 0)
        {
            loadDynamicLibrary(Overlay::MURD_REL, nullptr, false, nullptr, 0);
            MURD_tick(&PARTNER_ENTITY, false);
            Tamer_setSubState(1);
        }
        else if (Tamer_getSubState() == 1)
        {
            if (MURD_tick(&PARTNER_ENTITY, true) < 0) SOME_SCRIPT_SYNC_BIT = 1;
        }
    }

    /*
     * Tamer States Overworld
     *  0 -> walking/normal
     *  1 -> set idle state?
     *  5 -> change map
     *  6 -> idle
     *  7 -> pickup item
     *  8 -> training
     *  9 -> scold
     * 10 -> Dialogue/Script
     * 11 -> Fishing
     * 12 -> Curling
     * 13 -> praise
     * 14 -> take chest
     * 15 -> Opening??
     * 16 -> Ending
     * 17 -> lose battle
     * 18 -> Machinedramon entrance?
     * 19 -> lose life (sickness)
     * 20 -> award something
     */
    void Tamer_tickOverworld(int32_t instanceId)
    {
        switch (Tamer_getState())
        {
            case 0: Tamer_tickWalkingState(); break;
            case 1: Tamer_setState(6); break;
            case 5: Tamer_tickChangeMap(); break;
            case 6: Tamer_tickIdle(); break;
            case 7: Tamer_tickPickupItem(); break;
            case 8: Tamer_tickTraining(); break;
            case 9: Tamer_tickScold(); break;
            case 10: break; // do nothing
            case 11: Tamer_tickFishing(); break;
            case 12: KAR_tick(); break;
            case 13: Tamer_tickPraise(); break;
            case 14: Tamer_tickTakeChest(); break;
            case 15: Tamer_tickOpening(); break;
            case 16: Tamer_tickEnding(); break;
            case 17: Tamer_tickSicknessLostLife(); break;
            case 18: Tamer_tickMachinedramonSpawn(); break;
            case 19: Tamer_tickBattleLostLife(); break;
            case 20: Tamer_tickAwardSomething(); break;
        }
        tickAnimation(&TAMER_ENTITY);
    }

    void Tamer_tick(int32_t instanceId)
    {
        if (GAME_STATE != 0 || Tamer_getState() != 0)
        {
            if (HAS_BUTTERFLY == 0)
            {
                unsetButterfly(BUTTERFLY_ID);
                HAS_BUTTERFLY = -1;
            }
        }

        switch (GAME_STATE)
        {
            case 0: Tamer_tickOverworld(instanceId); break;
            case 1:
            case 2:
            case 3: Tamer_tickBattle(instanceId); break;
            case 4:
            case 5: STD_Tamer_tickTournament(instanceId); break;
        }
    }

    void initializeTamer(DigimonType type,
                         int32_t posX,
                         int32_t posY,
                         int32_t posZ,
                         int32_t rotX,
                         int32_t rotY,
                         int32_t rotZ)
    {
        PLAYER_SHADOW_ENABLED = 1;
        loadMMD(type, EntityType::PLAYER);
        ENTITY_TABLE.tamer = &TAMER_ENTITY;
        initializeDigimonObject(type, 0, Tamer_tick);
        setEntityPosition(0, posX, posY, posZ);
        setEntityRotation(0, rotX, rotY, rotZ);
        setupEntityMatrix(0);
        // vanilla sets the store pos here already, but since it overwrites it later it's redundant
        startAnimation(&TAMER_ENTITY, 0);
        GAME_STATE              = 0;
        TAMER_ENTITY.isOnMap    = 1;
        TAMER_ENTITY.isOnScreen = 1;

        // vanilla initializes HAS_ROTATION_DATA here, but we already moved it to Entity.cpp
        // vanilla initializes UNKNOWN_TAMER_DATA here, but it seems entirely unused
        // vanilla initializes PREVIOUS_CAMERA_POS_INITIALIZED here, but we moved it to Entity.cpp

        isStandingOnDrop      = false;
        STORED_TAMER_POS      = TAMER_ENTITY.posData->location;
        hasLevelsAwardPending = false;
        hasMedalAwardPending  = false;
        IS_IN_MENU            = 0;
    }

    void setupTamerOnWarp(int32_t posX, int32_t posY, int32_t posZ, int32_t rotation)
    {
        setEntityPosition(0, posX, posY, posZ);
        setEntityRotation(0, TAMER_ENTITY.posData->rotation.x, rotation, TAMER_ENTITY.posData->rotation.z);
        setupEntityMatrix(0);
        startAnimation(&TAMER_ENTITY, 0);
    }

    void startFeedingItem(ItemType item)
    {
        if (TAMER_ITEM.type != ItemType::NONE) return;

        TAMER_ITEM.type = item;
        Tamer_setState(6);
        Partner_setState(5);
        removeObject(ObjectID::GAME_MENU, 0);
        IS_IN_MENU = 0;
        startGameTime();
    }

    void removeOneSelectedItem()
    {
        removeItem(INVENTORY_ITEM_TYPES[INVENTORY_POINTER], 1);
    }

    void renderFeedingItem(int32_t instanceId)
    {
        if (instanceId != 0) return;

        auto& pos                   = TAMER_ENTITY.posData[9].posMatrix.work.t;
        TAMER_ITEM.spriteLocation.x = pos[0];
        TAMER_ITEM.spriteLocation.y = pos[1];
        TAMER_ITEM.spriteLocation.z = pos[2];
        renderOverworldItem(&TAMER_ITEM);
    }

    void removeTamerItem()
    {
        if (TAMER_ITEM.type == ItemType::NONE) return;

        removeObject(ObjectID::TAMER_ITEM, 0);
        TAMER_ITEM.type = ItemType::NONE;
    }
}