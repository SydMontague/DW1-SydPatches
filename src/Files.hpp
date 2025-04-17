#pragma once
#include "extern/libcd.hpp"

extern "C"
{
    struct FileLookup
    {
        CdlLoc pos;
        uint32_t size;
    };

    /*
     * Finds a file on the disc and writes its position and size into the passed FileLookup struct.
     * If the file couldn't be found false gets returned.
     */
    bool lookupFileTable(FileLookup* lookup, const char* filename);

    /*
     * Gets the size of a file or zero if it isn't found.
     */
    uint32_t lookupFileSize(const char* path);

    /*
     * Reads a given file from disc into the given buffer. If the given buffer is null, nothing happens.
     * The buffer must be large enough to hold all the *sectors* of the file, meaning that it's always a multiple of
     * 2048. If the file is larger, your memory is busted.
     *
     * TODO: write safe version of this function
     */
    void readFile(const char* path, void* buffer);

    /*
     * Reads a given file from disc into a given buffer. If the given buffer is null, nothing happens.
     * The given offset and sectors represent disc sectors of 2048.
     * 
     * The buffer must be large enough to hold all the *sectors* of the file, meaning that it's always a multiple of
     * 2048. If the file is larger, your memory is busted.
     */
    void readFileSectors(const char* path, void* buffer, uint32_t offset, uint32_t sectors);

    /*
     * Loads a TIM entry of a stacked TIM file from disc into a given buffer and VRAM. If the given buffer is null,
     * nothing happens.
     * The given offset and sectors represent disc sectors of 2048.
     * 
     * The buffer must be large enough to hold all the *sectors* of the file, meaning that it's always a multiple of
     * 2048. If the file is larger, your memory is busted.
     */
    void loadStackedTIMEntry(const char* fileName, uint8_t* buffer, uint32_t offset, uint32_t sectors);

    /*
     * Load a TIM texture from memory into VRAM. If the given buffer is null, nothing happens.
     * The textures texturePage and CLUT values are written in outTexPage and * outCLUT respectively, unless a nullptr
     * is passed.
     */
    void loadTexture(uint8_t* buffer, uint32_t* outTexPage, uint32_t* outCLUT);

    /*
     * Load a TIM texture from a disc file into VRAM.
     * The textures texturePage and CLUT values are written in outTexPage and * outCLUT respectively, unless a nullptr
     * is passed.
     */
    void loadTextureFile(const char* fileName, uint32_t* outTexPage, uint32_t* outCLUT);

    /*
     * Load a TIM texture from a disc file into a given buffer and into VRAM. If the buffer is null, nothing happens.
     */
    void loadTIMFile(const char* fileName, uint8_t* buffer);
}