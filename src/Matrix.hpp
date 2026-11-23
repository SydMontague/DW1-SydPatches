#pragma once
#include "extern/libgs.hpp"

extern "C"
{
    void calculatePosition(GsCOORDINATE2* coord, Matrix* matrix);
    void rotateVector(SVector* rotation, Vector* input, Vector* output);
    void toEulerAngles(SVector* output, int32_t deltaX, int32_t deltaY, int32_t deltaZ);
    void matrixToEuler2(Matrix* input, SVector* output);
    int32_t getDistance(int32_t diffX, int32_t diffY, int32_t diffZ);
}
