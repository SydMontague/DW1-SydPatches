#pragma once

#include "extern/stddef.hpp"

extern "C"
{
    void clearMapDigimon();
    void removeMapEntities();
    void setMovementEnabled(int32_t entityId, uint32_t mode);
}
