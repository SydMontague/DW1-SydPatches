#include "Loading.hpp"
#include "../extern/libgs.hpp"

namespace
{
    enum class Phase
    {
        TEXTURE,
        MODEL,
        FIND_SLOT,
        IDLE,
        INITIALIZE,
        SCRIPT,
        READING,
    };
    Phase phase = Phase::IDLE;
    int32_t slotIndex;

    void publishError(EFELoading::Request& request, int8_t reason)
    {
        auto* input  = request.moves - 1;
        auto* output = request.effectIds;
        while (*input++ >= 0)
            *output++ = -1;
        *request.status = reason;
    }

    bool rejectSetup(EFELoading::Request& request, int8_t reason)
    {
        releaseEFEModel(*request.model);
        phase = Phase::IDLE;
        publishError(request, reason);
        return false;
    }

    uint8_t* getSection(uint8_t* data, int32_t begin, int32_t end)
    {
        if (begin == end) return nullptr;
        return data + sizeof(EFELoading::Header) + begin;
    }
} // namespace

extern "C"
{

    [[gnu::optimize("Os")]]
    void EFE_loadNextFile(EFELoading::Request* request)
    {
        ++request->index;
        int32_t id = *request->moves++;
        if (id < 0) {
            *request->status = 0;
            return;
        }
        if (id < 256 || id >= 634) {
            publishError(*request, -4);
            return;
        }
        auto available = findFreeEFEModel();
        if (!available.has_value()) {
            publishError(*request, -5);
            return;
        }
        auto* acquired     = *available;
        acquired->useCount = id;
        acquired->mmdPtr   = EFE_FILE_CURSOR;
        request->model     = acquired;
        auto location      = *getEFEDATEntry(id);
        addFileReadRequest("", acquired->mmdPtr, nullptr, EFE_handleLoadedFile, request, &location, 20480);
        phase = Phase::READING;
    }

    bool EFE_handleLoadedFile(void* parameter)
    {
        phase = Phase::TEXTURE;
        setFileReadCallback2(EFE_setupLoadedFile, parameter);
        return false;
    }

    [[gnu::optimize("Os")]]
    bool EFE_setupLoadedFile(void* parameter)
    {
        if (phase == Phase::IDLE) return false;
        auto& request = *static_cast<EFELoading::Request*>(parameter);
        if (phase == Phase::SCRIPT) {

            EFE_CURRENT_SLOT = &EFE_SLOT_BASE[slotIndex];
            int32_t count    = 0;
            for (; count < 4; ++count) {
                EFE_CURRENT_OPCODE = *EFE_INSTRUCTION_POINTER;
                EFE_dispatchOpcode(EFE_CURRENT_OPCODE & 255);
                if (EFE_INSTRUCTION_POINTER == nullptr) break;
            }
            if (count == 4) return true;
            auto* sub = EFE_CURRENT_SUBEFFECT;
            if (sub->instance->frame == -1) sub->tickInstruction = nullptr;
            auto result                     = EFE_SCRIPT_RESULT;
            EFE_LOADED_MOVE_DATA[slotIndex] = result >= -1 ? EFE_CURRENT_SLOT->fileId : result;
            *request.effectIds++            = slotIndex;
            phase                           = Phase::IDLE;
            EFE_loadNextFile(&request);
            return false;
        }
        auto& model        = *request.model;
        auto* data         = model.mmdPtr;
        const auto& header = *reinterpret_cast<const EFELoading::Header*>(data);
        auto* texture      = getSection(data, header.textureOffset, header.textureEndOffset);
        auto* modelData    = getSection(data, header.modelOffset, header.retainedEndOffset);
        model.modelPtr     = reinterpret_cast<TMDModel*>(modelData);
        switch (phase) {
            case Phase::TEXTURE:
                if (texture != nullptr) {
                    int32_t page = 0;
                    while (page < 16 && EFE_MODEL_TAKEN[page] != 0)
                        ++page;
                    if (page == 16) return rejectSetup(request, -2);
                    EFE_MODEL_TAKEN[page] = 1;
                    model.pixelPage       = page / 8 + 25;
                    model.clutPage        = ((page % 4 * 6 + 488) << 6) | ((page / 4 * 16 + 128) >> 4 & 63);
                    model.pixelOffsetX    = 0;
                    model.pixelOffsetY    = page % 8 * 32;
                    model.modelId         = page;
                    uploadModelTexture(reinterpret_cast<uint32_t*>(texture), &model);
                }
                phase = Phase::MODEL;
                [[fallthrough]];
            case Phase::MODEL:
                if (modelData != nullptr) {
                    libgs_GsMapModelingData(reinterpret_cast<uint32_t*>(model.modelPtr) + 1);
                    updateTMDTextureData(model.modelPtr,
                                         model.pixelPage,
                                         model.pixelOffsetX,
                                         model.pixelOffsetY,
                                         model.clutPage - 0x7A08);
                }
                phase = Phase::FIND_SLOT;
                [[fallthrough]];
            case Phase::FIND_SLOT:
                if (header.fileId != request.moves[-1]) return rejectSetup(request, -3);
                EFE_LOAD_STATE = -1;
                slotIndex      = 0;
                while (slotIndex < 16 && EFE_LOADED_MOVE_DATA[slotIndex] != -1)
                    ++slotIndex;
                if (slotIndex == 16) return rejectSetup(request, -1);
                phase = Phase::INITIALIZE;
                return true;
            case Phase::INITIALIZE:
            {
                auto& slot       = EFE_SLOT_BASE[slotIndex];
                EFE_CURRENT_SLOT = &slot;
                slot.model       = &model;
                auto* base       = data + sizeof(EFELoading::Header);
                EFE_FILE_CURSOR += header.retainedEndOffset + sizeof(EFELoading::Header);
                slot.activeFile     = base;
                slot.uvCount        = modelData == nullptr ? 0 : model.modelPtr->objectCount;
                slot.subEffectCount = header.subEffectCount;
                slot.subEffects     = reinterpret_cast<EFESubEffect*>(base + header.subEffectOffset);
                for (int32_t i = 0; i < header.subEffectCount; ++i)
                    slot.subEffects[i].tickInstruction = nullptr;
                if (slot.uvCount != 0) {
                    slot.uvRecords = reinterpret_cast<EFELoading::UVAnimation*>(base + header.uvOffset);
                    for (int32_t i = 0; i < slot.uvCount; ++i) {
                        auto& uv        = slot.uvRecords[i];
                        uv.unk0         = 0;
                        uv.numKeyframes = 0;
                    }
                }
                slot.fileId             = header.fileId;
                slot.runInstruction     = reinterpret_cast<int16_t*>(base + header.runOffset);
                slot.setupInstruction   = reinterpret_cast<int16_t*>(base + header.setupOffset);
                slot.headerSection      = base + header.sectionOffset;
                EFE_INSTANCE_BASE       = nullptr;
                EFE_SCRIPT_BASE         = slot.activeFile;
                EFE_CURRENT_SUBEFFECT   = slot.subEffects;
                EFE_SCRIPT_RESULT       = -1;
                EFE_INSTRUCTION_POINTER = slot.setupInstruction;
                EFE_DATA_STACK          = EFE_SCRIPT_MEM1_DATA;
                EFE_RETURN_STACK        = EFE_RETURN_STACK_BASE;
                *EFE_RETURN_STACK++     = 0;
                phase                   = Phase::SCRIPT;
                return true;
            }
            default: return false;
        }
    }

    bool EFE_isSetupScriptActive()
    {
        return phase == Phase::SCRIPT;
    }

    [[gnu::optimize("Os")]]
    void EFE_quiesceLoading()
    {
        while (phase != Phase::IDLE)
            tickFileReadQueue();
    }
}
