#include "../Math.hpp"
#include "Transform.hpp"

using namespace EFETransform;

namespace
{
    void rotateZYX(SVector& direction, int16_t pitch, int16_t yaw, int16_t roll)
    {
        SVector rotation = {pitch, yaw, roll};
        Matrix matrix;
        libgte_RotMatrixZYX(&rotation, &matrix);
        libgte_ApplyMatrixSV(&matrix, &direction, &direction);
    }

    int32_t limitedTurn(int32_t angle, int32_t limit)
    {
        angle &= 0xFFF;
        if (angle > 0x800) angle -= 0x1000;
        return angle > 0 ? min(angle, limit) : -min(-angle, limit);
    }

    void advance(EFEInstance& instance, const SVector& direction)
    {
        instance.position[0] += direction.x >> 3;
        instance.position[1] += direction.y >> 3;
        instance.position[2] += direction.z >> 3;
    }

    int16_t forwardStep(int32_t speed)
    {
        return static_cast<int16_t>(-static_cast<uint32_t>(speed) * 8);
    }
} // namespace

extern "C"
{
    void EFE_ENTRY(rotateTransformTowardPoint)()
    {
        auto* target         = popPointer<int32_t>();
        auto& instance       = *reinterpret_cast<EFEInstance*>(EFE_INSTANCE_BASE);
        auto direction       = atan(target[0] - instance.position[0], target[2] - instance.position[2]);
        instance.rotation[1] = (0x400 - direction) & 0xFFF;
    }

    void EFE_ENTRY(steerTransformTowardPoint)()
    {
        auto limit     = pop();
        auto speed     = pop();
        auto* target   = popPointer<int32_t>();
        auto& instance = *reinterpret_cast<EFEInstance*>(EFE_INSTANCE_BASE);
        SVector delta  = {static_cast<int16_t>(target[0] - instance.position[0]),
                          0,
                          static_cast<int16_t>(target[2] - instance.position[2])};
        rotateZYX(delta, 0, -instance.rotation[1], 0);
        instance.rotation[1] += limitedTurn(0x400 - atan(delta.x, delta.z), limit);
        SVector direction = {0, 0, forwardStep(speed)};
        rotateZYX(direction, instance.rotation[0], instance.rotation[1], instance.rotation[2]);
        advance(instance, direction);
    }

    void EFE_ENTRY(applyHomingMovement)()
    {
        auto limit  = pop();
        auto* speed = popPointer<int32_t>();
        EFE_DATA_STACK -= sizeof(int32_t); // Unused script operand.
        auto maxSpeed  = pop();
        auto minSpeed  = pop();
        auto* target   = popPointer<int32_t>();
        auto& instance = *reinterpret_cast<EFEInstance*>(EFE_INSTANCE_BASE);
        auto& yaw      = instance.rotation[1];
        auto& pitch    = instance.rotation[2];
        SVector delta  = {static_cast<int16_t>(target[0] - instance.position[0]),
                          static_cast<int16_t>(target[1] - instance.position[1]),
                          static_cast<int16_t>(target[2] - instance.position[2])};
        rotateZYX(delta, 0, -yaw, 0);
        auto yawStep = limitedTurn(0x400 - atan(delta.x, delta.z), limit);
        yaw += yawStep;
        auto pitchStep = limitedTurn(atan(delta.y, delta.z) - 0x400 - pitch, limit);
        pitch += pitchStep;
        auto phase = pitch & 0xFFF;
        if (phase >= 0x400 && phase < 0xC00) pitch = 0x800 - pitch;

        if (abs(pitchStep) + abs(yawStep) <= limit * 40 / 100)
            *speed = min(*speed * 130 / 100, maxSpeed);
        else
            *speed = max(*speed * 82 / 100, minSpeed);

        SVector direction = {0, 0, forwardStep(*speed)};
        rotateZYX(direction, pitch, 0, 0);
        rotateZYX(direction, 0, yaw, 0);
        advance(instance, direction);
    }
}
