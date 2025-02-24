#pragma once
#include "extern/dw1.hpp"
#include "extern/libgte.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    struct AABB
    {
        SVector* center;
        SVector extent;
    };

    constexpr int32_t abs(int32_t val)
    {
        return val < 0 ? -val : val;
    }

    constexpr int32_t min(int32_t val1, int32_t val2)
    {
        return val1 < val2 ? val1 : val2;
    }

    constexpr int32_t max(int32_t val1, int32_t val2)
    {
        return val1 < val2 ? val2 : val1;
    }

    inline int32_t random2(int32_t limit)
    {
        return rand() % limit;
    }

    extern int32_t random(int32_t limit);
    extern int16_t sin(uint32_t input);
    extern int16_t cos(uint32_t input);
    extern int16_t atan(int32_t dx, int32_t dy);
    extern void setRotTransMatrix(Matrix* matrix);
    bool hasAABBOverlap(AABB* aabb1, AABB* aabb2);
    int32_t findAABBHitEntity(AABB* aabb, Entity* ignoreEntity, int32_t startId);
    void getRotationDifference(PositionData* pos, Vector* target, int16_t* outAngle, int16_t* ccDiff, int16_t* cwDiff);
    bool rotateEntity(SVector* rotVector, int16_t* targetAngle, int16_t* ccDiff, int16_t* dwDiff, int16_t speed);
}

template<class T> constexpr auto clamp(T val, decltype(val) min, decltype(val) max) -> decltype(val)
{
    if (min > val) return min;
    if (max < val) return max;
    return val;
}

template<class T> constexpr auto ring(T val, decltype(val) min, decltype(val) max) -> decltype(val)
{
    if (max < min) return val;

    auto diff = max - min;
    while (val >= max)
        val -= diff;
    while (val < min)
        val += diff;

    return val;
}

static_assert(max(100, 200) == 200);
static_assert(max(100, -200) == 100);
static_assert(max(-300, -200) == -200);

static_assert(min(100, 200) == 100);
static_assert(min(100, -200) == -200);
static_assert(min(-300, -200) == -300);

static_assert(abs(0) == 0);
static_assert(abs(1000) == 1000);
static_assert(abs(-1000) == 1000);
static_assert(abs(-1) == 1);

static_assert(ring(128, -160, 480) == 128);
static_assert(ring(490, -160, 480) == -150);
static_assert(ring(-170, -160, 480) == 470);
static_assert(ring(480, -160, 480) == -160);
static_assert(ring(-160, -160, 480) == -160);
static_assert(ring(490, 480, -160) == 490);

static_assert(clamp(128, 64, 256) == 128);
static_assert(clamp(31, 64, 256) == 64);
static_assert(clamp(1024, 64, 256) == 256);
static_assert(clamp(256, 64, 256) == 256);
static_assert(clamp(-10, 64, 256) == 64);
static_assert(clamp(64, 64, 256) == 64);