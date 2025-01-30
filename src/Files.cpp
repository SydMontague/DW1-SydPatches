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

    void loadTextureFile(const char* fileName, uint32_t* outTexPage, uint32_t* outCLUT)
    {
        lookupFileSize(fileName);
        GsIMAGE imageData;

        readFile(fileName, TEXTURE_BUFFER);

        libgs_GsGetTimInfo(reinterpret_cast<uint32_t*>(TEXTURE_BUFFER + 4), &imageData);
        RECT pixelRect = {
            .x      = imageData.pixelX,
            .y      = imageData.pixelY,
            .width  = static_cast<int16_t>(imageData.pixelWidth),
            .height = static_cast<int16_t>(imageData.pixelHeight),
        };
        libgpu_LoadImage(&pixelRect, imageData.pixelPtr);
        *outTexPage = libgpu_GetTPage(imageData.pixelMode & 3, 0, imageData.pixelX, imageData.pixelY);

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
            *outCLUT = libgpu_GetClut(imageData.clutX, imageData.clutY);
        }
    }
}