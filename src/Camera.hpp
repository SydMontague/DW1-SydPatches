#pragma once

#include "Math.hpp"
#include "extern/stddef.hpp"

void updateDrawingOffsets(const MapPos& oldPos, const MapPos& newPos);

extern "C"
{
    void moveCameraByDiff(Vector* start, Vector* end);
    bool isCameraFollowingPlayer();
    void unsetCameraFollowPlayer();
    void setCameraFollowPlayer();
    void createCameraMovement(Vector* target, int32_t instanceId);
}