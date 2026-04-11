#include "Main.hpp"

#include "CustomUI.hpp"
#include "Fade.hpp"
#include "Files.hpp"
#include "GameObjects.hpp"
#include "GameTime.hpp"
#include "Input.hpp"
#include "ItemFunctions.hpp"
#include "Map.hpp"
#include "MapObjects.hpp"
#include "Model.hpp"
#include "Movie.hpp"
#include "Partner.hpp"
#include "PlayerMedalView.hpp"
#include "Sound.hpp"
#include "Tamer.hpp"
#include "Timestamp.hpp"
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
        bool directionPressed = POLLED_INPUT & POLLED_INPUT_PREVIOUS & 0xf000f000;
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

    bool runLandingScreen()
    {
        bool confirmed = false;
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
                confirmed = true;
                break;
            }
            runFrame();
        }

        fadeToBlack(40);
        while (FADE_DATA.fadeOutCurrent < 40)
            runFrame();
        removeObject(ObjectID::MAIN_MENU_SCREEN, 0);
        return confirmed;
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
}