

#include "Model.hpp"

#include "Files.hpp"
#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Sound.hpp"
#include "extern/dw1.hpp"
#include "extern/libapi.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"
#include "extern/psx.hpp"

struct UV1Packet
{
    uint8_t u;
    uint8_t v;
    uint16_t clut;
};

struct UV2Packet
{
    uint8_t u;
    uint8_t v;
    uint16_t tpage     : 5;
    uint16_t abr       : 2;
    uint16_t colorMode : 2;
};

struct UVPacket
{
    uint8_t u;
    uint8_t v;
    uint16_t pad;
};

struct PrimHeader
{
    uint8_t olen;
    uint8_t ilen;
    uint8_t flag;
    uint8_t mode;
};

extern "C"
{
    static CVector colorInput = {0x80, 0x80, 0x80, 0};

    void renderDropShadow(Entity* entity)
    {
        auto* loc    = &entity->posData->location;
        auto trigger = getTileTrigger(loc);
        if (trigger == -1) return;

        auto radius = getDigimonData(entity->type)->radius;

        SVector p1 = {
            .x = static_cast<int16_t>(loc->x - radius),
            .y = static_cast<int16_t>(loc->y),
            .z = static_cast<int16_t>(loc->z - radius),
        };
        SVector p2 = {
            .x = static_cast<int16_t>(loc->x + radius),
            .y = static_cast<int16_t>(loc->y),
            .z = static_cast<int16_t>(loc->z - radius),
        };
        SVector p3 = {
            .x = static_cast<int16_t>(loc->x - radius),
            .y = static_cast<int16_t>(loc->y),
            .z = static_cast<int16_t>(loc->z + radius),
        };
        SVector p4 = {
            .x = static_cast<int16_t>(loc->x + radius),
            .y = static_cast<int16_t>(loc->y),
            .z = static_cast<int16_t>(loc->z + radius),
        };

        auto* prim = reinterpret_cast<POLY_FT4*>(libgs_GsGetWorkBase());
        libgpu_SetPolyFT4(prim);
        libgpu_SetSemiTrans(prim, 1);

        prim->tpage = 221;
        prim->clut  = libgpu_GetClut(0, 487);
        prim->u0    = 64;
        prim->v0    = 128;
        prim->u1    = 127;
        prim->v1    = 128;
        prim->u2    = 64;
        prim->v2    = 191;
        prim->u3    = 127;
        prim->v3    = 191;

        libgs_GsSetLsMatrix(&libgs_REFERENCE_MATRIX);
        ScreenCoord sXY0;
        ScreenCoord sXY1;
        ScreenCoord sXY2;
        ScreenCoord sXY3;
        int32_t i;
        int32_t flag;
        libgte_RotTransPers4(&p1, &p2, &p3, &p4, &sXY0.raw, &sXY1.raw, &sXY2.raw, &sXY3.raw, &i, &flag);
        prim->x0 = sXY0.x;
        prim->y0 = sXY0.y;
        prim->x1 = sXY1.x;
        prim->y1 = sXY1.y;
        prim->x2 = sXY2.x;
        prim->y2 = sXY2.y;
        prim->x3 = sXY3.x;
        prim->y3 = sXY3.y;
        prim->r0 = 0x30;
        prim->g0 = 0x30;
        prim->b0 = 0x30;
        libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + 0xFFD, prim);
        libgs_GsSetWorkBase(prim + 1);

        // vanilla creates some more SVectors here, based on loc, height and radius, but the function it gets passed to
        // is a no-op, so let's just skip it here
    }

    void updateTMDTextureData(TMDModel* model, uint32_t tpage, uint32_t pixelX, uint32_t pixelY, uint32_t clutPage)
    {
        auto objCount = model->objectCount;

        for (int32_t i = 0; i < objCount; i++)
        {
            auto& obj      = model->objects[i];
            auto primCount = obj.n_primitive;

            auto* currentPrim = obj.primitive_top;
            for (int32_t j = 0; j < primCount; j++)
            {
                auto* header = reinterpret_cast<PrimHeader*>(currentPrim);
                if ((header->mode & 4) != 0)
                {
                    auto* uv1 = reinterpret_cast<UV1Packet*>(currentPrim + 1);
                    uv1->clut += clutPage;
                    uv1->u += pixelX;
                    uv1->v += pixelY;
                    auto* uv2  = reinterpret_cast<UV2Packet*>(currentPrim + 2);
                    uv2->tpage = tpage;
                    uv2->u += pixelX;
                    uv2->v += pixelY;
                    auto* uv3 = reinterpret_cast<UVPacket*>(currentPrim + 3);
                    uv3->u += pixelX;
                    uv3->v += pixelY;
                    if ((header->mode & 8) != 0)
                    {
                        auto* uv4 = reinterpret_cast<UVPacket*>(currentPrim + 4);
                        uv4->u += pixelX;
                        uv4->v += pixelY;
                    }
                }
                // vanilla exits the loop if the mode isn't textured, but we just skip those primitives

                currentPrim += header->ilen + 1;
            }
        }
    }

    void renderFlatDigimon(Entity* entity)
    {
        auto entityType = getEntityType(entity);
        auto* model     = getEntityModelComponent(static_cast<int32_t>(entity->type), entityType);

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

    void renderDigimon(int32_t instanceId)
    {
        auto* entity = ENTITY_TABLE.getEntityById(instanceId);
        if (entity == nullptr) return;

        if (entity->isOnMap != 2 && (entity->isOnMap == 0 || entity->isOnScreen == 0)) return;

        auto boneCount = getDigimonData(entity->type)->boneCount;
        for (int i = 0; i < boneCount; i++)
        {
            auto* posData = &entity->posData[i];
            if (posData->obj.tmd == nullptr) continue;

            Matrix worldMatrix;
            Matrix screenMatrix;

            libgs_GsGetLws(posData->obj.coord2, &worldMatrix, &screenMatrix);
            libgs_GsSetLightMatrix(&worldMatrix);
            libgs_GsSetLsMatrix(&screenMatrix);

            if (entity->flatSprite != 0xFF) continue;

            auto wireFrameVal = 0x10;

            if (instanceId == 1)
            {
                if (PARTNER_WIREFRAME_TOTAL == 0x10)
                    wireFrameVal = PARTNER_WIREFRAME_SUB[i];
                else
                    wireFrameVal = PARTNER_WIREFRAME_TOTAL;
            }
            else if (instanceId == 2)
                wireFrameVal = ENTITY1_WIREFRAME_TOTAL;

            if (wireFrameVal == 0x10)
                libgs_GsSortObject4(&posData->obj, ACTIVE_ORDERING_TABLE, 2, &SCRATCHPAD);
            else
                renderWireframed(&posData->obj, wireFrameVal);
        }

        if (entity->flatSprite != 0xFF) renderFlatDigimon(entity);

        if (instanceId != 0)
            renderDropShadow(entity);
        else if (PLAYER_SHADOW_ENABLED != 0)
            renderDropShadow(entity);
    }

    void initializeDigimonObject(DigimonType type, int32_t instanceId, TickFunction tick)
    {
        auto* entity = ENTITY_TABLE.getEntityById(instanceId);
        if (entity == nullptr) return;

        auto boneCount  = getDigimonData(type)->boneCount;
        auto entityType = getEntityType(entity);
        entity->type    = type;
        if (entityType == EntityType::NPC)
        {
            entity->posData  = reinterpret_cast<PositionData*>(libapi_malloc3(boneCount * sizeof(PositionData)));
            entity->momentum = reinterpret_cast<MomentumData*>(libapi_malloc3(boneCount * sizeof(MomentumData)));
        }
        else if (entityType == EntityType::PARTNER)
        {
            entity->posData  = PARTNER_POSITION_DATA;
            entity->momentum = PARTNER_MOMENTUM_DATA;
        }
        else if (entityType == EntityType::PLAYER)
        {
            entity->posData  = TAMER_POSITION_DATA;
            entity->momentum = TAMER_MOMENTUM_DATA;
        }

        auto* model            = getEntityModelComponent(static_cast<int32_t>(type), entityType);
        entity->animPtr        = model->animTablePtr;
        SkeletonNode* skeleton = DIGIMON_SKELETONS[static_cast<int32_t>(type)];

        for (int32_t boneId = 0; boneId < boneCount; boneId++)
        {
            auto* node    = &skeleton[boneId];
            auto* posData = &entity->posData[boneId];
            auto* base    = &posData->posMatrix;
            auto* parent  = node->parent == -1 ? nullptr : &entity->posData[node->parent].posMatrix;

            if (node->object == -1)
                posData->obj.tmd = nullptr;
            else
                libgs_GsLinkObject4(reinterpret_cast<uint32_t*>(model->modelPtr->objects), &posData->obj, node->object);

            posData->obj.attribute = 0;
            libgs_GsInitCoordinate2(parent, base);
            posData->obj.coord2 = base;
        }

        auto* rootPosData     = &entity->posData[0];
        rootPosData->rotation = {};
        rootPosData->location = {};
        if (type == DigimonType::METEORMON)
            rootPosData->scale = {0x1800, 0x1800, 0x1800};
        else
            rootPosData->scale = {0x1000, 0x1000, 0x1000};

        // vanilla uses initializePosData here, but both functions look functional identical?
        setupModelMatrix(rootPosData);

        // in vanilla the game sets the posData for all the bones here as well, but since that's already done by
        // startAnimation it is redundant. It also didn't respect the hasScale property and thus is kinda bugged to
        // begin wtih.

        startAnimation(entity, 0);
        addObject(static_cast<ObjectID>(type), instanceId, tick, renderDigimon);
    }

    void removeEntity(DigimonType type, int32_t instanceId)
    {
        removeObject(static_cast<ObjectID>(type), instanceId);

        // never remove player or partner
        if (instanceId == 0 || instanceId == 1) return;

        auto* entity = ENTITY_TABLE.getEntityById(instanceId);
        if (entity == nullptr) return;

        libapi_free3(entity->momentum);
        libapi_free3(entity->posData);
        ENTITY_TABLE.setEntity(instanceId, nullptr);
    }

    void setupEntityMatrix(int32_t entityId)
    {
        auto* entity = ENTITY_TABLE.getEntityById(entityId);
        if (entity == nullptr) return;

        auto* posData = entity->posData;
        auto* matrix  = &posData->posMatrix.coord;

        libgte_RotMatrix(&posData->rotation, matrix);
        libgte_ScaleMatrix(matrix, &posData->scale);
        libgte_TransMatrix(matrix, &posData->location);
        posData->posMatrix.flag = 0;
    }

    void setEntityPosition(int32_t entityId, int32_t posX, int32_t posY, int32_t posZ)
    {
        auto* entity = ENTITY_TABLE.getEntityById(entityId);
        if (entity == nullptr) return;

        auto* posData       = entity->posData;
        posData->location.x = posX;
        posData->location.y = posY;
        posData->location.z = posZ;
        entity->locX        = posX << 0x0F;
        entity->locY        = posY << 0x0F;
        entity->locZ        = posZ << 0x0F;
    }

    void setEntityRotation(int32_t entityId, int32_t rotX, int32_t rotY, int32_t rotZ)
    {
        auto* entity = ENTITY_TABLE.getEntityById(entityId);
        if (entity == nullptr) return;

        auto* posData       = entity->posData;
        posData->rotation.x = rotX;
        posData->rotation.y = rotY;
        posData->rotation.z = rotZ;
    }

    inline void* _renderWireframedTriangle(void* primPtr,
                                           uint8_t* currentPrim,
                                           SVector* vertTop,
                                           SVector* normalTop,
                                           bool isPoly,
                                           uint8_t color)
    {
        ScreenCoord sXY0;
        ScreenCoord sXY1;
        ScreenCoord sXY2;
        int32_t i;
        int32_t otz;
        int32_t flag;
        SVector* v0   = vertTop + *reinterpret_cast<uint16_t*>(currentPrim + 0x12);
        SVector* v1   = vertTop + *reinterpret_cast<uint16_t*>(currentPrim + 0x16);
        SVector* v2   = vertTop + *reinterpret_cast<uint16_t*>(currentPrim + 0x1A);
        int32_t depth = libgte_RotNclip3(v0, v1, v2, &sXY0.raw, &sXY1.raw, &sXY2.raw, &i, &otz, &flag);

        // invalid primitive, not visible?
        if (depth <= 0) return primPtr;

        if (isPoly)
        {
            // draw normal
            POLY_GT3* prim = reinterpret_cast<POLY_GT3*>(primPtr);

            SVector* normal0 = normalTop + *reinterpret_cast<uint16_t*>(currentPrim + 0x10);
            SVector* normal1 = normalTop + *reinterpret_cast<uint16_t*>(currentPrim + 0x14);
            SVector* normal2 = normalTop + *reinterpret_cast<uint16_t*>(currentPrim + 0x18);
            CVector* color0  = reinterpret_cast<CVector*>(&prim->r0);
            CVector* color1  = reinterpret_cast<CVector*>(&prim->r1);
            CVector* color2  = reinterpret_cast<CVector*>(&prim->r2);
            libgte_NormalColorCol3(normal0, normal1, normal2, &colorInput, color0, color1, color2);

            // make sure to call this *after* NormalColorCol3, otherwise things get overwritten
            libgpu_SetPolyGT3(prim);

            prim->x0 = sXY0.x;
            prim->y0 = sXY0.y;
            prim->x1 = sXY1.x;
            prim->y1 = sXY1.y;
            prim->x2 = sXY2.x;
            prim->y2 = sXY2.y;

            prim->u0 = currentPrim[4];
            prim->v0 = currentPrim[5];
            prim->u1 = currentPrim[8];
            prim->v1 = currentPrim[9];
            prim->u2 = currentPrim[12];
            prim->v2 = currentPrim[13];

            prim->clut  = *reinterpret_cast<uint16_t*>(currentPrim + 6);
            prim->tpage = *reinterpret_cast<uint16_t*>(currentPrim + 10);

            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + otz / 4, prim);
            primPtr = prim + 1; // advance WorkBase
        }
        else
        {
            LINE_F4* prim = reinterpret_cast<LINE_F4*>(primPtr);
            libgpu_SetLineF4(prim);

            prim->x0 = sXY0.x;
            prim->y0 = sXY0.y;
            prim->x1 = sXY1.x;
            prim->y1 = sXY1.y;
            prim->x2 = sXY2.x;
            prim->y2 = sXY2.y;
            prim->x3 = prim->x0;
            prim->y3 = prim->y0;

            prim->r0 = color;
            prim->g0 = color;
            prim->b0 = color;
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + otz / 4, prim);
            primPtr = prim + 1; // advance WorkBase
        }

        return primPtr;
    }

    inline void* _renderWiredframedQuad(void* primPtr,
                                        uint8_t* currentPrim,
                                        SVector* vertTop,
                                        SVector* normalTop,
                                        bool isPoly,
                                        uint8_t color)
    {
        ScreenCoord sXY0;
        ScreenCoord sXY1;
        ScreenCoord sXY2;
        ScreenCoord sXY3;
        int32_t i;
        int32_t otz;
        int32_t flag;
        SVector* v0   = vertTop + *reinterpret_cast<uint16_t*>(currentPrim + 0x16);
        SVector* v1   = vertTop + *reinterpret_cast<uint16_t*>(currentPrim + 0x1A);
        SVector* v2   = vertTop + *reinterpret_cast<uint16_t*>(currentPrim + 0x1E);
        SVector* v3   = vertTop + *reinterpret_cast<uint16_t*>(currentPrim + 0x22);
        int32_t depth = libgte_RotNclip4(v0, v1, v2, v3, &sXY0.raw, &sXY1.raw, &sXY2.raw, &sXY3.raw, &i, &otz, &flag);

        // invalid primitive, not visible?
        if (depth <= 0) return primPtr;

        if (isPoly)
        {
            // draw normal
            POLY_GT4* prim = reinterpret_cast<POLY_GT4*>(primPtr);

            SVector* normal0 = normalTop + *reinterpret_cast<uint16_t*>(currentPrim + 0x14);
            SVector* normal1 = normalTop + *reinterpret_cast<uint16_t*>(currentPrim + 0x18);
            SVector* normal2 = normalTop + *reinterpret_cast<uint16_t*>(currentPrim + 0x1C);
            SVector* normal3 = normalTop + *reinterpret_cast<uint16_t*>(currentPrim + 0x20);
            CVector* color0  = reinterpret_cast<CVector*>(&prim->r0);
            CVector* color1  = reinterpret_cast<CVector*>(&prim->r1);
            CVector* color2  = reinterpret_cast<CVector*>(&prim->r2);
            CVector* color3  = reinterpret_cast<CVector*>(&prim->r3);
            libgte_NormalColorCol3(normal0, normal1, normal2, &colorInput, color0, color1, color2);
            libgte_NormalColorCol(normal3, &colorInput, color3);

            // make sure to call this *after* NormalColorCol3, otherwise things get overwritten
            libgpu_SetPolyGT4(prim);

            prim->x0 = sXY0.x;
            prim->y0 = sXY0.y;
            prim->x1 = sXY1.x;
            prim->y1 = sXY1.y;
            prim->x2 = sXY2.x;
            prim->y2 = sXY2.y;
            prim->x3 = sXY3.x;
            prim->y3 = sXY3.y;

            prim->u0 = currentPrim[4];
            prim->v0 = currentPrim[5];
            prim->u1 = currentPrim[8];
            prim->v1 = currentPrim[9];
            prim->u2 = currentPrim[12];
            prim->v2 = currentPrim[13];
            prim->u3 = currentPrim[16];
            prim->v3 = currentPrim[17];

            prim->clut  = *reinterpret_cast<uint16_t*>(currentPrim + 6);
            prim->tpage = *reinterpret_cast<uint16_t*>(currentPrim + 10);

            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + otz / 4, prim);
            primPtr = prim + 1; // advance WorkBase
        }
        else
        {
            LINE_F4* prim = reinterpret_cast<LINE_F4*>(primPtr);
            libgpu_SetLineF4(prim);

            prim->x0 = sXY0.x;
            prim->y0 = sXY0.y;
            prim->x1 = sXY1.x;
            prim->y1 = sXY1.y;
            prim->x2 = sXY3.x;
            prim->y2 = sXY3.y;
            prim->x3 = sXY2.x;
            prim->y3 = sXY2.y;
            prim->r0 = color;
            prim->g0 = color;
            prim->b0 = color;
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + otz / 4, prim);
            primPtr = prim + 1; // advance WorkBase

            LINE_F2* prim2 = reinterpret_cast<LINE_F2*>(primPtr);
            libgpu_SetLineF2(prim2);
            prim2->x0 = sXY2.x;
            prim2->y0 = sXY2.y;
            prim2->x1 = sXY0.x;
            prim2->y1 = sXY0.y;
            prim2->r0 = color;
            prim2->g0 = color;
            prim2->b0 = color;
            libgpu_AddPrim(ACTIVE_ORDERING_TABLE->origin + otz / 4, prim2);
            primPtr = prim2 + 1; // advance WorkBase
        }

        return primPtr;
    }

    void renderWireframed(GsDOBJ2* obj, int32_t wireFrameShare)
    {
        auto* tmd          = obj->tmd;
        SVector* vertTop   = reinterpret_cast<SVector*>(tmd->vert_top);
        SVector* normalTop = reinterpret_cast<SVector*>(tmd->normal_top);
        uint8_t* primTop   = reinterpret_cast<uint8_t*>(tmd->primitive_top);
        auto primCount     = tmd->n_primitive;
        Matrix combinedMatrix;
        uint8_t color = (WIREFRAME_COLOR_MIN + rand() % (WIREFRAME_COLOR_MAX - WIREFRAME_COLOR_MIN));

        auto* coord = obj->coord2;
        if (coord->flag == 0)
        {
            coord->flag = 1;
            libgte_MulMatrix0(&coord->coord, &coord->super->work, &coord->work);
        }
        libgte_MulMatrix0(&libgs_GsLIGHTWSMATRIX, &coord->work, &combinedMatrix);
        libgte_SetLightMatrix(&combinedMatrix);
        libgte_CompMatrix(&libgs_REFERENCE_MATRIX, &coord->work, &combinedMatrix);
        libgs_GsSetLsMatrix(&combinedMatrix);

        auto* primPtr = libgs_GsGetWorkBase();

        for (int32_t i = 0; i < primCount; i++)
        {
            auto currentPrim = primTop;
            // this should be a bug? It matches a whole lot of primitive types, but uses a lot of them wrong?

            auto primMode = currentPrim[3] & 0xFC;
            bool isPoly   = WIREFRAME_RNG_TABLE[i & 15] < wireFrameShare;

            // triangle
            if (primMode == 0x34)
            {
                primTop += 0x1C;
                primPtr = _renderWireframedTriangle(primPtr, currentPrim, vertTop, normalTop, isPoly, color);
            }
            else if (primMode == 0x3C)
            {
                primTop += 0x24;
                primPtr = _renderWiredframedQuad(primPtr, currentPrim, vertTop, normalTop, isPoly, color);
            }
        }

        libgs_GsSetWorkBase(primPtr);
    }

    void resetFlattenGlobal()
    {
        TAMER_ENTITY.flatSprite = 0xFF;
        TAMER_ENTITY.flatTimer  = 0;

        PARTNER_ENTITY.flatSprite = 0xFF;
        PARTNER_ENTITY.flatTimer  = 0;
        for (Entity& entity : NPC_ENTITIES)
        {
            entity.flatSprite = 0xFF;
            entity.flatTimer  = 0;
        }
    }

    void uploadModelTexture(uint32_t* timData, ModelComponent* model)
    {
        GsIMAGE imageData;
        libgs_GsGetTimInfo(timData + 1, &imageData);

        imageData.pixelX = (model->pixelPage & 15) * 0x40 + model->pixelOffsetX;
        imageData.pixelY = (model->pixelPage >> 4) * 0x100 + model->pixelOffsetY;
        imageData.clutX  = (model->clutPage & 63) << 4;
        imageData.clutY  = (model->clutPage >> 6);

        RECT pixelRect = {
            .x      = imageData.pixelX,
            .y      = imageData.pixelY,
            .width  = static_cast<int16_t>(imageData.pixelWidth),
            .height = static_cast<int16_t>(imageData.pixelHeight),
        };
        libgpu_LoadImage(&pixelRect, imageData.pixelPtr);
        // if has CLUT
        if ((imageData.pixelMode >> 3 & 1) != 0)
        {
            RECT clutRect = {
                .x      = imageData.clutX,
                .y      = imageData.clutY,
                .width  = static_cast<int16_t>(imageData.clutWidth),
                .height = static_cast<int16_t>(imageData.clutHeight),
            };
            libgpu_LoadImage(&clutRect, imageData.clutPtr);
        }
        libgpu_DrawSync(0);
    }

    void loadDigimonTexture(DigimonType type, ModelComponent* model)
    {
        uint32_t* buffer = reinterpret_cast<uint32_t*>(libapi_malloc3(0x4800));
        readFileSectors("CHDAT\\ALLTIM.TIM", buffer, static_cast<uint32_t>(type) * 9, 9);
        uploadModelTexture(buffer, model);
        libapi_free3(buffer);
    }

    void initializeModelComponents()
    {
        ModelComponent base = {0};
        base.modelId        = -1;
        base.digiType       = DigimonType::INVALID; // NOLINT: bitfield truncation expected

        for (auto& model : NPC_MODEL)
            model = base;
        for (auto& taken : NPC_MODEL_TAKEN)
            taken = 0;

        for (auto& model : UNKNOWN_MODEL)
            model = base;
        for (auto& taken : UNKNOWN_MODEL_TAKEN)
            taken = 0;
    }

    inline ModelComponent* getNPCComponent(DigimonType type)
    {
        for (auto& component : NPC_MODEL)
        {
            if (component.useCount == 0) return &component;
            if (component.digiType == type) return &component;
        }

        return nullptr;
    }

    void loadMMD(DigimonType digimonType, EntityType entityType)
    {
        if (entityType == EntityType::UNKNOWN) return;
        if (!isValidDigimon(digimonType)) return;

        ModelComponent* comp;
        uint8_t pathBuffer[32];
        sprintf(pathBuffer,
                "CHDAT\\MMD%d\\%s.MMD",
                static_cast<uint32_t>(digimonType) / 30,
                PTR_DIGIMON_FILE_NAMES[static_cast<uint32_t>(digimonType)]);

        if (entityType == EntityType::NPC)
        {
            comp = getNPCComponent(digimonType);
            if (comp == nullptr) return;
            if (comp->useCount > 0) return;

            comp->useCount++;
            // TODO this whole NPC_MODEL_TAKEN stuff can probably be removed, as texture sizes are static
            int32_t i = 0;
            for (; i < 5; i++)
                if (NPC_MODEL_TAKEN[i] == 0) break;

            if (i >= 5) return;

            NPC_MODEL_TAKEN[i] = 1;

            comp->pixelPage    = i / 2 + 0x16;
            comp->clutPage     = (((i * 0x10 + 0x20) / 16) & 0x3F) | 0x7A00;
            comp->pixelOffsetX = 0;
            comp->pixelOffsetY = 128 * (i & 1);
            comp->modelId      = i;
            comp->digiType     = digimonType;

            auto fileSize = lookupFileSize(reinterpret_cast<char*>(pathBuffer));
            comp->mmdPtr  = reinterpret_cast<uint8_t*>(libapi_malloc3((fileSize + 0x7FF) & 0xFFFFF800));
        }
        // TODO this is static data?
        if (entityType == EntityType::PLAYER)
        {
            comp               = &TAMER_MODEL;
            comp->pixelPage    = 0x15;
            comp->clutPage     = 0x7A00;
            comp->pixelOffsetX = 0;
            comp->pixelOffsetY = 0;
            comp->modelId      = 0;
            comp->mmdPtr       = TAMER_MODEL_BUFFER;
        }
        if (entityType == EntityType::PARTNER)
        {
            comp               = &PARTNER_MODEL;
            comp->pixelPage    = 0x15;
            comp->clutPage     = 0x7A01;
            comp->pixelOffsetX = 0;
            comp->pixelOffsetY = 0x80;
            comp->modelId      = 0;
            comp->mmdPtr       = PARTNER_MODEL_BUFFER;
        }

        // load MMD file
        readFile(reinterpret_cast<char*>(pathBuffer), comp->mmdPtr);
        comp->modelPtr     = reinterpret_cast<TMDModel*>(reinterpret_cast<uint8_t*>(comp->mmdPtr) +
                                                     reinterpret_cast<uint32_t*>(comp->mmdPtr)[0]);
        comp->animTablePtr = reinterpret_cast<int32_t*>(reinterpret_cast<uint8_t*>(comp->mmdPtr) +
                                                        reinterpret_cast<uint32_t*>(comp->mmdPtr)[1]);
        libgs_GsMapModelingData(&comp->modelPtr->flags);

        // load texture
        loadDigimonTexture(digimonType, comp);
        updateTMDTextureData(comp->modelPtr,
                             comp->pixelPage,
                             comp->pixelOffsetX,
                             comp->pixelOffsetY,
                             comp->clutPage - 0x7A00);
    }

    // this function blows up in size otherwise
    // NOLINTNEXTLINE: dunno why it doesn't know optimize...
    __attribute__((optimize("Os"))) inline void unloadNPCModel(DigimonType type)
    {
        for (auto& model : NPC_MODEL)
        {
            if (model.digiType != type) continue;

            model.useCount--;
            if (model.useCount <= 0)
            {
                model.digiType = DigimonType::INVALID; // NOLINT
                if (model.mmdPtr != nullptr) libapi_free3(model.mmdPtr);
                model.modelPtr     = nullptr;
                model.animTablePtr = nullptr;
                model.mmdPtr       = nullptr;
                if (model.modelId != -1) NPC_MODEL_TAKEN[model.modelId] = 0;
                model.modelId = -1;
            }
        }
    }

    // this function blows up in size otherwise
    // NOLINTNEXTLINE: dunno why it doesn't know optimize...
    __attribute__((optimize("Os"))) inline void unloadUnknowModel(int32_t id)
    {
        for (auto& model : UNKNOWN_MODEL)
        {
            if (model.useCount != id) continue;

            model.useCount = 0;
            if (model.modelId != -1) UNKNOWN_MODEL_TAKEN[model.modelId] = 0;
            model.modelId = -1;
        }
    }

    void unloadModel(int32_t id, EntityType type)
    {
        if (type == EntityType::NPC)
            unloadNPCModel(static_cast<DigimonType>(id));
        else if (type == EntityType::UNKNOWN)
            unloadUnknowModel(id);
    }

    ModelComponent* getEntityModelComponent(int32_t instance, EntityType type)
    {
        if (type == EntityType::PLAYER) return &TAMER_MODEL;
        if (type == EntityType::PARTNER) return &PARTNER_MODEL;

        if (type == EntityType::NPC)
        {
            auto digimonType = static_cast<DigimonType>(instance);
            if (!isValidDigimon(digimonType)) return nullptr;

            for (auto& model : NPC_MODEL)
                if (model.digiType == digimonType && model.useCount > 0) return &model;

            return nullptr;
        }

        if (type == EntityType::UNKNOWN)
        {
            // TODO this should be a bug? UNKNOWN_MODEL has only 16 entries
            if (instance < 0 || instance > 149) return nullptr;

            auto* model = &UNKNOWN_MODEL[instance];
            return model->useCount != 0 ? model : nullptr;
        }

        return nullptr;
    }

    EntityType getEntityTypeById(int32_t entityId)
    {
        if (entityId == 0) return EntityType::PLAYER;
        if (entityId == 1) return EntityType::PARTNER;
        if (entityId < 10) return EntityType::NPC;
        return EntityType::NONE;
    }

    EntityType getEntityType(Entity* entity)
    {
        for (int32_t i = 0; i < ENTITY_TABLE.table.size(); i++)
        {
            if (ENTITY_TABLE.table[i] != entity) continue;

            return getEntityTypeById(i);
        }

        return EntityType::NONE;
    }

    void loadMMDAsync(DigimonType digimonType,
                      EntityType entityType,
                      uint8_t* buffer,
                      EvoModelData* modelData,
                      int32_t* readComplete)
    {
        // vanilla has code for NPCs here, but since it's never used it got removed
        if (entityType != EntityType::PLAYER && entityType != EntityType::PARTNER) return;

        uint8_t pathBuffer[32];
        sprintf(pathBuffer,
                "CHDAT\\MMD%d\\%s.MMD",
                static_cast<uint32_t>(digimonType) / 30,
                PTR_DIGIMON_FILE_NAMES[static_cast<uint32_t>(digimonType)]);

        readFileSectors("CHDAT\\ALLTIM.TIM", buffer, static_cast<uint32_t>(digimonType) * 9, 9);
        modelData->imagePtr  = buffer;
        modelData->imageSize = 0x4800;
        modelData->modelPtr  = buffer + 0x4800;
        addFileReadRequest2(pathBuffer, modelData->modelPtr, readComplete, nullptr, 0);
        modelData->modelSize = lookupFileSize(reinterpret_cast<char*>(pathBuffer));
    }

    void applyMMD(DigimonType digimonType, EntityType entityType, EvoModelData* modelData)
    {
        // vanilla has some code loading TMD and MTN data separate for NPCs, but it never gets used and frankly makes no
        // sense given the purpose of the function
        if (!isValidDigimon(digimonType)) return;
        if (entityType != EntityType::PARTNER) return;

        PARTNER_MODEL.pixelPage    = 0x15;
        PARTNER_MODEL.clutPage     = 0x7a01;
        PARTNER_MODEL.pixelOffsetX = 0;
        PARTNER_MODEL.pixelOffsetY = 0x80;
        PARTNER_MODEL.modelId      = 0;
        PARTNER_MODEL.mmdPtr       = PARTNER_MODEL_BUFFER;
        PARTNER_MODEL.useCount     = 1;

        uploadModelTexture(reinterpret_cast<uint32_t*>(modelData->imagePtr), &PARTNER_MODEL);
        memcpy(PARTNER_MODEL.mmdPtr, modelData->modelPtr, modelData->modelSize);

        PARTNER_MODEL.modelPtr     = reinterpret_cast<TMDModel*>(reinterpret_cast<uint8_t*>(PARTNER_MODEL.mmdPtr) +
                                                             reinterpret_cast<uint32_t*>(PARTNER_MODEL.mmdPtr)[0]);
        PARTNER_MODEL.animTablePtr = reinterpret_cast<int32_t*>(reinterpret_cast<uint8_t*>(PARTNER_MODEL.mmdPtr) +
                                                                reinterpret_cast<uint32_t*>(PARTNER_MODEL.mmdPtr)[1]);
        libgs_GsMapModelingData(&PARTNER_MODEL.modelPtr->flags);
        updateTMDTextureData(PARTNER_MODEL.modelPtr,
                             PARTNER_MODEL.pixelPage,
                             PARTNER_MODEL.pixelOffsetX,
                             PARTNER_MODEL.pixelOffsetY,
                             PARTNER_MODEL.clutPage - 0x7a00);
    }

    static void calculatePosMatrix(PositionData* posData, bool hasTranslation)
    {
        // TODO hasRotation and hasScale are not used if used break animations?
        if (hasTranslation) libgte_TransMatrix(&posData->posMatrix.coord, &posData->location);
        libgte_RotMatrix(&posData->rotation, &posData->posMatrix.coord);
        libgte_ScaleMatrix(&posData->posMatrix.coord, &posData->scale);
        posData->posMatrix.flag = 0;
    }

    void resetMomentumData(MomentumData* data)
    {
        memset(data->delta, 0, sizeof(data->delta));
        memset(data->subDelta, 0, sizeof(data->subDelta));
    }

    constexpr int32_t getAnimatedTextureHeight(DigimonType type)
    {
        switch (type)
        {
            case DigimonType::DEMIMERAMON: return 44;
            case DigimonType::FLAREIZAMON: [[fallthrough]];
            case DigimonType::DARKRIZAMON: return 16;
            case DigimonType::BIRDRAMON: [[fallthrough]];
            case DigimonType::SABERDRAMON: [[fallthrough]];
            case DigimonType::NPC_BIRDRAMON: return 40;
            case DigimonType::MERAMON: [[fallthrough]];
            case DigimonType::NPC_MERAMON: [[fallthrough]];
            case DigimonType::BLUEMERAMON: return 30;
            default: return -1;
        }
    }

    void animateEntityTexture(Entity* entity)
    {
        auto width = getAnimatedTextureHeight(entity->type);
        if (width <= 0) return;
        if ((PLAYTIME_FRAMES % 2) != 0) return;

        auto frame = 0;
        if ((PLAYTIME_FRAMES % 6) == 0)
            frame = 2;
        else if ((PLAYTIME_FRAMES % 4) == 0)
            frame = 1;

        RECT rect{
            .x      = entity->textureX,
            .y      = static_cast<int16_t>(entity->textureY + 32 + (frame * 32)),
            .width  = static_cast<int16_t>(width),
            .height = 32,
        };
        libgpu_MoveImage(&rect, entity->textureX, entity->textureY);
    }

    void setupModelMatrix(PositionData* node)
    {
        libgte_TransMatrix(&node->posMatrix.coord, &node->location);
        libgte_RotMatrix(&node->rotation, &node->posMatrix.coord);
        libgte_ScaleMatrix(&node->posMatrix.coord, &node->scale);
        node->posMatrix.flag = 0;
    }

    void startAnimation(Entity* entity, uint32_t animId)
    {
        auto animOffset = entity->animPtr[animId];

        if (animOffset == 0) return;

        auto entityType        = getEntityType(entity);
        auto model             = getEntityModelComponent(static_cast<int32_t>(entity->type), entityType);
        auto boneCount         = getDigimonData(entity->type)->boneCount;
        auto nodePtr           = entity->posData;
        ReadBuffer animDataPtr = {reinterpret_cast<uint8_t*>(entity->animPtr) + animOffset};
        auto frameCount        = animDataPtr.read<uint16_t>();
        auto momentumPtr       = entity->momentum;
        auto hasScale          = (frameCount & 0x8000) != 0;

        entity->textureX     = (model->pixelPage - 16) * 64;
        entity->textureY     = model->pixelOffsetY + 256;
        entity->loopEndFrame = 1;
        entity->animId       = animId;
        entity->animFrame    = 1;
        entity->animFlag     = 1;
        entity->loopCount    = 0;
        entity->frameCount   = frameCount & 0x7FFF;
        entity->locX         = nodePtr->location.x << 15;
        entity->locY         = nodePtr->location.y << 15;
        entity->locZ         = nodePtr->location.z << 15;

        resetMomentumData(momentumPtr);
        setupModelMatrix(nodePtr);

        for (int32_t i = 1; i < boneCount; i++)
        {
            auto& node = nodePtr[i];

            if (hasScale)
            {
                node.scale.x = animDataPtr.read<int16_t>();
                node.scale.y = animDataPtr.read<int16_t>();
                node.scale.z = animDataPtr.read<int16_t>();
            }
            else
            {
                node.scale.x = 4096;
                node.scale.y = 4096;
                node.scale.z = 4096;
            }

            node.rotation.x = animDataPtr.read<int16_t>();
            node.rotation.y = animDataPtr.read<int16_t>();
            node.rotation.z = animDataPtr.read<int16_t>();
            node.location.x = animDataPtr.read<int16_t>();
            node.location.y = animDataPtr.read<int16_t>();
            node.location.z = animDataPtr.read<int16_t>();

            resetMomentumData(&momentumPtr[i]);
            setupModelMatrix(&node);
        }

        entity->animInstrPtr = reinterpret_cast<int16_t*>(animDataPtr.buffer);
    }

    // this function blows up in size otherwise
    // NOLINTNEXTLINE: dunno why it doesn't know optimize...
    __attribute__((optimize("Os"))) void handleKeyFrameInstruction(MomentumData* momentum, int16_t*& instructionPtrPtr)
    {
        while ((instructionPtrPtr[0] & 0x8000) != 0)
        {
            auto nodeId   = instructionPtrPtr[0] & 0x3F;
            auto axisMask = (instructionPtrPtr[0] >> 6) & 0x1FF;
            auto scale    = instructionPtrPtr[1];
            auto& data    = momentum[nodeId];

            instructionPtrPtr += 2;

            for (int32_t i = 0; i < 9; i++)
            {
                if ((axisMask & (0x100 >> i)) != 0)
                {
                    auto value       = instructionPtrPtr[0];
                    data.delta[i]    = value / scale;
                    int16_t subDelta = value % scale;
                    if (subDelta != 0)
                    {
                        data.subValue[i] = subDelta < 1 ? -1 : 1;
                        data.scale1[i]   = scale;
                        data.subScale[i] = scale;
                    }
                    data.subDelta[i] = abs(subDelta);
                    instructionPtrPtr += 1;
                }
            }
        }
    }

    static constexpr Pair<int32_t, int32_t>
    calculateMomentum(int16_t delta, int16_t scale, int16_t subDelta, int16_t scale2, int8_t subValue)
    {
        auto newValue  = delta;
        auto newScale2 = scale2;

        if (subDelta != 0)
        {
            newScale2 -= subDelta;
            if (newScale2 < 1)
            {
                newScale2 += scale;
                newValue += subValue;
            }
        }

        return {newValue, newScale2};
    }

    static void applyRootMomentum(Entity* entity)
    {
        auto& momentum = entity->momentum[0];

        Vector vel;
        for (int32_t i = 0; i < 3; i++)
        {
            if (i == 0 && (entity->animId == 0x23 || entity->animId == 0x24))
            {
                vel.x = 0;
                continue;
            }

            momentum.subScale[6 + i] -= momentum.subDelta[6 + i];
            if (momentum.subDelta[6 + i] == 0 || momentum.subScale[6 + i] > 0)
            {
                vel[i] = momentum.delta[6 + i] << 0x0F;
                continue;
            }

            vel[i] = (momentum.delta[6 + i] + momentum.subValue[6 + i]) << 0x0F;
            momentum.subScale[6 + i] += momentum.scale1[6 + i];
        }

        if ((entity->animFlag & 2) == 0)
        {
            Vector result;
            auto& posData = entity->posData[0];
            libgte_ApplyMatrixLV(&posData.posMatrix.coord, &vel, &result);

            if ((entity->animFlag & 0x08) && (result.z < 0)) result.z = 0;
            if ((entity->animFlag & 0x10) && (result.x < 0)) result.x = 0;
            if ((entity->animFlag & 0x20) && (result.z > 0)) result.z = 0;
            if ((entity->animFlag & 0x40) && (result.x > 0)) result.x = 0;

            entity->locX += result.x;
            entity->locY += result.y;
            entity->locZ += result.z;
            posData.location.x = entity->locX >> 0x0F;
            posData.location.y = entity->locY >> 0x0F;
            posData.location.z = entity->locZ >> 0x0F;
        }
    }

    static void tickMomentum(Entity* entity, MomentumData* momentum)
    {
        auto boneCount = getDigimonData(entity->type)->boneCount;

        for (int32_t i = 0; i < boneCount; i++)
        {
            auto& node          = entity->momentum[i];
            auto& posData       = entity->posData[i];
            auto hasScale       = false;
            auto hasRotation    = false;
            auto hasTranslation = false;

            for (int32_t j = 0; j < 9; j++)
            {
                auto delta    = node.delta[j];
                auto subDelta = node.subDelta[j];
                auto scale    = node.scale1[j];
                auto subScale = node.subScale[j];
                auto subValue = node.subValue[j];

                if (delta == 0 && subDelta == 0) continue;

                auto newVals     = calculateMomentum(delta, scale, subDelta, subScale, subValue);
                node.subScale[j] = newVals.second;

                if (j < 3)
                {
                    posData.scale[j] += newVals.first;
                    hasScale = true;
                }
                else if (j < 6)
                {
                    posData.rotation[j - 3] += newVals.first;
                    hasRotation = true;
                }
                else if (j < 9 && i != 0)
                {
                    posData.location[j - 6] += newVals.first;
                    hasTranslation = true;
                }
            }

            if (i == 0)
            {
                calculatePosMatrix(&posData, false);
                applyRootMomentum(entity);
                setupModelMatrix(&posData);
            }
            else
                calculatePosMatrix(&posData, hasTranslation);
        }
    }

    void tickAnimation(Entity* entity)
    {
        if ((entity->animFlag & 1) == 0) return;

        auto momentum           = entity->momentum;
        auto& instructionPtrPtr = entity->animInstrPtr;

        tickMomentum(entity, momentum);

        // vanilla checks for "& 0x1000 == 0", but this would detect 0x3000 as start
        if ((*instructionPtrPtr & 0xF000) == 0x1000)
        {
            entity->loopCount = instructionPtrPtr[0] & 0xFF;

            instructionPtrPtr += 1;
            entity->loopStart    = instructionPtrPtr;
            entity->loopEndFrame = instructionPtrPtr[0] & 0xFFF;
        }

        while (entity->animFrame == entity->loopEndFrame)
        {
            auto opCode = *instructionPtrPtr & 0xF000;

            switch (opCode)
            {
                case 0x4000: // play sound
                {
                    auto soundId = instructionPtrPtr[1] & 0xFF;
                    auto vabId   = instructionPtrPtr[1] >> 8;

                    // TODO this is invalid for the tamer? Only seems to matter in VS?
                    // is this even used correctly by the game?
                    if (vabId == 4 && entity->isOnScreen) vabId = ((DigimonEntity*)entity)->vabId;

                    playSound(vabId, soundId);
                    instructionPtrPtr += 2;
                    break;
                }
                case 0x3000: // change texture
                {
                    auto timecode = instructionPtrPtr[0] & 0x0FFF;
                    RECT rect;
                    rect.x      = entity->textureX + (instructionPtrPtr[1] >> 8);
                    rect.y      = entity->textureY + (instructionPtrPtr[1] & 0xFF);
                    rect.width  = instructionPtrPtr[2] >> 8;
                    rect.height = instructionPtrPtr[2] & 0xFF;
                    auto destX  = entity->textureX + (instructionPtrPtr[3] >> 8);
                    auto destY  = entity->textureY + (instructionPtrPtr[3] & 0xFF);
                    libgpu_MoveImage(&rect, destX, destY);
                    instructionPtrPtr += 4;
                    break;
                }
                case 0x2000: // loop end
                {
                    if (entity->loopCount != 0xFF) entity->loopCount--;

                    if (entity->loopCount == 0)
                        instructionPtrPtr += 2;
                    else
                    {
                        entity->animFrame = instructionPtrPtr[1];
                        instructionPtrPtr = reinterpret_cast<int16_t*>(entity->loopStart);
                    }
                    break;
                }
                case 0x1000: // loop start
                {
                    entity->loopCount = instructionPtrPtr[0] & 0xFF;

                    instructionPtrPtr += 1;
                    entity->loopStart = instructionPtrPtr;
                    break;
                }
                case 0x0000: // keyframe
                {
                    instructionPtrPtr += 1;
                    handleKeyFrameInstruction(momentum, instructionPtrPtr);
                    break;
                }
            }

            entity->loopEndFrame = instructionPtrPtr[0] & 0x0FFF;
        }

        if (entity->animFrame == entity->frameCount)
            entity->animFlag &= 0xFE;
        else
            entity->animFrame = entity->animFrame + 1;

        animateEntityTexture(entity);
    }

    void calculateBoneMatrix(Entity* entity, int32_t boneId, Matrix* out)
    {
        auto data = getDigimonData(entity->type);

        if (data->boneCount <= boneId) boneId = 0;

        auto& posData = entity->posData;
        auto& lMatrix = posData->posMatrix.coord;
        libgte_RotMatrix(&posData[0].rotation, &lMatrix);
        libgte_ScaleMatrix(&lMatrix, &posData[0].scale);
        libgte_TransMatrix(&lMatrix, &posData[0].location);
        calculatePosition(&posData[boneId].posMatrix, out);
    }
}