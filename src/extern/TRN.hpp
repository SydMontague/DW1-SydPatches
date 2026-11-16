#pragma once
#include "stddef.hpp"

extern "C"
{
    void TRN_setupHpTraining(int32_t mapId);
    void TRN_setupOffenseTraining(int32_t mapId);
    void TRN_setupSpeedTraining(int32_t mapId);
    void TRN_setupDefenseTraining(int32_t mapId);
    void TRN_setupMpTraining(int32_t mapId);
    void TRN_setupBrainsTraining(int32_t mapId);
}
