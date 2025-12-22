#include "Entity.hpp"
#include "Files.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "extern/dw1.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"

extern dtl::array<GsDOBJ2, 2> GENERAL_OBJECT;
extern dtl::array<GsCOORDINATE2, 2> GENERAL_COORDS;
extern GsDOBJ2 GENERAL_OBJECT3;
extern GsCOORDINATE2 GENERAL_COORDS3;
extern uint8_t ACTIVE_DIRT_CART_MODEL;
extern uint8_t DIRT_PILE_SIZE_COUNTER;

extern GsCOORDINATE2 WARP_CRYSTAL_COORD1;
extern GsCOORDINATE2 WARP_CRYSTAL_COORD2;
extern GsDOBJ2 WARP_CRYSTAL_OBJECT1;
extern GsDOBJ2 WARP_CRYSTAL_OBJECT2;

namespace
{
    struct WarpCrystalData
    {
        Vector pos1;
        Vector pos2;
        SVector rotation1;
        SVector rotation2;
        bool isSet;
    };

    dtl::array<uint8_t, 2048> CHEST_MESH_BUFFER;
    GsCOORDINATE2 CHEST_COORD1;
    GsCOORDINATE2 CHEST_COORD2;
    GsDOBJ2 CHEST_OBJECT1;
    GsDOBJ2 CHEST_OBJECT2;
    dtl::array<WarpCrystalData, 5> WARP_CRYSTAL_DATA;

    void renderDirtCartModel(int32_t instanceId)
    {
        if (!MAP_LAYER_ENABLED) return;
        if (ACTIVE_DIRT_CART_MODEL == 2) return;

        Matrix matrix;
        Vector vector = {.x = 0, .y = 0, .z = getDigimonData(PARTNER_ENTITY.type)->radius + 100};
        Vector result;

        libgte_RotMatrix(&PARTNER_ENTITY.posData->rotation, &matrix);
        libgte_ApplyMatrixLV(&matrix, &vector, &result);
        result.x += PARTNER_ENTITY.posData->location.x;
        result.z += PARTNER_ENTITY.posData->location.z;
        libgte_RotMatrix(&PARTNER_ENTITY.posData->rotation, &GENERAL_COORDS[ACTIVE_DIRT_CART_MODEL].coord);
        libgte_TransMatrix(&GENERAL_COORDS[ACTIVE_DIRT_CART_MODEL].coord, &result);

        GENERAL_COORDS[ACTIVE_DIRT_CART_MODEL].flag = 0;
        renderObject(&GENERAL_OBJECT[ACTIVE_DIRT_CART_MODEL], GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
    }

    void renderDirtPile(int32_t instance)
    {
        if (DIRT_PILE_SIZE_COUNTER <= 5) return;
        if (!MAP_LAYER_ENABLED) return;

        renderObject(&GENERAL_OBJECT3, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
    }

    Chest* getFreeChest()
    {
        for (auto& chest : CHEST_ARRAY)
        {
            if (chest.item == ItemType::NONE) return &chest;
        }

        return nullptr;
    }

    void clearChests()
    {
        for (auto& chest : CHEST_ARRAY)
        {
            chest.item    = ItemType::NONE;
            chest.isTaken = false;
        }
    }

    void renderChest(int32_t instance)
    {
        if (!MAP_LAYER_ENABLED) return;

        Vector scale = {4096, 4096, 4096, 0};

        for (auto& chest : CHEST_ARRAY)
        {
            if (chest.item == ItemType::NONE) continue;
            if (isBoxOffScreen(&chest.location, 100, 100)) continue;

            projectPosition(&CHEST_COORD1, &chest.location, &chest.rotation, &scale);
            renderObject(&CHEST_OBJECT1, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
            projectPosition(&CHEST_COORD2, &chest.trayLocation, &chest.trayRotation, &scale);
            renderObject(&CHEST_OBJECT2, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
        }
    }

    WarpCrystalData* getFreeWarpCrystal()
    {
        for (auto& crystal : WARP_CRYSTAL_DATA)
        {
            if (!crystal.isSet) return &crystal;
        }

        return nullptr;
    }

    void renderWarpCrystals(int32_t instance)
    {
        Vector scale = {4096, 4096, 4096, 0};

        for (auto& crystal : WARP_CRYSTAL_DATA)
        {
            if (!crystal.isSet) continue;
            if (isBoxOffScreen(&crystal.pos1, 100, 100)) continue;

            projectPosition(&WARP_CRYSTAL_COORD1, &crystal.pos1, &crystal.rotation1, &scale);
            renderObject(&WARP_CRYSTAL_OBJECT1, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
            projectPosition(&WARP_CRYSTAL_COORD2, &crystal.pos2, &crystal.rotation2, &scale);
            renderObject(&WARP_CRYSTAL_OBJECT2, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
        }
    }

    void addWarpCrystal(int32_t posX, int32_t posZ)
    {
        auto crystal = getFreeWarpCrystal();

        if (!crystal) return;

        crystal->isSet     = true;
        crystal->pos1      = {posX, -188, posZ};
        crystal->pos2      = {0, -188, 0};
        crystal->rotation1 = {0, 0, 0};
        crystal->rotation2 = {0, 0, 0};
    }

    void renderBoulder(int32_t instance)
    {
        if (!MAP_LAYER_ENABLED) return;
        renderObject(&GENERAL_OBJECT3, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
    }

    void renderToyTownBoxes(int32_t instance)
    {
        if (!MAP_LAYER_ENABLED) return;

        if (TOY_TOWN_SELECTED_BOX == 1)
        {
            BIG_BOX_LID_PROGRESS -= 100;
            BIG_BOX_LID_PROGRESS = max(-1024, BIG_BOX_LID_PROGRESS);

            SVector rot = {BIG_BOX_LID_PROGRESS, 0, 0, 0};
            libgte_RotMatrix(&rot, &WARP_CRYSTAL_COORD2.coord);
            WARP_CRYSTAL_COORD2.flag = 0;
        }
        if (TOY_TOWN_SELECTED_BOX == 2)
        {
            SMALL_BOX_LID_PROGRESS -= 100;
            SMALL_BOX_LID_PROGRESS = max(-1024, SMALL_BOX_LID_PROGRESS);
            SVector rot            = {SMALL_BOX_LID_PROGRESS, 0, 0, 0};
            libgte_RotMatrix(&rot, &GENERAL_COORDS[1].coord);
            GENERAL_COORDS[1].flag = 0;
        }

        renderObject(&WARP_CRYSTAL_OBJECT1, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
        renderObject(&WARP_CRYSTAL_OBJECT2, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
        renderObject(&GENERAL_OBJECT[0], GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
        renderObject(&GENERAL_OBJECT[1], GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
    }

    void renderGearbox(int32_t instance)
    {
        if (!MAP_LAYER_ENABLED) return;
        renderObject(&GENERAL_OBJECT3, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
    }

    void renderAngemonPedestal(int32_t instance)
    {
        if (!MAP_LAYER_ENABLED) return;

        Vector location = {ANGEMON_PEDESTAL_PROGRESS_X, 0, ANGEMON_PEDESTAL_PROGRESS_Z, 0};
        libgte_TransMatrix(&GENERAL_COORDS3.coord, &location);
        GENERAL_COORDS3.flag = 0;
        renderObject(&GENERAL_OBJECT3, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
    }

    void renderTrainingPoop(int32_t instance)
    {
        if (!MAP_LAYER_ENABLED) return;
        renderObject(&GENERAL_OBJECT3, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
    }
} // namespace

extern "C"
{
    void loadDirtCartModel()
    {
        loadStaticTMD("\\ETCNA\\DOSYA.TMD", GENERAL_MESH_BUFFER[0].data(), &GENERAL_OBJECT[0], &GENERAL_COORDS[0]);
        loadStaticTMD("\\ETCNA\\HAKO.TMD", GENERAL_MESH_BUFFER[1].data(), &GENERAL_OBJECT[1], &GENERAL_COORDS[1]);
        ACTIVE_DIRT_CART_MODEL = 2;
        addObject(ObjectID::DIRT_CART, 0, nullptr, renderDirtCartModel);
    }

    void loadDirtPileModel()
    {
        Vector location  = {2037, 0, 849, 0};
        SVector rotation = {0, 0, 0, 0};
        auto scaleValue  = (DIRT_PILE_SIZE_COUNTER * 4096) / 15;
        Vector scale     = {scaleValue, scaleValue, scaleValue, 0};

        loadStaticTMD("\\ETCNA\\T_YAMA.TMD", GENERAL_MESH_BUFFER[3].data(), &GENERAL_OBJECT3, &GENERAL_COORDS3);
        libgte_RotMatrix(&rotation, &GENERAL_COORDS3.coord);
        libgte_ScaleMatrix(&GENERAL_COORDS3.coord, &scale);
        libgte_TransMatrix(&GENERAL_COORDS3.coord, &location);
        GENERAL_COORDS3.flag = 0;
        addObject(ObjectID::DIRT_PILE, 0, nullptr, renderDirtPile);
    }

    void initializeChest()
    {
        clearChests();
        readFile("\\ETCNA\\TAKA.TMD", CHEST_MESH_BUFFER.data());
        libgs_GsMapModelingData(reinterpret_cast<uint32_t*>(CHEST_MESH_BUFFER.data() + 4));
        libgs_GsInitCoordinate2(nullptr, &CHEST_COORD1);
        libgs_GsInitCoordinate2(&CHEST_COORD1, &CHEST_COORD2);
        libgs_GsLinkObject4(reinterpret_cast<uint32_t*>(CHEST_MESH_BUFFER.data() + 0xC), &CHEST_OBJECT1, 0);
        libgs_GsLinkObject4(reinterpret_cast<uint32_t*>(CHEST_MESH_BUFFER.data() + 0xC), &CHEST_OBJECT2, 1);
        CHEST_OBJECT1.attribute = 0;
        CHEST_OBJECT1.coord2    = &CHEST_COORD1;
        CHEST_OBJECT2.attribute = 0;
        CHEST_OBJECT2.coord2    = &CHEST_COORD2;

        addObject(ObjectID::CHEST, 0, nullptr, renderChest);
    }

    void loadWarpCrystals(int32_t mapId)
    {
        // vanilla calls clearChests() here, but that's useless and probably a copy+paste error
        readFile("\\ETCNA\\WARP.TMD", GENERAL_MESH_BUFFER[0].data());
        libgs_GsMapModelingData(reinterpret_cast<uint32_t*>(GENERAL_MESH_BUFFER[0].data() + 4));
        libgs_GsInitCoordinate2(nullptr, &WARP_CRYSTAL_COORD1);
        libgs_GsInitCoordinate2(&WARP_CRYSTAL_COORD1, &WARP_CRYSTAL_COORD2);
        libgs_GsLinkObject4(reinterpret_cast<uint32_t*>(GENERAL_MESH_BUFFER[0].data() + 0xC), &WARP_CRYSTAL_OBJECT1, 0);
        libgs_GsLinkObject4(reinterpret_cast<uint32_t*>(GENERAL_MESH_BUFFER[0].data() + 0xC), &WARP_CRYSTAL_OBJECT2, 1);
        WARP_CRYSTAL_OBJECT1.attribute = 0;
        WARP_CRYSTAL_OBJECT1.coord2    = &WARP_CRYSTAL_COORD1;
        WARP_CRYSTAL_OBJECT2.attribute = 0;
        WARP_CRYSTAL_OBJECT2.coord2    = &WARP_CRYSTAL_COORD2;

        addObject(ObjectID::WARP_CRYSTAL, 0, nullptr, renderWarpCrystals);

        for (auto& crystal : WARP_CRYSTAL_DATA)
            crystal.isSet = false;

        switch (mapId)
        {
            case 0x65:
                addWarpCrystal(2237, -1667);
                addWarpCrystal(-2237, -1667);
                addWarpCrystal(2358, 419);
                addWarpCrystal(-2358, 419);
                break;
            case 0x66: addWarpCrystal(0, 0); break;
            case 0x67:
                addWarpCrystal(2376, -127);
                addWarpCrystal(1566, -3358);
                break;
            case 0x68:
                addWarpCrystal(-2100, 1900);
                addWarpCrystal(2100, 1900);
                addWarpCrystal(-2100, -2100);
                addWarpCrystal(2100, -2100);
                break;
        }
    }

    void spawnBoulder()
    {
        Vector scale = {4096, 4096, 4096, 0};
        loadStaticTMD("\\ETCNA\\IWA.TMD", GENERAL_MESH_BUFFER[0].data(), &GENERAL_OBJECT3, &GENERAL_COORDS3);
        loadTextureFile("\\ETCNA\\IWA1.TIM", nullptr, nullptr);
        MAP_3D_OBJECTS[0].translation = {-300, -500, 2000, 0};
        MAP_3D_OBJECTS[0].rotation    = {0, 0, 0, 0};

        projectPosition(&GENERAL_COORDS3, &MAP_3D_OBJECTS[0].translation, &MAP_3D_OBJECTS[0].rotation, &scale);
        addObject(ObjectID::BOULDER, 0, nullptr, renderBoulder);
    }

    void spawnToyTownBoxes()
    {
        Vector box1Pos = {500, -600, 600, 0};
        Vector box2Pos = {-400, -400, 600, 0};

        readFile("\\ETCNA\\BIGBOX.TMD", GENERAL_MESH_BUFFER[0].data());
        libgs_GsMapModelingData(reinterpret_cast<uint32_t*>(GENERAL_MESH_BUFFER[0].data() + 4));
        libgs_GsInitCoordinate2(nullptr, &WARP_CRYSTAL_COORD1);
        libgs_GsInitCoordinate2(&WARP_CRYSTAL_COORD1, &WARP_CRYSTAL_COORD2);
        libgs_GsLinkObject4(reinterpret_cast<uint32_t*>(GENERAL_MESH_BUFFER[0].data() + 0xC), &WARP_CRYSTAL_OBJECT1, 0);
        libgs_GsLinkObject4(reinterpret_cast<uint32_t*>(GENERAL_MESH_BUFFER[0].data() + 0xC), &WARP_CRYSTAL_OBJECT2, 1);
        WARP_CRYSTAL_OBJECT1.attribute = 0;
        WARP_CRYSTAL_OBJECT1.coord2    = &WARP_CRYSTAL_COORD1;
        WARP_CRYSTAL_OBJECT2.attribute = 0;
        WARP_CRYSTAL_OBJECT2.coord2    = &WARP_CRYSTAL_COORD2;

        readFile("\\ETCNA\\SMABOX.TMD", GENERAL_MESH_BUFFER[1].data());
        libgs_GsMapModelingData(reinterpret_cast<uint32_t*>(GENERAL_MESH_BUFFER[1].data() + 4));
        libgs_GsInitCoordinate2(nullptr, &GENERAL_COORDS[0]);
        libgs_GsInitCoordinate2(&GENERAL_COORDS[0], &GENERAL_COORDS[1]);
        libgs_GsLinkObject4(reinterpret_cast<uint32_t*>(GENERAL_MESH_BUFFER[1].data() + 0xC), &GENERAL_OBJECT[0], 0);
        libgs_GsLinkObject4(reinterpret_cast<uint32_t*>(GENERAL_MESH_BUFFER[1].data() + 0xC), &GENERAL_OBJECT[1], 1);
        GENERAL_OBJECT[0].attribute = 0;
        GENERAL_OBJECT[0].coord2    = &GENERAL_COORDS[0];
        GENERAL_OBJECT[1].attribute = 0;
        GENERAL_OBJECT[1].coord2    = &GENERAL_COORDS[1];

        libgte_TransMatrix(&WARP_CRYSTAL_COORD1.coord, &box1Pos);
        libgte_TransMatrix(&GENERAL_COORDS[0].coord, &box2Pos);
        WARP_CRYSTAL_COORD1.flag = 0;
        GENERAL_COORDS[0].flag   = 0;
        BIG_BOX_LID_PROGRESS     = 0;
        SMALL_BOX_LID_PROGRESS   = 0;
        TOY_TOWN_SELECTED_BOX    = 0;

        addObject(ObjectID::TOY_TOWN_BOXES, 0, nullptr, renderToyTownBoxes);
    }

    void spawnGearbox()
    {
        Vector location       = {-100, 0, 1700, 0};
        TOY_TOWN_SELECTED_BOX = 0;
        loadStaticTMD("\\ETCNA\\GAND.TMD", GENERAL_MESH_BUFFER[0].data(), &GENERAL_OBJECT3, &GENERAL_COORDS3);
        libgte_TransMatrix(&GENERAL_COORDS3.coord, &location);
        GENERAL_COORDS3.flag = 0;
        // vanilla sets a value to 0x294 here, but the value is never used
        addObject(ObjectID::GEARBOX, 0, nullptr, renderGearbox);
    }

    void spawnAngemonPedestal()
    {
        loadStaticTMD("\\ETCNA\\ABOX.TMD", GENERAL_MESH_BUFFER[0].data(), &GENERAL_OBJECT3, &GENERAL_COORDS3);
        // vanilla does TransMatrix here, but that's unnecessary since it's done in the render function every frame
        ANGEMON_PEDESTAL_PROGRESS_X = 0;
        ANGEMON_PEDESTAL_PROGRESS_Z = 1500;
        addObject(ObjectID::DAYTIME_TRANSITION, 0, nullptr, renderAngemonPedestal); // TODO why DAYTIME_TRANSTION???
    }

    void loadTrainingPoop()
    {
        Vector translation = {1922, 0, 503, 0};
        SVector rotation   = {0, 0, 0, 0};
        Vector scale       = {4096, 4096, 4096, 0};

        loadStaticTMD("\\ETCNA\\TRY.TMD", GENERAL_MESH_BUFFER[0].data(), &GENERAL_OBJECT3, &GENERAL_COORDS3);
        projectPosition(&GENERAL_COORDS3, &translation, &rotation, &scale);
        addObject(ObjectID::TRAINING_POOP, 0, nullptr, renderTrainingPoop);
    }

    void spawnChest(int32_t posX, int32_t posY, int32_t posZ, int16_t rotation, ItemType item, uint16_t trigger)
    {
        auto* chest = getFreeChest();
        if (!chest) return;

        if (isTriggerSet(trigger))
        {
            chest->isTaken      = true;
            chest->trayLocation = {0, 0, -160, 0};
        }
        else
        {
            chest->isTaken      = false;
            chest->trayLocation = {0, 0, 0, 0};
        }

        const int16_t tileX = getTileX(posX);
        const int16_t tileZ = getTileZ(posZ);
        setImpassableSquare(tileX, tileZ, 2);

        chest->rotation     = {0, rotation, 0, 0};
        chest->trayRotation = {0, 0, 0, 0};
        chest->trigger      = trigger;
        chest->item         = item;
        chest->location     = {convertTileToPosX(tileX), 0, convertTileToPosZ(tileZ), 0};
        chest->tileX        = tileX;
        chest->tileY        = tileZ;

        if (rotation == 0)
            chest->tileY += 4;
        else if (rotation == 0x800)
            chest->tileY -= 4;

        if (rotation == 0x400)
            chest->tileX -= 4;
        else if (rotation == 0xC00)
            chest->tileX += 4;
    }
}