#include "Main.hpp"

#include "CustomUI.hpp"
#include "Fade.hpp"
#include "Files.hpp"
#include "GameObjects.hpp"
#include "GameTime.hpp"
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
} // namespace

extern "C"
{
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
            FIRST_SCREEN_PRESSED_START = false;
            while (!FIRST_SCREEN_PRESSED_START)
            {
                playMovie(Movies::INTRO, true);
                initializeMusic();
                runLandingScreen();
                finalizeMusic();
            }

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

    void EntryPoint()
    {
        // vanilla resets r2-27 and r30 here
        gp = SECTION_DATA.globalPointer;
        sp = SECTION_DATA.stackTop;

        memset(SECTION_DATA.bss1Offset, 0, SECTION_DATA.bss1Size);
        memset(SECTION_DATA.bss2Offset, 0, SECTION_DATA.bss2Size);

        cop0_status = 0x40000000;

        main();

        asm volatile("break 0x20, 0x201");
    }
}