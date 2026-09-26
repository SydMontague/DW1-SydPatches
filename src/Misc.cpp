#include "extern/dtl/types.hpp"
#include "extern/dw1.hpp"

extern "C"
{
    [[deprecated("For legacy, use runtime_array/vector instead")]]
    uint8_t* allocateArray(int32_t minSize)
    {
        auto result = ARRAY_SECTION_START + ARRAY_SECTION_OFFSET + 4;
        auto size   = (minSize + 3) & ~3;

        *reinterpret_cast<size_t*>(ARRAY_SECTION_START + ARRAY_SECTION_OFFSET) = size;
        ARRAY_SECTION_OFFSET += (size + 4);
        return result;
    }

    [[deprecated("For legacy, use runtime_array/vector instead")]]
    void freeArray(size_t* ptr)
    {
        ARRAY_SECTION_OFFSET -= (ptr[-1] + 4);
    }
}
