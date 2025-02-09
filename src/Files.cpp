#include "Files.hpp"

#include "extern/dw1.hpp"
#include "extern/libc.hpp"
#include "extern/libcd.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    /*
     * Finds a file on the disc and writes its position and size into the passed FileLookup struct.
     * If the file couldn't be found false gets returned.
     */
    bool lookupFileTable(FileLookup* lookup, const char* filename)
    {
        uint8_t buffer[256];
        CdlFILE file;
        sprintf(buffer, "\\%s;1", filename);

        if (!libcd_CdSearchFile(&file, reinterpret_cast<char*>(buffer))) return false;

        lookup->pos  = file.pos;
        lookup->size = file.size;

        return true;
    }

    /*
     * Gets the size of a file or zero if it isn't found.
     */
    uint32_t lookupFileSize(const char* path)
    {
        FileLookup lookup;
        if (!lookupFileTable(&lookup, path)) return 0;

        return lookup.size;
    }

    void loadTexture(uint8_t* buffer, uint32_t* outTexPage, uint32_t* outCLUT)
    {
        GsIMAGE imageData;
        libgs_GsGetTimInfo(reinterpret_cast<uint32_t*>(buffer + 4), &imageData);
        RECT pixelRect = {
            .x      = imageData.pixelX,
            .y      = imageData.pixelY,
            .width  = static_cast<int16_t>(imageData.pixelWidth),
            .height = static_cast<int16_t>(imageData.pixelHeight),
        };
        libgpu_LoadImage(&pixelRect, imageData.pixelPtr);
        if (outTexPage) *outTexPage = libgpu_GetTPage(imageData.pixelMode & 3, 0, imageData.pixelX, imageData.pixelY);

        // if has CLUT
        if ((imageData.pixelMode >> 3 & 1) != 0)
        {
            RECT clutRect = {
                .x      = imageData.clutX,
                .y      = imageData.clutY,
                .width  = static_cast<int16_t>(imageData.clutWidth),
                .height = static_cast<int16_t>(imageData.clutHeight),
            };
            libgpu_LoadImage(&clutRect, imageData.clutPtr);
            if (outCLUT) *outCLUT = libgpu_GetClut(imageData.clutX, imageData.clutY);
        }
    }

    void loadTextureFile(const char* fileName, uint32_t* outTexPage, uint32_t* outCLUT)
    {
        GsIMAGE imageData;

        readFile(fileName, TEXTURE_BUFFER);
        loadTexture(TEXTURE_BUFFER, outTexPage, outCLUT);
    }

    /*
     * Reads given file from disc into the given buffer.
     * The buffer must be large enough to hold all the *sectors* of the file, meaning that it's always a multiple of
     * 2048. If the file is larger, your memory is busted.
     *
     * TODO: write safe version of this function 
     */
    void readFile(const char* file, void* buffer)
    {
        FileLookup lookup;

        if (!lookupFileTable(&lookup, file)) return;

        // seek file location
        while (!libcd_CdControl(CdCommand::CdlSetloc, reinterpret_cast<uint8_t*>(&lookup.pos), nullptr))
            ;
        // start reading
        while (!libcd_CdRead((lookup.size + 2047) >> 11, buffer, 0x80))
            ;
        // wait for reading to complete
        while (libcd_CdReadSync(0, nullptr) > 0)
            ;
    }

    void loadTIMFile(const char* fileName, uint8_t* buffer)
    {
        readFile(fileName, buffer);
        loadTexture(buffer, nullptr, nullptr);
    }

    void loadStackedTIMEntry(const char* fileName, uint8_t* buffer, uint32_t offset, uint32_t sectors)
    {
        readFileSectors(fileName, buffer, offset, sectors);
        loadTexture(buffer, nullptr, nullptr);
    }

    void readFileSectors(const char* path, void* buffer, uint32_t offset, uint32_t count)
    {
        FileLookup lookup;

        if (!lookupFileTable(&lookup, path)) return;

        int32_t pos = libcd_CdPosToInt(&lookup.pos);
        libcd_CdIntToPos(pos + offset, &lookup.pos);

        // seek file location
        while (!libcd_CdControl(CdCommand::CdlSetloc, reinterpret_cast<uint8_t*>(&lookup.pos), nullptr))
            ;
        // start reading
        while (!libcd_CdRead(count, buffer, 0x80))
            ;
        // wait for reading to complete
        while (libcd_CdReadSync(0, nullptr) > 0)
            ;
    }
}