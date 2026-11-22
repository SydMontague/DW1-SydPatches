#include "Control.hpp"
#include "Loading.hpp"

extern Entity* EFE_ROOT_SOURCE;
extern Entity* EFE_ROOT_TARGET;
extern int16_t EFE_SUBEFFECT_SPAWNED;

extern "C"
{
    [[gnu::optimize("Os")]]
    void EFE_ENTRY(spawnSubEffect)()
    {
        EFE_TARGET_INDEX        = 1;
        const auto* instruction = EFE_INSTRUCTION_POINTER;
        const int32_t count     = instruction[1];
        auto* storage           = EFE_SCRIPT_BASE + instruction[2];
        const int32_t stride    = instruction[4];
        int32_t index           = 0;
        for (; index < count; ++index, storage += stride)
            if (reinterpret_cast<EFEInstance*>(storage)->frame == -1) break;

        if (index == count) {
            EFE_ENTRY(returnFromEFESubroutine)();
            return;
        }
        if (EFE_SCRIPT_RESULT == -1) EFE_SCRIPT_RESULT = index;

        const auto* parent = EFE_CURRENT_SUBEFFECT;
        auto* next         = EFE_CURRENT_SUBEFFECT;
        while (next->tickInstruction != nullptr) {
            ++next;
            // Vanilla compares signed PS1 addresses. A file without a model has no model boundary.
            auto* end = EFE_CURRENT_SLOT->model->modelPtr;
            if (reinterpret_cast<int32_t>(next) >= reinterpret_cast<int32_t>(end)) {
                EFE_CURRENT_SUBEFFECT = next;
                EFE_ENTRY(returnFromEFESubroutine)();
                return;
            }
        }

        EFE_CURRENT_SUBEFFECT = next;
        auto& child           = *next;
        EFE_PARENT_INSTANCE   = reinterpret_cast<EFEInstance*>(EFE_INSTANCE_BASE);
        EFE_INSTANCE_BASE     = storage;
        child.instance        = reinterpret_cast<EFEInstance*>(storage);
        child.parentInstance  = EFE_PARENT_INSTANCE;
        child.sourceEntity    = child.parentInstance ? parent->sourceEntity : EFE_ROOT_SOURCE;
        child.targetEntity    = child.parentInstance ? parent->targetEntity : EFE_ROOT_TARGET;
        EFE_SUBEFFECT_SPAWNED = 1;
        EFEHelpers::advanceInstruction(12);
    }

    void EFE_ENTRY(initializeSubEffectInstructions)()
    {
        auto* render                                             = EFEOperands::popPointer<int16_t>();
        auto* tick                                               = EFEOperands::popPointer<int16_t>();
        reinterpret_cast<EFEInstance*>(EFE_INSTANCE_BASE)->frame = 0;
        EFE_CURRENT_SUBEFFECT->tickInstruction                   = tick;
        EFE_CURRENT_SUBEFFECT->renderInstruction                 = render;
    }
}
