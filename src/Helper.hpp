#pragma once
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

template<class First, class Second> struct Pair
{
    First first;
    Second second;
};

struct ReadBuffer
{
    uint8_t* buffer;

    template<class T> constexpr T read()
    {
        T val = *reinterpret_cast<T*>(buffer);
        buffer += sizeof(T);
        return val;
    }
};

struct TileCoord
{
    int8_t tileX;
    int8_t tileY;
};

struct TileIterator
{
private:
    int8_t stepCount;
    int8_t endX;
    int8_t endY;
    int8_t absX;
    int8_t absY;
    int8_t currentX;
    int8_t currentY;

    int8_t stepX;
    int8_t stepY;
    int8_t subStepX;
    int8_t subStepY;
    int8_t progressX;
    int8_t progressY;

public:
    TileIterator(int8_t startX, int8_t startY, int8_t endX, int8_t endY);

    TileCoord operator*();
    bool hasNext();
    TileIterator& operator++();
};

extern "C"
{
    constexpr auto SCREEN_WIDTH  = 320;
    constexpr auto SCREEN_HEIGHT = 240;

    /*
     * Calculates whether a given time is within a given timeframe, where start being larger than end represents the
     * timeframe covering the day-change (or equivalent).
     */
    constexpr bool isWithinTimeframe(int32_t currentTime, int32_t start, int32_t end)
    {
        if (start < end) { return currentTime >= start && currentTime < end; }
        else { return !(currentTime >= end && currentTime < start); }
    }

    /*
     * Get the difference between two timepoints in hours, accounting for the day change.
     */
    constexpr int32_t getTimeDiff(int32_t start, int32_t end)
    {
        // <= instead of < fixes day skip? // TODO decide if we actually want to fix that
        if (start <= end)
            return end - start;
        else
            return (24 - start) + end;
    }

    /*
     * Checks whether a given item is a food item.
     */
    constexpr bool isFoodItem(ItemType item)
    {
        if (item >= ItemType::MEAT && item <= ItemType::CHAIN_MELON) return true;
        if (item == ItemType::STEAK) return true;
        if (item == ItemType::RAIN_PLANT) return true;

        return false;
    }

    /*
     * converts a screen X coordinate to a coordinate relative to the center of the screen
     */
    constexpr int32_t toRelativeX(int32_t in)
    {
        return in - SCREEN_WIDTH / 2;
    }

    /*
     * converts a screen Y coordinate to a coordinate relative to the center of the screen
     */
    constexpr int32_t toRelativeY(int32_t in)
    {
        return in - SCREEN_HEIGHT / 2;
    }

    /*
     * Converts a world X coordinate to a tile coordinate.
     */
    constexpr int32_t getTileX(int32_t posX)
    {
        auto tileX = (posX / 100) + 50;
        if (posX < 0) tileX--;

        return tileX;
    }

    /*
     * Converts a world Z coordinate to a tile coordinate.
     */
    constexpr int32_t getTileZ(int32_t posZ)
    {
        auto tileZ = 50 - (posZ / 100);
        if (posZ > 0) tileZ--;

        return tileZ;
    }

    /*
     * Gets the collision value of a given tile. If the tile is out of range 0x80 (impassable) is returned.
     */
    constexpr uint8_t getTile(int32_t tileX, int32_t tileZ)
    {
        if (tileX < 0 || tileX >= 100) return 0x80;
        if (tileZ < 0 || tileZ >= 100) return 0x80;

        return MAP_COLLISION_DATA[tileX + tileZ * 100];
    }

    /*
     * Convert a tile coordinate to a world coordinate.
     */
    constexpr int32_t tileToPos(int16_t tile)
    {
        return (tile - 50) * 100 + 50;
    }

    /*
     * Check whether a given position is within an rectangular area defined by a center X+Z coordinate and an extent in
     * every direction. This check happens on the XZ 2D plane, used for most of the game.
     */
    constexpr bool isWithinRect(int32_t centerX, int32_t centerZ, int32_t extent, Vector* pos)
    {
        auto x = pos->x;
        auto z = pos->z;

        if (centerX - extent > x) return false;
        if (centerX + extent < x) return false;
        if (centerZ - extent > z) return false;
        if (centerZ + extent < z) return false;

        return true;
    }

    /*
     * Get the happiness lifetime penalty for a given happiness value.
     * The vanilla formula is (1 + (happiness - 80) / 50), for values < 80
     */
    constexpr int32_t getHappinessLifetimePenalty(int32_t happiness)
    {
        if (happiness >= 80) return 0;
        return 1 + (happiness - 80) / -50;
    }

    /*
     * Returns whether the DigimonType is a valid Digimon
     */
    constexpr bool isValidDigimon(DigimonType type)
    {
        if (type == DigimonType::INVALID) return false;
        if (type > DigimonType::NPC2_METALGREYMON) return false;

        return true;
    }

    /*
     * Gets the CLUT address of the given point in VRAM
     */
    constexpr uint16_t getClut(uint32_t dx, uint32_t dy)
    {
        return (dy & 0x3ff) << 6 | dx >> 4 & 0x3fU;
    }

    constexpr DigimonData* getDigimonData(DigimonType type)
    {
        return &DIGIMON_DATA[static_cast<uint32_t>(type)];
    }

    constexpr RaiseData* getRaiseData(DigimonType type)
    {
        return &RAISE_DATA[static_cast<uint32_t>(type)];
    }

    constexpr Item* getItem(ItemType type)
    {
        return &ITEM_PARA[static_cast<uint8_t>(type)];
    }

    constexpr Move* getMove(int32_t moveId)
    {
        return &MOVE_DATA[moveId];
    }

    /*
     * Sets the UV coordinates of a POLY_FT4. It makes sure that if the resulting coordinates are a multiple of 256,
     * they get reduced by 1. This is necessary because of a PSX hardware limitation.
     */
    constexpr void setPolyFT4UV(POLY_FT4* prim, int16_t uvX, int16_t uvY, int16_t uvWidth, int16_t uvHeight)
    {
        // The way the PSX works makes it impossible to address the last column and row of VRAM for textures, as
        // attempting to do so will create an integer overflow.
        // Therefore we must crop coordinates to the texture page boundaries.
        auto uMax = uvX + uvWidth;
        auto vMax = uvY + uvHeight;
        if (uMax != 0 && (uMax & 0xFF) == 0) uMax--;
        if (vMax != 0 && (vMax & 0xFF) == 0) vMax--;

        prim->u0 = uvX;
        prim->v0 = uvY;
        prim->u1 = uMax;
        prim->v1 = uvY;
        prim->u2 = uvX;
        prim->v2 = vMax;
        prim->u3 = uMax;
        prim->v3 = vMax;
    }

    inline void Partner_setState(uint8_t state)
    {
        PARTNER_STATE     = state;
        PARTNER_SUB_STATE = 0;
    }

    inline void Partner_setSubState(uint8_t state)
    {
        PARTNER_SUB_STATE = state;
    }

    inline uint8_t Partner_getState()
    {
        return PARTNER_STATE;
    }

    inline uint8_t Partner_getSubState()
    {
        return PARTNER_SUB_STATE;
    }

    inline bool isKeyDownRepeat(uint16_t inputMask)
    {
        return (CHANGED_INPUT & inputMask) != 0;
    }
}

static_assert(getTimeDiff(9, 18) == 9);
static_assert(getTimeDiff(18, 18) == 0);
static_assert(getTimeDiff(18, 3) == 9);

static_assert(!isWithinTimeframe(8, 9, 18));
static_assert(isWithinTimeframe(9, 9, 18));
static_assert(isWithinTimeframe(10, 9, 18));
static_assert(!isWithinTimeframe(18, 9, 18));
static_assert(!isWithinTimeframe(19, 9, 18));
static_assert(!isWithinTimeframe(23, 9, 18));

static_assert(isWithinTimeframe(6, 18, 9));
static_assert(!isWithinTimeframe(9, 18, 9));
static_assert(!isWithinTimeframe(10, 18, 9));
static_assert(isWithinTimeframe(18, 18, 9));
static_assert(isWithinTimeframe(19, 18, 9));
static_assert(isWithinTimeframe(23, 18, 9));

static_assert(getHappinessLifetimePenalty(81) == 0);
static_assert(getHappinessLifetimePenalty(80) == 0);
static_assert(getHappinessLifetimePenalty(79) == 1);
static_assert(getHappinessLifetimePenalty(31) == 1);
static_assert(getHappinessLifetimePenalty(30) == 2);
static_assert(getHappinessLifetimePenalty(0) == 2);
static_assert(getHappinessLifetimePenalty(-19) == 2);
static_assert(getHappinessLifetimePenalty(-20) == 3);
static_assert(getHappinessLifetimePenalty(-69) == 3);
static_assert(getHappinessLifetimePenalty(-70) == 4);
static_assert(getHappinessLifetimePenalty(-85) == 4);
static_assert(getHappinessLifetimePenalty(-100) == 4);