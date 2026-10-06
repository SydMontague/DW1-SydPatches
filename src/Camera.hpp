#pragma once

#include "Math.hpp"
#include "extern/stddef.hpp"

void updateDrawingOffsets(const MapPos& oldPos, const MapPos& newPos);

extern "C"
{
    void moveCameraByDiff(Vector* start, Vector* end);
    void moveCameraByOffset(int32_t diffX, int32_t diffY);
    bool tickCameraMoveTo(int32_t posX, int32_t posZ, int32_t speed);
    bool isCameraFollowingPlayer();
    void unsetCameraFollowPlayer();
    void setCameraFollowPlayer();
    void createCameraMovement(Vector* target, int32_t instanceId);
}
