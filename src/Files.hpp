#include "extern/libcd.hpp"

extern "C"
{
    struct FileLookup
    {
        CdlLoc pos;
        uint32_t size;
    };

    extern bool lookupFileTable(FileLookup* lookup, const char* filename);
    extern uint32_t lookupFileSize(const char* path);
    extern void readFile(const char* path, void* buffer);
    extern void readFileSectors(const char* path, void* buffer, uint32_t offset, uint32_t count);
}