
#include "Files.hpp"
#include "extern/MOV.hpp"
#include "extern/dw1.hpp"
#include "extern/libcd.hpp"
#include "extern/libgpu.hpp"

namespace
{
    void startMovie(Movies movie)
    {
        libcd_CdInit(0);
        libgpu_ResetGraph(0);
        libgpu_SetGraphDebug(0);
        loadDynamicLibrary(Overlay::MOV_REL, nullptr, false, nullptr, nullptr);
        MOV_startMovie(movie);
        libgpu_ResetGraph(3);
    };
} // namespace

extern "C"
{
    void playMovie(Movies movie, bool shouldPlay)
    {
        // TODO the shouldPlay argument is useless, it's always true and should be responsibility of the caller
        if (!shouldPlay) return;

        startMovie(movie);
        initializeFramebuffer();
    }
}