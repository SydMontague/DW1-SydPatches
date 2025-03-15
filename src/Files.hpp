#pragma once
#include "extern/libcd.hpp"

extern "C"
{
    struct FileLookup
    {
        CdlLoc pos;
        uint32_t size;
    };

    bool lookupFileTable(FileLookup* lookup, const char* filename);
    uint32_t lookupFileSize(const char* path);
    void readFile(const char* path, void* buffer);
    void readFileSectors(const char* path, void* buffer, uint32_t offset, uint32_t count);
    void loadStackedTIMEntry(const char* fileName, uint8_t* buffer, uint32_t offset, uint32_t sectors);
}