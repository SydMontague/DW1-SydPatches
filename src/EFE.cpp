#include "GameObjects.hpp"
#include "Math.hpp"
#include "extern/dw1.hpp"
#include "extern/libgte.hpp"
#include "extern/stddef.hpp"

template<class T> constexpr T pop()
{
    EFE_DATA_STACK -= sizeof(T); // NOLINT(bugprone-sizeof-expression)
    return *reinterpret_cast<T*>(EFE_DATA_STACK);
}

extern "C"
{
    constexpr uint8_t flashBaseU[4] = {64, 0, 0, 0};

    struct RGB32
    {
        int32_t red;
        int32_t green;
        int32_t blue;
    };

    void EFE_rotateVector()
    {
        Vector* vector    = pop<Vector*>();
        Vector* rotVector = pop<Vector*>();

        Matrix matrix;
        SVector work;
        work.x = rotVector->x;
        work.y = rotVector->y;
        work.z = rotVector->z;
        libgte_RotMatrixZYX(&work, &matrix);
        work.x = vector->x;
        work.y = vector->y;
        work.z = vector->z;
        libgte_ApplyMatrixSV(&matrix, &work, &work);
        vector->x = work.x;
        vector->y = work.y;
        vector->z = work.z;
    }

    uint8_t* initializeFlashData(uint8_t* data)
    {
        EFE_FLASH_DATA = reinterpret_cast<EFEFlashData*>(data);
        for (int32_t i = 0; i < 12; i++)
            EFE_FLASH_DATA[i].progress = -1;

        return data + sizeof(EFEFlashData) * 12;
    }

    void tickFlash(int32_t instanceId)
    {
        auto& data = EFE_FLASH_DATA[instanceId];
        data.progress++;
        if (data.progress >= data.tMax)
        {
            data.progress = -1;
            removeObject(ObjectID::EFE_FLASH, instanceId);
        }
    }

    void renderFlash(int32_t instanceId)
    {
        auto& data   = EFE_FLASH_DATA[instanceId];
        auto absMode = abs(data.mode);

        if (data.progress < 0) return;

        ParticleFlashData flashData;

        auto depth = worldPosToScreenPos(&data.worldPos, &flashData.screenPos);
        if (data.mode < 0)
        {
            flashData.screenPos.x += data.offsetX;
            flashData.screenPos.y += data.offsetY;
        }
        else
        {
            flashData.screenPos.x += ((data.offsetX * VIEWPORT_DISTANCE) / depth);
            flashData.screenPos.y += ((data.offsetY * VIEWPORT_DISTANCE) / depth);
        }
        flashData.sizeX = 64;
        flashData.sizeY = 64;
        if (absMode == 0x20) flashData.tpage = 0xDD;
        if (absMode == 1 || absMode == 0) flashData.tpage = 0xBD;

        flashData.uBase = flashBaseU[absMode % 4];
        flashData.vBase = 192;
        flashData.clut  = 0x79C0;

        auto red             = lerp(data.redMin, data.redMax, 0, data.tMax, data.progress);
        auto green           = lerp(data.greenMin, data.greenMax, 0, data.tMax, data.progress);
        auto blue            = lerp(data.blueMin, data.blueMax, 0, data.tMax, data.progress);
        auto colorFactor     = sin(lerp(128, 20, 0, data.tMax, data.progress));
        flashData.red        = red * colorFactor / 4096;
        flashData.green      = green * colorFactor / 4096;
        flashData.blue       = blue * colorFactor / 4096;
        flashData.colorScale = 0x80;

        auto scale       = lerp(data.scaleMin, data.scaleMax, 0, data.tMax, data.progress);
        scale            = (VIEWPORT_DISTANCE * scale * 10) / depth;
        auto factorScale = sin(lerp(0, 491, 0, 23, data.progress));
        flashData.scale  = scale + (factorScale * 300 / 4096) + 1;

        if (flashData.scale > 32767) return;
        flashData.depth = data.fixedDepth > 0 ? data.fixedDepth : (depth / 16);
        if (flashData.depth > 32 && flashData.depth < 4096) renderParticleFlash(&flashData);
    }

    void EFE_createFlash()
    {
        auto colorMax   = pop<RGB32*>();
        auto colorMin   = pop<RGB32*>();
        auto scaleMax   = pop<int32_t>();
        auto scaleMin   = pop<int32_t>();
        auto tMax       = pop<int32_t>();
        auto fixedDepth = pop<int32_t>();
        auto worldPos   = pop<Vector*>();
        auto mode       = pop<int32_t>();
        auto absMode    = abs(mode);

        FLASH_INSTANCE = -1;
        for (int32_t i = 0; i < 12; i++)
        {
            auto& data = EFE_FLASH_DATA[i];
            if (data.progress >= 0) continue;

            FLASH_INSTANCE = i;
            if (absMode == 0 || absMode == 1 || absMode == 0x20)
            {
                addObject(ObjectID::EFE_FLASH, i, tickFlash, renderFlash);

                data.progress   = 0;
                data.worldPos.x = worldPos->x;
                data.worldPos.z = worldPos->y;
                data.worldPos.y = worldPos->z;
                data.tMax       = tMax;
                data.mode       = mode;
                data.fixedDepth = fixedDepth;
                data.scaleMax   = scaleMax;
                data.scaleMin   = scaleMin;
                data.redMin     = colorMin->red;
                data.greenMin   = colorMin->green;
                data.blueMin    = colorMin->blue;
                data.redMax     = colorMax->red;
                data.greenMax   = colorMax->green;
                data.blueMax    = colorMax->blue;

                if (mode >= 0)
                {
                    data.offsetX = 0;
                    data.offsetY = 0;
                }
                else
                {
                    int32_t offsetX;
                    int32_t offsetY;
                    if (isTamerOnScreen())
                        getDrawingOffsetCopy(&offsetX, &offsetY);
                    else
                    {
                        offsetX = DRAWING_OFFSET_X;
                        offsetY = DRAWING_OFFSET_Y;
                    }
                    data.offsetX = offsetX;
                    data.offsetY = offsetY;
                }
            }
        }
    }
}