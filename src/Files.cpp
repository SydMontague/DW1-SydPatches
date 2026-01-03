#include "Files.hpp"

#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "Utils.hpp"
#include "extern/dw1.hpp"
#include "extern/libc.hpp"
#include "extern/libcd.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/stddef.hpp"

namespace
{
    constexpr dtl::array<dtl::array<char, 16>, 17> REL_BIN_PATHS = {
        "",
        "BTL_REL.BIN",
        "STD_REL.BIN",
        "FISH_REL.BIN",
        "EVL_REL.BIN",
        "KAR_REL.BIN",
        "VS_REL.BIN",
        "MOV_REL.BIN",
        "DOO2_REL.BIN",
        "DOOA_REL.BIN",
        "TRN_REL.BIN",
        "SHOP_REL.BIN",
        "DGET_REL.BIN",
        "TRN2_REL.BIN",
        "MURD_REL.BIN",
        "ENDI_REL.BIN",
        "EAB_REL.BIN",
    };

    constexpr dtl::array<uint32_t, 17> REL_BIN_OFFSETS = {
        0x00000000,
        0x80052ae0,
        0x80052ae0,
        0x80070000,
        0x80060000,
        0x80053800,
        0x80052ae0,
        0x80010000,
        0x80070000,
        0x80080000,
        0x80088800,
        0x80080800,
        0x80080800,
        0x80088800,
        0x8007c000,
        0x80060000,
        0x80060000,
    };

    struct FileRequest
    {
        uint8_t* targetBuffer{nullptr};
        uint8_t* isRunningPtr{nullptr};
        FileCallback finishCallback{nullptr};
        void* finishCallbackParam{nullptr};
        FileCallback waitCallback{nullptr};
        void* waitCallbackParam{nullptr};
        int8_t state{-1};
        dtl::array<uint8_t, 31> filename{};
        CdlLoc pos{};
        int32_t size{-1};
    };

    struct FileRequestCallback
    {
        bool isEnabled{false};
        FileCallback callback;
        void* parameter;
    };

    struct FileLookup
    {
        CdlLoc pos;
        uint32_t size;
    };

    FileRequestCallback FILE_REQUEST_CALLBACK2;
    dtl::RingQueue<FileRequest, 32> fileRequestQueue{};

    void _tickFileReadQueue(int32_t instance)
    {
        tickFileReadQueue();
    }

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
} // namespace

extern "C"
{
    uint32_t lookupFileSize(const char* path)
    {
        FileLookup lookup;
        if (!lookupFileTable(&lookup, path)) return 0;

        return lookup.size;
    }

    void loadTexture(uint8_t* buffer, uint32_t* outTexPage, uint32_t* outCLUT)
    {
        if (!buffer) return;

        GsIMAGE imageData;
        libgs_GsGetTimInfo(reinterpret_cast<uint32_t*>(buffer + 4), &imageData);
        RECT pixelRect = {
            .x      = imageData.pixelX,
            .y      = imageData.pixelY,
            .width  = static_cast<int16_t>(imageData.pixelWidth),
            .height = static_cast<int16_t>(imageData.pixelHeight),
        };
        libgpu_LoadImage(&pixelRect, imageData.pixelPtr);
        if (outTexPage) *outTexPage = getTPage(imageData.pixelMode & 3, 0, imageData.pixelX, imageData.pixelY);

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

    void readFile(const char* file, void* buffer)
    {
        if (!buffer) return;

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

    void readFileSectors(const char* path, void* buffer, uint32_t offset, uint32_t sectors)
    {
        FileLookup lookup;

        if (!lookupFileTable(&lookup, path)) return;

        int32_t pos = libcd_CdPosToInt(&lookup.pos);
        libcd_CdIntToPos(pos + offset, &lookup.pos);

        // seek file location
        while (!libcd_CdControl(CdCommand::CdlSetloc, reinterpret_cast<uint8_t*>(&lookup.pos), nullptr))
            ;
        // start reading
        while (!libcd_CdRead(sectors, buffer, 0x80))
            ;
        // wait for reading to complete
        while (libcd_CdReadSync(0, nullptr) > 0)
            ;
    }

    void loadDynamicLibrary(Overlay lib, uint8_t* isComplete, bool isAsync, FileCallback callback, void* param)
    {
        const auto& path = REL_BIN_PATHS[static_cast<int32_t>(lib)];
        auto* offset     = reinterpret_cast<uint8_t*>(REL_BIN_OFFSETS[static_cast<int32_t>(lib)]);

        if (isAsync)
            addFileReadRequestPath(path.data(), offset, isComplete, callback, param);
        else
            readFile(path.data(), offset);
    }

    void loadTrainingLibrary(int32_t map)
    {
        if (map == 0x70 || map == 0x4E || map == 0x77)
            loadDynamicLibrary(Overlay::TRN_REL, &TRN_LOADING_COMPLETE, true, nullptr, nullptr);
        else if (map == 0x6B || map == 0x6C || map == 0xA5 || map == 0x63)
            loadDynamicLibrary(Overlay::TRN2_REL, &TRN_LOADING_COMPLETE, true, nullptr, nullptr);

        TRAINING_COMPLETE = 0;
    }

    void setFileReadCallback2(FileCallback callback, void* param)
    {
        FILE_REQUEST_CALLBACK2.callback  = callback;
        FILE_REQUEST_CALLBACK2.parameter = param;
        FILE_REQUEST_CALLBACK2.isEnabled = true;
    }

    void tickFileReadQueue()
    {
        auto* request = fileRequestQueue.front();
        if (request == nullptr) return;

        if (request->state == 2)
        {
            if (request->size < 0)
            {
                dtl::array<uint8_t, 64> path;
                CdlFILE file;
                sprintf(path.data(), "\\%s;1", request->filename.data());
                auto result = libcd_CdSearchFile(&file, reinterpret_cast<const char*>(path.data()));
                if (result == nullptr) return;

                request->pos  = file.pos;
                request->size = file.size;
            }

            // seek file location
            while (!libcd_CdControl(CdCommand::CdlSetloc, reinterpret_cast<uint8_t*>(&request->pos), nullptr))
                ;
            // start reading
            while (!libcd_CdRead((request->size + 2047) >> 11, request->targetBuffer, 0x80))
                ;

            request->state = 1;
            return;
        }

        if (request->state == 1)
        {
            auto remainingSectors = libcd_CdReadSync(1, nullptr);
            if (remainingSectors < 0)
                request->state = 2; // on error, restart request
            else if (remainingSectors == 0)
            {
                FILE_REQUEST_CALLBACK2.isEnabled = false;
                if (request->finishCallback) request->finishCallback(request->finishCallbackParam);

                if (FILE_REQUEST_CALLBACK2.isEnabled)
                {
                    request->state             = 10;
                    request->waitCallback      = FILE_REQUEST_CALLBACK2.callback;
                    request->waitCallbackParam = FILE_REQUEST_CALLBACK2.parameter;
                }
            }
            else
                return;
        }

        if (request->state == 10 && request->waitCallback(request->waitCallbackParam)) return;

        request->state = 0;
        if (request->isRunningPtr != nullptr) *request->isRunningPtr = 0;
        fileRequestQueue.pop();
    }

    void initializeFileReadQueue()
    {
        addObject(ObjectID::FILE_READ_QUEUE, 0, _tickFileReadQueue, nullptr);
    }

    int32_t addFileReadRequest(const char* filename,
                               uint8_t* buffer,
                               uint8_t* readComplete,
                               FileCallback callback,
                               void* param,
                               CdlLoc* loc,
                               size_t size)
    {
        constexpr auto max_length = decltype(FileRequest::filename)::element_count;

        auto stringLength = strnlen_s(filename, max_length);
        if (stringLength == max_length) return -1;

        FileRequest request;
        request.isRunningPtr        = readComplete;
        request.state               = 2;
        request.targetBuffer        = buffer;
        request.finishCallback      = callback;
        request.finishCallbackParam = param;
        if (loc != nullptr) request.pos = *loc;
        request.size = size;
        memcpy(request.filename.data(), filename, stringLength);

        if (readComplete != nullptr) *readComplete = 1;

        while (!fileRequestQueue.push(request))
            tickFileReadQueue();

        return fileRequestQueue.getSize();
    }

    int32_t addFileReadRequestPath(const char* filename,
                                   uint8_t* buffer,
                                   uint8_t* readComplete,
                                   FileCallback callback,
                                   void* param)
    {
        return addFileReadRequest(filename, buffer, readComplete, callback, param, nullptr, -1);
    }

    int32_t addFileReadRequestSection(const char* filename,
                                      uint8_t* buffer,
                                      int32_t offset,
                                      int32_t sectors,
                                      uint8_t* readComplete,
                                      FileCallback callback,
                                      void* callbackParam)
    {
        FileLookup lookup;
        auto hasFile = lookupFileTable(&lookup, filename);
        if (!hasFile) return -1;

        auto pos = libcd_CdPosToInt(&lookup.pos);
        libcd_CdIntToPos(pos + offset, &lookup.pos);
        return addFileReadRequest(filename, buffer, readComplete, callback, callbackParam, &lookup.pos, sectors << 11);
    }

    int32_t addFileReadRequestLookup(const char* filename,
                                     uint8_t* buffer,
                                     uint8_t* readComplete,
                                     FileCallback callback,
                                     void* callbackParam)
    {
        FileLookup lookup;
        auto hasFile = lookupFileTable(&lookup, filename);
        if (!hasFile) return -1;

        return addFileReadRequest(filename, buffer, readComplete, callback, callbackParam, &lookup.pos, lookup.size);
    }

    void loadStackedTIMFile(const char* file)
    {
        auto* head = GENERAL_BUFFER.data();
        readFile(file, head);

        while (*head == 0x10)
        {
            GsIMAGE image;
            libgs_GsGetTimInfo(reinterpret_cast<uint32_t*>(head + 4), &image);
            head += 20 + (image.pixelWidth * image.pixelHeight) * 2;

            RECT pixelRect = {
                .x      = image.pixelX,
                .y      = image.pixelY,
                .width  = static_cast<int16_t>(image.pixelWidth),
                .height = static_cast<int16_t>(image.pixelHeight),
            };
            libgpu_LoadImage(&pixelRect, image.pixelPtr);

            if ((image.pixelMode >> 3 & 1) != 0)
            {
                RECT clutRect = {
                    .x      = image.clutX,
                    .y      = image.clutY,
                    .width  = static_cast<int16_t>(image.clutWidth),
                    .height = static_cast<int16_t>(image.clutHeight),
                };
                libgpu_LoadImage(&clutRect, image.clutPtr);
                head += 12 + (image.clutWidth * image.clutHeight) * 2;
            }
        }
    }
}
