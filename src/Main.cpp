#include "Main.hpp"

#include "Camera.hpp"
#include "CustomUI.hpp"
#include "EFE.hpp"
#include "Effects.hpp"
#include "Entity.hpp"
#include "Fade.hpp"
#include "Files.hpp"
#include "GameObjects.hpp"
#include "GameTime.hpp"
#include "Helper.hpp"
#include "Input.hpp"
#include "Inventory.hpp"
#include "InventoryUI.hpp"
#include "ItemFunctions.hpp"
#include "Map.hpp"
#include "MapObjects.hpp"
#include "Model.hpp"
#include "Movie.hpp"
#include "Partner.hpp"
#include "Pause.hpp"
#include "PlayerMedalView.hpp"
#include "Sound.hpp"
#include "Tamer.hpp"
#include "Timestamp.hpp"
#include "Tournament.hpp"
#include "extern/MOV.hpp"
#include "extern/dw1.hpp"
#include "extern/libapi.hpp"
#include "extern/libcd.hpp"
#include "extern/libetc.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"
#include "extern/libmcrd.hpp"
#include "extern/libsnd.hpp"
#include "extern/psx.hpp"

namespace
{
    int32_t inputRepeatTimer;
    int32_t gameInputRepeatTimer;
    bool firstScreenPressedStart  = false;
    int32_t firstScreenFrameCount = 0;

    void initializeGsTMDMap(void)
    {
        GS_TMD_MAP[0]  = libgte_GsTMDfastF3L;
        GS_TMD_MAP[16] = libgte_GsTMDfastTF3L;
        GS_TMD_MAP[18] = libgte_GsTMDfastTF3NL;
        GS_TMD_MAP[22] = libgte_GsTMDfastTNF3;
        GS_TMD_MAP[8]  = libgte_GsTMDfastG3L;
        GS_TMD_MAP[24] = libgte_GsTMDfastTG3L;
        GS_TMD_MAP[26] = libgte_GsTMDfastTG3NL;
        GS_TMD_MAP[30] = libgte_GsTMDfastTNG3;
        GS_TMD_MAP[32] = libgte_GsTMDfastF4L;
        GS_TMD_MAP[48] = libgte_GsTMDfastTF4L;
        GS_TMD_MAP[50] = libgte_GsTMDfastTF4NL;
        GS_TMD_MAP[54] = libgte_GsTMDfastTNF4;
        GS_TMD_MAP[40] = libgte_GsTMDfastG4L;
        GS_TMD_MAP[56] = libgte_GsTMDfastTG4L;
        GS_TMD_MAP[58] = libgte_GsTMDfastTG4NL;
        GS_TMD_MAP[62] = libgte_GsTMDfastTNG4;
        GS_TMD_MAP[51] = libgte_GsTMDdivTF4L;
        GS_TMD_MAP[53] = libgte_GsTMDdivTF4NL;
        GS_TMD_MAP[21] = libgte_GsTMDdivTF3NL;
        GS_TMD_MAP[61] = libgte_GsTMDdivTG4NL;
        GS_TMD_MAP[29] = libgte_GsTMDdivTG3NL;
        GS_TMD_MAP[63] = libgte_GsTMDdivTNG4;
        GS_TMD_MAP[31] = libgte_GsTMDdivTNG3;
        GS_TMD_MAP[55] = libgte_GsTMDdivTNF4;
        GS_TMD_MAP[23] = libgte_GsTMDdivTNF3;
        GS_TMD_MAP[34] = libgte_GsTMDfastF4NL;
        GS_TMD_MAP[38] = libgte_GsTMDfastNF4;
        return;
    }

    void cleanupGame()
    {
        removeObject(ObjectID::MAP, 0);
        removeObject(ObjectID::POOP, 0);
        // vanilla removes 0xFA6, but it seems to be never added anywhere?
        removeObject(ObjectID::PLAYTIME, 0);
        removeObject(ObjectID::GAME_CLOCK, 0);
        removeObject(ObjectID::CHEST, 0);

        for (int32_t i = 0; i < 10; i++)
        {
            auto* entity = ENTITY_TABLE.getEntityById(i);
            if (entity != nullptr)
            {
                removeEntity(entity->type, i);
                unloadModel(static_cast<int32_t>(entity->type), getEntityTypeById(i));
            }
        }

        initializeLoadedNPCModels();
    }

    void customInit()
    {
        initTimestamp();
        initCustomUI();
        fillItemTable();
    }

    void initializeHeap()
    {
        auto size = (SECTION_DATA.stackTop - SECTION_DATA.heapPtr) - (SECTION_DATA.stackFrames * 0x400);
        libapi_InitHeap3(SECTION_DATA.heapPtr, size);
    }

    void pollInputMenu()
    {
        POLLED_INPUT_PREVIOUS = POLLED_INPUT;
        POLLED_INPUT          = libetc_PadRead(0);
        auto directionPressed = POLLED_INPUT & POLLED_INPUT_PREVIOUS & 0xf000f000;
        if (directionPressed == 0)
            inputRepeatTimer = 0;
        else if (inputRepeatTimer + 1 < 15)
        {
            directionPressed = 0;
            inputRepeatTimer++;
        }
        else
            inputRepeatTimer -= 5;

        CHANGED_INPUT = POLLED_INPUT & ~POLLED_INPUT_PREVIOUS | directionPressed;
    }

    void pollInputGame()
    {
        POLLED_INPUT_PREVIOUS = POLLED_INPUT;
        POLLED_INPUT          = libetc_PadRead(0);
        auto directionPressed = POLLED_INPUT & POLLED_INPUT_PREVIOUS & 0xf000f000;
        if (directionPressed == 0)
            gameInputRepeatTimer = 0;
        else if (gameInputRepeatTimer + 1 < 5)
        {
            directionPressed = 0;
            gameInputRepeatTimer++;
        }
        else
            gameInputRepeatTimer--;

        CHANGED_INPUT = POLLED_INPUT & ~POLLED_INPUT_PREVIOUS | directionPressed;
    }

    void runFrame()
    {
        ACTIVE_FRAMEBUFFER = libgs_GsGetActiveBuffer();
        libgs_GsSetWorkBase(GS_WORK_BASES + ACTIVE_FRAMEBUFFER);
        libgs_GsClearOt(0, 0, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER);
        ACTIVE_ORDERING_TABLE = GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER;
        tickObjects();
        renderObjects();
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 0x20, &DR_OFFSETS[ACTIVE_FRAMEBUFFER]);
        libgpu_DrawSync(0);
        libetc_vsync(0);
        libgpu_ResetGraph(1);
        libgs_GsSetOrign(DRAWING_OFFSET_X, DRAWING_OFFSET_Y);
        libgs_GsSwapDispBuff();
        libgs_GsDrawOt(GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER);
    }

    void renderMainMenuBackground(int32_t instance)
    {
        auto* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyFT4(prim);
        prim->x0    = -160;
        prim->y0    = -120;
        prim->x1    = 96;
        prim->y1    = -120;
        prim->x2    = -160;
        prim->y2    = 120;
        prim->x3    = 96;
        prim->y3    = 120;
        prim->u0    = 0;
        prim->v0    = 0;
        prim->u1    = 255;
        prim->v1    = 0;
        prim->u2    = 0;
        prim->v2    = 240;
        prim->u3    = 255;
        prim->v3    = 240;
        prim->r0    = 128;
        prim->g0    = 128;
        prim->b0    = 128;
        prim->tpage = getTPage(1, 0, 0x300, 0);
        prim->clut  = getClut(0, 0x1e0);
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 30, prim);

        prim++;
        libgpu_SetPolyFT4(prim);
        prim->x0    = 96;
        prim->y0    = -120;
        prim->x1    = 160;
        prim->y1    = -120;
        prim->x2    = 96;
        prim->y2    = 120;
        prim->x3    = 160;
        prim->y3    = 120;
        prim->u0    = 0;
        prim->v0    = 0;
        prim->u1    = 64;
        prim->v1    = 0;
        prim->u2    = 0;
        prim->v2    = 240;
        prim->u3    = 64;
        prim->v3    = 240;
        prim->r0    = 128;
        prim->g0    = 128;
        prim->b0    = 128;
        prim->tpage = getTPage(1, 0, 0x380, 0);
        prim->clut  = getClut(0, 0x1e0);
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 30, prim);

        libgs_GsSetWorkBase(prim + 1);
    }

    void renderPressStartToContinue(int32_t instance)
    {
        auto* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());

        firstScreenFrameCount++;
        if (firstScreenPressedStart == 1) firstScreenFrameCount += 30;
        firstScreenFrameCount %= 60;

        if (firstScreenFrameCount < 30)
        {
            libgpu_SetPolyFT4(prim);
            prim->x0    = -54;
            prim->y0    = 50;
            prim->x1    = 67;
            prim->y1    = 50;
            prim->x2    = -54;
            prim->y2    = 60;
            prim->x3    = 67;
            prim->y3    = 60;
            prim->u0    = 0;
            prim->v0    = 241;
            prim->u1    = 121;
            prim->v1    = 241;
            prim->u2    = 0;
            prim->v2    = 251;
            prim->u3    = 121;
            prim->v3    = 251;
            prim->r0    = 0;
            prim->g0    = 128;
            prim->b0    = 0;
            prim->tpage = getTPage(1, 0, 0x300, 0);
            prim->clut  = getClut(0, 0x1e0);
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 30, prim);
            prim++;
        }

        libgpu_SetPolyFT4(prim);
        prim->x0    = -160;
        prim->y0    = -120;
        prim->x1    = 96;
        prim->y1    = -120;
        prim->x2    = -160;
        prim->y2    = 120;
        prim->x3    = 96;
        prim->y3    = 120;
        prim->u0    = 0;
        prim->v0    = 0;
        prim->u1    = 255;
        prim->v1    = 0;
        prim->u2    = 0;
        prim->v2    = 240;
        prim->u3    = 255;
        prim->v3    = 240;
        prim->r0    = 128;
        prim->g0    = 128;
        prim->b0    = 128;
        prim->tpage = getTPage(1, 0, 0x300, 0);
        prim->clut  = getClut(0, 0x1e0);
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 30, prim);
        prim++;

        libgpu_SetPolyFT4(prim);
        prim->x0    = 96;
        prim->y0    = -120;
        prim->x1    = 160;
        prim->y1    = -120;
        prim->x2    = 96;
        prim->y2    = 120;
        prim->x3    = 160;
        prim->y3    = 120;
        prim->u0    = 0;
        prim->v0    = 0;
        prim->u1    = 64;
        prim->v1    = 0;
        prim->u2    = 0;
        prim->v2    = 240;
        prim->u3    = 64;
        prim->v3    = 240;
        prim->r0    = 128;
        prim->g0    = 128;
        prim->b0    = 128;
        prim->tpage = getTPage(1, 0, 0x380, 0);
        prim->clut  = getClut(0, 0x1e0);
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 30, prim);

        libgs_GsSetWorkBase(prim + 1);
    }

    bool runLandingScreen()
    {
        firstScreenPressedStart = false;
        addObject(ObjectID::MAIN_MENU_SCREEN, 0, nullptr, renderPressStartToContinue);
        FADE_DATA.fadeOutCurrent = 0;
        fadeFromBlack(40);

        for (int32_t i = 0; i < 600; i++)
        {
            pollInputMenu();
            if (isKeyPressed(InputButtons::BUTTON_START) && FADE_DATA.fadeOutCurrent == 0 &&
                FADE_DATA.fadeInCurrent == 0)
            {
                playSound(0, 3);
                firstScreenPressedStart = true;
                break;
            }
            runFrame();
        }

        fadeToBlack(40);
        while (FADE_DATA.fadeOutCurrent < 40)
            runFrame();
        removeObject(ObjectID::MAIN_MENU_SCREEN, 0);
        return firstScreenPressedStart;
    }

    void runMainMenu()
    {
        CHECKED_MEMORY_CARD = 0x10;
        CURRENT_MENU        = -1;
        TARGET_MENU         = 0;
        loadTIMFile("\\ETCNA\\TITLE2.TIM", GENERAL_BUFFER.data());
        addObject(ObjectID::MAIN_MENU, 0, tickMainMenu, renderMainMenu);
        addObject(ObjectID::MAIN_MENU_SCREEN, 0, nullptr, renderMainMenuBackground);
        fadeFromBlack(40);

        do
        {
            ACTIVE_FRAMEBUFFER    = libgs_GsGetActiveBuffer();
            ACTIVE_ORDERING_TABLE = GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER;
            libgs_GsSetWorkBase(GS_WORK_BASES + ACTIVE_FRAMEBUFFER);
            libgs_GsClearOt(0, 0, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER);
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 0x20, &DR_OFFSETS[ACTIVE_FRAMEBUFFER]);
            pollInputMenu();
            tickObjects();
            renderObjects();
            libgpu_DrawSync(0);
            libetc_vsync(0);
            libgpu_ResetGraph(1);
            libgs_GsSwapDispBuff();
            libgs_GsSortClear(0, 0, 0, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER);
            libgs_GsDrawOt(GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER);

            if (CURRENT_MENU == -1 && FADE_DATA.fadeOutCurrent == 0)
            {
                fadeToBlack(40);
                removeObject(ObjectID::MAIN_MENU, 0);
            }
        } while (CURRENT_MENU != -1 && FADE_DATA.fadeOutCurrent < 40);

        removeObject(ObjectID::MAIN_MENU_SCREEN, 0);
    }

    void newGameScene()
    {
        // checkShopMap(0xda); // unused as of vanilla 1.1
        initializeNamingBuffer(0);
        initializeTextbox();
        fadeFromBlack(20);
        writePStat(0xfe, 0);
        writePStat(0xf3, 0xff);
        loadNewGameScene();

        auto result = false;
        do
        {
            pollInputGame();
            ACTIVE_FRAMEBUFFER    = libgs_GsGetActiveBuffer();
            ACTIVE_ORDERING_TABLE = GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER;
            libgs_GsSetWorkBase(GS_WORK_BASES + ACTIVE_FRAMEBUFFER);
            libgs_GsClearOt(0, 0, ACTIVE_ORDERING_TABLE);
            processInput();
            tickTextboxHandling(1);
            if (readPStat(0xf3) == 0) result = newGameStateMachine();
            tickObjects();
            renderObjects();
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 0x20, &DR_OFFSETS[ACTIVE_FRAMEBUFFER]);
            libgpu_DrawSync(0);
            libetc_vsync(3);
            libgs_GsSetOrign(DRAWING_OFFSET_X, DRAWING_OFFSET_Y);
            libgs_GsSwapDispBuff();
            libgs_GsSortClear(0, 0, 0, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER);
            libgs_GsDrawOt(ACTIVE_ORDERING_TABLE);
            if (result && FADE_DATA.fadeOutCurrent == 0) fadeToBlack(40);
        } while (!result || FADE_DATA.fadeOutCurrent < 40);

        unloadNewGameScene();
    }

    void gameLoop()
    {
        pollInputGame();
        ACTIVE_FRAMEBUFFER = libgs_GsGetActiveBuffer();
        libgs_GsSetWorkBase(GS_WORK_BASES + ACTIVE_FRAMEBUFFER);
        libgs_GsClearOt(0, 0, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER);
        ACTIVE_ORDERING_TABLE = GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER;
        processInput();
        updateTournamentRegistration();
        if (!IS_SCRIPT_PAUSED) tickScript();

        libgs_GsSetOrign(DRAWING_OFFSET_X, DRAWING_OFFSET_Y);
        libgs_GsSetRefView2(&GS_VIEWPOINT);
        libgs_GsSetProjection(VIEWPORT_DISTANCE);
        GS_VIEWPOINT_COPY      = GS_VIEWPOINT;
        VIEWPORT_DISTANCE_COPY = VIEWPORT_DISTANCE;
        DRAWING_OFFSET_X_COPY  = DRAWING_OFFSET_X;
        DRAWING_OFFSET_Y_COPY  = DRAWING_OFFSET_Y;
        tickObjects();
        renderObjects();
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 0x20, &DR_OFFSETS[ACTIVE_FRAMEBUFFER]);
        libgpu_DrawSync(0);
        libetc_vsync(3);
        libgs_GsSwapDispBuff();
        libgs_GsSortClear(0, 0, 0, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER);
        libgs_GsDrawOt(GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER);
        handlePause();
    }

    bool isProtectedNPC(DigimonType type, int32_t screen)
    {
        if (type == DigimonType::OTAMAMON) return true;
        if (type > DigimonType::ANALOGMAN) return true;
        if (type == DigimonType::GUARDROMON && screen == 155) return true;
        if (type == DigimonType::SABERDRAMON && screen == 85) return true;
        if (type == DigimonType::BOTAMON) return true;
        if (type == DigimonType::KOROMON) return true;
        if (type == DigimonType::PUNIMON) return true;
        if (type == DigimonType::TSUNOMON) return true;
        if (type == DigimonType::POYOMON) return true;
        if (type == DigimonType::TOKOMON) return true;
        if (type == DigimonType::YURAMON) return true;
        if (type == DigimonType::TANEMON) return true;

        return false;
    }

    void handleSpawnNPCCollision()
    {
        auto collisionResult = entityCheckCollision(&PARTNER_ENTITY, &TAMER_ENTITY, 0, 0);
        if (collisionResult <= CollisionCode::PARTNER || collisionResult > CollisionCode::NPC8) return;

        auto type = ENTITY_TABLE.getEntityById(static_cast<int32_t>(collisionResult))->type;
        if (!isProtectedNPC(type, SAVED_CURRENT_SCREEN))
        {
            ENTITY_TABLE.getEntityById(static_cast<int32_t>(collisionResult))->isOnMap = false;
            return;
        }

        auto posX     = MAP_WARPS.spawnX[SAVED_CURRENT_EXIT];
        auto posY     = MAP_WARPS.spawnY[SAVED_CURRENT_EXIT];
        auto posZ     = MAP_WARPS.spawnZ[SAVED_CURRENT_EXIT];
        auto rotation = MAP_WARPS.rotation[SAVED_CURRENT_EXIT];

        TAMER_ENTITY.posData->location.x = posX;
        TAMER_ENTITY.posData->location.y = posY;
        TAMER_ENTITY.posData->location.z = posZ;
        TAMER_ENTITY.posData->rotation.y = rotation;
        STORED_TAMER_POS                 = TAMER_ENTITY.posData->location;

        if (rotation <= 0x200 || rotation > 0xE00) posZ += 200;
        if (rotation > 0x200 && rotation <= 0x600) posX += 200;
        if (rotation > 0x600 && rotation <= 0xA00) posZ -= 200;
        if (rotation > 0xA00 && rotation <= 0xE00) posX -= 200;

        PARTNER_ENTITY.posData->location.x = posX;
        PARTNER_ENTITY.posData->location.y = posY;
        PARTNER_ENTITY.posData->location.z = posZ;
        // vanilla does startAnimation here, but we refactored a bit to make it redundant
    }

    void initializeLoadedMap()
    {
        TAMER_ENTITY.posData->location   = SAVED_PLAYER_POS;
        PARTNER_ENTITY.posData->location = SAVED_PARTNER_POS;
        STORED_TAMER_POS                 = TAMER_ENTITY.posData->location;
        initializeDrawingOffsets();

        for (auto& entity : NPC_ENTITIES)
            if (entity.type != DigimonType::INVALID && entity.type != DigimonType::TAMER) entity.isOnScreen = 1;

        handleSpawnNPCCollision();
        startAnimation(&TAMER_ENTITY, 0);
        startAnimation(&PARTNER_ENTITY, 0);

        PARTNER_ENTITY.stats   = SAVED_PARTNER_STATS;
        PARTNER_ENTITY.lives   = SAVED_LIVES;
        MONEY                  = SAVED_MONEY;
        PARTNER_PARA           = SAVED_PARTNER_PARA;
        PREVIOUS_SCREEN        = SAVED_PREVIOUS_SCREEN;
        CURRENT_EXIT           = SAVED_CURRENT_EXIT;
        PREVIOUS_EXIT          = SAVED_PREVIOUS_EXIT;
        TAMER_WAYPOINT_X       = SAVED_TAMER_WAYPOINT_X;
        TAMER_WAYPOINT_Y       = SAVED_TAMER_WAYPOINT_Y;
        TAMER_PREVIOUS_TILE_X  = SAVED_TAMER_PREVIOUS_TILE_X;
        TAMER_PREVIOUS_TILE_Y  = SAVED_TAMER_PREVIOUS_TILE_Y;
        TAMER_WAYPOINT_CURRENT = SAVED_TAMER_WAYPOINT_CURRENT;
        TAMER_WAYPOINT_COUNT   = SAVED_TAMER_WAYPOINT_COUNT;
        TAMER_START_TILE_X     = SAVED_TAMER_START_TILE_X;
        TAMER_START_TILE_Y     = SAVED_TAMER_START_TILE_Y;
        TAMER_WAYPOINT_ACTIVE  = SAVED_TAMER_WAYPOINT_ACTIVE;
        CAMERA_UPDATE_TILES    = 1;
        createCameraMovement(&TAMER_ENTITY.posData->location, 2);
        setImmortalHour();
        initializeDrawingOffsets();
        updateMapTile();
        uploadMapTileImages(MAP_TILE_DATA.data(), MAP_TILE_X + MAP_TILE_Y * MAP_WIDTH);
        updateMinuteHand(HOUR, MINUTE);
    }

    void view_init()
    {
        libgs_GsSetProjection(1024);
        GS_VIEWPOINT.viewpointX      = 0;
        GS_VIEWPOINT.viewpointY      = 0;
        GS_VIEWPOINT.viewpointZ      = -2000;
        GS_VIEWPOINT.refpointX       = 0;
        GS_VIEWPOINT.refpointY       = 0;
        GS_VIEWPOINT.refpointZ       = 0;
        GS_VIEWPOINT.viewpoint_twist = 0;
        GS_VIEWPOINT.super           = nullptr;
        libgs_GsSetRefView2(&GS_VIEWPOINT);
        libgs_GsSetNearClip(100);
    }

    void initializeEffectData()
    {
        initializeEFE();
        initializeParticleFX();
        initializeEntityParticleFX();
        initializeCloudFXData();
        initializeHealingParticles();
    }

    void initializeInventoryModules()
    {
        COMBAT_DATA_PTR = &COMBAT_DATA;
        resetFlattenGlobal();
        initializeDroppedItems();
        initializeInventoryUI();
        initializeInventory();
        initializeEntityText();
    }

} // namespace

void initializeFramebuffer()
{
    constexpr RECT frameBuffers{.x = 0, .y = 0, .width = 320, .height = 480};
    constexpr dtl::array<uint16_t, 2> drawOffset0 = {160, 360};
    constexpr dtl::array<uint16_t, 2> drawOffset1 = {160, 120};

    libgpu_SetDispMask(0);
    libgs_GsInitGraph(320, 240, 4, 0, 0);
    libgs_GsDefDispBuff(0, 0, 0, 0xf0);
    libgpu_ClearImage(&frameBuffers, 0, 0, 0);
    GS_ORDERING_TABLE[0].length = 12;
    GS_ORDERING_TABLE[0].origin = GSOT_TAGS_0.data();
    GS_ORDERING_TABLE[1].length = 12;
    GS_ORDERING_TABLE[1].origin = GSOT_TAGS_1.data();
    libgs_GsInit3D();
    DRAWING_OFFSET_X = 160;
    DRAWING_OFFSET_Y = 120;
    libgpu_SetDrawOffset(&DR_OFFSETS[0], drawOffset0.data());
    libgpu_SetDrawOffset(&DR_OFFSETS[1], drawOffset1.data());
    MAP_LAYER_ENABLED = true;
    libgpu_SetDispMask(1);
}

int32_t main()
{
    initializeHeap();
    libsnd_SsInit();
    libetc_ResetCallback();
    libgpu_ResetGraph(0);
    libgpu_SetGraphDebug(0);
    initializeFramebuffer();
    libetc_PadInit();
    libcd_CdInit(0);
    libmcrd_MemCardInit(1);
    libmcrd_MemCardStart();
    initializeModelComponents();
    initializeWorldObjects();
    initializeGsTMDMap();
    initializeFileReadQueue();
    initializeAttackObjects();
    initializeFontCLUT();
    // fillEFEXTable(); does nothing
    initializeFadeData();
    initializeScripts();
    view_init();
    initializeFadeData();
    initializeEffectData();
    initializeInventoryModules();
    initializeMedalModel();
    initializeClockData();
    initializeStatusObjects();
    customInit();

    while (true)
    {
        loadTIMFile("\\ETCDAT\\FI_INFO.TIM", GENERAL_BUFFER.data());
        loadTIMFile("\\ETCNA\\TITLE256.TIM", GENERAL_BUFFER.data());

        bool confirmed = false;
        do
        {
            playMovie(Movies::INTRO, true);
            initializeMusic();
            confirmed = runLandingScreen();
            finalizeMusic();
        } while (!confirmed);

        loadStackedTIMFile("\\ETCDAT\\ETCTIM.BIN");
        initializeMusic();
        runMainMenu();

        switch (MAIN_STATE)
        {
            case 0:
            {
                newGameScene();
                finalizeMusic();
                playMovie(Movies::NEWGAME, true);
                initializeMusic();
                loadStackedTIMFile("\\ETCDAT\\ETCTIM.BIN");
                initializeTamer(DigimonType::TAMER, 0, 0, 0, 0, 0, 0);
                auto starter = readPStat(0xfe) == 0 ? DigimonType::AGUMON : DigimonType::GABUMON;
                initializePartner(starter, 0, 0, 0, 0, 0, 0);
                setDigimonRaised(starter);
                initializeMap();
                initializeChest();
                runMapHeadScript(204);
                addClock();
                stopGameTime();
                break;
            }
            case 1:
            {
                loadStackedTIMFile("\\ETCDAT\\ETCTIM.BIN");
                initializeTamer(DigimonType::TAMER, 0, 0, 0, 0, 0, 0);
                initializePartner(SAVED_PARTNER_TYPE, 0, 0, 0, 0, 0, 0);
                initializeMap();
                initializeChest();
                runMapHeadScript(SAVED_CURRENT_SCREEN);
                initializeLoadedMap();
                addClock();
                if (Tamer_getState() != 0) Tamer_setState(0);
                break;
            }
            case 2:
            {
                finalizeMusic();
                playMovie(Movies::POST_CREDITS, true);
                initializeMusic();
                loadStackedTIMFile("\\ETCDAT\\ETCTIM.BIN");
                initializeTamer(DigimonType::TAMER, 0, 0, 0, 0, 0, 0);
                initializePartner(SAVED_PARTNER_TYPE, 0, 0, 0, 0, 0, 0);
                initializeMap();
                initializeChest();
                runMapHeadScript(SAVED_CURRENT_SCREEN);
                initializeLoadedMap();
                addClock();
                if (Tamer_getState() != 0) Tamer_setState(0);
                break;
            }
        }

        fadeFromBlack(0x28);
        recalculatePPandArena();
        while (MAIN_STATE != 3)
            gameLoop();
        cleanupGame();
        finalizeMusic();
        playMovie(Movies::CREDITS, true);
    }

    return 0;
}

extern "C"
{
    void EntryPoint()
    {
        // vanilla resets r2-27 and r30 here
        gp = SECTION_DATA.globalPointer;
        sp = reinterpret_cast<uint32_t*>(SECTION_DATA.stackTop);

        memset(SECTION_DATA.bss1Offset, 0, SECTION_DATA.bss1Size);
        memset(SECTION_DATA.bss2Offset, 0, SECTION_DATA.bss2Size);

        cop0_status = 0x40000000;

        main();

        asm volatile("break 0x20, 0x201");
    }

    void recalculatePPandArena()
    {
        auto prosperity = 0;
        for (int32_t i = 3; i < 59; i++)
        {
            auto level = DIGIMON_DATA[i].level;
            if (level < Level::ROOKIE) continue;
            if (!isTriggerSet(200 + i)) continue;

            if (i == 11 || i == 39 || i == 53)
                prosperity++;
            else
                prosperity += static_cast<int32_t>(level) - 2;
        }

        writePStat(1, prosperity);

        if (readPStat(3) > 22) return;
        if (isTriggerSet(37)) unsetTrigger(37);
        if (isTriggerSet(38)) unsetTrigger(38);
        if (isTriggerSet(39)) unsetTrigger(39);
    }
}