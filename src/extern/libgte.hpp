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

    struct CVector
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t code;
    };

    extern void libgte_SetRotMatrix(Matrix* matrix);
    extern void libgte_SetTransMatrix(Matrix* matrix);
    extern Matrix* libgte_RotMatrix(SVector* rotation, Matrix* matrix);
    extern Matrix* libgte_ScaleMatrix(Matrix* matrix, Vector* scale);
    extern Matrix* libgte_TransMatrix(Matrix* matrix, Vector* translation);
    extern SVector* libgte_ApplyMatrixSV(Matrix* matrix, SVector* input, SVector* output);
    extern Matrix* libgte_MulMatrix0(Matrix* in1, Matrix* in2, Matrix* out);
    extern Matrix* libgte_CompMatrix(Matrix* in1, Matrix* in2, Matrix* out);
    extern void libgte_SetLightMatrix(Matrix* input);
    extern int32_t libgte_RotNclip3(SVector* vec0,
                                    SVector* vec1,
                                    SVector* vec2,
                                    int32_t* screenXY0,
                                    int32_t* screenXY1,
                                    int32_t* screenXY2,
                                    int32_t* interpolValue,
                                    int32_t* otz,
                                    int32_t* flag);
    extern int32_t libgte_RotNclip4(SVector* vec0,
                                    SVector* vec1,
                                    SVector* vec2,
                                    SVector* vec3,
                                    int32_t* screenXY0,
                                    int32_t* screenXY1,
                                    int32_t* screenXY2,
                                    int32_t* screenXY3,
                                    int32_t* interpolValue,
                                    int32_t* otz,
                                    int32_t* flag);
    extern void libgte_NormalColorCol3(SVector* normal0,
                                       SVector* normal1,
                                       SVector* normal2,
                                       CVector* colorInput,
                                       CVector* colorOut0,
                                       CVector* colorOut1,
                                       CVector* colorOut2);
    extern void libgte_NormalColorCol(SVector* normal, CVector* colorInput, CVector* colorOut);
}