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

    struct GsIMAGE
    {
        uint32_t pixelMode;
        int16_t pixelX;
        int16_t pixelY;
        uint16_t pixelWidth;
        uint16_t pixelHeight;
        uint32_t* pixelPtr;
        int16_t clutX;
        int16_t clutY;
        uint16_t clutWidth;
        uint16_t clutHeight;
        uint32_t* clutPtr;
    };

    struct GsSPRITE
    {
        uint32_t attribute;
        int16_t x;
        int16_t y;
        uint16_t width;
        uint16_t height;
        uint16_t tpage;
        uint8_t u;
        uint8_t v;
        int16_t clutX;
        int16_t clutY;
        uint8_t r;
        uint8_t g;
        uint8_t b;
        int16_t mx;
        int16_t my;
        int16_t scaleX;
        int16_t scaleY;
        int32_t rotate;
    };

    struct GsBOXF
    {
        uint32_t attribute;
        int16_t x;
        int16_t y;
        uint16_t width;
        uint16_t height;
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    struct GsF_LIGHT
    {
        int32_t x;
        int32_t y;
        int32_t z;
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    extern Matrix libgs_REFERENCE_MATRIX;
    extern Matrix libgs_GsLIGHTWSMATRIX;

    void* libgs_GsGetWorkBase();
    void libgs_GsSetWorkBase(void* newBase);
    void libgs_GsMapModelingData(uint32_t* p);
    void libgs_GsLinkObject4(uint32_t* p, GsDOBJ2* objBase, int32_t index);
    void libgs_GsInitCoordinate2(GsCOORDINATE2* super, GsCOORDINATE2* base);
    void libgs_GsGetLws(GsCOORDINATE2* coord, Matrix* worldMatrix, Matrix* screenMatrix);
    void libgs_GsSetLightMatrix(Matrix* worldMatrix);
    void libgs_GsSetLsMatrix(Matrix* screenMatrix);
    void libgs_GsSortObject4(GsDOBJ2* objp, GsOT* otp, int32_t shift, uint32_t* scratch);
    void libgs_GsGetTimInfo(uint32_t* tim, GsIMAGE* image);
    int32_t libgs_GsGetActiveBuffer();
    void libgs_GsClearOt(uint16_t offset, uint16_t point, GsOT* ot);
    void libgs_GsSortSprite(GsSPRITE* sprite, GsOT* ot, uint16_t depth);
    void libgs_GsSortBoxFill(GsBOXF* box, GsOT* ot, uint16_t layer);
    void libgs_GsSetProjection(uint32_t distance);
    int32_t libgs_GsSetRefView2(GsRVIEW2* view);
    void libgs_GsSetFlatLight(uint32_t id, const GsF_LIGHT* light);
    GsOT* libgs_GsSortOt(GsOT* src, GsOT* dest);
    void libgs_GsSetAmbient(uint32_t red, uint32_t green, uint32_t blue);
    void libgs_GsMulCoord3(Matrix* m1, Matrix* m2);
    void libgs_GsSortClear(uint8_t r, uint8_t g, uint8_t b, GsOT* ot);
    void libgs_GsSwapDispBuff();
    void libgs_GsDrawOt(GsOT* ot);
    void libgs_GsSetOrign(int32_t x, int32_t y);
}

static_assert(sizeof(GsOT) == 20);
static_assert(sizeof(GsSPRITE) == 0x24);