

#include "Model.hpp"

#include "Helper.hpp"
#include "extern/dw1.hpp"
#include "extern/libapi.hpp"
#include "extern/libgpu.hpp"
#include "extern/libgs.hpp"
#include "extern/libgte.hpp"
#include "extern/psx.hpp"

extern "C"
{
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

    void renderDigimon(int32_t instanceId)
    {
        auto* entity = ENTITY_TABLE.getEntityById(instanceId);
        if (entity == nullptr) return;

        if (entity->isOnMap != 2 && (entity->isOnMap == 0 && entity->isOnScreen == 0)) return;

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

        auto* model            = getEntityModelComponent(type, entityType);
        entity->animPtr        = model->animTablePtr;
        SkeletonNode* skeleton = DIGIMON_SKELETONS[static_cast<uint32_t>(type)];

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
        auto* matrix  = &posData->posMatrix.modelMatrix;

        libgte_RotMatrix(&posData->rotation, matrix);
        libgte_ScaleMatrix(matrix, &posData->scale);
        libgte_TransMatrix(matrix, &posData->location);
        posData->posMatrix.someValue = 0;
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
}