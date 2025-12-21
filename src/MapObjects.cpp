#include "Files.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
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
}