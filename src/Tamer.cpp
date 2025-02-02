#include "Model.hpp"
#include "extern/STD.hpp"
#include "extern/dw1.hpp"
#include "extern/libgte.hpp"

extern "C"
{
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
}