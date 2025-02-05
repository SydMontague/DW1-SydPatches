#include "Font.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Model.hpp"
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
    static uint8_t takeItemFrameCount;

    void Tamer_setState(uint8_t state)
    {
        TAMER_STATE     = state;
        TAMER_SUB_STATE = 0;
    }

    void Tamer_setSubState(uint8_t state)
    {
        TAMER_SUB_STATE = state;
    }

    void Tamer_setFullState(uint8_t state, uint8_t subState)
    {
        TAMER_STATE     = state;
        TAMER_SUB_STATE = subState;
    }

    uint8_t Tamer_getState()
    {
        return TAMER_STATE;
    }

    uint8_t Tamer_getSubState()
    {
        return TAMER_SUB_STATE;
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

    void Tamer_tickTakeChest()
    {
        RECT textArea = {.x = 0, .y = 12, .width = 256, .height = 200};
        Chest& chest  = CHEST_ARRAY[INTERACTED_CHEST];
        auto itemType = chest.item;

        if (Tamer_getSubState() == 0)
        {
            startAnimation(&TAMER_ENTITY, 0);
            Partner_setState(11);
            unsetCameraFollowPlayer();
            entityLookAtLocation(&TAMER_ENTITY, &chest.location);
            clearTextSubArea(&textArea);

            drawStringNew(&vanillaFont, getDigimonData(DigimonType::TAMER)->name, 704 + 0, 256 + 12);
            if (chest.isTaken)
            {
                drawStringNew(&vanillaFont, reinterpret_cast<const uint8_t*>("Hey! It's empty!"), 704 + 0, 256 + 24);
                TAKE_CHEST_STATE = 2;
            }
            else
            {
                drawStringNew(&vanillaFont, getItem(itemType)->name, 704 + 0, 256 + 24);
                drawStringNew(&vanillaFont, reinterpret_cast<const uint8_t*>("Woah!"), 704 + 0, 256 + 36);
                TAKE_CHEST_STATE = 0;
            }
            Tamer_setSubState(1);
        }
        else if (Tamer_getSubState() == 1)
        {
            if (!tickOpenChestTray(INTERACTED_CHEST)) return;
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
            Tamer_setSubState(TAKE_CHEST_STATE == 0 ? 2 : 4);
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
            if (!giveItem(itemType, 0))
            {
                drawStringNew(&vanillaFont,
                              reinterpret_cast<const uint8_t*>("I can't hold anymore."),
                              704 + 0,
                              256 + 24);
                TAKE_CHEST_STATE = 1;
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
            if (!tickCloseChestTray(INTERACTED_CHEST)) return;
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

            unsetUIBoxAnimated(1, &target);

            if (TAKE_CHEST_STATE == 0) giveItem(itemType, 1);

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
        auto itemType = DROPPED_ITEMS[PICKED_UP_DROP_ID].type;

        if (Tamer_getSubState() == 0)
        {
            startAnimation(&TAMER_ENTITY, 12);
            unsetCameraFollowPlayer();
            clearTextSubArea(&textArea);
            drawStringNew(&vanillaFont, getDigimonData(DigimonType::TAMER)->name, 704 + 0, 256 + 12);
            drawStringNew(&vanillaFont, getItem(itemType)->name, 704 + 0, 256 + 24);
            drawStringNew(&vanillaFont, reinterpret_cast<const uint8_t*>("Woah!"), 704 + 0, 256 + 36);

            TAKE_CHEST_STATE   = 0;
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
                drawStringNew(&vanillaFont,
                              reinterpret_cast<const uint8_t*>("I can't hold anymore."),
                              704 + 0,
                              256 + 24);
                TAKE_CHEST_STATE   = 1;
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

            unsetUIBoxAnimated(1, &target);

            if (TAKE_CHEST_STATE == 0) pickupItem(PICKED_UP_DROP_ID);

            Tamer_setState(0);
            setCameraFollowPlayer();
        }

        // Vanilla check for takeItemFrameCount to be below 60 before calling playSound(0, 3),
        // but it also limits the value to 10. So that code is dead and got removed.
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
        addTriangleMenu();
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
        switch (TAMER_STATE)
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
            case 17: Tamer_tickBattleLostLife(); break;
            case 18: Tamer_tickMachinedramonSpawn(); break;
            case 19: Tamer_tickSicknessLostLife(); break;
            case 20: Tamer_tickAwardSomething(); break;
        }
        tickAnimation(&TAMER_ENTITY);
    }

    void Tamer_tick(int32_t instanceId)
    {
        if (GAME_STATE != 0 || TAMER_STATE != 0)
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

        for (auto& val : HAS_ROTATION_DATA)
            val = 0;
        for (auto& val : UNKNOWN_TAMER_DATA)
            val = 0; // unused?

        PREVIOUS_CAMERA_POS_INITIALIZED = 0;
        HAS_PICKED_UP_ITEM              = 0;
        STORED_TAMER_POS                = TAMER_ENTITY.posData->location;
        TAMER_LEVEL_AWARD_PENDING       = 0;
        MEDAL_AWARD_PENDING             = 0;
        IS_IN_MENU                      = 0;
        TAMER_LEVELS_AWARDED            = 1;
    }

    void setupTamerOnWarp(int32_t posX, int32_t posY, int32_t posZ, int32_t rotation)
    {
        setEntityPosition(0, posX, posY, posZ);
        setEntityRotation(0, TAMER_ENTITY.posData->rotation.x, rotation, TAMER_ENTITY.posData->rotation.z);
        setupEntityMatrix(0);
        startAnimation(&TAMER_ENTITY, 0);
    }
}