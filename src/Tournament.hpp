#pragma once
#include "extern/stddef.hpp"

extern "C"
{
    void startTournament();
    uint8_t* getCupDataJumpTable(uint32_t section, uint32_t id);
    uint8_t* getCupDataJumpTableEntry(uint8_t* scriptPtr, int32_t id);
}