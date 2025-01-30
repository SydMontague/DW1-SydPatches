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
}