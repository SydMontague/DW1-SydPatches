#include "../Matrix.hpp"
#include "Transform.hpp"

using namespace EFETransform;

extern "C"
{
    void EFE_ENTRY(getVectorLength)()
    {
        auto* vector = popPointer<int32_t>();
        auto* out    = popPointer<int32_t>();
        *out         = getDistance(vector[0], vector[1], vector[2]);
    }

    void EFE_ENTRY(getVectorEulerAngles)()
    {
        auto* vector = popPointer<int32_t>();
        auto* out    = popPointer<int32_t>();
        SVector rotation;
        toEulerAngles(&rotation, vector[0], vector[1], vector[2]);
        writeVector(out, rotation);
    }
}
