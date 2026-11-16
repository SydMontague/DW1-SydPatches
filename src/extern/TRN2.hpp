#pragma once
#include "stddef.hpp"

extern "C"
{
    void TRN2_setupHpTraining(int32_t mapId);
    void TRN2_setupOffenseTraining(int32_t mapId);
    void TRN2_setupSpeedTraining(int32_t mapId);
    void TRN2_setupDefenseTraining(int32_t mapId);
    void TRN2_setupMpTraining(int32_t mapId);
}
