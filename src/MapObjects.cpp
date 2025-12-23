#include "Entity.hpp"
#include "Files.hpp"
#include "GameData.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "extern/dw1.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"

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

    struct DoorObject
    {
        uint8_t modelId{0xFF};
        GsDOBJ2 obj;
        GsCOORDINATE2 coords;
    };

    dtl::array<DoorObject, 4> doorObjects;
    dtl::array<uint8_t, 2048> CHEST_MESH_BUFFER;
    GsCOORDINATE2 CHEST_COORD1;
    GsCOORDINATE2 CHEST_COORD2;
    GsDOBJ2 CHEST_OBJECT1;
    GsDOBJ2 CHEST_OBJECT2;
    dtl::array<WarpCrystalData, 5> WARP_CRYSTAL_DATA;
    int8_t doorRotationTimer;
    int8_t dirtPileSizeCounter = 15; // TODO: this value isn't persisted in the savegame
    int8_t toyTownSelectedBox;
    uint8_t activeDirtCartModel;
    int16_t bigBoxLidProgress;
    int16_t smallBoxLidProgress;
    int16_t angemonPedestalProgressX;
    int16_t angemonPedestalProgressZ;

    void renderDirtCartModel(int32_t instanceId)
    {
        if (!MAP_LAYER_ENABLED) return;
        if (activeDirtCartModel == 2) return;

        Matrix matrix;
        Vector vector = {.x = 0, .y = 0, .z = getDigimonData(PARTNER_ENTITY.type)->radius + 100};
        Vector result;

        libgte_RotMatrix(&PARTNER_ENTITY.posData->rotation, &matrix);
        libgte_ApplyMatrixLV(&matrix, &vector, &result);
        result.x += PARTNER_ENTITY.posData->location.x;
        result.z += PARTNER_ENTITY.posData->location.z;
        libgte_RotMatrix(&PARTNER_ENTITY.posData->rotation, &GENERAL_COORDS[activeDirtCartModel].coord);
        libgte_TransMatrix(&GENERAL_COORDS[activeDirtCartModel].coord, &result);

        GENERAL_COORDS[activeDirtCartModel].flag = 0;
        drawObject(&GENERAL_OBJECT[activeDirtCartModel], GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
    }

    void renderDirtPile(int32_t instance)
    {
        if (dirtPileSizeCounter <= 5) return;
        if (!MAP_LAYER_ENABLED) return;

        drawObject(&GENERAL_OBJECT3, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
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
            if (!isBoxOnScreen(&chest.location, 100, 100)) continue;

            projectPosition(&CHEST_COORD1, &chest.location, &chest.rotation, &scale);
            drawObject(&CHEST_OBJECT1, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
            projectPosition(&CHEST_COORD2, &chest.trayLocation, &chest.trayRotation, &scale);
            drawObject(&CHEST_OBJECT2, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
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
            if (!isBoxOnScreen(&crystal.pos1, 100, 100)) continue;

            projectPosition(&WARP_CRYSTAL_COORD1, &crystal.pos1, &crystal.rotation1, &scale);
            drawObject(&WARP_CRYSTAL_OBJECT1, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
            projectPosition(&WARP_CRYSTAL_COORD2, &crystal.pos2, &crystal.rotation2, &scale);
            drawObject(&WARP_CRYSTAL_OBJECT2, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
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
        drawObject(&GENERAL_OBJECT3, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
    }

    void renderToyTownBoxes(int32_t instance)
    {
        if (!MAP_LAYER_ENABLED) return;

        if (toyTownSelectedBox == 1)
        {
            bigBoxLidProgress -= 100;
            bigBoxLidProgress = max(-1024, bigBoxLidProgress);

            SVector rot = {bigBoxLidProgress, 0, 0, 0};
            libgte_RotMatrix(&rot, &WARP_CRYSTAL_COORD2.coord);
            WARP_CRYSTAL_COORD2.flag = 0;
        }
        if (toyTownSelectedBox == 2)
        {
            smallBoxLidProgress -= 100;
            smallBoxLidProgress = max(-1024, smallBoxLidProgress);
            SVector rot         = {smallBoxLidProgress, 0, 0, 0};
            libgte_RotMatrix(&rot, &GENERAL_COORDS[1].coord);
            GENERAL_COORDS[1].flag = 0;
        }

        drawObject(&WARP_CRYSTAL_OBJECT1, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
        drawObject(&WARP_CRYSTAL_OBJECT2, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
        drawObject(&GENERAL_OBJECT[0], GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
        drawObject(&GENERAL_OBJECT[1], GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
    }

    void renderGearbox(int32_t instance)
    {
        if (!MAP_LAYER_ENABLED) return;
        drawObject(&GENERAL_OBJECT3, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
    }

    void renderAngemonPedestal(int32_t instance)
    {
        if (!MAP_LAYER_ENABLED) return;

        Vector location = {angemonPedestalProgressX, 0, angemonPedestalProgressZ, 0};
        libgte_TransMatrix(&GENERAL_COORDS3.coord, &location);
        GENERAL_COORDS3.flag = 0;
        drawObject(&GENERAL_OBJECT3, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
    }

    void renderTrainingPoop(int32_t instance)
    {
        if (!MAP_LAYER_ENABLED) return;
        drawObject(&GENERAL_OBJECT3, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
    }

    void renderDoors(int32_t instance)
    {
        if (!MAP_LAYER_ENABLED) return;

        Vector scale = {4096, 4096, 4096, 0};

        for (auto& obj : MAP_3D_OBJECTS)
        {
            if (obj.modelId == 0xFF) continue;

            if (!isBoxOnScreen(&obj.translation, 700, 800)) continue;

            int32_t instanceId = 0;
            for (; instanceId < doorObjects.size(); instanceId++)
                if (doorObjects[instanceId].modelId == obj.modelId) break;

            projectPosition(&doorObjects[instanceId].coords, &obj.translation, &obj.rotation, &scale);
            drawObject(&doorObjects[instanceId].obj, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);

            if (obj.modelId == 10)
            {
                auto copy = obj;
                copy.translation.z += (obj.rotation.y < 0 ? 680 : -680);
                copy.rotation.y = -obj.rotation.y;
                projectPosition(&doorObjects[instanceId].coords, &copy.translation, &copy.rotation, &scale);
                drawObject(&doorObjects[instanceId].obj, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
            }
        }
    }
} // namespace

extern "C"
{
    void loadDirtCartModel()
    {
        loadStaticTMD("\\ETCNA\\DOSYA.TMD", GENERAL_MESH_BUFFER[0].data(), &GENERAL_OBJECT[0], &GENERAL_COORDS[0]);
        loadStaticTMD("\\ETCNA\\HAKO.TMD", GENERAL_MESH_BUFFER[1].data(), &GENERAL_OBJECT[1], &GENERAL_COORDS[1]);
        activeDirtCartModel = 2;
        addObject(ObjectID::DIRT_CART, 0, nullptr, renderDirtCartModel);
    }

    void loadDirtPileModel()
    {
        Vector location  = {2037, 0, 849, 0};
        SVector rotation = {0, 0, 0, 0};
        auto scaleValue  = (dirtPileSizeCounter * 4096) / 15;
        Vector scale     = {scaleValue, scaleValue, scaleValue, 0};

        loadStaticTMD("\\ETCNA\\T_YAMA.TMD", GENERAL_MESH_BUFFER[3].data(), &GENERAL_OBJECT3, &GENERAL_COORDS3);

        projectPosition(&GENERAL_COORDS3, &location, &rotation, &scale);
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
        bigBoxLidProgress        = 0;
        smallBoxLidProgress      = 0;
        toyTownSelectedBox       = 0;

        addObject(ObjectID::TOY_TOWN_BOXES, 0, nullptr, renderToyTownBoxes);
    }

    void spawnGearbox()
    {
        Vector location    = {-100, 0, 1700, 0};
        toyTownSelectedBox = 0;
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
        angemonPedestalProgressX = 0;
        angemonPedestalProgressZ = 1500;
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

    bool tickRotateDoor(int32_t instance, int32_t target)
    {
        doorRotationTimer++;
        switch (MAP_3D_OBJECTS[instance].modelId)
        {
            case 0:
            {
                if (doorRotationTimer > 32)
                {
                    doorRotationTimer = 0;
                    return 1;
                }

                MAP_3D_OBJECTS[instance].rotation.y -= target * 33;
                return 0;
            }
            case 5:
            case 31:
            case 32:
                if (doorRotationTimer > 9)
                {
                    doorRotationTimer = 0;
                    return 1;
                }
                MAP_3D_OBJECTS[instance].translation.y -= target * 200;
                return 0;
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 16:
            case 17:
            case 18:
            case 19:
            {
                if (doorRotationTimer > 32)
                {
                    MAP_3D_OBJECTS[instance].rotation.y = MAP_3D_OBJECTS[instance].direction;
                    doorRotationTimer                   = 0;
                    return 1;
                }

                MAP_3D_OBJECTS[instance].rotation.y += target * 33;
                return 0;
            }
            case 14:
            case 15:
            {
                if (doorRotationTimer > 32)
                {
                    doorRotationTimer = 0;
                    return 1;
                }

                MAP_3D_OBJECTS[instance].rotation.y += target * 33;
                return 0;
            }
            case 20:
            case 23:
            case 25:
            case 28:
            case 30:
            {
                if (doorRotationTimer > 9)
                {
                    doorRotationTimer = 0;
                    return 1;
                }
                if (CURRENT_SCREEN == 0x81)
                    MAP_3D_OBJECTS[instance].translation.z -= 30;
                else
                    MAP_3D_OBJECTS[instance].translation.x -= 30;
                return 0;
            }
            case 21:
            case 22:
            case 24:
            case 27:
            case 29:
            {
                if (doorRotationTimer > 9)
                {
                    doorRotationTimer = 0;
                    return 1;
                }
                if (CURRENT_SCREEN == 0x81)
                    MAP_3D_OBJECTS[instance].translation.z += 30;
                else
                    MAP_3D_OBJECTS[instance].translation.x += 30;
                return 0;
            }
            case 26:
            {
                MAP_3D_OBJECTS[instance].rotation.x -= target * 110;
                MAP_3D_OBJECTS[instance].rotation.x = clamp(MAP_3D_OBJECTS[instance].rotation.x, -1024, 0);
                if (doorRotationTimer > 19)
                {
                    doorRotationTimer = 0;
                    return 1;
                }
                return 0;
            }
        }

        return 0;
    }

    void loadDoors(int32_t doorEntryId)
    {
        // vanilla has a bool here, for disabling door loading, but it's never set
        doorObjects = {};

        const auto& data = DOOR_MAPDATA[doorEntryId];

        for (auto entry : data.modelId)
        {
            for (auto& slot : doorObjects)
            {
                if (slot.modelId == 0xFF)
                {
                    slot.modelId = entry;
                    break;
                }
                if (slot.modelId == entry) break;
            }
        }

        for (int32_t i = 0; i < doorObjects.size(); i++)
        {
            auto model = doorObjects[i].modelId;
            if (model == 0xFF) continue;

            dtl::array<uint8_t, 32> buffer;
            sprintf(buffer.data(), "\\DOOR\\DOOR%02d.TMD", model);
            loadStaticTMD(reinterpret_cast<char*>(buffer.data()),
                          GENERAL_MESH_BUFFER[i].data(),
                          &doorObjects[i].obj,
                          &doorObjects[i].coords);
        }

        for (int32_t i = 0; i < MAP_3D_OBJECTS.size(); i++)
        {
            auto& entry = MAP_3D_OBJECTS[i];

            entry.modelId     = data.modelId[i];
            entry.translation = {data.posX[i], data.posY[i], data.posZ[i], 0};
            entry.rotation    = {0, data.rotation[i], 0, 0};
            entry.direction   = data.rotation[i];
        }

        doorRotationTimer = 0;
        addObject(ObjectID::DOORS, 0, nullptr, renderDoors);
    }

    bool tickOpenChestTray(uint32_t chestId)
    {
        auto& chest = CHEST_ARRAY[chestId];

        chest.trayLocation.z -= 20;

        if (chest.trayLocation.z <= -160)
        {
            chest.trayLocation.z = -160;
            chest.isTaken        = true;
            return true;
        }
        return false;
    }

    bool tickCloseChestTray(uint32_t chestId)
    {
        auto& chest = CHEST_ARRAY[chestId];

        chest.trayLocation.z += 20;

        if (chest.trayLocation.z >= 0)
        {
            chest.trayLocation.z = 0;
            chest.isTaken        = true;
            return true;
        }
        return false;
    }

    void decreaseDirtPileSize()
    {
        dirtPileSizeCounter -= 1;
        SVector rotation = {0, 0, 0, 0};
        auto scaleValue  = (dirtPileSizeCounter * 4096) / 15;
        Vector scale     = {scaleValue, scaleValue, scaleValue, 0};

        libgte_RotMatrix(&rotation, &GENERAL_COORDS3.coord);
        libgte_ScaleMatrix(&GENERAL_COORDS3.coord, &scale);
        GENERAL_COORDS3.flag = 0;
    }

    void moveBoulder(int32_t diffX, int32_t diffZ)
    {
        Vector scale = {4096, 4096, 4096, 0};

        MAP_3D_OBJECTS[0].translation.y += diffX;
        MAP_3D_OBJECTS[0].translation.z += diffZ;
        MAP_3D_OBJECTS[0].rotation.x -= 10;
        projectPosition(&GENERAL_COORDS3, &MAP_3D_OBJECTS[0].translation, &MAP_3D_OBJECTS[0].rotation, &scale);
    }

    void openToyTownBox(int8_t boxId)
    {
        toyTownSelectedBox = boxId;
    }

    void somethingToyTown(int8_t boxId)
    {
        // TODO this is completely useless?
        toyTownSelectedBox = boxId;
        if (boxId == 0 && isTriggerSet(350)) readMapTFS(0x92);
    }

    bool moveAngemonPedestal()
    {
        angemonPedestalProgressZ += 10;
        angemonPedestalProgressX = angemonPedestalProgressZ % 20 == 0 ? 5 : -5;
        angemonPedestalProgressZ = min(2000, angemonPedestalProgressZ);

        return angemonPedestalProgressZ >= 2000;
    }

    void setDirtCartModel(int32_t model)
    {
        activeDirtCartModel = model;
    }
}