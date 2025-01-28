

#include "Model.hpp"

#include "Helper.hpp"
#include "extern/dw1.hpp"
#include "extern/libapi.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"
#include "extern/psx.hpp"

union ScreenCoord
{
    struct
    {
        int16_t x;
        int16_t y;
    };
    int32_t raw;
};

extern "C"
{
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

        if (entity->flatSprite != 0xFF)
            renderFlatDigimon(entity);
        else
        {
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
        }

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
        entity->locX        = posX;
        entity->locY        = posY;
        entity->locZ        = posZ;
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

    CVector colorInput = {0x80, 0x80, 0x80, 0};

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
        setRotTransMatrix(&combinedMatrix);

        auto* primPtr = libgs_GsGetWorkBase();

        for (int32_t i = 0; i < primCount; i++)
        {
            auto currentPrim = primTop;
            // this should be a bug? It matches a whole lot of primitive types, but uses a lot of them wrong?

            auto primMode = currentPrim[3] & 0xFC;
            bool isLine   = WIREFRAME_RNG_TABLE[i % 16] < wireFrameShare;

            // triangle
            if (primMode == 0x34)
            {
                primTop += 0x1C;
                primPtr = _renderWireframedTriangle(primPtr, currentPrim, vertTop, normalTop, isLine, color);
            }
            else if (primMode == 0x3C)
            {
                primTop += 0x24;
                primPtr = _renderWiredframedQuad(primPtr, currentPrim, vertTop, normalTop, isLine, color);
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

    void loadDigimonTexture(DigimonType type, const char* name, ModelComponent* model)
    {
        uint32_t* buffer = reinterpret_cast<uint32_t*>(libapi_malloc3(0x4800));
        readFileSectors("CHDAT\\ALLTIM.TIM", buffer, static_cast<uint32_t>(type) * 9, 9);
        uploadModelTexture(buffer, model);
        libapi_free3(buffer);
    }

    void concatStrings(char* out, char* in1, char* in2)
    {
        sprintf(reinterpret_cast<uint8_t*>(out), "%s%s", in1, in2);
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

            auto fileSize = lookupFileSize(pathBuffer);
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
        loadDigimonTexture(digimonType, "", comp);
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

    EntityType getEntityType(Entity* entity)
    {
        int32_t i = 0;
        while (ENTITY_TABLE.table[i] != entity)
            i++;

        if (i == 0) return EntityType::PLAYER;
        if (i == 1) return EntityType::PARTNER;
        if (i == 10) return EntityType::NONE;
        return EntityType::NPC;
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
        modelData->modelSize = lookupFileSize(pathBuffer);
    }
}