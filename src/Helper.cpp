#include "Helper.hpp"

#include "Math.hpp"

TileIterator::TileIterator(int8_t startX, int8_t startY, int8_t endX, int8_t endY)
    : endX(endX)
    , endY(endY)
    , currentX(startX)
    , currentY(startY)
{
    int8_t diffX = endX - startX;
    int8_t diffY = endY - startY;
    absX         = abs(diffX);
    absY         = abs(diffY);

    if (absX < absY)
    {
        stepX     = diffX < 0 ? -1 : 1;
        stepY     = diffY < 1 ? -1 : 1;
        subStepX  = abs(diffX % absY);
        subStepY  = 0;
        progressX = absY;
        progressY = 0;
        stepCount = absY;

        if (subStepX == 0) stepX = diffX / absY;
    }
    else
    {
        stepX     = diffX < 1 ? -1 : 1;
        stepY     = diffY < 0 ? -1 : 1;
        subStepX  = 0;
        subStepY  = abs(diffY % absX);
        progressX = 0;
        progressY = absX;
        stepCount = absX;

        if (subStepY == 0) stepY = diffY / absX;
    }
}

TileCoord TileIterator::operator*()
{
    return {currentX, currentY};
}

bool TileIterator::hasNext()
{
    return stepCount > 0;
}

TileIterator& TileIterator::operator++()
{
    stepCount--;
    if (subStepX == 0)
        currentX += stepX;
    else
    {
        progressX -= subStepX;
        if (progressX < 1)
        {
            currentX += stepX;
            progressX += absY;
        }
    }

    if (subStepY == 0)
        currentY += stepY;
    else
    {
        progressY -= subStepY;
        if (progressY < 1)
        {
            currentY += stepY;
            progressY += absX;
        }
    }

    return *this;
}