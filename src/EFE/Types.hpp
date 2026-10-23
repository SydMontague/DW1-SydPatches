#pragma once

#include "../extern/dtl/array.hpp"

struct Entity;

// Fixed records shared with the vanilla EFE engine.
struct EFEInstance
{
    int32_t frame;
    dtl::array<int32_t, 3> position;
    dtl::array<int32_t, 3> rotation;
};

struct EFEBoneOffset;

struct EFESubEffect
{
    int16_t* tickInstruction;
    int16_t* renderInstruction;
    EFEInstance* instance;
    EFEInstance* parentInstance;
    Entity* sourceEntity;
    Entity* targetEntity;
    EFEBoneOffset* boneOffset;
};

using EFEOpcodeHandler = void (*)();
using EFEComparison    = int32_t (*)(int32_t);

struct EFEOpcodeEntry
{
    uint32_t opcode;
    EFEOpcodeHandler handler;
};

static_assert(sizeof(EFEInstance) == 28 && alignof(EFEInstance) == 4);
static_assert(__builtin_offsetof(EFEInstance, frame) == 0);
static_assert(__builtin_offsetof(EFEInstance, position) == 4);
static_assert(__builtin_offsetof(EFEInstance, rotation) == 16);
static_assert(sizeof(EFESubEffect) == 28 && alignof(EFESubEffect) == 4);
static_assert(__builtin_offsetof(EFESubEffect, tickInstruction) == 0);
static_assert(__builtin_offsetof(EFESubEffect, renderInstruction) == 4);
static_assert(__builtin_offsetof(EFESubEffect, instance) == 8);
static_assert(__builtin_offsetof(EFESubEffect, parentInstance) == 12);
static_assert(__builtin_offsetof(EFESubEffect, sourceEntity) == 16);
static_assert(__builtin_offsetof(EFESubEffect, targetEntity) == 20);
static_assert(__builtin_offsetof(EFESubEffect, boneOffset) == 24);
static_assert(sizeof(EFEOpcodeEntry) == 8 && alignof(EFEOpcodeEntry) == 4);
static_assert(__builtin_offsetof(EFEOpcodeEntry, opcode) == 0);
static_assert(__builtin_offsetof(EFEOpcodeEntry, handler) == 4);
