#pragma once

#include "../Files.hpp"
#include "../Model.hpp"
#include "Operands.hpp"

namespace EFELoading
{
    struct Request
    {
        int16_t index;
        int16_t padding;
        int8_t* status;
        int16_t* moves;
        int16_t* effectIds;
        ModelComponent* model;
    };

    struct UVKeyframe
    {
        int16_t frames;
        int8_t deltaU;
        int8_t deltaV;
    };

    struct UVAnimation
    {
        int16_t unk0;
        int16_t unk2;
        int16_t unk4;
        int16_t unk6;
        int16_t unk8;
        int16_t unkA;
        int16_t numKeyframes;
        int16_t keyframe;
        int16_t uvFrame;
        int16_t frame;
        int16_t numFrames;
        int16_t pad;
        UVKeyframe* uvData;
        UVKeyframe* uv;
    };

    struct Slot
    {
        int32_t fileId;
        int16_t* setupInstruction;
        uint8_t* activeFile;
        int16_t* runInstruction;
        EFESubEffect* subEffects;
        int16_t subEffectCount;
        int16_t untouched;
        ModelComponent* model;
        int32_t uvCount;
        UVAnimation* uvRecords;
        uint8_t* headerSection;
    };

    struct Header
    {
        int32_t runOffset;
        int32_t setupOffset;
        int32_t uvOffset;
        int32_t sectionOffset;
        int32_t subEffectOffset;
        int32_t modelOffset;
        int32_t retainedEndOffset;
        int32_t textureOffset;
        int32_t unknown32;
        int32_t textureEndOffset;
        int32_t unknown40;
        int32_t subEffectCount;
        int32_t fileId;
    };

    static_assert(sizeof(UVKeyframe) == 4 && sizeof(UVAnimation) == 32);
    static_assert(__builtin_offsetof(UVAnimation, numKeyframes) == 12 && __builtin_offsetof(UVAnimation, uvData) == 24);
    static_assert(sizeof(Request) == 20 && __builtin_offsetof(Request, model) == 16);
    static_assert(sizeof(Slot) == 40 && __builtin_offsetof(Slot, model) == 24);
    static_assert(sizeof(Header) == 52 && __builtin_offsetof(Header, fileId) == 48);
} // namespace EFELoading

extern EFELoading::Slot* EFE_CURRENT_SLOT;
extern EFELoading::Slot* EFE_SLOT_BASE;
extern uint8_t* EFE_FILE_CURSOR;

extern "C"
{
    CdlLoc* getEFEDATEntry(int32_t id);
    void updateTMDTextureData(TMDModel* model, uint32_t page, uint32_t x, uint32_t y, uint32_t clut);
    void EFE_dispatchOpcode(int32_t opcode);
    int32_t EFE_runScript(int16_t* instruction);
    void EFE_loadNextFile(EFELoading::Request* request);
    bool EFE_setupLoadedFile(void* parameter);
    bool EFE_handleLoadedFile(void* parameter);
    bool EFE_isSetupScriptActive();
    void EFE_quiesceLoading();
}
