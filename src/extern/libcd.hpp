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

    extern CdlFILE* libcd_CdSearchFile(CdlFILE* filePtr, const char* name);
}