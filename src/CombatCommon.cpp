#include "Math.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    struct TwoPoints
    {
        Position min;
        Position max;
    };

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

        while (left < right)
        {
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

    constexpr int32_t getSomeIndex(TwoPoints* point1, Position* point2)
    {
        auto val = 4;

        if (point1->max.x < point2->x)
            val = 5;
        else if (point2->x < point1->min.x)
            val = 3;

        if (point1->max.y < point2->y)
            val += 3;
        else if (point2->y < point1->min.y)
            val -= 3;

        return val;
    }

    int32_t doSomethingWithSomePoints(TwoPoints* point1, TwoPoints* point2)
    {
        auto indexMin = getSomeIndex(point1, &point2->min);
        auto indexMax = getSomeIndex(point1, &point2->max);

        if (indexMin == 4 || indexMax == 4) return -1;

        if (indexMin + indexMax == 8) return -1;
        if (indexMin / 3 == indexMax / 3) return 0;
        if (indexMin % 3 == indexMax % 3) return 0;

        int32_t matrixX[4];
        int32_t matrixY[4];
        auto diffX = point2->max.x - point2->min.x;
        auto diffY = point2->max.y - point2->min.y;

        matrixX[0] = point1->min.x - point2->min.x;
        matrixX[1] = point1->max.x - point2->min.x;
        matrixX[2] = point1->min.x - point2->min.x;
        matrixX[3] = point1->max.x - point2->min.x;
        matrixY[0] = point1->min.y - point2->min.y;
        matrixY[1] = point1->min.y - point2->min.y;
        matrixY[2] = point1->max.y - point2->min.y;
        matrixY[3] = point1->max.y - point2->min.y;

        for (int32_t i = 1; i < 4; i++)
        {
            auto factor1 = (diffX * matrixY[0] - diffY * matrixX[0]);
            auto factor2 = (diffX * matrixY[i] - diffY * matrixX[i]);

            if (factor1 * factor2 < 0) return -1;
        }

        return 0;
    }
}