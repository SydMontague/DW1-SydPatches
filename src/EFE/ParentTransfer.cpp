#include "Transform.hpp"

using namespace EFETransform;

namespace
{
    void copyField(uint8_t* destination, const uint8_t* source, int32_t size)
    {
        switch (size) {
            case 1: *destination = *source; break;
            case 2: *reinterpret_cast<uint16_t*>(destination) = *reinterpret_cast<const uint16_t*>(source); break;
            case 4:
            case 8:
            case 12:
                for (int i = 0; i < size / 4; ++i)
                    reinterpret_cast<uint32_t*>(destination)[i] = reinterpret_cast<const uint32_t*>(source)[i];
                break;
        }
    }
} // namespace

extern "C"
{
    void EFE_ENTRY(copyFromParentTransform)()
    {
        auto size         = pop();
        auto offset       = pop();
        auto* destination = popPointer<uint8_t>();
        if (size != 8) copyField(destination, reinterpret_cast<uint8_t*>(EFE_PARENT_INSTANCE) + offset, size);
    }

    void EFE_ENTRY(copyToParentTransform)()
    {
        auto size    = pop();
        auto offset  = pop();
        auto* source = popPointer<uint8_t>();
        copyField(reinterpret_cast<uint8_t*>(EFE_PARENT_INSTANCE) + offset, source, size);
    }
}
