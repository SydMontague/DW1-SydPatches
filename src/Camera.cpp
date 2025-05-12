#include "Camera.hpp"

#include "GameObjects.hpp"
#include "Helper.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

struct CameraData
{
    int16_t drawOffsetX;
    int16_t drawOffsetY;
    int16_t diffX;
    int16_t diffY;
    int16_t finalX;
    int16_t finalY;
    int16_t deltaX;
    int16_t deltaY;
};

struct CameraAtEdge
{
    bool canMoveX;
    bool canMoveY;
};

// data
static CameraData data;
static bool cameraFollowPlayer;

// forward declarations
extern "C"
{
    void tickCameraMovement(int32_t speed);
}

// translation unit local functions
namespace
{
    Vector getEntityLocation(uint32_t entityId)
    {
        if (entityId == 0xfd) return TAMER_ENTITY.posData->location;
        if (entityId == 0xfc) return PARTNER_ENTITY.posData->location;

        for (auto& entity : NPC_ENTITIES)
            if (entity.scriptId == entityId) return entity.posData->location;

        return {};
    }

    bool checkCameraMovement(int32_t speed)
    {
        if (CAMERA_REACHED_TARGET == -1)
        {
            CAMERA_REACHED_TARGET = 0;
            addObject(ObjectID::CAMERA_MOVEMENT, speed, tickCameraMovement, nullptr);
            return false;
        }
        if (CAMERA_REACHED_TARGET == 1)
        {
            CAMERA_REACHED_TARGET = -1;
            return true;
        }

        return false;
    }

    CameraAtEdge cameraIsAtEdge()
    {
        CameraAtEdge result{.canMoveX = true, .canMoveY = true};

        if (CAMERA_X < 0)
        {
            DRAWING_OFFSET_X = DRAW_OFFSET_LIMIT_X_MAX;
            CAMERA_X         = 0;
            result.canMoveX  = false;
        }
        else if (CAMERA_X > (MAP_WIDTH * 128 - SCREEN_WIDTH))
        {
            DRAWING_OFFSET_X = DRAW_OFFSET_LIMIT_X_MIN;
            CAMERA_X         = (MAP_WIDTH * 128 - SCREEN_WIDTH);
            result.canMoveX  = false;
        }

        if (CAMERA_Y < 0)
        {
            DRAWING_OFFSET_Y = DRAW_OFFSET_LIMIT_Y_MAX;
            CAMERA_Y         = 0;
            result.canMoveY  = false;
        }
        else if (CAMERA_Y > (MAP_HEIGHT * 128 - SCREEN_HEIGHT))
        {
            DRAWING_OFFSET_Y = DRAW_OFFSET_LIMIT_Y_MIN;
            CAMERA_Y         = (MAP_HEIGHT * 128 - SCREEN_HEIGHT);
            result.canMoveY  = false;
        }

        return result;
    }
} // namespace

void updateDrawingOffsets(const MapPos& oldPos, const MapPos& newPos)
{
    auto result = cameraIsAtEdge();
    auto diffX  = oldPos.screenX - newPos.screenX;
    auto diffY  = oldPos.screenY - newPos.screenY;

    PLAYER_OFFSET_X += diffX;
    PLAYER_OFFSET_Y += diffY;

    if (result.canMoveX)
    {
        DRAWING_OFFSET_X += diffX;
        if ((POLLED_INPUT & InputButtons::BUTTON_LEFT) != 0 && PLAYER_OFFSET_X < DRAWING_OFFSET_X)
        {
            DRAWING_OFFSET_X -= diffX;
            CAMERA_X += diffX;
        }
        else if ((POLLED_INPUT & InputButtons::BUTTON_RIGHT) != 0 && DRAWING_OFFSET_X < PLAYER_OFFSET_X)
        {
            DRAWING_OFFSET_X -= diffX;
            CAMERA_X += diffX;
        }
    }
    if (result.canMoveY)
    {
        DRAWING_OFFSET_Y += diffY;
        if ((POLLED_INPUT & InputButtons::BUTTON_UP) != 0 && PLAYER_OFFSET_Y < DRAWING_OFFSET_Y)
        {
            DRAWING_OFFSET_Y -= diffY;
            CAMERA_Y += diffY;
        }
        else if ((POLLED_INPUT & InputButtons::BUTTON_DOWN) != 0 && DRAWING_OFFSET_Y < PLAYER_OFFSET_Y)
        {
            DRAWING_OFFSET_Y -= diffY;
            CAMERA_Y += diffY;
        }
    }
}

extern "C"
{
    void tickCameraMovement(int32_t speed)
    {
        if (!CAMERA_HAS_TARGET)
        {
            auto viewPos   = getMapPosition(GS_VIEWPOINT.refpointX, GS_VIEWPOINT.refpointY, GS_VIEWPOINT.refpointZ);
            auto cameraPos = getMapPosition(CAMERA_TARGET);

            data.drawOffsetX = DRAWING_OFFSET_X;
            data.drawOffsetY = DRAWING_OFFSET_Y;
            data.diffX       = viewPos.screenX - cameraPos.screenX + (SCREEN_WIDTH / 2);
            data.diffY       = viewPos.screenY - cameraPos.screenY + (SCREEN_HEIGHT / 2);

            PLAYER_OFFSET_X += (data.diffX - PLAYER_OFFSET_X);
            PLAYER_OFFSET_Y += (data.diffY - PLAYER_OFFSET_Y);

            data.diffX  = clamp(data.diffX, DRAW_OFFSET_LIMIT_X_MIN, DRAW_OFFSET_LIMIT_X_MAX);
            data.diffY  = clamp(data.diffY, DRAW_OFFSET_LIMIT_Y_MIN, DRAW_OFFSET_LIMIT_Y_MAX);
            data.finalX = CAMERA_X + (data.drawOffsetX - data.diffX);
            data.finalY = CAMERA_Y + (data.drawOffsetY - data.diffY);
            data.deltaX = abs((data.diffX - data.drawOffsetX) % speed);
            data.deltaY = abs((data.diffY - data.drawOffsetY) % speed);
            unsetCameraFollowPlayer();
            CAMERA_HAS_TARGET = true;
        }

        auto drawDiffX   = data.diffX - data.drawOffsetX;
        auto drawDiffY   = data.diffY - data.drawOffsetY;
        auto cameraDiffX = -drawDiffX;
        auto cameraDiffY = -drawDiffY;

        CAMERA_X += (cameraDiffX / speed);
        CAMERA_Y += (cameraDiffY / speed);
        DRAWING_OFFSET_X += (drawDiffX / speed);
        DRAWING_OFFSET_Y += (drawDiffY / speed);

        if (data.deltaX >= 0)
        {
            CAMERA_X += cameraDiffX < 1 ? -1 : 1;
            DRAWING_OFFSET_X += drawDiffX < 1 ? -1 : 1;
            data.deltaX--;
        }

        if (data.deltaY >= 0)
        {
            CAMERA_Y += cameraDiffY < 1 ? -1 : 1;
            DRAWING_OFFSET_Y += drawDiffY < 1 ? -1 : 1;
            data.deltaY--;
        }

        auto result = cameraIsAtEdge();

        if (result.canMoveX)
        {
            if (drawDiffX < 0)
                DRAWING_OFFSET_X = max(DRAWING_OFFSET_X, data.diffX);
            else
            {
                DRAWING_OFFSET_X = min(DRAWING_OFFSET_X, data.diffX);
                if (data.drawOffsetX == data.diffX) DRAWING_OFFSET_X = data.diffX;
            }

            if (DRAWING_OFFSET_X == data.diffX)
            {
                CAMERA_X        = data.finalX;
                result.canMoveX = false;
            }
        }

        if (result.canMoveY)
        {
            if (drawDiffY < 0)
                DRAWING_OFFSET_Y = max(DRAWING_OFFSET_Y, data.diffY);
            else
            {
                DRAWING_OFFSET_Y = min(DRAWING_OFFSET_Y, data.diffY);
                if (data.drawOffsetY == data.diffY) DRAWING_OFFSET_Y = data.diffY;
            }

            if (DRAWING_OFFSET_Y == data.diffY)
            {
                CAMERA_Y        = data.finalY;
                result.canMoveY = false;
            }
        }

        handleTileUpdate(0, (cameraDiffX / speed) >= 80);

        if (!result.canMoveX && !result.canMoveY)
        {
            CAMERA_HAS_TARGET = false;
            setCameraFollowPlayer();
            if (CAMERA_REACHED_TARGET == 0) CAMERA_REACHED_TARGET = 1;

            removeObject(ObjectID::CAMERA_MOVEMENT, speed);
            if (CAMERA_UPDATE_TILES == 1)
            {
                uploadMapTileImages(MAP_TILE_DATA.data(), MAP_TILE_X + MAP_TILE_Y * MAP_WIDTH);
                CAMERA_UPDATE_TILES = false;
            }
        }
    }

    void createCameraMovement(Vector* target, int32_t instanceId)
    {
        CAMERA_TARGET = *target;
        addObject(ObjectID::CAMERA_MOVEMENT, instanceId, tickCameraMovement, nullptr);
        CAMERA_REACHED_TARGET = -1;
    }

    bool tickCameraMoveTo(int32_t posX, int32_t posZ, int32_t speed)
    {
        CAMERA_TARGET = {.x = posX, .y = TAMER_ENTITY.posData->location.y, .z = posZ};
        return checkCameraMovement(speed);
    }

    bool tickCameraMoveToEntity(uint32_t entityId, int32_t speed)
    {
        CAMERA_TARGET = getEntityLocation(entityId);
        return checkCameraMovement(speed);
    }

    void moveCameraByDiff(Vector* start, Vector* end)
    {
        auto startPos = getMapPosition(*start);
        auto endPos   = getMapPosition(*end);

        CAMERA_X += (endPos.screenX - startPos.screenX);
        CAMERA_Y += (endPos.screenY - startPos.screenY);

        updateDrawingOffsets(startPos, endPos);
        handleTileUpdate(0, false);
    }

    void moveCameraByOffset(int32_t diffX, int32_t diffY)
    {
        CAMERA_X += diffX;
        CAMERA_Y += diffY;
        DRAWING_OFFSET_X -= diffX;
        DRAWING_OFFSET_Y -= diffY;

        handleTileUpdate(0, true);
    }

    bool isCameraFollowingPlayer()
    {
        return cameraFollowPlayer;
    }
    
    void unsetCameraFollowPlayer()
    {
        cameraFollowPlayer = false;
    }

    void setCameraFollowPlayer()
    {
        cameraFollowPlayer = true;
    }
}