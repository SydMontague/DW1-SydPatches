#pragma once
#include "stddef.hpp"

extern "C"
{
    struct CdlLoc
    {
        uint8_t minute;
        uint8_t second;
        uint8_t sector;
        uint8_t track;
    };

    struct CdlFILE
    {
        CdlLoc pos;
        uint32_t size;
        char name[16];
    };

    struct StHEADER
    {
        uint16_t id;
        uint16_t type;
        uint16_t sector;
        uint16_t sectors;
        uint32_t frameCount;
        uint32_t frameSize;
        uint16_t width;
        uint16_t height;
        uint32_t reserved[2];
        CdlLoc location;
    };

    static_assert(sizeof(CdlLoc) == 4 && sizeof(CdlFILE) == 24 && sizeof(StHEADER) == 32);

    enum class CdCommand
    {
        CdlSync      = 0,
        CdlNop       = 1,
        CdlSetloc    = 2,
        CdlPlay      = 3,
        CdlForward   = 4,
        CdlBackward  = 5,
        CdlReadN     = 6,
        CdlStandby   = 7,
        CdlStop      = 8,
        CdlPause     = 9,
        CdlMute      = 11,
        CdlDemute    = 12,
        CdlSetfilter = 13,
        CdlSetmode   = 14,
        CdlGetparam  = 15,
        CdlGetlocL   = 16,
        CdlGetLocP   = 17,
        CdlGetTN     = 19,
        CdlGetTD     = 20,
        CdlSeekL     = 21,
        CdlSeekP     = 22,
    };

    extern CdlFILE* libcd_CdSearchFile(CdlFILE* filePtr, const char* name);
    extern bool libcd_CdControl(CdCommand command, uint8_t* params, uint8_t* result);
    int32_t libcd_CdControlB(CdCommand command, uint8_t* params, uint8_t* result);
    int32_t libcd_CdRead2(int32_t mode);
    extern bool libcd_CdRead(int32_t sectors, void* buffer, int32_t mode);
    extern int32_t libcd_CdReadSync(int32_t mode, uint8_t* result);
    extern int32_t libcd_CdPosToInt(CdlLoc* loc);
    extern CdlLoc* libcd_CdIntToPos(int32_t pos, CdlLoc* loc);
    void libcd_CdInit(int32_t mode);

    void libcd_StSetRing(uint32_t* buffer, uint32_t sectors);
    void libcd_StUnSetRing();
    void libcd_StSetStream(uint32_t mode,
                           uint32_t firstFrame,
                           uint32_t lastFrame,
                           void (*callback)(),
                           void (*interruptCallback)());
    uint32_t libcd_StGetNext(uint32_t** buffer, StHEADER** header);
    uint32_t libcd_StFreeRing(uint32_t* buffer);
    int32_t libcd_StGetBackloc(CdlLoc* location);
    void libcd_StCdInterrupt();
}

// The resident streaming interrupt sets this while MDEC output DMA is busy.
extern volatile int32_t libcd_STREAM_INTERRUPT_PENDING;
