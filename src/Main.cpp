#include "Main.hpp"

#include "GameObjects.hpp"
#include "Model.hpp"
#include "extern/dw1.hpp"
#include "extern/libetc.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"
#include "extern/psx.hpp"

extern "C"
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

    void pauseFrame()
    {
        ACTIVE_FRAMEBUFFER    = libgs_GsGetActiveBuffer();
        ACTIVE_ORDERING_TABLE = GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER;
        libgs_GsSetWorkBase(GS_WORK_BASES + ACTIVE_FRAMEBUFFER);
        libgs_GsClearOt(0, 0, ACTIVE_ORDERING_TABLE);
        tickObjects();
        renderObjects();
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 32, &DRAW_OFFSETS[ACTIVE_FRAMEBUFFER]);
        libgpu_DrawSync(0);
        libetc_vsync(3);
        libgs_GsSetOrign(DRAWING_OFFSET_X, DRAWING_OFFSET_Y);
        libgs_GsSwapDispBuff();
        libgs_GsSortClear(0, 0, 0, ACTIVE_ORDERING_TABLE);
        libgs_GsDrawOt(ACTIVE_ORDERING_TABLE);
    }
}