#pragma once
#include "extern/dw1.hpp"

extern "C"
{
    void createCloudFX(SVector* position);
    void removeAllCloudFX();
}

void initializeCloudFXData();