#include "../Helper.hpp"
#include "Operands.hpp"

namespace
{
    void writeDigimonSize(const Entity* entity)
    {
        auto* out        = EFEOperands::popPointer<int32_t>();
        const auto* data = getDigimonData(entity->type);
        out[0]           = data->radius;
        out[1]           = data->height;
        out[2]           = data->radius;
    }
} // namespace

extern "C"
{
    void EFE_ENTRY(getTargetDigimonSize)()
    {
        writeDigimonSize(EFE_CURRENT_SUBEFFECT->targetEntity);
    }

    void EFE_ENTRY(getSourceDigimonSize)()
    {
        writeDigimonSize(EFE_CURRENT_SUBEFFECT->sourceEntity);
    }

    void EFE_ENTRY(isTargetUnhit)()
    {
        auto* out          = EFEOperands::popPointer<int32_t>();
        const auto* target = static_cast<DigimonEntity*>(EFE_CURRENT_SUBEFFECT->targetEntity);
        *out               = target->stats.isHit == 0;
    }
}
