

#include "Helper.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"

extern "C"
{
    void initializePosData(PositionData* data)
    {
        Matrix* matrix = &data->posMatrix.modelMatrix;
        libgte_RotMatrix(&data->rotation, matrix);
        libgte_ScaleMatrix(matrix, &data->scale);
        libgte_TransMatrix(matrix, &data->location);
        data->posMatrix.someValue = 0;
    }

    void renderFlatDigimon(Entity* entity)
    {
        auto entityType = getEntityType(entity);
        auto model      = getEntityModelComponent(entity->type, entityType);

        POLY_FT4* prim = (POLY_FT4*)libgs_GsGetWorkBase();
        libgpu_SetPolyFT4(prim);
        prim->r0    = 0x80;
        prim->g0    = 0x80;
        prim->b0    = 0x80;
        prim->clut  = model->clutPage + 0x3C0;
        prim->tpage = model->pixelPage;

        if (entity->flatSprite < 2)
        {
            if (entity->flatTimer++ % 10 == 0) entity->flatSprite ^= 1;
        }

        auto activeSprite = entity->flatSprite % 2;

        // TODO: Vanilla only creates a 15x15 sprite here, since the PSX doesn't allow the last row/column in a texture
        //       page to be addressed. A workaround would be to copy the texture into an addressable memory region.
        prim->u0 = (activeSprite == 0 ? 224 : 240);
        prim->v0 = (entity->flatSprite < 2 ? 0x60 : 0x70) + model->pixelOffsetY;

        prim->u1 = (activeSprite == 0 ? 224 : 240) + 15;
        prim->v1 = (entity->flatSprite < 2 ? 0x60 : 0x70) + model->pixelOffsetY;

        prim->u2 = (activeSprite == 0 ? 224 : 240);
        prim->v2 = (entity->flatSprite < 2 ? 0x60 : 0x70) + model->pixelOffsetY + 15;

        prim->u3 = (activeSprite == 0 ? 224 : 240) + 15;
        prim->v3 = (entity->flatSprite < 2 ? 0x60 : 0x70) + model->pixelOffsetY + 15;

        Matrix matrix;
        auto height   = getDigimonData(entity->type)->height;
        auto* posData = entity->posData;

        SVector inVecs[4];
        SVector outVecs[4];
        inVecs[0].x = 0;
        inVecs[0].y = -height;
        inVecs[0].z = -height / 2;

        inVecs[1].x = 0;
        inVecs[1].y = -height;
        inVecs[1].z = height / 2;

        inVecs[2].x = 0;
        inVecs[2].y = 0;
        inVecs[2].z = -height / 2;

        inVecs[3].x = 0;
        inVecs[3].y = 0;
        inVecs[3].z = height / 2;

        libgte_RotMatrix(&posData->rotation, &matrix);
        libgte_ScaleMatrix(&matrix, &posData->scale);

        for (int32_t i = 0; i < 4; i++)
        {
            libgte_ApplyMatrixSV(&matrix, &inVecs[i], &outVecs[i]);

            outVecs[i].x += posData->location.x;
            outVecs[i].y += posData->location.y;
            outVecs[i].z += posData->location.z;
        }

        add3DSpritePrim(prim, &outVecs[0], &outVecs[1], &outVecs[2], &outVecs[3]);
    }
}