#include "Math.hpp"

#include "Helper.hpp"
#include "extern/dw1.hpp"
#include "extern/libc.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"
#include "extern/psx.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    constexpr int16_t SIN_LOOKUP_TABLE[] = {
        0x0,   0x32,  0x64,  0x96,  0xC8,  0xFB,  0x12D, 0x15F, 0x191,  0x1C3, 0x1F5, 0x227, 0x259, 0x28A, 0x2BC,
        0x2ED, 0x31F, 0x350, 0x381, 0x3B2, 0x3E3, 0x413, 0x444, 0x474,  0x4A5, 0x4D5, 0x504, 0x534, 0x563, 0x593,
        0x5C2, 0x5F0, 0x61F, 0x64D, 0x67B, 0x6A9, 0x6D7, 0x704, 0x731,  0x75E, 0x78A, 0x7B7, 0x7E2, 0x80E, 0x839,
        0x864, 0x88F, 0x8B9, 0x8E3, 0x90D, 0x936, 0x95F, 0x987, 0x9B0,  0x9D7, 0x9FF, 0xA26, 0xA4D, 0xA73, 0xA99,
        0xABE, 0xAE3, 0xB08, 0xB2C, 0xB50, 0xB73, 0xB96, 0xBB8, 0xBDA,  0xBFC, 0xC1D, 0xC3E, 0xC5E, 0xC7D, 0xC9D,
        0xCBB, 0xCD9, 0xCF7, 0xD14, 0xD31, 0xD4D, 0xD69, 0xD84, 0xD9F,  0xDB9, 0xDD2, 0xDEB, 0xE04, 0xE1C, 0xE33,
        0xE4A, 0xE60, 0xE76, 0xE8B, 0xEA0, 0xEB4, 0xEC8, 0xEDB, 0xEED,  0xEFF, 0xF10, 0xF21, 0xF31, 0xF40, 0xF4F,
        0xF5D, 0xF6B, 0xF78, 0xF85, 0xF91, 0xF9C, 0xFA7, 0xFB1, 0xFBA,  0xFC3, 0xFCB, 0xFD3, 0xFDA, 0xFE1, 0xFE7,
        0xFEC, 0xFF0, 0xFF4, 0xFF8, 0xFFB, 0xFFD, 0xFFE, 0xFFF, 0x1000, 0x0,
    };

    constexpr int16_t ATAN_LOOKUP_TABLE[] = {
        0x0,   0x1,   0x3,   0x4,   0x5,   0x6,   0x8,   0x9,   0xA,   0xB,   0xD,   0xE,   0xF,   0x11,  0x12,  0x13,
        0x14,  0x16,  0x17,  0x18,  0x19,  0x1B,  0x1C,  0x1D,  0x1F,  0x20,  0x21,  0x22,  0x24,  0x25,  0x26,  0x27,
        0x29,  0x2A,  0x2B,  0x2C,  0x2E,  0x2F,  0x30,  0x32,  0x33,  0x34,  0x35,  0x37,  0x38,  0x39,  0x3A,  0x3C,
        0x3D,  0x3E,  0x3F,  0x41,  0x42,  0x43,  0x45,  0x46,  0x47,  0x48,  0x4A,  0x4B,  0x4C,  0x4D,  0x4F,  0x50,
        0x51,  0x52,  0x54,  0x55,  0x56,  0x57,  0x59,  0x5A,  0x5B,  0x5C,  0x5E,  0x5F,  0x60,  0x61,  0x63,  0x64,
        0x65,  0x66,  0x68,  0x69,  0x6A,  0x6B,  0x6C,  0x6E,  0x6F,  0x70,  0x71,  0x73,  0x74,  0x75,  0x76,  0x78,
        0x79,  0x7A,  0x7B,  0x7D,  0x7E,  0x7F,  0x80,  0x81,  0x83,  0x84,  0x85,  0x86,  0x88,  0x89,  0x8A,  0x8B,
        0x8C,  0x8E,  0x8F,  0x90,  0x91,  0x92,  0x94,  0x95,  0x96,  0x97,  0x98,  0x9A,  0x9B,  0x9C,  0x9D,  0x9F,
        0xA0,  0xA1,  0xA2,  0xA3,  0xA4,  0xA6,  0xA7,  0xA8,  0xA9,  0xAA,  0xAC,  0xAD,  0xAE,  0xAF,  0xB0,  0xB2,
        0xB3,  0xB4,  0xB5,  0xB6,  0xB7,  0xB9,  0xBA,  0xBB,  0xBC,  0xBD,  0xBE,  0xC0,  0xC1,  0xC2,  0xC3,  0xC4,
        0xC5,  0xC7,  0xC8,  0xC9,  0xCA,  0xCB,  0xCC,  0xCE,  0xCF,  0xD0,  0xD1,  0xD2,  0xD3,  0xD4,  0xD6,  0xD7,
        0xD8,  0xD9,  0xDA,  0xDB,  0xDC,  0xDE,  0xDF,  0xE0,  0xE1,  0xE2,  0xE3,  0xE4,  0xE5,  0xE7,  0xE8,  0xE9,
        0xEA,  0xEB,  0xEC,  0xED,  0xEE,  0xEF,  0xF1,  0xF2,  0xF3,  0xF4,  0xF5,  0xF6,  0xF7,  0xF8,  0xF9,  0xFA,
        0xFC,  0xFD,  0xFE,  0xFF,  0x100, 0x101, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107, 0x109, 0x10A, 0x10B, 0x10C,
        0x10D, 0x10E, 0x10F, 0x110, 0x111, 0x112, 0x113, 0x114, 0x115, 0x116, 0x117, 0x119, 0x11A, 0x11B, 0x11C, 0x11D,
        0x11E, 0x11F, 0x120, 0x121, 0x122, 0x123, 0x124, 0x125, 0x126, 0x127, 0x128, 0x129, 0x12A, 0x12B, 0x12C, 0x12D,
        0x12E, 0x12F, 0x130, 0x131, 0x132, 0x133, 0x134, 0x135, 0x136, 0x137, 0x138, 0x139, 0x13A, 0x13B, 0x13C, 0x13D,
        0x13E, 0x13F, 0x140, 0x141, 0x142, 0x143, 0x144, 0x145, 0x146, 0x147, 0x148, 0x149, 0x14A, 0x14B, 0x14C, 0x14D,
        0x14E, 0x14F, 0x150, 0x151, 0x152, 0x153, 0x154, 0x155, 0x156, 0x157, 0x158, 0x159, 0x15A, 0x15A, 0x15B, 0x15C,
        0x15D, 0x15E, 0x15F, 0x160, 0x161, 0x162, 0x163, 0x164, 0x165, 0x166, 0x167, 0x168, 0x168, 0x169, 0x16A, 0x16B,
        0x16C, 0x16D, 0x16E, 0x16F, 0x170, 0x171, 0x172, 0x173, 0x173, 0x174, 0x175, 0x176, 0x177, 0x178, 0x179, 0x17A,
        0x17B, 0x17B, 0x17C, 0x17D, 0x17E, 0x17F, 0x180, 0x181, 0x182, 0x183, 0x183, 0x184, 0x185, 0x186, 0x187, 0x188,
        0x189, 0x189, 0x18A, 0x18B, 0x18C, 0x18D, 0x18E, 0x18F, 0x18F, 0x190, 0x191, 0x192, 0x193, 0x194, 0x195, 0x195,
        0x196, 0x197, 0x198, 0x199, 0x19A, 0x19A, 0x19B, 0x19C, 0x19D, 0x19E, 0x19F, 0x19F, 0x1A0, 0x1A1, 0x1A2, 0x1A3,
        0x1A3, 0x1A4, 0x1A5, 0x1A6, 0x1A7, 0x1A8, 0x1A8, 0x1A9, 0x1AA, 0x1AB, 0x1AC, 0x1AC, 0x1AD, 0x1AE, 0x1AF, 0x1B0,
        0x1B0, 0x1B1, 0x1B2, 0x1B3, 0x1B3, 0x1B4, 0x1B5, 0x1B6, 0x1B7, 0x1B7, 0x1B8, 0x1B9, 0x1BA, 0x1BA, 0x1BB, 0x1BC,
        0x1BD, 0x1BE, 0x1BE, 0x1BF, 0x1C0, 0x1C1, 0x1C1, 0x1C2, 0x1C3, 0x1C4, 0x1C4, 0x1C5, 0x1C6, 0x1C7, 0x1C7, 0x1C8,
        0x1C9, 0x1CA, 0x1CA, 0x1CB, 0x1CC, 0x1CD, 0x1CD, 0x1CE, 0x1CF, 0x1D0, 0x1D0, 0x1D1, 0x1D2, 0x1D2, 0x1D3, 0x1D4,
        0x1D5, 0x1D5, 0x1D6, 0x1D7, 0x1D7, 0x1D8, 0x1D9, 0x1DA, 0x1DA, 0x1DB, 0x1DC, 0x1DC, 0x1DD, 0x1DE, 0x1DF, 0x1DF,
        0x1E0, 0x1E1, 0x1E1, 0x1E2, 0x1E3, 0x1E3, 0x1E4, 0x1E5, 0x1E6, 0x1E6, 0x1E7, 0x1E8, 0x1E8, 0x1E9, 0x1EA, 0x1EA,
        0x1EB, 0x1EC, 0x1EC, 0x1ED, 0x1EE, 0x1EE, 0x1EF, 0x1F0, 0x1F0, 0x1F1, 0x1F2, 0x1F2, 0x1F3, 0x1F4, 0x1F4, 0x1F5,
        0x1F6, 0x1F6, 0x1F7, 0x1F8, 0x1F8, 0x1F9, 0x1FA, 0x1FA, 0x1FB, 0x1FC, 0x1FC, 0x1FD, 0x1FD, 0x1FE, 0x1FF, 0x1FF,
    };

    int32_t random(int32_t limit)
    {
        return (limit * rand()) >> 0xF;
    }

    int16_t sin(uint32_t input)
    {
        auto val = input & 0x1FF;

        if (val < 0x80)
            return SIN_LOOKUP_TABLE[val];
        else if (val < 0x100)
            return SIN_LOOKUP_TABLE[0x100 - val];
        else if (val < 0x180)
            return -SIN_LOOKUP_TABLE[val - 0x100];
        else
            return -SIN_LOOKUP_TABLE[0x200 - val];
    }

    int16_t cos(uint32_t input)
    {
        return sin(input + 0x80);
    }

    int16_t atan(int32_t dy, int32_t dx)
    {
        uint32_t generalDirection;
        uint32_t offset;
        uint32_t tmpVal;

        if (dy >= 0 && dy >= 0) generalDirection = 0;
        if (dy < 0 && dx >= 0) generalDirection = 1;
        if (dy < 0 && dx < 0) generalDirection = 2;
        if (dy >= 0 && dx < 0) generalDirection = 3;

        dx = abs(dx);
        dy = abs(dy);

        if (dy < dx)
        {
            if (dx < 1)
                offset = 0;
            else
                offset = (dy << 9) / dx;
            if (offset > 0x1FF) offset = 0x1FF;
            tmpVal = 0x3FF - ATAN_LOOKUP_TABLE[offset];
        }
        else
        {
            if (dy < 1)
                offset = 0;
            else
                offset = (dx << 9) / dy;
            if (offset > 0x1FF) offset = 0x1FF;
            tmpVal = ATAN_LOOKUP_TABLE[offset];
        }

        if (generalDirection == 1 || generalDirection == 2) tmpVal = 0x800 - tmpVal;
        if (generalDirection >= 2) tmpVal = -tmpVal;

        return (tmpVal + 0x800) & 0xFFF;
    }

    int32_t lerp(int32_t start, int32_t end, int32_t tMin, int32_t tMax, int32_t tCurrent)
    {
        auto diff     = end - start;
        auto tDiff    = tMax - tMin;
        auto progress = tCurrent - tMin;

        if (tDiff == 0) return 0;

        auto val = (diff * progress) / tDiff;
        if (diff < 0)
        {
            val %= (diff - 1);
            if (val > 0) val += diff;
        }
        else
        {
            val %= (diff + 1);
            if (val < 0) val += diff;
        }

        return start + val;
    }

    void setRotTransMatrix(Matrix* matrix)
    {
        libgs_GsSetLsMatrix(matrix);
    }

    int32_t worldPosToScreenPos(SVector* worldPos, Position* screenPos)
    {
        auto pos     = getMapPosition(*worldPos);
        screenPos->x = pos.screenX;
        screenPos->y = pos.screenY;
        return pos.depth;
    }

    void worldPosToScreenPos2(int16_t* x, int16_t* y, int16_t* z)
    {
        auto pos = getScreenPosition(*x, *y, *z);
        *x       = pos.screenX;
        *y       = pos.screenY;
    }

    bool hasAABBOverlap(AABB* aabb1, AABB* aabb2)
    {
        auto x1     = aabb1->center->x;
        auto x2     = aabb2->center->x;
        auto width1 = aabb1->extent.x;
        auto width2 = aabb2->extent.x;

        if (x1 - width1 > x2 + width2) return false;
        if (x1 + width1 < x2 - width2) return false;

        auto y1      = aabb1->center->y;
        auto y2      = aabb2->center->y;
        auto height1 = aabb1->extent.y;
        auto height2 = aabb2->extent.y;

        if (y1 - height1 > y2 + height2) return false;
        if (y1 + height1 < y2 - height2) return false;

        auto z1     = aabb1->center->z;
        auto z2     = aabb2->center->z;
        auto depth1 = aabb1->extent.z;
        auto depth2 = aabb2->extent.z;

        if (z1 - depth1 > z2 + depth2) return false;
        if (z1 + depth1 < z2 - depth2) return false;

        return true;
    }

    int32_t findAABBHitEntity(AABB* aabb, Entity* ignoreEntity, int32_t startId)
    {
        for (int32_t i = startId; i < 10; i++)
        {
            auto* entity = ENTITY_TABLE.getEntityById(i);
            if (entity == ignoreEntity || entity == nullptr) continue;

            auto loc       = entity->posData->location;
            SVector center = {
                .x   = static_cast<int16_t>(loc.x),
                .y   = static_cast<int16_t>(loc.y),
                .z   = static_cast<int16_t>(loc.z),
                .pad = static_cast<int16_t>(loc.pad),
            };
            int16_t radius = getDigimonData(entity->type)->radius >> 1;
            int16_t height = getDigimonData(entity->type)->height >> 1;
            AABB digimonBB{.center = &center, .extent = {.x = radius, .y = height, .z = radius}};

            if (hasAABBOverlap(aabb, &digimonBB)) return i;
        }

        return -1;
    }

    void getRotationDifference(PositionData* pos, Vector* target, int16_t* outAngle, int16_t* ccDiff, int16_t* cwDiff)
    {
        auto targetAngle  = atan(target->z - pos->location.z, target->x - pos->location.x);
        auto currentAngle = pos->rotation.y;

        *outAngle = targetAngle;
        if (currentAngle < targetAngle)
        {
            *cwDiff = targetAngle - currentAngle;
            *ccDiff = currentAngle + (4096 - targetAngle);
        }
        else
        {
            *cwDiff = targetAngle + (4096 - currentAngle);
            *ccDiff = currentAngle - targetAngle;
        }
    }

    bool rotateEntity(SVector* rotVector, int16_t targetAngle, int16_t ccDiff, int16_t cwDiff, int16_t speed)
    {
        // vanilla doesn't handle the case ccDiff == cwDiff, resulting in it never returning true and thus softlocking
        auto currentAngle = rotVector->y;

        if (currentAngle == targetAngle)
        {
            rotVector->y = targetAngle;
            return true;
        }

        if (currentAngle < targetAngle)
        {
            if (ccDiff < cwDiff)
            {
                rotVector->y -= speed;
                if (rotVector->y < (targetAngle - 4096))
                {
                    rotVector->y = targetAngle;
                    return true;
                }
            }
            else
            {
                rotVector->y += speed;
                if (rotVector->y > targetAngle)
                {
                    rotVector->y = targetAngle;
                    return true;
                }
            }
        }
        else if (currentAngle > targetAngle)
        {
            if (ccDiff < cwDiff)
            {
                rotVector->y -= speed;
                if (rotVector->y < targetAngle)
                {
                    rotVector->y = targetAngle;
                    return true;
                }
            }
            else
            {
                rotVector->y += speed;
                if (rotVector->y > (targetAngle + 4096))
                {
                    rotVector->y = targetAngle;
                    return true;
                }
            }
        }

        return false;
    }

    void addWithLimit(int16_t* valuePtr, int16_t value, int32_t limit)
    {
        *valuePtr += value;
        if (*valuePtr > limit) *valuePtr = limit;
    }

    /*
     * Sets a int16_t 'count' times every 'step' bytes to 'value', starting at 'ptr'.
     */
    void setInt16WithStride(int16_t* ptr, int16_t value, int32_t count, int32_t step)
    {
        for (int32_t i = 0; i < count; i++)
        {
            *ptr = value;
            ptr  = reinterpret_cast<int16_t*>(reinterpret_cast<uint8_t*>(ptr) + step);
        }
    }
}

ScreenPos getScreenPosition(int16_t x, int16_t y, int16_t z)
{
    auto mapPos = getMapPosition(x, y, z);

    return {
        .screenX = static_cast<int16_t>(mapPos.screenX - ((SCREEN_WIDTH / 2) - DRAWING_OFFSET_X)),
        .screenY = static_cast<int16_t>(mapPos.screenY - ((SCREEN_HEIGHT / 2) - DRAWING_OFFSET_Y)),
        .depth   = mapPos.depth,
    };
}

ScreenPos getScreenPosition(const Vector& pos)
{
    return getScreenPosition(pos.x, pos.y, pos.z);
}

ScreenPos getScreenPosition(const SVector& pos)
{
    return getScreenPosition(pos.x, pos.y, pos.z);
}

ScreenPos getScreenPosition(int16_t tileX, int16_t tileZ)
{
    return getScreenPosition(convertTileToPosX(tileX), 0, convertTileToPosZ(tileZ));
}

MapPos getMapPosition(int16_t x, int16_t y, int16_t z)
{
    libgs_GsSetLsMatrix(&libgs_REFERENCE_MATRIX);

    ScreenCoord raw{.x = x, .y = y};
    cop2_v0xy = raw.raw;
    cop2_v0z  = z;

    asm volatile("cop2 0x180001");

    return {
        .screenX = static_cast<int16_t>(cop2_sxy2 & 0xFFFF),
        .screenY = static_cast<int16_t>(cop2_sxy2 >> 0x10),
        .depth   = static_cast<int16_t>(cop2_sz3 & 0xFFFFFFFC),
    };
}

MapPos getMapPosition(const Vector& pos)
{
    return getMapPosition(pos.x, pos.y, pos.z);
}

MapPos getMapPosition(const SVector& pos)
{
    return getMapPosition(pos.x, pos.y, pos.z);
}

MapPos getMapPosition(int16_t tileX, int16_t tileZ)
{
    return getMapPosition(convertTileToPosX(tileX), 0, convertTileToPosZ(tileZ));
}
