#include "Helper.hpp"
#include "Model.hpp"
#include "extern/KAR.hpp"
#include "extern/STD.hpp"
#include "extern/dw1.hpp"
#include "extern/libgte.hpp"

extern "C"
{
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
            case 6: Tamer_tickEvolution(); break;
            case 7: Tamer_tickPickupItem(); break;
            case 8: Tamer_tickTraining(); break;
            case 9:
            case 13: Tamer_tickPraiseScold(); break;
            case 11: Tamer_tickFishing(); break;
            case 12: KAR_tick(); break;
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
        PICKED_UP_ITEM                  = 0;
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