#include "Effects.hpp"

#include "CloudFX.hpp"
#include "DigimonData.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "extern/dw1.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"
#include "extern/psx.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    struct MeramonShakeData
    {
        int32_t backupOffsetX;
        int32_t backupOffsetY;
        int16_t height;
        int16_t width;
        int16_t posY;
        int16_t posX;
        uint8_t frameCount;
    };

    static MeramonShakeData meramonShakeData;

    void addFXPrim(POLY_FT4* prim, int16_t x, int16_t y, uint32_t scaleX, int16_t scaleY, uint32_t distance)
    {
        const auto scaledX = (scaleX * VIEWPORT_DISTANCE) / distance;
        const auto scaledY = (scaleY * VIEWPORT_DISTANCE) / distance;
        const auto minX    = x - scaledX / 2;
        const auto maxX    = x + scaledX / 2;
        const auto minY    = y - scaledY / 2;
        const auto maxY    = y + scaledY / 2;
        const auto depth   = distance / 16;

        prim->x0 = minX;
        prim->x1 = maxX;
        prim->x2 = minX;
        prim->x3 = maxX;
        prim->y0 = minY;
        prim->y1 = minY;
        prim->y2 = maxY;
        prim->y3 = maxY;
        if (depth <= 32 || depth >= 4096) return;

        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + depth, prim);
        libgs_GsSetWorkBase(prim + 1);
    }

    POLY_FT4* renderParticleFlashLoop(POLY_FT4* prim,
                                      ParticleFlashData* data,
                                      int32_t finalScaledX,
                                      int32_t finalScaledY,
                                      int32_t loopCount,
                                      int32_t innerLoopCount,
                                      int32_t heightStep,
                                      int32_t widthStep,
                                      int32_t uStep,
                                      int32_t vStep,
                                      uint32_t red,
                                      uint32_t green,
                                      uint32_t blue)
    {
        // vanilla checks if the sprite would be fully off screen but we do that because
        // HOLY FUCK THIS FUNCTION PISSES ME OFF AND I WANT TO MOVE ON
        // TODO make nicer
        for (int32_t i = 0; i <= loopCount; i++)
        {
            auto yPos = (data->screenPos.y + finalScaledY) + (heightStep * i);

            // if (yPos > (-DRAWING_OFFSET_Y + 240) || (yPos + heightStep) < -DRAWING_OFFSET_Y) continue;

            for (int32_t j = 0; j <= innerLoopCount; j++)
            {
                auto xPos = (data->screenPos.x + finalScaledX) + (widthStep * j);
                // if (xPos > (-DRAWING_OFFSET_X + 320) || (xPos + widthStep) < -DRAWING_OFFSET_X) continue;

                libgpu_SetPolyFT4(prim);
                libgpu_SetSemiTrans(prim, 1);
                prim->tpage = data->tpage;
                prim->clut  = data->clut;
                prim->x0    = xPos;
                prim->y0    = yPos;
                prim->x1    = xPos + widthStep;
                prim->y1    = yPos;
                prim->x2    = xPos;
                prim->y2    = yPos + heightStep;
                prim->x3    = xPos + widthStep;
                prim->y3    = yPos + heightStep;
                if (j == innerLoopCount)
                {
                    prim->x1 = data->screenPos.x;
                    prim->x3 = data->screenPos.x;
                }
                if (i == loopCount)
                {
                    prim->y2 = data->screenPos.y;
                    prim->y3 = data->screenPos.y;
                }
                prim->r0 = red;
                prim->g0 = green;
                prim->b0 = blue;

                prim->u0 = data->uBase + uStep * j;
                prim->u1 = data->uBase + uStep * (j + 1) - 1;
                prim->u2 = data->uBase + uStep * j;
                prim->u3 = data->uBase + uStep * (j + 1) - 1;

                prim->v0 = data->vBase + vStep * i;
                prim->v1 = data->vBase + vStep * i;
                prim->v2 = data->vBase + vStep * (i + 1) - 1;
                prim->v3 = data->vBase + vStep * (i + 1) - 1;

                libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + data->depth, prim);
                prim += 1;
            }
        }
        return prim;
    }

    void renderParticleFlash(ParticleFlashData* data)
    {
        // TODO is this ever going to loop? This function is ugly af
        if (data->depth <= 32 || data->depth >= 4096) return;
        if (abs(data->screenPos.x) >= 1024 || abs(data->screenPos.y) >= 512) return;

        uint8_t sizeX = data->sizeX - 1;
        uint8_t sizeY = data->sizeY - 1;
        auto scaledX  = (data->scale * sizeX) / 1024;
        auto scaledY  = (data->scale * sizeY) / 1024;

        if (scaledX <= 0) return;
        if (scaledY <= 0) return;

        auto finalScaledX = scaledX + 1;
        auto finalScaledY = scaledY + 1;

        uint32_t innerLoopCount;
        if (finalScaledX < 1024 && finalScaledY < 512)
            innerLoopCount = 0;
        else
            innerLoopCount = (((scaledX + 2) / 1024 + 1) | 3) + 1;

        uint32_t loopCount = ((scaledY + 2) / 512) + 1;
        auto widthStep     = (scaledX + 2) / (innerLoopCount + 1);
        auto heightStep    = (scaledY + 2) / (loopCount + 1);
        auto uStep         = (sizeX + 1) / (innerLoopCount + 1);
        auto vStep         = (sizeY + 1) / (loopCount + 1);
        auto red           = data->red * data->colorScale / 128;
        auto green         = data->green * data->colorScale / 128;
        auto blue          = data->blue * data->colorScale / 128;

        POLY_FT4* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());

        prim = renderParticleFlashLoop(prim,
                                       data,
                                       -finalScaledX,
                                       -finalScaledY,
                                       loopCount,
                                       innerLoopCount,
                                       heightStep,
                                       widthStep,
                                       uStep,
                                       vStep,
                                       red,
                                       green,
                                       blue);
        prim = renderParticleFlashLoop(prim,
                                       data,
                                       finalScaledX,
                                       -finalScaledY,
                                       loopCount,
                                       innerLoopCount,
                                       heightStep,
                                       -widthStep,
                                       uStep,
                                       vStep,
                                       red,
                                       green,
                                       blue);
        prim = renderParticleFlashLoop(prim,
                                       data,
                                       -finalScaledX,
                                       finalScaledY,
                                       loopCount,
                                       innerLoopCount,
                                       -heightStep,
                                       widthStep,
                                       uStep,
                                       vStep,
                                       red,
                                       green,
                                       blue);
        prim = renderParticleFlashLoop(prim,
                                       data,
                                       finalScaledX,
                                       finalScaledY,
                                       loopCount,
                                       innerLoopCount,
                                       -heightStep,
                                       -widthStep,
                                       uStep,
                                       vStep,
                                       red,
                                       green,
                                       blue);

        libgs_GsSetWorkBase(prim);
    }

    void renderFXParticle(SVector* pos, int16_t scale, const RGB8* color)
    {
        auto* prim  = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        auto mapPos = getMapPosition(*pos);
        libgpu_SetPolyFT4(prim);
        libgpu_SetSemiTrans(prim, 1);

        prim->code |= 2;
        prim->r0    = color->red;
        prim->g0    = color->green;
        prim->b0    = color->blue;
        prim->tpage = 0x3C;
        prim->clut  = 0x7A4C;
        prim->u0    = 0;
        prim->u1    = 15;
        prim->u2    = 0;
        prim->u3    = 15;
        prim->v0    = 160;
        prim->v1    = 160;
        prim->v2    = 175;
        prim->v3    = 175;
        addFXPrim(prim, mapPos.screenX, mapPos.screenY, scale, scale, mapPos.depth);
    }

    static void tickMeramonShake(int32_t instanceId)
    {
        auto frameCount = meramonShakeData.frameCount;

        auto offsetX = 1;
        if (frameCount < 31)
            offsetX = 1;
        else if (frameCount < 61)
            offsetX = 2;
        else if (frameCount < 121)
            offsetX = 3;
        else if (frameCount < 181)
            offsetX = 2;

        if (frameCount % 2 == 0)
        {
            meramonShakeData.posX   = max(meramonShakeData.posX - 32, -200);
            meramonShakeData.posY   = max(meramonShakeData.posY - 32, -200);
            meramonShakeData.width  = meramonShakeData.posX * -2;
            meramonShakeData.height = meramonShakeData.posY * -2;
            offsetX                 = -offsetX;
        }

        DRAWING_OFFSET_X += offsetX;

        auto prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyFT4(prim);
        libgpu_SetSemiTrans(prim, 1);
        prim->tpage = 0xdd;
        prim->clut  = getClut(0, 0x1e7);
        setPosDataPolyFT4(prim,
                          meramonShakeData.posX,
                          meramonShakeData.posY,
                          meramonShakeData.width,
                          meramonShakeData.height);
        setUVDataPolyFT4(prim, 64, 128, 63, 63);
        prim->r0 = 0;
        prim->g0 = 255;
        prim->b0 = 255;
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 9, prim);
        libgs_GsSetWorkBase(prim + 1);

        meramonShakeData.frameCount++;
        if (meramonShakeData.frameCount > 209)
        {
            DRAWING_OFFSET_X = meramonShakeData.backupOffsetX;
            DRAWING_OFFSET_Y = meramonShakeData.backupOffsetY;
            removeObject(ObjectID::MERAMON_SHAKE, instanceId);
        }
    }

    void createMeramonShake()
    {
        meramonShakeData.backupOffsetX = DRAWING_OFFSET_X;
        meramonShakeData.backupOffsetY = DRAWING_OFFSET_Y;
        meramonShakeData.frameCount    = 0;
        meramonShakeData.width         = 0;
        meramonShakeData.height        = 0;
        meramonShakeData.posX          = 0;
        meramonShakeData.posY          = 0;
        addObject(ObjectID::MERAMON_SHAKE, 0, tickMeramonShake, nullptr);
    }

    void getRViewCopy(GsRVIEW2* val)
    {
        *val = GS_VIEWPOINT_COPY;
    }

    void getViewportDistanceCopy(uint32_t* val)
    {
        *val = VIEWPORT_DISTANCE_COPY;
    }

    void getDrawingOffsetCopy(int32_t* offsetX, int32_t* offsetY)
    {
        *offsetX = DRAWING_OFFSET_X_COPY;
        *offsetY = DRAWING_OFFSET_Y_COPY;
    }

    void translateConditionFXToEntity(Entity* entity, SVector* out)
    {
        auto type    = getOriginalType(entity->type);
        auto& offset = CONDITION_FX_OFFSETS[static_cast<int32_t>(type)];
        auto& node   = entity->posData[offset.pad].posMatrix.work;

        libgte_ApplyMatrixSV(&node, &offset, out);
        out->x += node.t[0];
        out->y += node.t[1];
        out->z += node.t[2];
    }

    void addScreenPolyFT3(POLY_FT3* prim, SVector* pos1, SVector* pos2, SVector* pos3)
    {
        setRotTransMatrix(&libgs_REFERENCE_MATRIX);

        int32_t dummy1;
        int32_t dummy2;
        ScreenCoord xy0;
        ScreenCoord xy1;
        ScreenCoord xy2;
        auto screenZ = libgte_RotTransPers3(pos1, pos2, pos3, &xy0.raw, &xy1.raw, &xy2.raw, &dummy1, &dummy2);

        if (screenZ / 4 <= 0x20 || screenZ / 4 >= 0x1000) return;

        prim->x0 = xy0.x;
        prim->x1 = xy1.x;
        prim->x2 = xy2.x;
        prim->y0 = xy0.y;
        prim->y1 = xy1.y;
        prim->y2 = xy2.y;

        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + screenZ, prim);
        libgs_GsSetWorkBase(prim + 1);
    }

    void addScreenPolyFT4(POLY_FT4* prim, SVector* pos1, SVector* pos2, SVector* pos3, SVector* pos4)
    {
        setRotTransMatrix(&libgs_REFERENCE_MATRIX);
        int32_t emp;
        int32_t ty;
        ScreenCoord xy0;
        ScreenCoord xy1;
        ScreenCoord xy2;
        ScreenCoord xy3;
        auto screenZ = libgte_RotTransPers4(pos1, pos2, pos3, pos4, &xy0.raw, &xy1.raw, &xy2.raw, &xy3.raw, &emp, &ty);

        if (screenZ / 4 <= 0x20 || screenZ / 4 >= 0x1000) return;

        prim->x0 = xy0.x;
        prim->x1 = xy1.x;
        prim->x2 = xy2.x;
        prim->x3 = xy3.x;
        prim->y0 = xy0.y;
        prim->y1 = xy1.y;
        prim->y2 = xy2.y;
        prim->y3 = xy3.y;

        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + screenZ, prim);
        libgs_GsSetWorkBase(prim + 1);
    }

    void renderSprite(GsSPRITE* sprite, int16_t x, int16_t y, uint32_t distance, int32_t scaleX, int32_t scaleY)
    {
        auto depth     = distance / 16;
        sprite->x      = x;
        sprite->y      = y;
        sprite->scaleX = (scaleX * VIEWPORT_DISTANCE) / distance;
        sprite->scaleY = (scaleY * VIEWPORT_DISTANCE) / distance;
        if (depth <= 0 || depth >= 4096) return;

        libgs_GsSortSprite(sprite, ACTIVE_ORDERING_TABLE, depth);
    }

    void renderTMDModel(TMDModel* model,
                        int32_t objId,
                        GsCOORDINATE2* baseCoord,
                        GsCOORDINATE2* superCoord,
                        Vector* translation,
                        SVector* rotation,
                        Vector* scale)
    {
        GsDOBJ2 obj;
        libgs_GsLinkObject4(reinterpret_cast<uint32_t*>(model->objects), &obj, objId);
        libgs_GsInitCoordinate2(superCoord, baseCoord);

        obj.attribute = 0;
        obj.coord2    = baseCoord;

        projectPosition(baseCoord, translation, rotation, scale);
        drawObject(&obj, ACTIVE_ORDERING_TABLE, 2);
    }
}