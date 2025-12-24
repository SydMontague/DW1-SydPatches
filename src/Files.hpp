#pragma once
#include "extern/dw1.hpp"
#include "extern/libcd.hpp"

extern "C"
{
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

    /*
     * Ticks the FileReadQueue once, attempting to load its current head.
     * You must call this if you want to wait for files to load while the GameObject tick loop isn't running.
     */
    void tickFileReadQueue();

    /*
     * Adds a read request to the file read queue.
     * If you pass a location, passing a path is optional but strongly encouraged.
     *
     * @param path the path to the file to load in the file system
     * @param buffer the buffer to read the file into
     * @param readComplete a pointer to a byte that will be initialized to 1 and set to 0 once the request is done
     * @param callback a callback to be called on the read is complete
     * @param param the parameter passed to the callback
     * @param location a pointer to a CdlLoc pointing at the file
     * @param size the size of the file to read when using the 'location' parameter or a negative value when 'path' is
     * to be used
     * @return the position in the queue or -1 on failure
     */
    int32_t addFileReadRequest(const char* path,
                               uint8_t* buffer,
                               uint8_t* readComplete,
                               FileCallback callback,
                               void* param,
                               CdlLoc* location,
                               size_t size);

    /*
     * Adds a read request to the file read queue, using a given path.
     *
     * @param path the path to the file to load in the file system
     * @param buffer the buffer to read the file into
     * @param readComplete a pointer to a byte that will be initialized to 1 and set to 0 once the request is done
     * @param callback a callback to be called on the read is complete
     * @param param the parameter passed to the callback
     * @return the position in the queue or -1 on failure
     */
    int32_t addFileReadRequestPath(const char* path,
                                   uint8_t* buffer,
                                   uint8_t* readComplete,
                                   FileCallback callback,
                                   void* callbackParam);

    /*
     * Adds a read request to the file read queue, reading a subsection of a file.
     *
     * @param path the path to the file to load in the file system
     * @param buffer the buffer to read the file into
     * @param readComplete a pointer to a byte that will be initialized to 1 and set to 0 once the request is done
     * @param callback a callback to be called on the read is complete
     * @param param the parameter passed to the callback
     * @param location a pointer to a CdlLoc pointing at the file
     * @param size the size of the file to read when using the 'location' parameter or a negative value when 'path' is
     * to be used
     * @return the position in the queue or -1 on failure
     */
    int32_t addFileReadRequestSection(const char* path,
                                      uint8_t* buffer,
                                      int32_t offset,
                                      int32_t sectors,
                                      uint8_t* readComplete,
                                      FileCallback callback,
                                      void* callbackParam);
    /*
     * Adds a read request to the file read queue.
     * It will lookup file position and size from the file lookup table instead of searching the file system.
     * (This has no impact in this implementation, as the file lookup table is disabled)
     *
     * @param path the path to the file to load in the file system
     * @param buffer the buffer to read the file into
     * @param readComplete a pointer to a byte that will be initialized to 1 and set to 0 once the request is done
     * @param callback a callback to be called on the read is complete
     * @param param the parameter passed to the callback
     * @param location a pointer to a CdlLoc pointing at the file
     * @param size the size of the file to read when using the 'location' parameter or a negative value when 'path' is
     * to be used
     * @return the position in the queue or -1 on failure
     */
    int32_t addFileReadRequestLookup(const char* path,
                                     uint8_t* buffer,
                                     uint8_t* readComplete,
                                     FileCallback callback,
                                     void* callbackParam);
}