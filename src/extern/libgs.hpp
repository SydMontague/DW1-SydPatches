#pragma once

#include "libgpu.hpp"
#include "libgte.hpp"
#include "stddef.hpp"

extern "C"
{
    struct TMD_STRUCT
    {
        uint32_t* vert_top;
        uint32_t n_vert;
        uint32_t* normal_top;
        uint32_t n_normal;
        uint32_t* primitive_top;
        uint32_t n_primitive;
        int32_t scale;
    };

    struct GsOT
    {
        uint32_t length;
        uint32_t* origin;
        uint32_t offset;
        uint32_t point;
        uint32_t* current;
    };

    struct GsCOORD2PARAM
    {
        Vector scale;
        SVector rotate;
        Vector translation;
    };

    struct GsCOORDINATE2
    {
        int32_t flag;
        Matrix coord;
        Matrix work;
        GsCOORD2PARAM* param;
        GsCOORDINATE2* super;
        GsCOORDINATE2* sub;
    };

    struct GsDOBJ2
    {
        uint32_t attribute;
        GsCOORDINATE2* coord2;
        TMD_STRUCT* tmd;
        uint32_t id;
    };

    struct GsRVIEW2
    {
        int32_t viewpointX;
        int32_t viewpointY;
        int32_t viewpointZ;

        int32_t refpointX;
        int32_t refpointY;
        int32_t refpointZ;

        int32_t viewpoint_twist;
        GsCOORDINATE2* super;
    };

    extern Matrix libgs_REFERENCE_MATRIX;
    extern Matrix libgs_GsLIGHTWSMATRIX;

    extern void* libgs_GsGetWorkBase();
    extern void libgs_GsSetWorkBase(void* newBase);
    extern void libgs_GsMapModelingData(uint32_t* p);
    extern void libgs_GsLinkObject4(uint32_t* p, GsDOBJ2* objBase, int32_t index);
    extern void libgs_GsInitCoordinate2(GsCOORDINATE2* super, GsCOORDINATE2* base);
    extern void libgs_GsGetLws(GsCOORDINATE2* coord, Matrix* worldMatrix, Matrix* screenMatrix);
    extern void libgs_GsSetLightMatrix(Matrix* worldMatrix);
    extern void libgs_GsSetLsMatrix(Matrix* screenMatrix);
    extern void libgs_GsSortObject4(GsDOBJ2* objp, GsOT* otp, int32_t shift, uint32_t* scratch);
    
}

static_assert(sizeof(GsOT) == 20);