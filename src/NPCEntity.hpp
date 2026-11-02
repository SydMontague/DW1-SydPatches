#pragma once

#include "extern/dw1.hpp"
#include "extern/stddef.hpp"

extern "C"
{
    void clearMapDigimon();
    void removeMapEntities();
    void setMovementEnabled(int32_t entityId, uint32_t mode);
    bool scriptSetDigimon(DigimonType digimonId, int32_t actorId, uint8_t autoTalk);
    void scriptUnloadEntity(int32_t entityId);
    void resetEntityOrigin(uint32_t scriptId);
}
