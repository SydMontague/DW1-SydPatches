#include "../Math.hpp"
#include "Operands.hpp"

extern "C"
{
    void EFE_ENTRY(checkTargetCollision)()
    {
        const auto radius    = EFEOperands::pop();
        auto* hit            = EFEOperands::popPointer<int32_t>();
        auto* angle          = EFEOperands::popPointer<int32_t>();
        auto* direction      = EFEOperands::popPointer<int32_t>();
        const auto& position = reinterpret_cast<const EFEInstance*>(EFE_INSTANCE_BASE)->position;
        const auto& target   = EFE_CURRENT_SUBEFFECT->targetEntity->posData->posMatrix.work.t;
        const int32_t dx     = static_cast<uint32_t>(position[0]) - target[0];
        const int32_t dz     = static_cast<uint32_t>(position[2]) - target[2];
        if (EFEHelpers::sumOfSquares(dx, dz) > EFEHelpers::sumOfSquares(radius, 0)) {
            *hit = 0;
            return;
        }
        *hit               = 1;
        const auto facing  = ((0x400 - atan(dx, dz)) & 0xFFF) + 0x800;
        *angle             = facing;
        const auto heading = 0x80 - (facing >> 3);
        direction[2]       = static_cast<int32_t>(static_cast<uint32_t>(radius) * sin(heading)) >> 12;
        direction[0]       = static_cast<int32_t>(static_cast<uint32_t>(radius) * cos(heading)) >> 12;
    }
}
