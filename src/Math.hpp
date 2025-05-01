#pragma once
#include "extern/dw1.hpp"
#include "extern/libgte.hpp"
#include "extern/stddef.hpp"

struct ScreenPos
{
    int16_t screenX;
    int16_t screenY;
    int16_t depth;
};

extern "C"
{
    struct AABB
    {
        SVector* center;
        SVector extent;
    };

    struct BoundingBox2D
    {
        int32_t minX;
        int32_t maxX;
        int32_t minY;
        int32_t maxY;

        constexpr BoundingBox2D(int32_t minX, int32_t maxX, int32_t minY, int32_t maxY)
            : minX(minX)
            , maxX(maxX)
            , minY(minY)
            , maxY(maxY)
        {
        }

        constexpr BoundingBox2D(int32_t centerX, int32_t centerY, int32_t extend)
            : minX(centerX - extend)
            , maxX(centerX + extend)
            , minY(centerY - extend)
            , maxY(centerY + extend)
        {
        }

        constexpr bool overlaps(int32_t minX2, int32_t maxY2, int32_t maxX2, int32_t minY2)
        {
            if (maxX < minX2) return false;
            if (minX > maxX2) return false;
            if (minY > maxY2) return false;
            if (maxY < minY2) return false;

            return true;
        }

        constexpr bool overlaps(const BoundingBox2D& other)
        {
            if (minX > other.maxX) return false;
            if (maxX < other.minX) return false;
            if (minY > other.maxY) return false;
            if (maxY < other.minY) return false;

            return true;
        }
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

    int32_t random(int32_t limit);
    int16_t sin(uint32_t input);
    int16_t cos(uint32_t input);
    int16_t atan(int32_t dx, int32_t dy);
    void setRotTransMatrix(Matrix* matrix);
    bool hasAABBOverlap(AABB* aabb1, AABB* aabb2);
    int32_t findAABBHitEntity(AABB* aabb, Entity* ignoreEntity, int32_t startId);
    void getRotationDifference(PositionData* pos, Vector* target, int16_t* outAngle, int16_t* ccDiff, int16_t* cwDiff);
    bool rotateEntity(SVector* rotVector, int16_t targetAngle, int16_t ccDiff, int16_t cwDiff, int16_t speed);
    void addWithLimit(int16_t* valuePtr, int16_t value, int32_t limit);
}

/*
 * Gets the 2D screen position of a 3D position.
 * The returned coordinates are relative to the center of the visibale screen area.
 */
ScreenPos getScreenPosition(int16_t x, int16_t y, int16_t z);
/*
 * Gets the 2D screen position of a 3D position.
 * The returned coordinates are relative to the center of the visibale screen area.
 */
ScreenPos getScreenPosition(SVector pos);
/*
 * Gets the 2D screen position of a 3D position.
 * The returned coordinates are relative to the center of the visibale screen area.
 */
ScreenPos getScreenPosition(Vector pos);
/*
 * Gets the 2D screen position of a 3D position.
 * The returned coordinates are relative to the center of the visibale screen area.
 */
ScreenPos getScreenPosition(int16_t tileX, int16_t tileZ);

/*
 * Gets the 2D map position of a 3D position.
 * The returned coordinates are relative to the center of the map.
 */
ScreenPos getMapPosition(int16_t x, int16_t y, int16_t z);
/*
 * Gets the 2D map position of a 3D position.
 * The returned coordinates are relative to the center of the map.
 */
ScreenPos getMapPosition(SVector pos);
/*
 * Gets the 2D map position of a 3D position.
 * The returned coordinates are relative to the center of the map.
 */
ScreenPos getMapPosition(Vector pos);
/*
 * Gets the 2D map position of a 3D position.
 * The returned coordinates are relative to the center of the map.
 */
ScreenPos getMapPosition(int16_t tileX, int16_t tileZ);

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

constexpr int16_t convertTileToPosX(int16_t tileX)
{
    return (tileX - 50) * 100 + 50;
}

constexpr int16_t convertTileToPosZ(int16_t tileZ)
{
    return (50 - tileZ) * 100 - 50;
}

constexpr int32_t posmod(int32_t val, int32_t mod)
{
    auto newVal = val % mod;
    if (newVal < 0) newVal += mod;

    return newVal;
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

static_assert(posmod(2048, 4096) == 2048);
static_assert(posmod(6144, 4096) == 2048);
static_assert(posmod(128, 4096) == 128);
static_assert(posmod(-1, 4096) == 4095);
static_assert(posmod(-2048, 4096) == 2048);
static_assert(posmod(-4095, 4096) == 1);
