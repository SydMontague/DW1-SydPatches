
#include "Files.hpp"
#include "extern/MOV.hpp"
#include "extern/dw1.hpp"
#include "extern/libcd.hpp"
#include "extern/libgpu.hpp"

extern "C"
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
}