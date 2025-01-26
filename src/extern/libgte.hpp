#pragma once 
#include "stddef.hpp"

extern "C"
{
    struct Matrix
    {
        int16_t m00;
        int16_t m01;
        int16_t m02;
        int16_t m10;
        int16_t m11;
        int16_t m12;
        int16_t m20;
        int16_t m21;
        int16_t m22;
        int16_t pad;
        int32_t t[3];
    };

    struct SVector
    {
        int16_t x;
        int16_t y;
        int16_t z;
        int16_t pad;
    };

    struct Vector
    {
        int32_t x;
        int32_t y;
        int32_t z;
        int32_t pad;
    };

    extern void libgte_SetRotMatrix(Matrix* matrix);
    extern void libgte_SetTransMatrix(Matrix* matrix);
    extern Matrix* libgte_RotMatrix(SVector* rotation, Matrix* matrix);
    extern Matrix* libgte_ScaleMatrix(Matrix* matrix, Vector* scale);
    extern Matrix* libgte_TransMatrix(Matrix* matrix, Vector* translation);
    extern SVector* libgte_ApplyMatrixSV(Matrix* matrix, SVector* input, SVector* output);
}