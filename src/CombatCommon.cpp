#include "Math.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    void initializeSomeArenaArrays(int32_t count, int32_t* array1, int32_t* array2, int32_t* array3)
    {
        for (int32_t i = 0; i < count; i++)
            array3[i] = 0;
    }

    int32_t processSomeArenaArrays(int32_t count, int32_t value, int32_t* array1, int32_t* array2, int32_t* array3)
    {
        // TODO the fuck is this?
        auto left  = 0;
        auto right = count - 1;

        while (left < right) {
            auto mid = (left + right) / 2;

            if (array1[mid] < value)
                left = mid + 1;
            else
                right = mid;
        }

        auto index = max(left - 1, 0);

        auto val0      = array1[index];
        auto val1      = array1[index + 1];
        auto val2      = array3[index];
        auto val3      = array3[index + 1];
        auto val4      = array2[index];
        auto val5      = array2[index + 1];
        auto valueDiff = value - val0;
        auto diff1     = val1 - val0;
        auto diff2     = val3 - val2;
        auto diff3     = val5 - val4;

        return val4 + valueDiff * (((diff3 / diff1 - diff1 * (val3 * val2 * 2)) +
                                    valueDiff * (val2 * 3 + (valueDiff * diff2) / diff1)));
    }
}
