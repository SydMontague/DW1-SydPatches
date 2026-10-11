#include "../extern/MOV.hpp"
#include "../extern/dtl/array.hpp"
#include "../extern/dw1.hpp"
#include "../extern/libcd.hpp"
#include "../extern/libetc.hpp"
#include "../extern/libgpu.hpp"
#include "../extern/libpress.hpp"

namespace
{
    struct Movie
    {
        const char* path;
        uint32_t firstFrame;
        uint32_t lastFrame;
    };

    constexpr dtl::array<Movie, 4> MOVIES = {{
        {"\\MOVIE\\OP1.STR;1", 1, 3637},
        {"\\MOVIE\\OP2.STR;1", 1, 3266},
        {"\\MOVIE\\ED2.STR;1", 1, 2652},
        {"\\MOVIE\\EDR.STR;1", 1, 3447},
    }};

    constexpr RECT SCREEN              = {0, 0, 480, 480};
    constexpr int32_t RING_SECTORS     = 24;
    constexpr uint8_t CD_DOUBLE_SPEED  = 0x80;
    constexpr int32_t CD_STREAM_MODE   = 0x100 | CD_DOUBLE_SPEED | 0x40 | 0x20;
    constexpr int32_t FRAME_RETRIES    = 2000;
    constexpr uint32_t FLIP_WAIT_COUNT = 0x800000;

    constexpr int32_t RUN_LEVEL_WORDS = 40960;
    constexpr int32_t SLICE_WORDS     = 24 * 240 / 2;
    dtl::array<dtl::array<uint32_t, RUN_LEVEL_WORDS>, 2> runLevelBuffers;
    dtl::array<dtl::array<uint32_t, SLICE_WORDS>, 2> sliceBuffers;
    dtl::array<uint32_t, RING_SECTORS * 512> ringBuffer;
    dtl::array<uint16_t, 34818> vlcTable;

    struct Environment
    {
        dtl::array<RECT, 2> frames;
        RECT slice;
        int32_t runLevelIndex;
        int32_t sliceIndex;
        int32_t frameIndex;
        int16_t width;
        int16_t height;
        volatile bool frameReady;
        bool completed;
    };

    Environment environment;

    void initializeEnvironment()
    {
        environment = {.frames = {{{0, 32, 0, 0}, {0, 272, 0, 0}}}, .slice = {0, 32, 24, 0}};
    }

    void onSliceDecoded()
    {
        if (libcd_STREAM_INTERRUPT_PENDING != 0) {
            libcd_StCdInterrupt();
            libcd_STREAM_INTERRUPT_PENDING = 0;
        }

        // The upload still belongs to the completed buffer after the next DMA starts.
        const auto rectangle   = environment.slice;
        const auto index       = environment.sliceIndex;
        environment.sliceIndex = !environment.sliceIndex;
        environment.slice.x += environment.slice.width;
        const auto& frame = environment.frames[environment.frameIndex];
        const bool ready  = environment.slice.x >= frame.x + frame.width;

        if (!ready)
            libpress_DecDCTout(sliceBuffers[environment.sliceIndex].data(),
                               environment.slice.width * environment.slice.height / 2);
        else {
            environment.frameIndex = !environment.frameIndex;
            environment.slice.x    = environment.frames[environment.frameIndex].x;
            environment.slice.y    = environment.frames[environment.frameIndex].y;
        }

        libgpu_LoadImage(&rectangle, sliceBuffers[index].data());
        if (ready) environment.frameReady = true;
    }

    void startRead(CdlLoc& location)
    {
        uint8_t mode = CD_DOUBLE_SPEED;
        do {
            while (!libcd_CdControl(CdCommand::CdlSetloc, reinterpret_cast<uint8_t*>(&location), nullptr))
                ;
            while (!libcd_CdControl(CdCommand::CdlSetmode, &mode, nullptr))
                ;
            libetc_vsync(3);
        } while (!libcd_CdRead2(CD_STREAM_MODE));
    }

    void startStream(CdlFILE& file, const Movie& movie)
    {
        libpress_DecDCTReset(0);
        libpress_DecDCToutCallback(onSliceDecoded);
        libcd_StSetRing(ringBuffer.data(), RING_SECTORS);
        libcd_StSetStream(1, movie.firstFrame, 0xFFFFFFFF, nullptr, nullptr);
        startRead(file.pos);
    }

    uint32_t* readFrame(const Movie& movie)
    {
        uint32_t* bitstream = nullptr;
        StHEADER* header    = nullptr;
        int32_t retries     = FRAME_RETRIES;
        while (libcd_StGetNext(&bitstream, &header))
            if (--retries == 0) return nullptr;

        if (header->frameCount >= movie.lastFrame) environment.completed = true;
        if (environment.width != header->width || environment.height != header->height) {
            libgpu_ClearImage(&SCREEN, 0, 0, 0);
            environment.width  = header->width;
            environment.height = header->height;
        }
        for (auto& frame : environment.frames) {
            frame.width  = environment.width * 3 / 2;
            frame.height = environment.height;
        }
        environment.slice.height = environment.height;
        return bitstream;
    }

    bool decodeFrame(const Movie& movie)
    {
        uint32_t* bitstream;
        int32_t retries = FRAME_RETRIES;
        while ((bitstream = readFrame(movie)) == nullptr)
            if (--retries == 0) return false;

        environment.runLevelIndex = !environment.runLevelIndex;
        libpress_DecDCTvlc2(bitstream, runLevelBuffers[environment.runLevelIndex].data(), vlcTable.data());
        libcd_StFreeRing(bitstream);
        return true;
    }

    bool waitForFlip()
    {
        uint32_t timeout = FLIP_WAIT_COUNT;
        while (!environment.frameReady)
            if (--timeout == 0) return false;

        environment.frameReady = false;
        return true;
    }
} // namespace

extern "C"
{
    void MOV_startMovie(Movies movieId)
    {
        const auto& movie = MOVIES[static_cast<int32_t>(movieId)];
        CdlFILE file{};
        if (!libcd_CdSearchFile(&file, movie.path)) return;

        initializeEnvironment();
        startStream(file, movie);
        libpress_DecDCTvlcBuild(vlcTable.data());

        CdlLoc location{};
        while (!decodeFrame(movie)) {
            location = file.pos;
            startRead(location);
        }

        environment.completed = false;
        libgpu_ClearImage(&SCREEN, 0, 0, 0);
        libgpu_DrawSync(0);

        DISPENV display{};
        DRAWENV draw{};
        // Vanilla submitted an uninitialized DRAWENV, including its automatic-clear flag.
        libgpu_SetDefDrawEnv(&draw, 0, 0, SCREEN.width, SCREEN.height);
        bool decodeTimedOut = false;
        do {
            libpress_DecDCTin(runLevelBuffers[environment.runLevelIndex].data(), 3);
            libpress_DecDCTout(sliceBuffers[environment.sliceIndex].data(),
                               environment.slice.width * environment.slice.height / 2);
            while (!decodeFrame(movie)) {
                const auto backFrame = libcd_StGetBackloc(&location);
                if (backFrame <= 0 || movie.lastFrame < static_cast<uint32_t>(backFrame)) location = file.pos;
                startRead(location);
            }

            if (!waitForFlip()) {
                // Vanilla forced a flip while decoder output could still be pending.
                // Abort before displaying an incomplete frame; teardown removes the callback and resets DMA.
                decodeTimedOut = true;
                break;
            }
            libetc_vsync(0);
            libgpu_SetDefDispEnv(&display, 0, !environment.frameIndex * 240, 480, 240);
            display.rgb24         = 1;
            display.display.width = display.display.width * 2 / 3;
            libgpu_PutDispEnv(&display);
            libgpu_PutDrawEnv(&draw);
            libgpu_SetDispMask(1);
        } while (!environment.completed &&
                 (movieId == Movies::CREDITS || !(libetc_PadRead(1) & InputButtons::BUTTON_START)));

        libpress_DecDCToutCallback(nullptr);
        // A timeout must not hand a still-running decoder buffers from the next frame/overlay.
        if (decodeTimedOut) libpress_DecDCTReset(1);
        libcd_StUnSetRing();
        libcd_CdControlB(CdCommand::CdlPause, nullptr, nullptr);
        libgpu_ClearImage(&SCREEN, 0, 0, 0);
        libgpu_DrawSync(0);
    }
}
